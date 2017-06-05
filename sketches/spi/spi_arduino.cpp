// Written by Nick Gammon
// February 2011
/**
 * Send arbitrary number of bits at whatever clock rate (tested at 500 KHZ and 500 HZ).
 * This script will capture the SPI bytes, when a '\n' is recieved it will then output
 * the captured byte stream via the serial.
 */

#include "../../lib/SPI/SPI.h"
#define CMD_SIZE 5 
#define RES_SIZE 10
// command buffer 
byte cmd_buf [CMD_SIZE];
// response buffer
byte res_buf [RES_SIZE];

void setup (void)
{
  Serial.begin (9600);   // debugging
  SPCR |= _BV(SPE);
  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);
  // turn on SPI in slave mode
 Serial.println("Finish setup");
  // disable
  //SPI.attachInterrupt();

}  // end of setup

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
    Serial.println("reading sensor datas:");
    res_buf[0] = 0xFE;
    res_buf[1] = 347 & 0xFF;
    res_buf[2] = (347 >> 8) & 0xFF;
    res_buf[3] = 132 & 0xFF;
    res_buf[4] = (132 >> 8) & 0xFF;
    res_buf[5] = 520 & 0xFF;
    res_buf[6] = (520 >> 8) & 0xFF;
    res_buf[7] = 200;
    res_buf[8] = 255;
    res_buf[9] = 0xFF;
    
    SPI.transfer(res_buf,RES_SIZE);
}

void set_actuators()
{
    Serial.println("Setting actuators:");
    Serial.print("Status 1:");
    Serial.println(cmd_buf[1]);
    
    Serial.print("Speed 1:");
    Serial.println(cmd_buf[2]);
    
    Serial.print("Status 2:");
    Serial.println(cmd_buf[3]);
    
    Serial.print("Speed 2:");
    Serial.println(cmd_buf[4]);
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

// main loop - wait for flag set in interrupt routine
void loop (void)
{
    reset_buffer();
    read_cmd();
    process_cmd();
}  // end of loop








