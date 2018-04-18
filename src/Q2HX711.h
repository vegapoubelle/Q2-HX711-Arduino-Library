#ifndef Q2HX711_h   //Bibliotheque modifiee pour fonctionnner sur ESP32
#define Q2HX711_h
#include "Arduino.h"

class Q2HX711
{
  private:
    byte CLOCK_PIN;
    byte OUT_PIN;
    byte GAIN;
    bool pinsConfigured;

  public:
    Q2HX711(byte output_pin, byte clock_pin);
    virtual ~Q2HX711();
    bool readyToSend();
    void setGain(byte gain = 128);
    long read();
	uint8_t shiftInSlow(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder); //fonction ajoutee Pour ESP32
};

#endif /* Q2HX711_h */
