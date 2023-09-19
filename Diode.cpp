#include "Diode.h"

Diodo::Diodo(char* name, float maxCurrent, float minCurrent, float maxTemp, float minTemp, float currentValue, float tempValue) {
    this->name = name;
    this->maxCurrent = maxCurrent;
    this->minCurrent = minCurrent;
    this->maxTemp = maxTemp;
    this->minTemp = minTemp;
    this->currentValue = currentValue;
    this->tempValue = tempValue;
}
char* Diodo::getName() {
    return this->name;
}
float Diodo::getMaxCurrentValue() {
    return this->maxCurrent;
} 
float Diodo::getMinCurrentValue() {
    return this->minCurrent;
} 
float Diodo::getMaxTempValue() {
    return this->maxTemp;
}

float Diodo::getMinTempValue() {
    return this->minTemp;
}

float Diodo::getCurrentValue() {
    return this->currentValue;
}

float Diodo::getTempValue() {
    return this->tempValue;
}

float Diodo::setCurrentValue(float currentValue) {
    this->currentValue = currentValue;
}

float Diodo::setTempValue(float tempValue) {
    this->tempValue = tempValue;
}
