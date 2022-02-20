/*
 Adapted from the Adafruit graphicstest sketch, see original header at end
 of sketch.

 This sketch uses the GLCD font (font 1) only.

 Make sure all the display driver and pin connections are correct by
 editing the User_Setup.h file in the TFT_eSPI library folder.

 #########################################################################
 ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
 #########################################################################
*/


#include "SPI.h"
#include "./display_config.h"
#include "TFT_eSPI.h"
#include "SPIFFS.h"  // For ESP32 only

TFT_eSPI tft = TFT_eSPI();

unsigned long total = 0;
unsigned long tn = 0;
  uint pins[] = {
TFT_RST,
TFT_DC,
TFT_CS,
TFT_SCLK,
TFT_MOSI,
TFT_BL,
  };
  uint fuzz[] = {0,1,0,0,1,0,1,0,1,0,0,1,0};

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println(""); Serial.println("");
  Serial.println("TFT_eSPI library test!");

if (!SPIFFS.begin()) {
    Serial.println("SPIFFS initialisation failed!");
    while (1) yield(); // Stay here twiddling thumbs waiting
  }
  Serial.println("\r\nSPIFFS initialised.");


  tft.init();

  tn = micros();
  tft.fillScreen(TFT_WHITE);

  // for (uint i = 0; i < 6; i++) {
  //   pinMode(pins[i], OUTPUT);
  //   digitalWrite(pins[i], HIGH);
  // }

  // delay(1000);

  // for (uint i = 0; i < 6; i++) {
  //   digitalWrite(i, LOW);
  // }

}

void loop(void) {
  // uint fuzzat;
  // for (uint i = 0; i < 6; i++) {
  //   digitalWrite(vars[i], fuzz[(fuzzat++ % 10)]);
  // }
  // delay(400);
  // Serial.println("LN");

  int x = random(tft.width()  - 128);
  int y = random(tft.height() - 160);

  tft.fillScreen(TFT_WHITE);

  drawBmp("/parrot.bmp", x, y);

  delay(1000);
  // for (uint8_t rotation = 0; rotation < 4; rotation++) {
  //   tft.setRotation(rotation);
  //   testText();
  //   delay(2000);
  // }
}
