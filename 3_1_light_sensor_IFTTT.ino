//IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS, FEED_KEY
#include "arduino_secrets.h"
#include <BH1750.h>
#include <Wire.h>
#include <AdafruitIO_Feed.h>

// the two states we'll switch between
#define ON "SUN_ON"
#define OFF "SUN_OFF"

BH1750 lightMeter;

// const String PATH_NAME   = "/api/v2/" + IO_USERNAME + "/feeds/" + FEED_KEY; // IO_USERNAME from arduino_secrets.h

AdafruitIO_Feed *feed = io.feed(FEED_KEY);

bool sun_on = false;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.print("Connecting to Adafruit IO");
  
  // connect to io.adafruit.com
  io.connect();
  
  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  
  Wire.begin();

  lightMeter.begin();

  // Serial.println(F("BH1750 Test begin"));
}

void loop() {
  io.run();

  float lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");

  if (lux > 1000 && !sun_on) {
    updateFeed(ON);
    sun_on = true;
  } else if (lux < 1000 && sun_on) {
    updateFeed(OFF);
    sun_on = false;
  }
  delay(3000);
}

void updateFeed(String newVal) {
  // save count to the 'counter' feed on Adafruit IO
  Serial.print("sending -> ");
  Serial.println(newVal);
  feed->save(newVal);
}
