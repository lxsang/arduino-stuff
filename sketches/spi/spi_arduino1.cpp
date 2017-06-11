// Written by Nick Gammon
// February 2011
/**
 * Send arbitrary number of bits at whatever clock rate (tested at 500 KHZ and 500 HZ).
 * This script will capture the SPI bytes, when a '\n' is recieved it will then output
 * the captured byte stream via the serial.
 */

#include "../../lib/SPI/SPI.h"
#include "../../lib/Motor/Motor.h"
// motor left
#define M1_D1 7 
#define M1_D2 8 
#define M1_S 6 
//motor right
#define M2_D1 5 
#define M2_D2 4 
#define M2_S 9 

// IR sensor
#define RF_L A7
#define RF_R A6
#define RF_TH 50

// sonar
#define S_TRIG A1
#define S_ECHO A0


#define RES 0xF0
#define UNK 0xF1
#define ACK 0xF2

#define CMD 0 
#define INDEX 1 
#define EXEC 2

#define S_IRL       0
#define S_IRR       1
#define S_SONAR_L   2
#define S_SONAR_H   3
#define S_ML        4
#define S_MR        5
#define S_MLS       6 
#define S_MRS       7

#define BUF_SIZE 16

byte m_cmd, m_idx, state = 0;// 0 cmd 1 value 

byte data_buffer[BUF_SIZE]; // 16 bytes, 8 bytes needed

// motor
Motor left_motor(M1_D1,M1_D2, M1_S);
Motor right_motor(M2_D1,M2_D2,M2_S);

void setup (void)
{
   // setup sonar sensor
  Serial.begin (9600);   // debugging
  SPCR |= _BV(SPE);
  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);
  // turn on SPI in slave mode

  // now turn on interrupts
  SPI.attachInterrupt();
  
  pinMode(S_TRIG, OUTPUT);
  pinMode(S_ECHO,INPUT);
  int i;
  for(i=0; i < BUF_SIZE;i++)
    data_buffer[i] = 0;
  state = CMD;
  left_motor.init();
  right_motor.init();
    //left_motor.setSpeed(speed);
    //right_motor.setSpeed(speed);
  left_motor.stop();
  right_motor.stop();
	
  Serial.println("Finish setup");
}  // end of setup


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

// SPI interrupt routine
ISR (SPI_STC_vect)
{
    byte d;
    byte c = SPDR;  // grab byte from SPI Data Register
    
    switch(state) // command 
    {
        case CMD:
            if(c == 0 || c == 1) 
            {
                m_cmd = c;
                state = INDEX;
            }
            break;
        case INDEX:
            m_idx = c;
            state = EXEC;
            break;
        case EXEC:
            if(m_cmd == 0) //read
            {
                d = data_buffer[m_idx];
                SPI.transfer(&d,1);
            }
            else if(m_cmd == 1) // write
            {
                switch(m_idx)
                {
                    case S_IRL:
                    case S_IRR:
                    case S_SONAR_L:
                    case S_SONAR_H: break; //read only value
                    default:data_buffer[m_idx] = c;break;
                }
            }
            state = CMD;
            break;
        default: state = CMD;
    }
    
}  // end of interrupt routine SPI_STC_vect

// main loop - read sensor data
void loop (void)
{
    data_buffer[S_IRL] = analogRead(RF_L) > 50?0:1;
    data_buffer[S_IRR] = analogRead(RF_R) > 50?0:1;
    long distance = sonar_read();
    data_buffer[S_SONAR_L] = distance & 0xFF;//low bytes
    data_buffer[S_SONAR_H] = (distance >> 8) & 0xFF;
    
    // actualtor driven 
    left_motor.setSpeed((int)data_buffer[S_MLS]);
    right_motor.setSpeed((int) data_buffer[S_MRS]);
    left_motor.move((int) data_buffer[S_ML]);
    right_motor.move((int) data_buffer[S_MR]);
    
    Serial.print("LEFT RF:");
    Serial.println(data_buffer[S_IRL]);
    
    Serial.print("RIGHT RF:");
    Serial.println(data_buffer[S_IRR]);
    
    Serial.print("sonar:");
    Serial.println(data_buffer[S_SONAR_L] + (data_buffer[S_SONAR_H] << 8));
    
    
    Serial.print("ML:");
    Serial.println(data_buffer[S_ML]);
    
    Serial.print("MR:");
     Serial.println(data_buffer[S_MR]);
    
    Serial.print("MLS:");
     Serial.println(data_buffer[S_MLS]);
    
    Serial.print("MRS:");
    Serial.println(data_buffer[S_MRS]);
    
    
    Serial.print("cmd:");
    Serial.println(m_cmd);
    
    Serial.print("idx:");
    Serial.println(m_idx);
    
    Serial.println("#######");
    
    delay(500);
}  // end of loop




