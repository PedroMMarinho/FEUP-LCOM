#include "button.h"

Button* newButton(xpm_image_t img, xpm_image_t imgSelected, vector_t position, vector_t size){
  Button* button = (Button*)malloc(sizeof(Button));
  button->img = img;
  button->imgSelected = imgSelected;
  button->position = position;
  button->size = size;
  button->selected = false;
  return button;
}
void destroyButton(Button* button){
  free(button);
}
bool isMouseOverButton(Button* button, vector_t mousePos){
  return mousePos.x >= button->position.x && mousePos.x <= button->position.x + button->size.x && mousePos.y >= button->position.y && mousePos.y <= button->position.y + button->size.y;
}