#include "serialPort.h"
#include "../model/cue.h"
#include "../model/mouseModel.h"

static int sp_hookId = 7;

static Queue *sendQueue;
static Queue *receiveQueue;
static Queue *standByQueue;
static DATATYPES currentDataType;

static int transmitter_empty = 1;
bool sendTrySync = false;
bool player = false; // false -- player2

static int sp_read(int port, uint8_t *value) {
  if (util_sys_inb(port + COM1, value))
    return 1;
  return 0;
}

static int sp_write(int port, uint8_t value) {
  if (sys_outb(COM1 + port, value))
    return 1;
  return 0;
}

static int sp_bitrate(int bitrate) {
  uint16_t rate = 115200 / bitrate;

  uint8_t ratemsb = 0, ratelsb = 0;
  util_get_LSB(rate, &ratelsb);
  util_get_MSB(rate, &ratemsb);

  // SET DLAB
  uint8_t lcr;

  if (sp_read(LCR, &lcr))
    return 1;

  lcr = lcr | LCR_DLAB;

  if (sp_write(LCR, lcr))
    return 1;

  // SET BITRATE
  if (sp_write(DIVISOR_LATCH_LSB, ratelsb))
    return 1;

  if (sp_write(DIVISOR_LATCH_MSB, ratemsb))
    return 1;

  // RESET DLAB
  if (sp_read(LCR, &lcr))
    return 1;

  lcr = lcr & ~LCR_DLAB;

  if (sp_write(LCR, lcr))
    return 1;

  return 0;
}

static int sp_init() {

  // ENABLE INTERRUPTIONS
  uint8_t ier;
  if (sp_read(IER, &ier))
    return 1;
  ier = ier | (IER_RDAI | IER_THREI | IER_RLSI);
  if (sp_write(IER, ier))
    return 1;

  // ENABLE FIFO AND CLEAR TRANSMITTER AND RECEIVER FIFO
  uint8_t fcr;
  if (sp_read(FCR, &fcr))
    return 1;
  fcr = fcr | (FCR_ENABLE_FIFO | FCR_CLEAR_RCVR_FIFO | FCR_CLEAR_XMIT_FIFO);
  if (sp_write(FCR, fcr))
    return 1;

  sendQueue = new_queue();
  receiveQueue = new_queue();
  standByQueue = new_queue();

  // SET BITRATE
  if (sp_bitrate(115200))
    return 1;

  // SET PARITY AND BITS PER CHAR
  uint8_t lcr;

  if (sp_read(LCR, &lcr))
    return 1;

  lcr = (lcr & 0xC4) | (LCR_8_BITS) | (LCR_ODD_PARITY);

  if (sp_write(LCR, lcr))
    return 1;

  return 0;
}

static int sp_end() {

  // DISABLE INTERRUPTIONS
  uint8_t ier;
  if (sp_read(IER, &ier))
    return 1;
  ier = ier & (~IER_RDAI | ~IER_THREI | ~IER_RLSI);
  if (sp_write(IER, ier))
    return 1;

  // DISABLE FIFO AND CLEAR TRANSMITTER AND RECEIVER FIFO
  uint8_t fcr;
  if (sp_read(FCR, &fcr))
    return 1;
  fcr = fcr & (~FCR_ENABLE_FIFO | ~FCR_CLEAR_RCVR_FIFO | ~FCR_CLEAR_XMIT_FIFO);
  if (sp_write(FCR, fcr))
    return 1;

  free(sendQueue->values);
  free(receiveQueue->values);
  free(standByQueue->values);
  free(sendQueue);
  free(receiveQueue);
  free(standByQueue);

  return 0;
}

int(sp_subscribe)(uint8_t *bit_no) {

  if (sp_init())
    return 1;

  if (bit_no == NULL)
    return 1;

  *bit_no = BIT(sp_hookId);

  if (sys_irqsetpolicy(COM1_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &sp_hookId) != 0) {
    printf("Error: could not subscribe SP interruption\n");
    return 1;
  }
  sp_ih();

  return 0;
}

int(sp_unsubscribe)() {
  if (sp_end())
    return 1;

  if (sys_irqrmpolicy(&sp_hookId) != 0) {
    return 3;
  }

  return 0;
}

int sp_clear() {
  uint8_t fcr;
  if (sp_read(FCR, &fcr))
    return 1;
  fcr |= (FCR_CLEAR_RCVR_FIFO | FCR_CLEAR_XMIT_FIFO | FCR_ENABLE_FIFO);
  if (sp_write(FCR, fcr))
    return 1;
  while (queue_pop(receiveQueue) != 0);
  return 0;
}

int send_queue_data() { // send stored info in queue

  if (queue_isEmpty(sendQueue)) {
    printf("queue is empty\n");
    transmitter_empty = 1;
    return 1;
  }

  uint8_t lsr;

  while (!queue_isEmpty(sendQueue)) {
    sp_write(THR, queue_front(sendQueue));
    queue_pop(sendQueue);
    sp_read(LSR, &lsr);
    transmitter_empty = lsr & LSR_TRANSMITTER_HOLD_EMPTY;
    if (!transmitter_empty)
      return 1;
  }
  return 0;
}

int send_SP_data(uint8_t data) {
  printf("send_byte %x\n", data);
  queue_push(sendQueue, data);

  if (transmitter_empty) {
    return send_queue_data();
  }
  else
    return 1;
}

int establish_connection() {
  bool connection = false;
  uint8_t frontByte = queue_front(receiveQueue);
  if (frontByte == TRYSYNC) {
    send_SP_data(FIRSTSYNC);
  }
  else if (frontByte == FIRSTSYNC) {
    connection = true;
    player = true;
    send_SP_data(SECONDSYNC);
  }
  else if (frontByte == SECONDSYNC) { // 1 more?? dizer que tbm entrou
    player = false;
    connection = true;
  }
  else if (!sendTrySync) {
    send_SP_data(TRYSYNC);
    sendTrySync = true;
  }

  queue_pop(receiveQueue);
  return connection;
}

int receive_SP_data() {
  uint8_t lsr, rbr;
  sp_read(LSR, &lsr);

  if (lsr & LSR_RECEIVER_DATA) {
    sp_read(RBR, &rbr);

    int lsrError = lsr & (LSR_OVERRUN_ERROR | LSR_PARITY_ERROR | LSR_FRAMING_ERROR);

    if (!(lsrError)) {
      queue_push(receiveQueue, rbr);
      printf("read_byte %x\n", rbr);
      return 0;
    }
  }
  return 1;
}

void sp_ih() {
  uint8_t interrupt;
  sp_read(IIR, &interrupt);
  while (!(interrupt & IIR_PENDING)) { // Interrupt Pending
    if (interrupt & IIR_DATA_AVAILABLE) {
      printf("data available\n");
      while (!receive_SP_data());
      sp_read(IIR, &interrupt);
    }
    if (interrupt & IIR_TRANSMIT_HOLD_EMPTY) {
      printf("transmit hold empty\n");
      send_queue_data();
      sp_read(IIR, &interrupt);
    }
  }
}

void sendMouseData(Mouse *mouse) {
  uint8_t dataType = MOUSE_DATA;
  uint8_t xFirst = (uint16_t) mouse->pos.x & 0x00FF;
  uint8_t xSecond = ((uint16_t) mouse->pos.x & 0xFF00) >> 8;
  uint8_t yFirst = (uint16_t) mouse->pos.y & 0x00FF;
  uint8_t ySecond = ((uint16_t) mouse->pos.y & 0xFF00) >> 8;

  send_SP_data(dataType);
  send_SP_data(xFirst);
  send_SP_data(xSecond);
  send_SP_data(yFirst);
  send_SP_data(ySecond);
}

void sendShotData(Cue *cue) {
  uint8_t dataType = SHOT_DATA;

  FloatUnion charge = cue->charge;

  send_SP_data(dataType);
  send_SP_data(charge[0]);
  send_SP_data(charge[1]);
  send_SP_data(charge[2]);
  send_SP_data(charge[3]);

  FloatUnion sideSpin = cue->sideEnglish;

  send_SP_data(dataType);
  send_SP_data(sideSpin[0]);
  send_SP_data(sideSpin[1]);
  send_SP_data(sideSpin[2]);
  send_SP_data(sideSpin[3]);

  FloatUnion verticalSpin = cue->verticalEnglish;

  send_SP_data(dataType);
  send_SP_data(verticalSpin[0]);
  send_SP_data(verticalSpin[1]);
  send_SP_data(verticalSpin[2]);
  send_SP_data(verticalSpin[3]);
}


void sendCueBallData(vector_t ballPos){
    uint8_t dataType = BALL_DATA;
    uint8_t xFirst = (uint16_t) ballPos.x & 0x00FF;
    uint8_t xSecond = ((uint16_t) ballPos.x & 0xFF00) >> 8;
    uint8_t yFirst = (uint16_t) ballPos.y & 0x00FF;
    uint8_t ySecond = ((uint16_t) ballPos.y & 0xFF00) >> 8;

    send_SP_data(dataType);
    send_SP_data(xFirst);
    send_SP_data(xSecond);
    send_SP_data(yFirst);
    send_SP_data(ySecond);
}



void resetMultiplayer() {
  sendTrySync = false;
  transmitter_empty = 1;
  while (queue_pop(sendQueue) != 0);
  while (queue_pop(receiveQueue) != 0);
}

bool player1() {
  return player;
}

int handleMultiplayerData(Table *table) { // receive
  if (queue_isEmpty(receiveQueue))
    return;
  while (!queue_isEmpty(receiveQueue)) {
    uint8_t byte = pop(receiveQueue);

    // CHANGE
    if (!queue_isEmpty(standByQueue)) {
      queue_push(standByQueue, byte);

      switch (currentDataType) {

        case MOUSE_DATA:
          if (standByQueue->size == 4)
            processMouseData(standByQueue);
          break;
        case SHOT_DATA:
          if (standByQueue->size == 12)
            processShotData(standByQueue);
          break;
        case BALL_DATA:
          if (standByQueue->size == 4)
            processBallData(standByQueue);
          break;
        default:
          printf("DataType not found\n");
          queue_clear(standByQueue);
          break;
      }
    }

    currentDataType = byte;
    queue_push(standByQueue, byte);
  }
}

int processMouseData(Queue *queue, Table *table) {

  uint8_t dataType = queue_pop(queue);
  if (dataType != MOUSE_DATA)
    return 1;

  uint16_t x1 = queue_pop(queue);
  uint16_t x2 = queue_pop(queue);
  uint16_t y1 = queue_pop(queue);
  uint16_t y2 = queue_pop(queue);

  uint16_t posX = (x2 << 8) | x1;
  uint16_t posY = (y2 << 8) | y1;
  vector_t position = {posX, posY};

  table->mouse->pos = position;
  updateCueState(table, false);
}

int processShotData(Queue *queue, Table *table) {

  uint8_t dataType = queue_pop(queue);
  if (dataType != SHOT_DATA)
    return 1;

  FloatUnion charge;

  charge.bytes[0] = queue_pop(queue);
  charge.bytes[1] = queue_pop(queue);
  charge.bytes[2] = queue_pop(queue);
  charge.bytes[3] = queue_pop(queue);

  table->cue->charge = charge.f;


  FloatUnion sideSpin;

  sideSpin.bytes[0] = queue_pop(queue);
  sideSpin.bytes[1] = queue_pop(queue);
  sideSpin.bytes[2] = queue_pop(queue);
  sideSpin.bytes[3] = queue_pop(queue);

  table->cue->sideEnglish = sideSpin.f;

  FloatUnion verticalSpin;

  verticalSpin.bytes[0] = queue_pop(queue);
  verticalSpin.bytes[1] = queue_pop(queue);
  verticalSpin.bytes[2] = queue_pop(queue);
  verticalSpin.bytes[3] = queue_pop(queue);

  table->cue->verticalEnglish = verticalSpin.f;

  startSimulation(table);
}

int processBallData(Queue *queue, Table* table){

  uint8_t dataType = queue_pop(queue);
  if (dataType != BALL_DATA)
    return 1;

  uint16_t x1 = queue_pop(queue);
  uint16_t x2 = queue_pop(queue);
  uint16_t y1 = queue_pop(queue);
  uint16_t y2 = queue_pop(queue);

  uint16_t posX = (x2 << 8) | x1;
  uint16_t posY = (y2 << 8) | y1;
  vector_t position = {posX, posY};

  table->balls[0]->position = position;
}

