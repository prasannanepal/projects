#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include "Index.h" 
#define ON_Board_LED 2

//SSID and Password of your WiFi router
const char* ssid = "Prasanna";  //Your wifi name
const char* password = "Nepal123"; // Your wifi password


unsigned long previousMillisGetHR = 0; // will store the last time Millis (to get Heartbeat) was updated.
unsigned long previousMillisHR = 0; //will store the last time Millis (to get BPM) was updated.

const long intervalGetHR = 10; //Interval for reading heart rate (Heartbeat) = 10ms.
const long intervalHR = 10000; //Interval for obtaining the BPM value based on the sample is 10 seconds.

const int PulseSensorHRWire = A0; //PulseSensor 
const int LED_D1 = D1; //LED to detect when the heart is beating.
int Threshold = 555; //Determine which Signal to "count as a beat" and which to ignore.

int cntHB = 0; // Variable for counting the number of heartbeats.
boolean ThresholdStat = true; //Variable for triggers in calculating heartbeats.
int BPMval = 0; // Variable to hold the result of heartbeats calculation.

ESP8266WiFiMulti WiFiMulti;
HTTPClient ask;

const char* apiKeyIn = "QjJTzXPaFwF3BAawBSZa0fUyS2cbkILB";      // API KEY IN
const unsigned int writeInterval = 25000;   // write interval (in ms)

// ASKSENSORS API host config
const char* host = "api.asksensors.com";  // API host name
const int httpPort = 80; 



void handleHeartRate() {
  digitalWrite(ON_Board_LED, LOW); // "ON_Board_LED" will be turned on when the request from the browser / client starts.
  String BPMvalSend = String(BPMval); 
  digitalWrite(ON_Board_LED, HIGH); //Turn off the LED because the client request / browser has been processed.
}

// To get a BPM value based on a heart rate reading for 10 seconds.
void GetHeartRate() {
  unsigned long currentMillisGetHR = millis();

  if (currentMillisGetHR - previousMillisGetHR >= intervalGetHR) {
    previousMillisGetHR = currentMillisGetHR;

    int PulseSensorHRVal = analogRead(PulseSensorHRWire);

    if (PulseSensorHRVal > Threshold && ThresholdStat == true) {
      cntHB++;
      ThresholdStat = false;
      digitalWrite(LED_D1,HIGH);
    }

    if (PulseSensorHRVal < Threshold) {
      ThresholdStat = true;
      digitalWrite(LED_D1,LOW);
    }
  }

  unsigned long currentMillisHR = millis();

  if (currentMillisHR - previousMillisHR >= intervalHR) {
    previousMillisHR = currentMillisHR;

    BPMval = cntHB * 6; // The taken heart rate is for 10 seconds. So to get the BPM value, the total heart rate in 10 seconds x 6.
    Serial.print("BPM : ");
    Serial.println(BPMval);
    
    cntHB = 0;
    if(BPMval>40&&BPMval<110){
      asksensor();
    }
    
  }
 
}
void asksensor(){

  // Use WiFiClient class to create TCP connections
  WiFiClient client;


  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }else {

    
  String url = "http://api.asksensors.com/write/";
  url += apiKeyIn;
  url += "?module1=";
  url += BPMval;
    
  Serial.print("********** requesting URL: ");
  Serial.println(url);
   // send data 
   ask.begin(url); //Specify the URL
  
    //Check for the returning code
    int httpCode = ask.GET();          
 
    if (httpCode > 0) { 
 
        String payload = ask.getString();
        Serial.println(httpCode);
        Serial.println(payload);
      } else {
      Serial.println("Error on HTTP request");
    }
 
    ask.end(); //End 
    Serial.println("********** End ");
    Serial.println("*****************************************************");

  }

  client.stop();  // stop client
  
}

//--------------------------------------------------------------------------------void setup()
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(500);
    
  pinMode(ON_Board_LED,OUTPUT); //--> On Board LED port Direction output
  digitalWrite(ON_Board_LED, HIGH); //--> Turn off Led On Board

  pinMode(LED_D1,OUTPUT); //--> Set LED_3 PIN as Output.

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); //--> Connect to your WiFi router
  Serial.println("");


  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
   
    digitalWrite(ON_Board_LED, LOW);
    delay(250);
    digitalWrite(ON_Board_LED, HIGH);
    delay(250);
    
  }
  digitalWrite(ON_Board_LED, HIGH); //Turn off the On Board LED when it is connected to the wifi router.
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Serial.println();
  Serial.println("Please wait 10 seconds to get the BPM Value");
}
void loop() {

  GetHeartRate(); //Calling the GetHeartRate() subroutine
}
