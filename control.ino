#define CONVERTER_DEFS_H
#include <Arduino.h>
#include <Adafruit_MCP4725.h>
#include <Adafruit_ADS1X15.h>

#define ADC_ADDR 0x48  // ADDR pin = GND

#define I_DAC_ADDR 0x60
#define T_DAC_ADDR 0x61

#define RT0 1000   // Ω
#define B 3460      // K
//--------------------------------------
#define R 9830  //R=10KΩ

const int8_t ADC_I_CH = 2;
const int8_t ADC_T_CH = 1;
const int8_t ADC_ADDR_PIN = 7; // Note: this might not matter

const int16_t DAC_12BIT_MAX = 4095;
const int32_t ADC_16BIT_MAX = 65535;

const float DAC_INPUT_RANGE = 5.0;
const float DAC_BIT_VOLTAGE = DAC_INPUT_RANGE / (DAC_12BIT_MAX + 1);
const float T_DAC_X0 = 2458;  // 3.0V on CTLI port
const float I_DAC_X0 = 0;

const float ADC_INPUT_RANGE = 6.144;
const double ADC_BIT_VOLTAGE = 2*ADC_INPUT_RANGE / (ADC_16BIT_MAX);  // V/bit

// I reading constant
const float I_SENSE_GAIN = 1000*((10.0/100.0)/1.8); // V -> mA

const float VCTLI_OFFSET = 3.0;
const float MAX_VCTLI = 2.0;
const float MIN_VCTLI = -2.0;

float Vin;

// Current control parameters
const float IKc = 0.01;
const float Iz = 0.7;

// State variables
float Ie_km1, Iu_km1;

const float VCC = 5.0;
double RT, VR, ln, TX, VRT;
const double T0 = 25.0 + 273.15;
const double RX = RT0 * exp(-B/T0);

float VCTLI;

// Temperature control parameters
const float TKc = 10;
const float Tz = 0.8;

// State variables
float Te_km1, Tu_km1;

Adafruit_ADS1115 adc;

Adafruit_MCP4725 Tdac;
Adafruit_MCP4725 Idac;


void writeOnDAC(Adafruit_MCP4725 dac, float vout)
{
  int dac_bits = min(int((vout/DAC_INPUT_RANGE)*DAC_12BIT_MAX), DAC_12BIT_MAX); 
  dac.setVoltage(dac_bits,false);
}

void initConverters() {
    // A/D converter initialization
    pinMode(ADC_ADDR_PIN, OUTPUT);
    digitalWrite(ADC_ADDR_PIN, LOW);
    adc.setGain(GAIN_TWOTHIRDS);  // +/- 6.144V -> 1 bit = 0.1875mV
    adc.begin(ADC_ADDR);

    // D/A converters address initialization
    Tdac.begin(T_DAC_ADDR);
    Idac.begin(I_DAC_ADDR); 

    // D/A converters output initialization
    Tdac.setVoltage(T_DAC_X0, false);
    Idac.setVoltage(I_DAC_X0, false);
}
float getCurrent() {
    int16_t intI = adc.readADC_SingleEnded(ADC_I_CH);
    float vI = intI * ADC_BIT_VOLTAGE;
    float I = vI * I_SENSE_GAIN + 2;
    return I;
}

float computeIControl(float IRef) {
  // Compute control
  float I = getCurrent(); 
  float Ie_k = IRef - I;
  float Iu_k = Iu_km1 + IKc * Ie_k - IKc * Iz * Ie_km1;

  // Saturation
  Iu_k = max(Iu_k, 0.0);
  Iu_k = min(Iu_k, DAC_INPUT_RANGE);
  // Output value update
  Vin = Iu_k;
  writeVin();
  // State variables update
  Iu_km1 = Iu_k;
  Ie_km1 = Ie_k;

  writeVin();

  return Vin;
}
void writeVin() {
  float x = Vin / DAC_BIT_VOLTAGE;
  //float VADC_bits = IRef_raw*4.00293255132;
  x = min(DAC_12BIT_MAX, x);
  //Vin = x * 5.0 / DAC_12BIT_MAX;
  Idac.setVoltage(x, false);
}
double getTemperature() {
  int16_t intT = adc.readADC_SingleEnded(ADC_T_CH);
  VR = intT * ADC_BIT_VOLTAGE;     //Conversion to voltage
  VRT = VCC - VR;
  // Determina a resistência do termistor
    
  RT = (VCC*R)/VR - R;
 
  // Calcula a temperatura
  double T = B / log(RT/RX);
  T = T - 273.15;

  return T;
}

float computeTControl(float TRef) {
  float T = getTemperature(); 
  // Compute control
  float Te_k = T - TRef;
  float Tu_k = Tu_km1 + TKc * Te_k - TKc * Tz * Te_km1;

 Tu_k = Tu_km1 + TKc * Te_k;

  // Offset and saturation
  Tu_k = max(Tu_k, MIN_VCTLI);
  Tu_k = min(Tu_k, MAX_VCTLI);

  // Output value update
  VCTLI = Tu_k;
  writeCTLI();
  // State variables update
  Tu_km1 = Tu_k;
  Te_km1 = Te_k;
  return VCTLI;
}

void writeCTLI() {
  float x = (VCTLI + VCTLI_OFFSET) / DAC_BIT_VOLTAGE;
  x = min(DAC_12BIT_MAX, x);
  Tdac.setVoltage(x, false);
}