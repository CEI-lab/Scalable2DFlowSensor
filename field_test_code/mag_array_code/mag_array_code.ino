/*
 * Created 09/01/20
 * By Jack A. Defay
 * 
 * Code for reading the array of manitude data from the array sensor.
 * This was the code used for the field tests in the paper.
 * 
 * This code is published under the MIT Open Source Liscense
 */

int Thermistor1 = A0;  //thermistor 3
int Thermistor2 = A1;  //thermistor 4
int Thermistor3 = A2;  //thermistor 5
int Thermistor4 = A3;  //thermistor 6
int Thermistor5 = A4;  //thermistor 7
int Thermistor6 = A5;  //thermistor 8
int Thermistor7 = A6;  //thermistor 2 offset like 200 points higher than the rest
int Thermistor8 = A7;  //thermistor 1 same, must be a bad connection

int pins[] = {Thermistor1, Thermistor2, Thermistor3, Thermistor4, Thermistor5, Thermistor6, Thermistor7, Thermistor8};
int data[8];

void setup() {
  
  pinMode(Thermistor1, INPUT);
  pinMode(Thermistor2, INPUT);
  pinMode(Thermistor3, INPUT);
  pinMode(Thermistor4, INPUT);
  pinMode(Thermistor5, INPUT);
  pinMode(Thermistor6, INPUT);
  pinMode(Thermistor7, INPUT);
  pinMode(Thermistor8, INPUT);
  
  Serial.begin(115200);
}

void loop() {
//  Serial.print(millis());
//  Serial.print(", ");
  
  for(int n=0; n<7; n++){
    data[n] = analogRead(pins[n]);
    Serial.print(data[n]);
    Serial.print(", ");
  }
  data[7] = analogRead(pins[7]);
  Serial.print(data[7]);
  Serial.println();

//  Serial.println(analogRead(pins[3]));

  delay(100);
}
