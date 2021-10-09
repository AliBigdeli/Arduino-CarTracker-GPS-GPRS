#include <TinyGPS++.h>

// Choose two Arduino pins to use for software serial
int GPS_RXPin = 2; // rx pin
int GPS_TXPin = 3; // tx pin

int GPSBaud = 9600;

// Create a TinyGPS++ object
TinyGPSPlus gps;

// Create a software serial port called "gpsSerial"
SoftwareSerial gpsSerial(GPS_RXPin, GPS_TXPin);


double lat;
double lon;

void GPSSetup()
{
    Serial.println("Initializing GPS ...");
    gpsSerial.begin(GPSBaud);
}


bool get_coordinates(double *lat, double *lon)
{
    // This sketch displays information every time a new sentence is correctly encoded.
    while (gpsSerial.available() > 0)
        if (gps.encode(gpsSerial.read()))
        {
            if (gps.location.isValid())
            {

                *lat = gps.location.lat();
                *lon = gps.location.lng();
                Serial.print("Latitude: ");
                Serial.println(gps.location.lat(), 6);
                Serial.print("Longitude: ");
                Serial.println(gps.location.lng(), 6);
                Serial.print("Altitude: ");
                Serial.println(gps.altitude.meters());
                return true;
            }
            else
            {
                Serial.println("Location: Not Available");
                return false;
            }
        }
    // If 5000 milliseconds pass and there are no characters coming in
    // over the software serial port, show a "No GPS detected" error
    if (millis() > 5000 && gps.charsProcessed() < 10)
    {
        Serial.println("No GPS detected");
        while (true)
            ;
    }
    return false;
}