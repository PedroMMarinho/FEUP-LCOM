#include "cueViewer.h"
#include "../model/vector.h"
#include "../labs/graphics.h"
#include "xpms/cue.xpm"
#include "lineViewer.h"
#include "math.h"
#include "xpms/ball.xpm"
#include "xpms/ballRing.xpm"

static uint8_t minimalDistance = 20;
static uint8_t maximalDistance = 100;

static int distance = 20;
static float powerSensitivity = 0.5;
static int guideLinesMaximumSize = 50;


int drawCueCharged(vector_t ball, vector_t origin, int16_t deltaX, int16_t deltaY){

  double dX = ball.x - origin.x;
  double dY = origin.y - ball.y;
  double magnitude = sqrt(dX*dX + dY*dY);
  dX /= magnitude;
  dY /= magnitude;

  // Calculate the dot product 
  double charge = dX*deltaX + dY * deltaY;
  distance += charge * powerSensitivity;
  distance = distance > (int8_t)maximalDistance ? maximalDistance : distance;
  distance = distance < (int8_t)minimalDistance ? minimalDistance : distance;


  double angle = atan2(ball.y-origin.y, ball.x-origin.x);

  double cueX = cos(angle) * (212 + distance);
  double cueY = sin(angle) * (212 + distance);

  xpm_image_t cueImage;
  xpm_load(cue, XPM_8_8_8, &cueImage);
  if (drawXPMImage(cueImage, ball.x + cueX, ball.y + cueY, -angle + M_PI)) return 1;
  return 0;
}

int drawCue(vector_t ball, vector_t mouse){


  distance = 20;
  double dX = mouse.x - ball.x;
  double dY = mouse.y - ball.y;
  double magnitude = sqrt(dX*dX + dY*dY);
  dX = dX / magnitude;
  dY = dY / magnitude;


  double angle = atan2(ball.y-mouse.y, ball.x-mouse.x);

  double cueX = cos(angle) * (212 + minimalDistance);
  double cueY = sin(angle) * (212 + minimalDistance);

  // Do the colision logic of line with ball 
  uint16_t targetX = 400;
  uint16_t targetY = 400;
  uint16_t ballRadius = 40;

  xpm_image_t ballImage;
  xpm_load(ballXpm, XPM_8_8_8, &ballImage);
  drawXPMImage(ballImage, targetX, targetY , 0);

  vector_t s = {ball.x - targetX, ball.y - targetY};
  double b = s.x * dX + s.y * dY;
  double c = s.x*s.x + s.y*s.y - ballRadius * ballRadius;
  double h = b * b -c;
  if (h < 0){
    drawThickLine(ball.x, ball.y,ball.x+dX*500, ball.y+dY*500, 3, 0xffffff);
  }else{
    h = sqrt(h);
    double t = 0-b-h;
    if (t < 0)
     drawThickLine(ball.x, ball.y,ball.x+dX*500, ball.y+dY*500, 3, 0xffffff);
    else{
      // There is collision
      vector_t colision = {ball.x + dX * t,ball.y + dY * t};
      drawThickLine(ball.x, ball.y, colision.x, colision.y , 3, 0xffffff);

      
      xpm_image_t ballRingImage;
      xpm_load(ballRingXpm, XPM_8_8_8, &ballRingImage);
      drawXPMImage(ballRingImage, colision.x, colision.y , 0);

      // Draw lines coming out of the target ball
      vector_t ballCenterVector = {targetX-colision.x, targetY-colision.y};
      double magnitude = sqrt(ballCenterVector.x*ballCenterVector.x + ballCenterVector.y*ballCenterVector.y);
      ballCenterVector.x /= magnitude;
      ballCenterVector.y /= magnitude;

      double targetLineScalar = ballCenterVector.x * dX + ballCenterVector.y * dY;
      double whiteBallScalar = 1 - targetLineScalar;
      targetLineScalar *= guideLinesMaximumSize;
      whiteBallScalar *= guideLinesMaximumSize;
      // Draw target line
      drawThickLine(targetX, targetY, targetX + targetLineScalar * ballCenterVector.x,targetY + targetLineScalar * ballCenterVector.y, 3, 0x00ff00);
      // Draw whtie ball line
      vector_t whiteBallVector;
      if (dY > ballCenterVector.y || dX < ballCenterVector.x){
        whiteBallVector.x = ballCenterVector.y;
        whiteBallVector.y = -ballCenterVector.x;
      }else{
        whiteBallVector.x = -ballCenterVector.y;
        whiteBallVector.y = ballCenterVector.x;
      }
      drawThickLine(colision.x, colision.y, colision.x + whiteBallVector.x * whiteBallScalar , colision.y + whiteBallVector.y * whiteBallScalar , 3, 0x00ff00);
      
    }
  }
  

  drawThickLine(ball.x, ball.y,ball.x+dX, ball.y+dY, 3, 0xffffff);
  
      xpm_image_t cueImage;
      xpm_load(cue, XPM_8_8_8, &cueImage);
  if (drawXPMImage(cueImage, ball.x + cueX, ball.y + cueY, -angle + M_PI)) return 1;
  return 0;
}


// int drawLine(position_t start, position_t end){

//   uint16_t x = start.x;
//   uint16_t y = start.y;
  
//   int dx = end.x - start.x;
//   int dy = end.y - start.y;

//   int32_t d = 2 * dy - dx;

//   double D = 0;


//   double length = sqrt(dx * dx + dy * dy); 

//   double sin = dx / length;     
//   double cos = dy / length;
//   while (x <= end.x) {
//     draw_pixel(x, y, 0xffffff);
//       // IntensifyPixels(x, y - 1, D + cos);
//       // IntensifyPixels(x, y, D);
//       // IntensifyPixels(x, y + 1, D - cos);
//       x = x + 1;
//       if (d <= 0) {
//           D = D + sin;
//           d = d + 2 * dy;
//       } else {
//           D = D + sin - cos;
//           d = d + 2 * (dy - dx);
//           y = y + 1;
//       }
//   }

//   return 0;
// }
