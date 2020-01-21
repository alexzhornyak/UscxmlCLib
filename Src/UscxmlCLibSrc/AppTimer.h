#pragma once

#include <chrono>

#include <thread>
#include <mutex>
#include <shared_mutex>

class AppTimer
{
public:

	enum class State { atStopped, atStarted, atPaused, atResumed };

	AppTimer();
	virtual ~AppTimer();

	void start();

	void pause();

	void resume();

	void stop();

	void reset();

	std::chrono::nanoseconds getTime() const;

	State getState() const;

	enum NanoToSecConvertationPrecision { nscpMili, nscpMicro, nscpNano };
	static double NanosecondsToFloatSeconds(const std::chrono::nanoseconds &nano, const NanoToSecConvertationPrecision precision = nscpMicro);

private:
	
	bool _active = false;
	
	mutable std::shared_mutex _mtx;

	std::chrono::time_point<std::chrono::high_resolution_clock> _tp_start;
	std::chrono::time_point<std::chrono::high_resolution_clock> _tp_pause;
	std::chrono::nanoseconds _td_elapsed = std::chrono::nanoseconds::zero();
	std::chrono::nanoseconds _td_paused = std::chrono::nanoseconds::zero();

	State _state = State::atStopped;
	
};


