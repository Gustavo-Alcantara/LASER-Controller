#include <ClickEncoder.h>
#include <TimerOne.h>

class DriverEncoder{
  public:
    DriverEncoder(uint8_t A, uint8_t B, uint8_t BTN, void (*funcHandClick));
    int16_t getValue();
    void begin();

  private:
    ClickEncoder *encoder;
    void (*handClick);
    void timerIsr();


};