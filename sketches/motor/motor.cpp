#include "../../lib/Motor/Motor.h"
#define M1_D1 7 
#define M1_D2 8 
#define M1_S 6 

#define M2_D1 5 
#define M2_D2 4 
#define M2_S 9 

Motor left_motor(M1_D1,M1_D2, M1_S);
Motor right_motor(M2_D1,M2_D2,M2_S);
int speed = 150;
void setup (void)
{
    Serial.begin (9600);   // debugging
    left_motor.init();
    right_motor.init();
    left_motor.setSpeed(speed);
    right_motor.setSpeed(speed);
	left_motor.stop();
	right_motor.stop();
    Serial.println("Finish setup");
    // disable
    //SPI.attachInterrupt();

}  // end of setup

void forward(){
    //Serial.println("Move forward");
    left_motor.forward();
    right_motor.forward();
}
void backward()
{
    //Serial.println("Move backward");
    left_motor.backward();
    right_motor.backward();
}
void rotate()
{
    //Serial.println("Rotate");
    left_motor.forward();
    right_motor.backward();
}
void stop()
{
    //Serial.println("Stop");
    left_motor.stop();
    right_motor.stop();
}
// main loop - wait for flag set in interrupt routine
void loop() {
  if(Serial.available())
  {
    
    char v = (char)Serial.read();
    switch(v)
    {
      case 'f':
        forward();
        break;
      case 'b':
        backward();
        break; 
      case 'r':
        rotate();
        break;
      case 's':
        stop();
        break;
    }
    
  } 
  //forward();
  delay(100);
}

