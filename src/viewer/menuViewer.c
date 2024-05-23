#include "menuViewer.h"
#include "../labs/graphics.h"
#include "../model/vector.h"
#include "../model/button.h"
#include "../model/mouseModel.h"
void drawMenuBackground(xpm_image_t img){
  drawXPMImage(img,0,0,0);
}
void drawMenuButton(Button* button){
    if(button->selected){
        drawXPMImage(button->imgSelected,button->pos.x,button->pos.y,0);
        
    }
    else{
        drawXPMImage(button->img,button->pos.x,button->pos.y,0);
    }
}
void drawMenuMouse(Mouse* mouse, xpm_image_t mouseImg, xpm_image_t imgMouseHover, bool isHovering){
    if(isHovering){
        drawXPMImage(mouseImgHover,mouse->pos.x,mouse->pos.y,0);
        
    }
    else{
        drawXPMImage(mouseImg,mouse->pos.x,mouse->pos.y,0);
    }
}
    