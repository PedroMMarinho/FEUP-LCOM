#include "simulate.h"
#include "evolve.h"
#include "resolve/resolver.h"
#include "events.h"
#include "utilities.h"



void processShot(Table* table){
  printf("Shot processed\n");
  Event shotEvent = {0, STICK_BALL, table->balls[0], NULL,NULL, -1};
  table->nextEvent = shotEvent;
}

bool updatePhysics(Table* table, double dt){

  printf("Simulation physcis\n");

  Event nextEvent = table->nextEvent;
  if (nextEvent.time != 0){
    evolveBalls(table, nextEvent.time);
  }
  resolveEvent(table, nextEvent);

  printf("Ball velocity!");
  printFloat(table->balls[0]->velocity.x);
  printFloat(table->balls[0]->velocity.y);

  double elapsed = nextEvent.time;
  while (true){

    printf("Before getNextEvent\n");
    Event event = getNextEvent(table);

    printf("NEXT EVENT FOUND\n");
    printEvent(&event);


    if (event.type == INVALID){
      return false;
    }

    if (elapsed + event.time < dt){
      printf("NEED TO FIND ANOTHER\n");
      return false;
      // Event still happens before dt passed
      evolveBalls(table, event.time);
      resolveEvent(table, event);
      elapsed+=event.time;
    }else{
      printf("Time to cook\n");
      return false;
      evolveBalls(table, dt-elapsed);
      event.time -= dt - elapsed;
      table->nextEvent = event;
      break;
    }

    return false;

  }
  return true;
} 



void evolveBalls(Table *table, double time) {
  printf("EVOLVING BALLS\n");
  for (size_t i = 0; i < table->ballNumber; i++) {
    evolveBallMotion(table, table->balls[i], time);
  }
}
