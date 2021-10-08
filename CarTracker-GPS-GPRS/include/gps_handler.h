#include <TinyGPS++.h>

// Choose two Arduino pins to use for software serial
int RXPin = 2; // rx pin
int TXPin = 3; // tx pin

int GPSBaud = 9600;

// Create a TinyGPS++ object
TinyGPSPlus gps;

// Create a software serial port called "gpsSerial"
SoftwareSerial gpsSerial(RXPin, TXPin);

DynamicJsonDocument doc(300);

void displayInfo()
{
    if (gps.location.isValid())
    {
        doc["lat"] = gps.location.lat();
        doc["lon"] = gps.location.lng();
        Serial.print("Latitude: ");
        Serial.println(gps.location.lat(), 6);
        Serial.print("Longitude: ");
        Serial.println(gps.location.lng(), 6);
        Serial.print("Altitude: ");
        Serial.println(gps.altitude.meters());
    }
    else
    {
        Serial.println("Location: Not Available");
    }

    Serial.print("Date: ");
    if (gps.date.isValid())
    {
        Serial.print(gps.date.month());
        Serial.print("/");
        Serial.print(gps.date.day());
        Serial.print("/");
        Serial.println(gps.date.year());
        // String Date;
        // sprintf(Date, "%s %s!", "Hello", "world");
        // doc["date"] = Date;
    }
    else
    {
        Serial.println("Not Available");
    }

    Serial.print("Time: ");
    if (gps.time.isValid())
    {
        if (gps.time.hour() < 10)
            Serial.print(F("0"));
        Serial.print(gps.time.hour());
        Serial.print(":");
        if (gps.time.minute() < 10)
            Serial.print(F("0"));
        Serial.print(gps.time.minute());
        Serial.print(":");
        if (gps.time.second() < 10)
            Serial.print(F("0"));
        Serial.print(gps.time.second());
        Serial.print(".");
        if (gps.time.centisecond() < 10)
            Serial.print(F("0"));
        Serial.println(gps.time.centisecond());
    }
    else
    {
        Serial.println("Not Available");
    }

    Serial.println();
    serializeJson(doc, Serial);
    Serial.println();
    delay(1000);
    doc.clear();
}

void get_coordinates()
{
    // This sketch displays information every time a new sentence is correctly encoded.
    while (gpsSerial.available() > 0)
        if (gps.encode(gpsSerial.read()))
            displayInfo();

    // If 5000 milliseconds pass and there are no characters coming in
    // over the software serial port, show a "No GPS detected" error
    if (millis() > 5000 && gps.charsProcessed() < 10)
    {
        Serial.println("No GPS detected");
        while (true)
            ;
    }
}