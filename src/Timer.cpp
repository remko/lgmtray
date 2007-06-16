#include <algorithm>
#include <unistd.h>
#include <signal.h>

#include "Timer.h"

Timer::Timer() : interval_(0)
{
	signal(SIGALRM, Timer::signal_handler);
}

Timer* Timer::instance() 
{
	if (!instance_) {
		instance_ = new Timer();
	}
	return instance_;
}

void Timer::setInterval(unsigned int interval) 
{
	interval_ = interval;
	alarm(interval_);
}

void Timer::addListener(TimerListener* listener)
{
	if (std::find(listeners_.begin(),listeners_.end(),listener) == listeners_.end()) {
		listeners_.push_back(listener);
	}
}

void Timer::notifyListeners()
{
	for (std::vector<TimerListener*>::iterator it = listeners_.begin(); it != listeners_.end(); ++it) {
		(*it)->alarm();
	}
}

void Timer::signal_handler(int)
{
	Timer::instance()->notifyListeners();
	alarm(Timer::instance()->interval_);
}

Timer* Timer::instance_;
