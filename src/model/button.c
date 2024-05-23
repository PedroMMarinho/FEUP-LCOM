#include "button.h"

Button* newButton(xpm_image_t img, xpm_image_t imgSelected, vector_t pos, vector_t size){
  Button* button = (Button*)malloc(sizeof(Button));
  button->img = img;
  button->imgSelected = imgSelected;
  button->pos = pos;
  button->size = size;
  button->selected = false;
  return button;
}
void destroyButton(Button* button){
  free(button);
}
bool isMouseOverButton(Button* button, vector_t mousePos){
  return mousePos.x >= button->pos.x && mousePos.x <= button->pos.x + button->size.x && mousePos.y >= button->pos.y && mousePos.y <= button->pos.y + button->size.y;
}