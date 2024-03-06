#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include "RF24.h"

// Neopixel Pin
#define PIN            1
// Num of pixels
#define NUMPIXELS      8


Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN);
RF24 radio(13,0);


byte addresses[][6] = {"3Node","4Node"};
bool radioNumber = 1;
int timerMultiplier = 2;


void setAllPixels(uint32_t color) {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, color);
  }
  
  pixels.show();
}


void clearPixels() {
  for (int i = 0; i < NUMPIXELS; i++) {
   pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  }
  pixels.show();
}



void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
  radio.begin();

  radio.setPALevel(RF24_PA_MAX);
  radio.enableAckPayload();
  radio.setRetries(0, 15);
  radio.setPayloadSize(1);
 
  if(radioNumber){
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);
  }else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }
  
  radio.startListening();

  // Startup light sequence
  for (int k = 0; k < 1; k++) {
    for (int j = 0; j < 3; j++) {
      for (int i = 0; i < NUMPIXELS; i++) {
        if (j == 0) {
          pixels.setPixelColor(i, pixels.Color(10, 0, 0));
        }
        else if (j == 1) {
          pixels.setPixelColor(i, pixels.Color(0, 10, 0));
        }
        else {
          pixels.setPixelColor(i, pixels.Color(0, 0, 10));
        }
        pixels.show();
        delay(75);
      }
    }
  }

  clearPixels();

  delay(250);

  for (int j = 0; j < 3; j++) {
      for (int i = 0; i < NUMPIXELS; i++) {
        if (j == 0) {
          pixels.setPixelColor(i, pixels.Color(10, 0, 0));
        }
        else if (j == 1) {
          pixels.setPixelColor(i, pixels.Color(0, 10, 0));
        }
        else {
          pixels.setPixelColor(i, pixels.Color(0, 0, 10));
        }
      }

      pixels.show();
      delay(100);
      clearPixels();
      delay(150);
    }

    clearPixels();
    delay(1000);

  

}

void loop() {

  radio.flush_rx();
  radio.stopListening();

  char data = 0;
  uint8_t signalStrength = 0;
  
  if(radio.write(&data, 1)) {
    signalStrength = radio.getARC();
  }
  else {
    signalStrength = 0;
  }
  
  setAllPixels(pixels.Color(0, 0, 7 * signalStrength));
  
  radio.startListening();

  if (radioNumber == 0) {
    delay(1 * random(1000, 2000));

  }
  else {
    delay(2 * random(1000, 2000));

  }
  

  clearPixels();
  if (radio.isChipConnected()) {
    pixels.setPixelColor(0, pixels.Color(0, 50, 0));
  }
  else {
    pixels.setPixelColor(0, pixels.Color(50, 0, 0));
  }
  
  pixels.show();
  delay(100);
  clearPixels();
  
}
