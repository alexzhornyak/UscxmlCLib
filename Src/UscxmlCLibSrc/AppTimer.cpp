#include "AppTimer.h"

using namespace std::chrono;

AppTimer::AppTimer()
{	
}


AppTimer::~AppTimer()
{
}

void AppTimer::start()
{
	std::unique_lock<std::shared_mutex> lock(_mtx);
	_active = true;
	_td_elapsed = nanoseconds::zero();
	_td_paused = nanoseconds::zero();
	_tp_start = high_resolution_clock::now();
	_state = State::atStarted;
}

void AppTimer::pause()
{
	std::unique_lock<std::shared_mutex> lock(_mtx);
	_active = false;
	_tp_pause = high_resolution_clock::now();
	_td_elapsed = (high_resolution_clock::now() - _tp_start) - _td_paused;
	_state = State::atPaused;
}

void AppTimer::resume()
{
	std::unique_lock<std::shared_mutex> lock(_mtx);
	_active = true;
	_td_paused = _td_paused + (high_resolution_clock::now() - _tp_pause);
	_state = State::atResumed;
}

void AppTimer::stop()
{
	std::unique_lock<std::shared_mutex> lock(_mtx);
	_active = false;
	_td_elapsed = (high_resolution_clock::now() - _tp_start) - _td_paused;
	_state = State::atStopped;
}

void AppTimer::reset()
{
	std::unique_lock<std::shared_mutex> lock(_mtx);
	_active = false;
	_td_elapsed = std::chrono::nanoseconds::zero();
	_td_paused = std::chrono::nanoseconds::zero();
	_state = State::atStopped;
}

std::chrono::nanoseconds AppTimer::getTime() const
{
	std::shared_lock<std::shared_mutex> lock(_mtx);
	return _active ? ((high_resolution_clock::now() - _tp_start) - _td_paused) : _td_elapsed;
}

AppTimer::State AppTimer::getState() const {
	std::shared_lock<std::shared_mutex> lock(_mtx);
	return _state; 
}

double AppTimer::NanosecondsToFloatSeconds(const std::chrono::nanoseconds & nano, const NanoToSecConvertationPrecision precision)
{
	switch (precision) {
	case nscpMili:
		return (double)std::chrono::duration_cast<std::chrono::milliseconds>(nano).count() / 1000.0f;
	case nscpMicro:
		return (double)std::chrono::duration_cast<std::chrono::microseconds>(nano).count() / 1000000.0f;
	case nscpNano:
		return (double)std::chrono::duration_cast<std::chrono::microseconds>(nano).count() / 1000000000.0f;
	}
	return 0.0;
}

