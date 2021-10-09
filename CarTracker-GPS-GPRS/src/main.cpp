#include <Arduino.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include "gps_handler.h"
#include "gprs_handler.h"

String api_key = "";
String token = "";
String domain = "https://gpscartracker.herokuapp.com";

void setup()
{
  // Start the Arduino hardware serial port at 9600 baud
  Serial.begin(9600);
  GSMSetup();
  GPSSetup();
  // Start the software serial port at the GPS's default baud
  //
}

void loop()
{
  //updateSerial();
  if (get_coordinates (&lat, &lat))
  {
    get_request(domain, token, &lat, &lat);
    //post_request(domain, token,api_key, &lat, &lat);
  }
}
