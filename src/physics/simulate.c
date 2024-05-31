#include "simulate.h"
#include "evolve.h"
#include "resolve/resolver.h"
#include "events.h"



void processShot(Table* table){
  Event shotEvent = {0, STICK_BALL, table->balls[0], NULL,NULL, -1};
  table->nextEvent = shotEvent;
}

bool updatePhysics(Table* table, double dt){

  Event nextEvent = table->nextEvent;
  if (nextEvent.time != 0){
    evolveBalls(table, nextEvent.time);
  }
  resolveEvent(table, nextEvent);

  double elapsed = nextEvent.time;
  while (true){
    Event event = getNextEvent(table);

    if (event.type == INVALID){
      return false;
    }

    if (elapsed + event.time < dt){
      // Event still happens before dt passed
      evolveBalls(table, event.time);
      resolveEvent(table, event);
      elapsed+=event.time;
    }else{
      evolveBalls(table, dt-elapsed);
      event.time -= dt - elapsed;
      table->nextEvent = event;
      break;
    }

  }
  return true;
} 



void evolveBalls(Table *table, double time) {
  for (size_t i = 0; i < table->ballNumber; i++) {
    evolveBallMotion(table, table->balls[i], time);
  }
}
