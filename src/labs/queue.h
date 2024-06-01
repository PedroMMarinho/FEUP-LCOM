#ifndef __QUEUE_H_
#define __QUEUE_H_

#define QUEUESIZE 128

#include <lcom/lcf.h>


typedef struct {
    int front, back, size;
    unsigned capacity;
    uint8_t* values;
}Queue;

Queue* new_queue();

bool queue_push(Queue* queue, uint8_t value);

uint8_t queue_pop(Queue* queue);

uint8_t queue_front(Queue* queue);

uint8_t queue_back(Queue* queue);

bool queue_isEmpty(Queue* queue);

bool queue_isFull(Queue* queue);

void queue_clear(Queue* queue);

#endif
