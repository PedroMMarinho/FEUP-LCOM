

#include "resources.h"
#include "model/table.h"
#include "model/ball/ball.h"

Resources* loadResources(){

  Resources* resources = (Resources*)malloc(sizeof(Resources));
  resources->table = newTable();
  resources->state = PLAYING;
  return resources;
}

void destroyResources(Resources* resources){
  destroyTable(resources->table);
  free(resources->table);
  free(resources);
}
