#include "Arduino.h"
/*
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(500);              // wait for a second
}*/

//#include "Arduino.h"

// define pin for sensor & motor

// IR sensor
#define RF_L A7
#define RF_R A6
#define RF_TH 50

// sonar
#define S_TRIG A1
#define S_ECHO A0

// motor
long sonar_read()
{
    long duration, distance;
    digitalWrite(S_TRIG, LOW);  
    delayMicroseconds(2); 
    digitalWrite(S_TRIG, HIGH);
    //  delayMicroseconds(1000); 
    delayMicroseconds(10); 
    digitalWrite(S_TRIG, LOW);
    duration = pulseIn(S_ECHO, HIGH);
    distance = (duration/2) / 29.1;
    //delay(50);
    return distance;
}
void setup()
{
    // setup sonar sensor
    pinMode(S_TRIG, OUTPUT);
    pinMode(S_ECHO,INPUT);
    // serial for debug
    Serial.begin(9600);
}

void loop()
{
    int left_rf_v = analogRead(RF_L);
    int right_rf_v = analogRead(RF_R);
    long distance = sonar_read();
    
    Serial.print("LEFT RF:");
    Serial.println(left_rf_v);
    
    Serial.print("RIGHT RF:");
    Serial.println(right_rf_v);
    
    Serial.print("sonar:");
    Serial.println(distance);
    
    Serial.println("#######");
    
    // add some delay
    delay(500);
}


