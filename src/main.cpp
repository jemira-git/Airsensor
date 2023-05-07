#include <Arduino.h>
#include <ESP8266WiFi.h>                                   
#include <SoftwareSerial.h> 

#include "credentials.hpp"
#include "MHZ19.h"   

MHZ19 myMHZ19;                                             // Constructor for library
SoftwareSerial serial2(D1, D2);                            // (Uno example) create device to MH-Z19 serial
unsigned long getDataTimer = 0;

void setup() {
  // initialize digital pin LED_BUILTIN as an output:
  pinMode(LED_BUILTIN, OUTPUT);

  // Initialize serial monitor (UART0):
  Serial.begin(115200);
  Serial.println();

  // Initialize soft serial for CO2 sensor:
  serial2.begin(9600);   
  myMHZ19.begin(serial2);

  // Turn auto calibration ON (OFF autoCalibration(false)):
  myMHZ19.autoCalibration();                             

  // Connect to WiFi:
  WiFi.hostname(wifi_host); 
  WiFi.begin(wifi_ssid, wifi_pass);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

// the loop function runs over and over again forever
void loop() {
  // Do not block in main loop to allow back processes
  // to run:
  if (millis() - getDataTimer >= 2000) {

    // Request CO2 as ppm:
    int co2 =  myMHZ19.getCO2();
    Serial.print("CO2 (ppm): ");                      
    Serial.println(co2);

    // Request Temperature (as Celsius):
    int8_t temp = myMHZ19.getTemperature(); 
    Serial.print("Temperature (C): ");                  
    Serial.println(temp);

    // Flash the LED for visual feedback:
    digitalWrite(LED_BUILTIN, LOW);
    delay(50);                     
    digitalWrite(LED_BUILTIN, HIGH);

    // Update milliseconds variable for loop:
    getDataTimer = millis();
  }
}
