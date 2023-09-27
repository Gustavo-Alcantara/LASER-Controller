const int16_t DAC_12BIT_MAX = 4095;
const float DAC_INPUT_RANGE = 5.0;
const float DAC_BIT_VOLTAGE = DAC_INPUT_RANGE / (DAC_12BIT_MAX + 1);

void writeOnDAC(Adafruit_MCP4725 dac, float vout)
{
  int dac_bits = min(int((vout/DAC_INPUT_RANGE)*DAC_12BIT_MAX), DAC_12BIT_MAX); 
  dac.setVoltage(dac_bits,false);
}