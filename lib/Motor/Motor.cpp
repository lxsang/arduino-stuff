#include "Motor.h"

Motor::Motor(int p1,int p2, int sp)
{
	this->m_pin1 = p1;
	this->m_pin2 = p2;
	this->m_speed_pin = sp;
	this->m_speed = 255;
	this->m_status = 0; // stop
}
void Motor::init()
{
	pinMode(this->m_pin1, OUTPUT);
	pinMode(this->m_pin2, OUTPUT);
	pinMode(this->m_speed_pin, OUTPUT);
    this->stop();
}
int Motor::getStatus()
{
    return this->m_status;
}
void Motor::move(int s)
{
    if(s != this->m_status) // then move
    {
        switch(s)
        {
            case 0: this->stop(); break;
            case 1: this->forward(); break;
            case 2: this->backward();break;
            default:this->stop();break;
        }
    }
}
void Motor::setSpeed(int sp)
{
    int _st = this->m_status;
    this->m_status = 0;
	this->m_speed = sp;
	this->move(_st);
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
	this->m_status = 0;
}
void Motor::forward()
{
        this->stop();
	digitalWrite(this->m_pin1, LOW);
	digitalWrite(this->m_pin2, HIGH);
	analogWrite(this->m_speed_pin,this->m_speed);
	this->m_status = 1;
}
void Motor::backward()
{
        this->stop();
	digitalWrite(this->m_pin1, HIGH);
	digitalWrite(this->m_pin2, LOW);
	analogWrite(this->m_speed_pin,this->m_speed);
	this->m_status = 2;
}

Motor::~Motor()
{
	//do nothing here
}
