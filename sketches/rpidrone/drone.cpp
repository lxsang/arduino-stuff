#include "Arduino.h"
// i2c header
#include "../../lib/Wire/Wire.h"

// define pin for sensor & motor

// IR sensor
#define RF_L A7
#define RF_R A6
#define RF_TH 50

// sonar
#define S_TRIG 12
#define S_ECHO 13

// motor

// use arduino as slave
#define SLAVE_ADDRESS 0x04

// i2c call back
void receiveData(int);
void sendData();
// I2c  buffer is 2 bytes
uint8_t buff[3] = {0};
// read sonar distance
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
    // setup i2c
    Wire.begin(SLAVE_ADDRESS);
    // define callbacks for i2c communication
    Wire.onReceive(receiveData);
    Wire.onRequest(sendData);
    // serial for debug
    Serial.begin(9600);
}

void loop()
{
    /*int left_rf_v = analogRead(RF_L);
    int right_rf_v = analogRead(RF_R);
    long distance = sonar_read();
    
    Serial.print("LEFT RF:");
    Serial.println(left_rf_v);
    
    Serial.print("RIGHT RF:");
    Serial.println(right_rf_v);
    
    Serial.print("sonar:");
    Serial.println(distance);
    
    Serial.println("#######");*/
    
    // add some delay
    delay(10);
}

void send_sensor_data()
{
   buff[0] = 0;
   int lrf =  analogRead(RF_L) > RF_TH?0:1;
   int rrf =  analogRead(RF_R) > RF_TH?0:1;
   long dist = sonar_read();
   
   buff[1] = (uint8_t)(rrf| (lrf<<1));
   Serial.println((int)buff[1]);
   
   buff[2] = (uint8_t)(dist > 200?200:dist);
   Serial.print("sonar:");
   Serial.println(dist);
   buff[0]= 1;
  // buff[2] = '\0';
   // data is now ready to be sent
}
void actuator_handler()
{
    // do nothing
}
void receiveData(int howMany) {
    // first by is the command
    char cmd = Wire.read();
    char c;
    switch(cmd)
    {
        case 'r': // read command
        Serial.println("User request sensor data");
        send_sensor_data();
        break;
        
        case 't': // motor control with speed
        Serial.println("User request to control the motors");
        actuator_handler();
        break;
        
        case 'd': // motor control with distance, not implemented yet
        default:
        while (0 < Wire.available()) { 
            char c = Wire.read(); 
             //Serial.print(c);         
        }
        Serial.println("Unkow command");
        break;
    }
  //int x = Wire.read();    // receive byte as an integer
  //Serial.println(x);         // print the integer
}

// callback for sending data
// alway 2 bytes data is sent back
void sendData(){
    Wire.write(buff,3);
    // reset status byte
    buff[0] = 0;
}

