#include "serialPort.h"

static int sp_hookId = 7;

static Queue* sendQueue;
static Queue* receiveQueue;

static int hold_reg_empty;

static int sp_read(int port, uint8_t* value){
    if(util_sys_inb(port + COM1, value)) return 1;
    return 0;
}

static int sp_write(int port, uint8_t value){
    if(sys_outb(COM1 + port,value)) return 1;
    return 0;
}

static int sp_bitrate(int bitrate) {
    uint16_t rate = 115200 / bitrate;

    uint8_t ratemsb = 0, ratelsb = 0;
    util_get_LSB(rate, &ratelsb);
    util_get_MSB(rate, &ratemsb);

    //SET DLAB
    uint8_t lcr;
    
    if(sp_read(LCR,&lcr)) return 1;

    lcr = lcr | LCR_DLAB;

    if(sp_write(LCR,lcr)) return 1;

    //SET BITRATE
    if(sp_write(DIVISOR_LATCH_LSB,ratelsb))return 1;

    if(sp_write(DIVISOR_LATCH_MSB,ratemsb))return 1;

    //RESET DLAB
    if(sp_read(LCR,&lcr)) return 1;

    lcr = lcr & ~LCR_DLAB;

    if(sp_write(LCR,lcr)) return 1;

  return 0;
}

static int sp_init() {
    //SET PARITY AND BITS PER CHAR
    uint8_t lcr;

    if(sp_read(LCR,&lcr)) return 1;
    
    lcr = (lcr & 0xC4) | (LCR_8_BITS) | (LCR_ODD_PARITY);

    if(sp_write(LCR,lcr)) return 1;


    //SET BITRATE
    if (sp_bitrate(115200)) return 1; 

    //ENABLE INTERRUPTIONS
    uint8_t ier;
    if(sp_read(IER,&ier)) return 1;
    ier = ier | (IER_RDAI | IER_THREI | IER_RLSI);
    if(sp_write(IER, ier)) return 1;

    //ENABLE FIFO AND CLEAR TRANSMITTER AND RECEIVER FIFO
    uint8_t fcr;
    if(sp_read(FCR,&fcr)) return 1;
    fcr = fcr | (FCR_ENABLE_FIFO | FCR_CLEAR_RCVR_FIFO | FCR_CLEAR_XMIT_FIFO);
    if(sp_write(FCR, fcr)) return 1;

  return 0;
}


static int sp_end() {

    //DISABLE INTERRUPTIONS
    uint8_t ier;
    if(sp_read(IER,&ier)) return 1;
    ier = ier & (~IER_RDAI | ~IER_THREI | ~IER_RLSI);
    if(sp_write(IER, ier)) return 1;

    //DISABLE FIFO AND CLEAR TRANSMITTER AND RECEIVER FIFO
    uint8_t fcr;
    if(sp_read(FCR,&fcr)) return 1;
    fcr = fcr & (~FCR_ENABLE_FIFO | ~FCR_CLEAR_RCVR_FIFO | ~FCR_CLEAR_XMIT_FIFO);
    if(sp_write(FCR, fcr)) return 1;

    free(sendQueue->values);
    free(receiveQueue->values);
    free(sendQueue);
    free(receiveQueue);

  return 0;
}


int (sp_subscribe)(uint8_t *bit_no) {

    if (bit_no == NULL) return 1;    

    *bit_no = BIT(sp_hookId);

    if (sys_irqsetpolicy(COM1_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &sp_hookId) != 0){
        printf("Error: could not subscribe SP interruption\n");
        return 1;
    } 

    if (sp_init()) return 1;

    sendQueue = new_queue();
    receiveQueue = new_queue();

    return 0;
}


int (sp_unsubscribe)() {
    if (sp_end()) return 1;

    if (sys_irqrmpolicy(&sp_hookId) != 0) {return 3;}

    return 0;
}


int sp_clear(){ //ver //copiado
    uint8_t reg;
    reg = (FCR_CLEAR_RCVR_FIFO | FCR_CLEAR_XMIT_FIFO | FCR_ENABLE_FIFO);
    if(sp_write(FCR, reg)) return 1;
    while(queue_pop(receiveQueue) != 0);
    return 0;
}


int send_bytes_in_queue(){ //copiado //ver
    if(queue_isEmpty(sendQueue)){
        hold_reg_empty = 1;
        return 1;
    }

    uint8_t empty_transmitter;

    while(!queue_isEmpty(sendQueue)){
        sp_write(THR,queue_front(sendQueue));
        queue_pop(sendQueue);
        sp_read(LSR, &empty_transmitter);
        empty_transmitter &= LSR_TRANSMITTER_EMPTY_REG;
        hold_reg_empty = empty_transmitter;
        if(!empty_transmitter) return 1;
    }
    return 0;
}


int send_byte(uint8_t byte){ //copiado //ver
    int pushed = queue_push(sendQueue,byte);
    if(hold_reg_empty){
        return send_bytes_in_queue();
    }else return pushed;
    
}


int establish_connection(){ //copiado
    bool connection = false;
    uint8_t frontByte = queue_front(receiveQueue);

    if(frontByte == 0x53){
        send_byte(0x54);
    }else if(frontByte == 0x54){
        send_byte(0x55);
    }else if(frontByte == 0x55){
        // Generate a random byte that isn't one of the special protocol values
        uint8_t srandByte;
        do {
            srandByte = rand();
        } while(srandByte == ACK || srandByte == NACK || srandByte == END || srandByte == 0);

        // Send synchronization bytes
        send_byte(0x56);
        send_byte(srandByte);

        // Seed the random number generator
        srandom(srandByte);
    }else if(frontByte == 0x56){
        queue_pop(receiveQueue); // Remove the 0x56 byte

        // Extract the srandByte for synchronization
        uint8_t srandByte;
        do {
            srandByte = queue_pop(receiveQueue); // Get the next byte
        } while(srandByte == 0);

        // Seed the random number generator
        srandom(srandByte);

        // Perform game synchronization tasks
        connection = true;

        // Acknowledge synchronization completion
        send_byte(0x57);
    }else if(frontByte == 0x57){
        connection = true;
    }

    queue_pop(receiveQueue); // Ensure to always queue_pop the processed byte
    return connection;
}

int read_byte(){ //copiado
    uint8_t reg, byte;
    sp_read(LSR, &reg);
    if(reg & LSR_RECEIVER_DATA){
        sp_read(RBR, &byte);
        if(!(reg & (LSR_OVERRUN_ERROR | LSR_PARITY_ERROR | LSR_FRAMING_ERROR))){
            queue_push(receiveQueue, byte);
            return 0;
        }
    }
    return 1;
}


void sp_ih(){ //copiado
    uint8_t reg;
    sp_read(IIR, &reg);
    while(!(reg & IIR_PENDING)) {
        if(reg & IIR_DATA_AVAILABLE){
            while(!read_byte());
            sp_read(IIR, &reg);
        }
        if(reg & IIR_TRANSMIT_HOLD_EMPTY){
            send_bytes_in_queue();
            sp_read(IIR, &reg);
        }
    }
}
