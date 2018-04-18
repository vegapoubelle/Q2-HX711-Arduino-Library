//Bibliotheque modifiee pour fonctionnner sur ESP32 (ajout de delays de 1s et ajout de la fonction shiftInSlow au lieu de shiftIn)
//Autrement le signal d'horloge est trop rapide! (L'esp32 fonctionne beaucoup plus vite qu'une Arduino)

#include <Arduino.h>
#include "Q2HX711.h"

Q2HX711::Q2HX711(byte output_pin, byte clock_pin) {
  CLOCK_PIN  = clock_pin;
  OUT_PIN  = output_pin;
  GAIN = 1;
  pinsConfigured = false;
}

Q2HX711::~Q2HX711() {
}

uint8_t Q2HX711::shiftInSlow(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder) {  //fonction ajoutee Pour ESP32
    uint8_t value = 0;
    uint8_t i;
	//Serial.print("SCK :");
    //Serial.println(clockPin);

    for(i = 0; i < 8; ++i) {
        digitalWrite(clockPin, HIGH);
        delayMicroseconds(1);
        if(bitOrder == LSBFIRST)
            value |= digitalRead(dataPin) << i;
        else
            value |= digitalRead(dataPin) << (7 - i);
        digitalWrite(clockPin, LOW);
        delayMicroseconds(1);
    }
    return value;
}

bool Q2HX711::readyToSend() {
  if (!pinsConfigured) {
    // We need to set the pin mode once, but not in the constructor
    pinMode(CLOCK_PIN, OUTPUT);
	//Serial.println("bug");
    pinMode(OUT_PIN, INPUT);
    pinsConfigured = true;
  }
  return digitalRead(OUT_PIN) == LOW;
}

void Q2HX711::setGain(byte gain) {
  switch (gain) {
    case 128:                    //gain par defaut = 128
      GAIN = 1;
      break;
    case 64:
      GAIN = 3;
      break;
    case 32:
      GAIN = 2;
      break;
  }

  digitalWrite(CLOCK_PIN, LOW);
  read();
}

long Q2HX711::read() {
   while (!readyToSend());
   /* Serial.println("bug");
   Serial.print("SCK :");
   Serial.println(CLOCK_PIN);
   if (pinsConfigured) 
   {	Serial.println("configured");
   }
   */

  byte data[3];

  for (byte j = 3; j--;) {
      data[j] = shiftInSlow(OUT_PIN,CLOCK_PIN, MSBFIRST);
  }

  // set gain
  for (int i = 0; i < GAIN; i++) {
    digitalWrite(CLOCK_PIN, HIGH);
	delayMicroseconds(1);      //Ajoute pour ESP32        
    digitalWrite(CLOCK_PIN, LOW);
	delayMicroseconds(1);		//Ajoute pour ESP32
  }

  data[2] ^= 0x80;
  return ((uint32_t) data[2] << 16) | ((uint32_t) data[1] << 8) | (uint32_t) data[0];
}