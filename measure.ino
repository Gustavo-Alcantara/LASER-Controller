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
const int8_t ADC_ADDR_PIN = 17; // Note: this might not matter

const float T_DAC_X0 = 2458;  // 3.0V on CTLI port
const float I_DAC_X0 = 0;

const int32_t ADC_16BIT_MAX = 65535;
const float ADC_INPUT_RANGE = 6.144;
const double ADC_BIT_VOLTAGE = 2*ADC_INPUT_RANGE / (ADC_16BIT_MAX);  // V/bit

// I reading constant
const float I_SENSE_GAIN = 1000*((10.0/100.0)/1.8); // V -> mA

const float VCTLI_OFFSET = 3.0;
const float MAX_VCTLI = 2.0;
const float MIN_VCTLI = -2.0;


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

float Vin;

// Current control parameters
const float IKc = 0.01;
const float Iz = 0.7;

// State variables
float Ie_km1, Iu_km1;

Adafruit_ADS1115 adc;




void initConverters() {
    // A/D converter initialization
    pinMode(ADC_ADDR_PIN, OUTPUT);
    digitalWrite(ADC_ADDR_PIN, LOW);
    adc.setGain(GAIN_TWOTHIRDS);  // +/- 6.144V -> 1 bit = 0.1875mV
    if(!adc.begin(ADC_ADDR))
      while(1);

    // D/A converters address initialization
    Tdac.begin(0x60);
    Idac.begin(0x61); 

    Serial.println(getTemperature());
    // D/A converters output initialization
    Tdac.setVoltage(0, false);
    Idac.setVoltage(0, false);
}

void updateMeasurement(){
  //encoder->service();
  //currentValue = getCurrent();
  //tempValue = getTemperature();
  writeOnDAC(Tdac, 5);
}

float getCurrent() {
    int16_t intI = adc.readADC_SingleEnded(ADC_I_CH);
    float vI = intI * ADC_BIT_VOLTAGE;
    float I = vI * I_SENSE_GAIN + 2;
    return I;
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