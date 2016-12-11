#include "Arduino.h"
#include "../../lib/Wire/Wire.h"

#define SLAVE_ADDRESS 0x04
int number = 0;
int state = 0;

void receiveData(int);
void sendData();

void setup() {
    pinMode(13, OUTPUT);
    Serial.begin(9600); // start serial for output
    // initialize i2c as slave
    Wire.begin(SLAVE_ADDRESS);

    // define callbacks for i2c communication
    Wire.onReceive(receiveData);
    Wire.onRequest(sendData);
    Serial.println("Ready!");
}

void loop() {
    //Serial.println("I2c test");
    delay(100);
}

void receiveData(int howMany) {
  while (0 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  //int x = Wire.read();    // receive byte as an integer
  //Serial.println(x);         // print the integer
}

// callback for sending data
void sendData(){
    Wire.write("hellor");
}



