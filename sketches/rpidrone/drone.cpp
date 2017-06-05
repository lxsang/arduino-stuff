#include "Arduino.h"
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

#define CMD_SIZE 5 
#define RES_SIZE 10

// define pin for sensor & motor

// IR sensor
#define RF_L A7
#define RF_R A6
#define RF_TH 50

// sonar
#define S_TRIG A1
#define S_ECHO A0

// command buffer 
byte cmd_buf [CMD_SIZE];
// response buffer
byte res_buf [RES_SIZE];

// motor
Motor left_motor(M1_D1,M1_D2, M1_S);
Motor right_motor(M2_D1,M2_D2,M2_S);

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
    // SPI set up
    SPCR |= _BV(SPE);
    // have to send on master in, *slave out*
    pinMode(MISO, OUTPUT);
    // set up motor 
    left_motor.init();
    right_motor.init();
    //left_motor.setSpeed(speed);
    //right_motor.setSpeed(speed);
	left_motor.stop();
	right_motor.stop();
	
    Serial.println("Finish setup");
}

void reset_buffer()
{
    int i =0;
    for(i=0;i< CMD_SIZE;i++)
        cmd_buf[i]=0;
    for(i=0;i< RES_SIZE;i++)
        res_buf[i]=0;
}
void read_cmd()
{
    // readin 5 bytes
    int i = 0;
    for(i=0; i < CMD_SIZE; i++)
    {
        while(!(SPSR & (1<<SPIF)));    
        byte c = SPDR;
        cmd_buf[i] = c;
    }
}

void read_sensors()
{
    //Serial.println("reading sensor datas:");
    int left_rf_v = analogRead(RF_L);
    int right_rf_v = analogRead(RF_R);
    long distance = sonar_read();
    
    res_buf[0] = 0xFE;
    res_buf[1] = left_rf_v & 0xFF;
    res_buf[2] = (left_rf_v >> 8) & 0xFF;
    res_buf[3] = right_rf_v & 0xFF;
    res_buf[4] = (right_rf_v >> 8) & 0xFF;
    res_buf[5] = distance & 0xFF;
    res_buf[6] = (distance >> 8) & 0xFF;
    res_buf[7] = (byte) left_motor.getSpeed();
    res_buf[8] = (byte) right_motor.getSpeed();
    res_buf[9] = 0xFF;
    
    SPI.transfer(res_buf,RES_SIZE);
}

void motor_driven(Motor* m,byte stat, int speed)
{
    if(speed > 0)
        m->setSpeed(speed);
    switch(stat)
    {
        case 1: // go forward
            m->forward();
            //Serial.println("Go forward");
            break;
        case 2: // go backward
            //Serial.println("Go backward");
            m->backward();
            break;
        case 3: // stop
            //Serial.println("STOP");
            m->stop();
            break;
        default:;
    }
}

void set_actuators()
{
    byte m1_status = cmd_buf[1];
    byte m2_status = cmd_buf[3];
    int speed1 = (int) cmd_buf[2];
    int speed2 = (int) cmd_buf[4];
    motor_driven(&left_motor,m1_status,speed1);
    motor_driven(&right_motor,m2_status,speed2);
}


void process_cmd()
{
    byte status = cmd_buf[0];
    switch(status){
        case 0xF1: // read sensor
            read_sensors();
            break;
        case 0xF2: // set actuator
            set_actuators();
            break;
        default:
            // do nothing
            Serial.print("Invalid command:");
            Serial.println(status);
    }
}

void loop()
{
    reset_buffer();
    read_cmd();
    process_cmd();
    // add some delay
    //delay(100);
}

