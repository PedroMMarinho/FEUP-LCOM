#include <lcom/lcf.h>
#include "vector.h"

typedef struct Button {
  xpm_image_t img;
  xpm_image_t imgSelected;
  vector_t pos;
  vector_t size;
  bool selected;
} Button;


Button* newButton(xpm_image_t img, xpm_image_t imgSelected, vector_t position, vector_t size);
void destroyButton(Button* button);
bool isMouseOverButton(Button* button, vector_t mousePos);





