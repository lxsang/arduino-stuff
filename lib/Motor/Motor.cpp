#include "Motor.h"

Motor::Motor(int p1,int p2, int sp)
{
	this->m_pin1 = p1;
	this->m_pin2 = p2;
	this->m_speed_pin = sp;
	this->m_speed = 255;
}
void Motor::init()
{
	pinMode(this->m_pin1, OUTPUT);
	pinMode(this->m_pin2, OUTPUT);
	pinMode(this->m_speed_pin, OUTPUT);
    this->stop();
}
void Motor::setSpeed(int sp)
{
	this->m_speed = sp;
}
int Motor::getSpeed()
{
	return this->m_speed;
}
void Motor::stop()
{
	digitalWrite(this->m_pin1, LOW);
	digitalWrite(this->m_pin2, LOW);
	analogWrite(this->m_speed_pin,0);
}
void Motor::forward()
{
        this->stop();
	digitalWrite(this->m_pin1, LOW);
	digitalWrite(this->m_pin2, HIGH);
	analogWrite(this->m_speed_pin,this->m_speed);
}
void Motor::backward()
{
        this->stop();
	digitalWrite(this->m_pin1, HIGH);
	digitalWrite(this->m_pin2, LOW);
	analogWrite(this->m_speed_pin,this->m_speed);
}

Motor::~Motor()
{
	//do nothing here
}
