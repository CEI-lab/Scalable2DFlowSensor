/*
 * Created 07/08/20
 * By Jack A. Defay
 * 
 * Code for reading data from the flex sensor used in the field tests.
 * Prints out the magnitude as an analog value for later conversion.
 * Prints out either the calculated direction in degrees or the two cross proportions for more detail.
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
double proportionX, proportionY;
double dir = 0;

const double offset[4] = {2, -4, -2, 6};
double average = 0;

void setup() {
  pinMode(ThermistorMag, INPUT);
  pinMode(ThermistorN, INPUT);
  pinMode(ThermistorE, INPUT);
  pinMode(ThermistorS, INPUT);
  pinMode(ThermistorW, INPUT);
  
  
  Serial.begin(9600);
}

void loop() {
//  Serial.print(millis());
//  Serial.print(", ");

  mag = analogRead(ThermistorMag);
  Serial.print(mag);
//  Serial.print(", ");

  average = 0;
  
  for(int n=0; n<4; n++){
    data[n] = analogRead(pins[n]);
//    Serial.print(data[n]);
//    Serial.print(", ");
    average += data[n];
  }
  Serial.println();

  average = average/4;

  for(int n=0; n<4; n++){
    data[n] = data[n] - average - offset[n];
//      data[n] = data[n] - average;
//      Serial.print(data[n]);
//      Serial.print(", ");
  }
//  Serial.println();

  proportionY = data[0] - data[2];  //sensor 1 - 5
  proportionX = data[1] - data[3];  //sensor 3 - 7

//  Serial.print(proportionX);
//  Serial.print(", ");
//  Serial.print(proportionY);
//  Serial.print(", ");

  dir = atan2(proportionY,proportionX);
  dir = (180/3.14159) * dir;

//  Serial.println(dir);

  delay(100);
}
