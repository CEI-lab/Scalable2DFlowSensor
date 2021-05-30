/*
 * Created 03/18/21
 * By Jack A. Defay
 * 
 * Code for collecting data at different angle steps to calculate appropriate offset values to calibrate the sensor.
 * 
 * This code is published under the MIT Open Source Liscense
 */

int Thermistor1 = A0;  //thermistor 1
int Thermistor2 = A1;  //thermistor 2
int Thermistor3 = A2;  //thermistor 3
int Thermistor4 = A3;  //thermistor 4
int ThermistorMag = A4;  //thermistor 5

int fanControl = 2;

//for stepper motor control
int mode1pin = 3;
int mode2pin = 4;
int stepPin = 5;
int dirPin = 6;

int pins[] = {ThermistorMag, Thermistor1, Thermistor2, Thermistor3, Thermistor4};
int data[5];

int increment = 4;  //the value to increment the fan. experiment will take 250/increment minutes to run
long stepTime = 60000;
int numTrials = 5;

//for stepper control waveform
int toggle1 = 0;
int timerDelay = 0;

//temp variables for adc read
uint8_t vlow, vhigh;
uint8_t hlow, hhigh;

//variable to keep track of the angle moved from the starting position. In this case vertical
int stepsFromStart = 0;

void setup() {
  
  pinMode(Thermistor1, INPUT);
  pinMode(Thermistor2, INPUT);
  pinMode(Thermistor3, INPUT);
  pinMode(Thermistor4, INPUT);
  pinMode(ThermistorMag, INPUT);

  pinMode(fanControl, OUTPUT);

  pinMode(mode1pin, OUTPUT);
  pinMode(mode2pin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  Serial.begin(115200);

  //configure for full step
  digitalWrite(mode1pin, LOW);
  digitalWrite(mode2pin, LOW);

  //configure for forwards
  digitalWrite(dirPin, HIGH);
  
  //setupt timer for stepper control
  cli();//stop interrupts

  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 1562;// = (16*10^6) / (10*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei();//allow interrupts

  //setup diff adc
  ADMUX = 1<<REFS0;                //choose AVCC for reference ADC voltage
  ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);    //enable ADC, ADC frequency=16MB/128=125kHz (set of prescaler)
  ADCSRB = 0x00;

  //set up the fan so it starts running early and can settle to a steady state
  analogWrite(fanControl, 90);
}

void loop() {

  static int loop = 1;

  delay(10000);

//resolution test
  while(loop){
    for(int k=0; k<numTrials; k++){

      Serial.println("250, 250, 250");

      delay(stepTime/2);
      
      //go clockwise a certain amount
      digitalWrite(dirPin, LOW);
      for(int i=0; i<increment; i++){  
        digitalWrite(stepPin,HIGH);
        delayMicroseconds(1000);
        digitalWrite(stepPin,LOW);
        delayMicroseconds(1000);
  
        //increment steps
        stepsFromStart++;
      }
  
      delay(stepTime);
  
      //then return to neutral
      digitalWrite(dirPin, HIGH);
      for(int i=0; i<increment; i++){  
        digitalWrite(stepPin,HIGH);
        delayMicroseconds(1000);
        digitalWrite(stepPin,LOW);
        delayMicroseconds(1000);
  
        //increment steps
        stepsFromStart--;
      }
      
      delay(stepTime/2);  //hold that position for 20 seconds
    }
    Serial.println("250, 250, 250");
    loop = 0;
  }

//  delay(5000);
//
////slowly rotate 360
//  //configure for backwards
//  digitalWrite(dirPin, LOW);
//  //200 steps per rotation
//  for(int i=0; i<200; i++){  
//    digitalWrite(stepPin,HIGH);
//    delayMicroseconds(64000);
//    digitalWrite(stepPin,LOW);
//    delayMicroseconds(64000);
//  }

  analogWrite(fanControl, 250);
  delay(50000);

}






ISR(TIMER1_COMPA_vect){//timer1 interrupt 1Hz toggles pin 13 (LED)
//  Serial.print(millis());  //time
//  Serial.print(", ");
//  Serial.print(analogRead(ThermistorMag));  //magnitude
//  Serial.print(", ");

  Serial.print(stepsFromStart);
  Serial.print(", ");

  ADCSRA |= (1 << ADIF);
  Serial.print(read_differential_vert());  //vertical differential read
  Serial.print(", ");

  ADCSRA |= (1 << ADIF);  //clear the adc interrupt flag
  Serial.println(read_differential_horiz());  //horizontal differential read
}




//int16_t read_differential() {
int read_differential_vert() {
//  ADMUX |= (1<<MUX4);             //set MUX5:0 to 010000. Positive Differential Input => ADC0 and Negative Differential Input => ADC1 with Gain 1x.

  ADMUX &= 0xF8;
//  ADMUX |= (0b001001);
  ADMUX |= (1<<MUX4);
           
  ADCSRA |= (1<<ADSC);            //start conversion
  while (ADCSRA & (1<<ADSC));     //wait untill coversion be completed(ADSC=0);

  vlow = ADCL;
  vhigh = ADCH;
  if(vhigh & (1<<1)){              //in differential mode our value is between -512 to 511 (not 0 to 1023). it means we have 9 bits and 10th bit is the sign bit. but because
    vhigh |= 0b11111110;           //the number of ADCH and ADCL bits are 10, for signed number we dont have repeatition of 1 in "ADCH" byte.
  }                               //so we repeat 1 Ourselves.:)
  return (vhigh << 8) | vlow;       //arrange (ADCH ADCL) to a 16 bit and return it's value.
}

//int16_t read_differential() {
int read_differential_horiz() {
//  ADMUX |= (1<<MUX4);             //set MUX5:0 to 010000. Positive Differential Input => ADC0 and Negative Differential Input => ADC1 with Gain 1x.
  ADMUX &= 0xF8;
//  ADMUX |= (0b001101);
  ADMUX |= (0b011011);
  
  ADCSRA |= (1<<ADSC);            //start conversion
  while (ADCSRA & (1<<ADSC));     //wait untill coversion be completed(ADSC=0);

  hlow = ADCL;
  hhigh = ADCH;
  if(hhigh & (1<<1)){              //in differential mode our value is between -512 to 511 (not 0 to 1023). it means we have 9 bits and 10th bit is the sign bit. but because
    hhigh |= 0b11111110;           //the number of ADCH and ADCL bits are 10, for signed number we dont have repeatition of 1 in "ADCH" byte.
  }                               //so we repeat 1 Ourselves.:)
  return (hhigh << 8) | hlow;       //arrange (ADCH ADCL) to a 16 bit and return it's value.
}
