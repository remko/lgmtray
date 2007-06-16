#ifndef TIMER_H
#define TIMER_H

#include <vector>

#include "TimerListener.h"

class Timer 
{
public:
	static Timer* instance();

	void setInterval(unsigned int secs);
	void addListener(TimerListener* listener);

protected:
	static void signal_handler(int);
	void notifyListeners();

private:
	Timer();

	static Timer* instance_;
	unsigned int interval_;
	std::vector<TimerListener*> listeners_;
};

#endif

