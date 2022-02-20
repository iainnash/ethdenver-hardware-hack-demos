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

#define MAX_IMAGE_WDITH 240 // Adjust for your images

// #include "LittleFS.h"
// #include "FS.h"
// #include <LITTLEFS.h>
// #define FileSys LittleFS

// Include the PNG decoder library
#include <PNGdec.h>
#include "./PNG_functions.h"



PNG png;
#define MAX_IMAGE_WIDTH 320 // Adjust for your images


int16_t xpos = 0;
int16_t ypos = 0;


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

  // Initialise FS
  // if (!FileSys.begin()) {
  //   Serial.println("LittleFS initialisation failed!");
  //   while (1) yield(); // Stay here twiddling thumbs waiting
  // }

  Serial.println("\r\nInitialisation done.");

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


//=========================================v==========================================
//                                      pngDraw
//====================================================================================
// This next function will be called during decoding of the png file to
// render each image line to the TFT.  If you use a different TFT library
// you will need to adapt this function to suit.
// Callback function to draw pixels to the display
void pngDraw(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WDITH];
  png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
  tft.pushImage(xpos, ypos + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}

void loop(void) {

  fs::File bmpFS;

  String fname = "/buff-small.png";

  // Open requested file on SD card
  bmpFS = SPIFFS.open(fname, "r");

  if (!bmpFS)
  {
    Serial.print("File not found");
    return;
  }

    int16_t rc = png.open(fname.c_str(), pngOpen, pngClose, pngRead, pngSeek, pngDraw);
      if (rc == PNG_SUCCESS) {
        tft.startWrite();
        Serial.printf("image specs: (%d x %d), %d bpp, pixel type: %d\n", png.getWidth(), png.getHeight(), png.getBpp(), png.getPixelType());
        uint32_t dt = millis();
        if (png.getWidth() > MAX_IMAGE_WDITH) {
          Serial.println("Image too wide for allocated line buffer size!");
        }
        else {
          rc = png.decode(NULL, 0);
          png.close();
        }
        tft.endWrite();
        // How long did rendering take...
        Serial.print(millis()-dt); Serial.println("ms");
      }


  // uint fuzzat;
  // for (uint i = 0; i < 6; i++) {
  //   digitalWrite(vars[i], fuzz[(fuzzat++ % 10)]);
  // }
  // delay(400);
  // Serial.println("LN");

  //int x = random(tft.width()  - 128);
  //int y = random(tft.height() - 160);

  // for (uint8_t rotation = 0; rotation < 4; rotation++) {
  //   tft.setRotation(rotation);
  //   testText();
  //   delay(2000);
  // }
}
