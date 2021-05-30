/*
 * Created 05/30/21
 * By Jack A. Defay
 * 
 * Code for reading the raw analog values from the sensor.
 * 
 * This code is published under the MIT Open Source Liscense
 */

#include <math.h>

int ThermistorMag = A0;  //thermistor 5
int ThermistorN = A1;  //thermistor 8
int ThermistorE = A2;  //thermistor 2
int ThermistorS = A3;  //thermistor 7
int ThermistorW = A4;  //thermistor 6

int pins[] = {ThermistorN, ThermistorE, ThermistorS, ThermistorW};
double data[4];
double mag;

void setup() {
  pinMode(ThermistorMag, INPUT);
  pinMode(ThermistorN, INPUT);
  pinMode(ThermistorE, INPUT);
  pinMode(ThermistorS, INPUT);
  pinMode(ThermistorW, INPUT);
  
  
  Serial.begin(9600);
}

void loop() {
  Serial.print(millis());
  Serial.print(", ");
  
  mag = analogRead(ThermistorMag);
  Serial.print(mag);
  Serial.print(", ");


  for(int n=0; n<4; n++){
    data[n] = analogRead(pins[n]);
    Serial.print(data[n]);
    Serial.print(", ");
  }
  Serial.println();

  delay(100);
}
