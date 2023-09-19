/*
#include <TimerOne.h>

DriverEncoder::DriverEncoder(uint8_t A, uint8_t B, uint8_t BTN, void (*funcHandClick)){
  encoder = new ClickEncoder(A,B,BTN);
  handClick = funcHandClick;
}
void DriverEncoder::timerIsr(){
  encoder->service();
}
void DriverEncoder::begin(){
  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);
}

int16_t DriverEncoder::getValue(){
  int16_t value = encoder->getValue();
  return value;
}*/