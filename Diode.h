class Diodo {
  private:
    char* name;
    float maxCurrent, minCurrent, maxTemp, minTemp, currentValue, tempValue;

  public:
    Diodo(char* name, float maxCurrent, float minCurrent, float maxTemp, float minTemp, float currentValue, float tempValue);
    char* getName();
    float getMaxCurrentValue();
    float getMinCurrentValue();
    float getMaxTempValue();
    float getMinTempValue();
    float getCurrentValue();
    float getTempValue();
    float setCurrentValue(float currentValue);
    float setTempValue(float tempValue);
};