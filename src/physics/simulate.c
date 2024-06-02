#include "simulate.h"
#include "events.h"
#include "evolve.h"
#include "resolve/resolver.h"
#include "utilities.h"

void processShot(Table *table) {
  printf("Shot processed\n");
  Event shotEvent = {0, STICK_BALL, table->balls[0], NULL, NULL,NULL,-1};
  table->nextEvent = shotEvent;
}

bool updatePhysics(Table *table, double dt) {

  Event nextEvent = table->nextEvent;
  double elapsed = 0;
  while (nextEvent.type != INVALID) {

    if (elapsed + nextEvent.time < dt) {
      // Event still happens before dt passed
      evolveBalls(table, nextEvent.time);
      resolveEvent(table, nextEvent);
      elapsed += nextEvent.time;
    }
    else {
      evolveBalls(table, dt - elapsed);
      nextEvent.time -= dt - elapsed;
      table->nextEvent = nextEvent;
      return true;
    }
    printf("GETTING NEXT EVENT IN SAME UPDATE!!!!\n");
    nextEvent = getNextEvent(table);
    table->nextEvent = nextEvent;

  }
  printf("FINAL EVENT - SIMULATION TERMINATED\n");
  return false;

  printf("Simulation physcis\n");

  if (nextEvent.time != 0) {
    evolveBalls(table, nextEvent.time);
  }
  resolveEvent(table, nextEvent);

  printf("Ball velocity!");
  printFloat(table->balls[0]->velocity.x);
  printFloat(table->balls[0]->velocity.y);

  // double elapsed = nextEvent.time;
  while (true) {

    printf("Before getNextEvent\n");
    Event event = getNextEvent(table);

    printf("NEXT EVENT FOUND\n");
    printEvent(&event);

    if (event.type == INVALID) {
      return false;
    }

    if (elapsed + event.time < dt) {
      printf("NEED TO FIND ANOTHER\n");
      // Event still happens before dt passed
      evolveBalls(table, event.time);
      resolveEvent(table, event);
      elapsed += event.time;
    }
    else {
      printf("Time to cook\n");
      evolveBalls(table, dt - elapsed);
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
