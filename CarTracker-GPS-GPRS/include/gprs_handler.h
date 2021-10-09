// Choose two Arduino pins to use for software serial
int GPRS_RXPin = 4; // rx pin
int GPRS_TXPin = 5; // tx pin

long int GPRSBaud = 115200;

// Create a software serial port called "gpsSerial"
SoftwareSerial gprsSerial(GPRS_RXPin, GPRS_TXPin);

void updateSerial();

void GSMSetup()
{
  //Begin serial communication with Arduino and A6
  gprsSerial.begin(GPRSBaud);

  Serial.println("Initializing GPRS ...");
  delay(1000);
  gprsSerial.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
  gprsSerial.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
  updateSerial();
  gprsSerial.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
  updateSerial();
  gprsSerial.println("AT+CREG?"); //Check whether it has registered in the network
  updateSerial();
  gprsSerial.println("AT+SAPBR=3,1,\"APN\",\"mcinet\""); //configuring Access Point Name for GPRS
  updateSerial();
  gprsSerial.println("AT+SAPBR=1,1"); //activating network
}

void updateSerial()
{
  delay(500);
  while (Serial.available())
  {
    gprsSerial.write(Serial.read()); //Forward what Serial received to Software Serial Port
  }
  while (gprsSerial.available())
  {
    Serial.write(gprsSerial.read()); //Forward what Software Serial received to Serial Port
  }
}

void get_request(String domain, String token, double *lat, double *lon)
{
  String url = domain + "tracker/api/device" + token + "coordinate/get?lat=" + *lat + "&lon=" + *lon;
  gprsSerial.println("AT+HTTPINIT"); //initializing HTTP method and make it ready for it
  updateSerial();
  gprsSerial.println("AT+HTTPPARA=\"CID\",1"); //activating header for request
  updateSerial();
  gprsSerial.println("AT+HTTPPARA=\"URL\"," + url); //configuring Access Point Name for GPRS
  updateSerial();
  gprsSerial.println("AT+HTTPACTION=0"); //telling to setup for get request
  updateSerial();
  gprsSerial.println("AT+HTTPREAD"); //reading the response
  updateSerial();
  gprsSerial.println("AT+HTTPTERM"); //terminating http request
}

void post_request(String domain, String token,String api_key,double *lat, double *lon)
{
  DynamicJsonDocument doc(200);
  doc["lat"] = *lat;
  doc["lon"] = *lon;
  String output;
  serializeJson(doc, output);
  doc.clear();
  String url = domain + "tracker/api/device" + token + "coordinate";
  gprsSerial.println("AT+HTTPINIT"); //initializing HTTP method and make it ready for it
  updateSerial();
  gprsSerial.println("AT+HTTPPARA=\"CID\",1"); //activating header for request
  updateSerial();
  gprsSerial.println("AT+HTTPPARA=\"URL\"," + url); //configuring Access Point Name for GPRS
  updateSerial();
  gprsSerial.println("AT+HTTPPARA=\"CONTENT\",\"application/json\""); //configuring header
  updateSerial();
  gprsSerial.println("AT+HTTPPARA=\"USERDATA\",\"Authorization:JWT " + String(api_key) + "\""); //configuring header
  updateSerial();
  gprsSerial.println("AT+HTTPDATA=" + String(output.length()) + ",100000");  
  updateSerial();
  gprsSerial.println(output);
  updateSerial();  
  gprsSerial.println("AT+HTTPACTION=1"); //telling to setup for post request
  updateSerial();
  gprsSerial.println("AT+HTTPREAD"); //reading the response
  updateSerial();
  gprsSerial.println("AT+HTTPTERM"); //terminating http request
}