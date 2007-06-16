#ifndef TIMERLISTENER_H
#define TIMERLISTENER_H

class TimerListener
{
public:
	virtual ~TimerListener() { }
	virtual void alarm() = 0;
};

#endif
