/*
 * Created 02/26/21
 * By Jack A. Defay
 * 
 * Code for collecting data at different flow speeds to map speed to voltage.
 * 
 * This code is published under the MIT Open Source Liscense
 */

int Thermistor1 = A15;  //thermistor 1
int Thermistor2 = A14;  //thermistor 2
int Thermistor3 = A13;  //thermistor 3
int Thermistor4 = A12;  //thermistor 4
int Thermistor5 = A11;  //thermistor 5
int Thermistor6 = A10;  //thermistor 6
int Thermistor7 = A9;  //thermistor 7
int Thermistor8 = A8;  //thermistor 8

int fanControl = 2;

int pins[] = {Thermistor1, Thermistor2, Thermistor3, Thermistor4, Thermistor5, Thermistor6, Thermistor7, Thermistor8};
int data[8];

int thermistorToCalibrate = 4;
int increment = 25;  //the value to increment the fan. experiment will take 250/increment minutes to run

void setup() {
  
  pinMode(Thermistor1, INPUT);
  pinMode(Thermistor2, INPUT);
  pinMode(Thermistor3, INPUT);
  pinMode(Thermistor4, INPUT);
  pinMode(Thermistor5, INPUT);
  pinMode(Thermistor6, INPUT);
  pinMode(Thermistor7, INPUT);
  pinMode(Thermistor8, INPUT);

  pinMode(fanControl, OUTPUT);
  
  Serial.begin(115200);
}

void loop() {
  static int loop = 1;

  //turn on fan to max
  //wait 1 minute to let value settle
  //read values for 1 minute
  //increment fan down
  //loop

  //set up the fan so it starts running early and can settle to a steady state
  analogWrite(fanControl, 250);

  delay(2000);

  while(loop){
    for(int f=250;f>=0;f-=increment){
      analogWrite(fanControl, f);
      delay(6000); //wait 60 seconds for heat to stabalize
      for(int r=0;r<600;r++){
        Serial.println(analogRead(pins[thermistorToCalibrate]));  //read pin 10 times a second for a minute and send to python script
        delay(100);
      }
      Serial.print("/");  //use this as code to start a new read
      Serial.println(f);  //tells the python script that this is the value of the fan control
    }
    Serial.println("/01");
    loop = 0;
  }
  
}
