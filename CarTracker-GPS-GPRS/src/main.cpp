#include <Arduino.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include "gps_handler.h"
#include "gprs_handler.h"



void setup()
{
  // Start the Arduino hardware serial port at 9600 baud
  Serial.begin(9600);

  // Start the software serial port at the GPS's default baud
  gpsSerial.begin(GPSBaud);
}

void loop()
{
  get_coordinates();
  
}
