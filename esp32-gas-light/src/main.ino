#include "./secret.h"

#include <WiFi.h>
#include "heltec.h"
#include <HTTPClient.h>

#include <FastLED.h>

#define LED_PIN     2
#define NUM_LEDS   120 
#define BRIGHTNESS  64
#define LED_TYPE    WS2812
#define COLOR_ORDER RGB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100


const char* ssid = WIFI_NETWORK;
const char* password = WIFI_PASSWORD;

//Your Domain name with URL path or IP address with path
String serverName = "http://gas-monitor.isiain.workers.dev/get-gas-level";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

void scanWifi() {

  int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0) {
        Serial.println("no networks found");
    } else {
        Serial.print(n);
        Serial.println(" networks found");
        for (int i = 0; i < n; ++i) {
            // Print SSID and RSSI for each network found
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");
            Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
            delay(10);
        }
    }
}

void setupLeds() {
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
}

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void display(String gasLevel) {
  // Font Demo1
  Heltec.display->clear();
  // create more fonts at http://oleddisplay.squix.ch/
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_16);
  Heltec.display->drawString(0, 0, "Connected to WIFI");
  // Heltec.display->setFont(ArialMT_Plain_16);
  Heltec.display->drawString(0, 10, "Gas Status" + gasLevel);
  // Heltec.display->setFont(ArialMT_Plain_16);
  Heltec.display->drawString(0, 26, "Hello world");
  Heltec.display->display();
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
  FastLED.show();
}


void setColor(CRGB color)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; ++i) {
        leds[i] = color;
    }
    FastLED.show();
}




void setup() {
  Serial.begin(115200); 

  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, true /*Serial Enable*/);
  Heltec.display->flipScreenVertically();

  setupLeds();

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop() {
  //Send an HTTP POST request every 10 minutes
  // Serial.println("Update");
  // delay(1000);
  // setColor(CRGB(200, 0, 0));
  // delay(1000);
  // setColor(CRGB(0, 200, 0));
  // delay(1000);
  // setColor(CRGB(200, 200, 0));
  // delay(1000);

  bpm();
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status() == WL_CONNECTED){
      // setColor(CRGB(200, 200, 200));
      HTTPClient http;

      const char* headerKeys[] = {"x-gas-level"};
      http.collectHeaders(headerKeys, 1);

      // Your Domain name with URL path or IP address with path
      http.begin(serverName.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);

        // String payload = http.getString();
        String gasLevel = http.header("x-gas-level");

        if (gasLevel == "R") {
          setColor(CRGB(200, 0, 0));
        }
        if (gasLevel == "Y") {
          setColor(CRGB(200, 200, 0));
        }
        if (gasLevel == "G") {
          setColor(CRGB(0, 200, 0));
        }

        delay(2000);

        // Serial.println(payload);
        Serial.println(gasLevel);

        // display(gasLevel);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}