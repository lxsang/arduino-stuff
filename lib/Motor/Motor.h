#include <Arduino.h>

class Motor
{
public:
	Motor (int,int, int);
	virtual ~Motor ();
	void init();
	void forward();
	void backward();
	void stop();
	void setSpeed(int);
	int getSpeed();
private:
	int m_pin1;
	int m_pin2;
	int m_speed_pin;
	int m_speed;
};
