#pragma once
#include "lcom/lcf.h"
#include "vector.h"


typedef struct Cue{

  vector_t position;
  double angle;
  vector_t directionVector;
  xpm_image_t img;
  float charge;
  float powerSensitivity;
  uint8_t maximumDistance;
  uint8_t minimalDistance;

  // Info about the guide lines
  vector_t colisionPoint;
  vector_t targetBallCenter;
  vector_t targetBallVec;
  vector_t whiteBallVec;
  double guideScalar;
  uint8_t guideLineMaximumSize;
  xpm_image_t guideImg;
  bool colides;
}Cue;


Cue* newCue();

void destroyCue(Cue* cue);

Cue* updateCueCharge(Cue* cue, vector_t mouseDelta);

