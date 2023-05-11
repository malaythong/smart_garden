#define BLYNK_TEMPLATE_ID "TMPLKDr6-e1x"
#define BLYNK_DEVICE_NAME "smart farm"
#define BLYNK_AUTH_TOKEN "FoTVirhqum4s6pXWI5esJbeTPc9UgwGW"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include "DHT.h"     


const int relay = 26;

int sensorpin=A0;
int sensorvalue=0;
int outputvalue=0;

int relayState = 1; // RELAY OFF 

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Gg";
char pass[] = "00000003";

char auth[] = BLYNK_AUTH_TOKEN;

DHT dhtA(14, DHT22);

#define WATER_PUMP 25
boolean state = false;

BLYNK_WRITE(V5) {
  relayState = param.asInt();
  digitalWrite(relay, relayState);
  }

BLYNK_CONNECTED() {
  // Request the latest state from the server
  Blynk.syncVirtual(V5);
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
   
  pinMode(relay, OUTPUT);
  pinMode(WATER_PUMP, OUTPUT);
  dhtA.begin(); 

 //On power ON rely in OFF state
  digitalWrite(relay, relayState);

}

void loop()
{
  sensorvalue=analogRead(sensorpin);
  outputvalue=( 100 - ( (sensorvalue/4095.00) * 100 ) );

  if(outputvalue>74)
  {
       Serial.println("water your plant");
       Serial.print(outputvalue);
       Serial.print('\n');
       Blynk.notify("water your plant");
       delay(1000); 
  }
  else if(outputvalue<45)
  {
       Serial.println("soil is wet enough to water");
       Serial.print(outputvalue);
       Serial.print('\n');
       Blynk.notify("soil is wet enough to water");
       delay(1000);
  }
  Blynk.virtualWrite(V1,outputvalue);
  Blynk.run();
  climateRoutine();                           // Climate routine
  delay(4700);

  //light
  if(relayState==1){
    digitalWrite(relay, HIGH);
    Serial.println("Light ON");
  }
  else{
    digitalWrite(relay, LOW);
    Serial.println("Light OFF");
  }
}

BLYNK_WRITE(V4)
{
  if (state == false) {
    state = true;
    Blynk.notify("You just watered your plant.");
    digitalWrite(WATER_PUMP,HIGH);
  }
  else {
    state = false;
    digitalWrite(WATER_PUMP,LOW);
  }
}

void climateRoutine() {
    float h1 = dhtA.readHumidity();            // f1 and h1 are celsius and humidity readings
    // byte t1 = dhtA.readTemperature(true);  // for temperature in farenheits
    float t1 = dhtA.readTemperature();         // from DHT/A
    Blynk.virtualWrite(V7, t1);               //  Set Virtual Pin 0 frequency to PUSH in Blynk app
    Blynk.virtualWrite(V6, h1);               //  Set Virtual Pin 1 frequency to PUSH in Blynk app
}


//// soil sensor
//int _moisture,sensor_analog;
//const int sensor_pin = A0;
//
//void setup()
//{
//  // Debug console
//  Serial.begin(9600);
//  WiFi.begin(ssid, pass);
//}
//
//void loop()
//{
//  sensor_analog = analogRead(sensor_pin);
//  _moisture = ( 100 - ( (sensor_analog/4095.00) * 100 ) );
//  Serial.print("Moisture = ");
//  Serial.print(_moisture);  /* Print Temperature on the serial window */
//  Serial.println("%");
//  delay(3000);
//}
