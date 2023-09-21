#include <Arduino.h>
#include <ClickEncoder.h>
#include <TimerOne.h>
#include <LCDWIKI_KBV.h>
#include "System.h"
#include "Diode.h"


ClickEncoder *encoder; 


System lasersystem = System();
Diodo selectedDiode = Diodo("L785P090", 165.0, 0.0, 70.0, -10.0, 125.0, 23.0);
System::State currentState;

int16_t value;
int16_t last;
float currentValue = 0;
float tempValue = 0;

Diodo diodoList[] = {
  Diodo("L785P090", 165.0, 0.0, 70.0, -10.0, 125.0, 23.0),
  Diodo("L780P010", 40.0, 0.0, 60.0, -10.0, 24.0, 23.0),
  Diodo("L840P200", 340.0, 0.0, 60.0, -10.0, 255.0, 23.0),
  Diodo("DL7140-201S", 140.0, 0.0, 60.0, -10.0, 100.0, 23.0)
};


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  initDisplay();

  encoder = new ClickEncoder(12, 13, 11); //DT,CLK,SW
  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);
  Timer1.attachInterrupt(controlLoop)

  last=-1;
}

void loop() {
  // put your main code here, to run repeatedly:
  currentState = lasersystem.getState();

  switch(currentState){

    case System::DiodeSelecting:
      diodeSelecting();
      break;
    case System::CurrentCoarseSetting:
      currentCoarseSetting();
      break;
    case System::CurrentFineSetting:
      currentFineSetting();
      break;
    case System::TempCoarseSetting:
      tempCoarseSetting();
      break;
    case System::TempFineSetting:
      tempFineSetting();
      break;
  }
  drawInterface();
}
void timerIsr() {
  encoder->service();
}
void diodeSelecting(){
  value += encoder->getValue();
  if (value != last) {
    last = value;
    if (value >= 4)
      value =0;
    else if(value < 0)
      value = 3;
    selectedDiode = diodoList[value];
  }
  ClickEncoder::Button_e buttonState = encoder->getButton();
  if(buttonState != ClickEncoder::Open){
    lasersystem.setState(System::CurrentCoarseSetting);
    selectedDiode = diodoList[value];
    value = 0;
  }
}
void currentCoarseSetting(){
  value += 3*encoder->getValue();
  currentValue = selectedDiode.getMinCurrentValue();
  currentValue = map(value,1,100, selectedDiode.getMinCurrentValue(), selectedDiode.getMaxCurrentValue());
  if(currentValue >= selectedDiode.getMaxCurrentValue()){
    currentValue = selectedDiode.getMaxCurrentValue();
  }
  else if(currentValue <= selectedDiode.getMinCurrentValue()){
    currentValue = selectedDiode.getMinCurrentValue();
  }
  if(value != last){
    last = value;
  }
  ClickEncoder::Button_e buttonState = encoder->getButton();
  if(buttonState != ClickEncoder::Open){
    currentValue = currentValue;
    lasersystem.setState(System::CurrentFineSetting);

  }
}
void currentFineSetting(){
  int16_t encoderinstantvalue = encoder->getValue();
  value += encoderinstantvalue;
  currentValue += encoderinstantvalue * 0.1;
  if(currentValue >= selectedDiode.getMaxCurrentValue()){
    currentValue = selectedDiode.getMaxCurrentValue();
  }
  else if(currentValue <= selectedDiode.getMinCurrentValue()){
    currentValue = selectedDiode.getMinCurrentValue();
  }
  if(value != last){
    last = value;
  }
  ClickEncoder::Button_e buttonState = encoder->getButton();
  if(buttonState != ClickEncoder::Open){
    lasersystem.setState(System::TempCoarseSetting);
  }
}
void tempCoarseSetting(){
  value += encoder->getValue();
  tempValue = selectedDiode.getMinTempValue();
  tempValue = map(value,1,100, selectedDiode.getMinTempValue(), selectedDiode.getMaxTempValue());
  if(tempValue >= selectedDiode.getMaxTempValue()){
    tempValue = selectedDiode.getMaxTempValue();
  }
  else if(tempValue < selectedDiode.getMinTempValue()){
    tempValue = selectedDiode.getMinTempValue();
  }
  if(value != last){
    last = value;
  }
  ClickEncoder::Button_e buttonState = encoder->getButton();
  if(buttonState != ClickEncoder::Open){
    lasersystem.setState(System::TempFineSetting);
  }
}
void tempFineSetting(){
  int16_t encoderinstantvalue = encoder->getValue();
  value += encoderinstantvalue;
  tempValue += encoderinstantvalue * 0.1;
  if(tempValue >= selectedDiode.getMaxTempValue()){
    tempValue = selectedDiode.getMaxTempValue();
  }
  else if(tempValue <= selectedDiode.getMinTempValue()){
    tempValue = selectedDiode.getMinTempValue();
  }
  if(value != last){
    last = value;
  }
  ClickEncoder::Button_e buttonState = encoder->getButton();
  if(buttonState != ClickEncoder::Open){
    lasersystem.setState(System::DiodeSelecting);
  }
}