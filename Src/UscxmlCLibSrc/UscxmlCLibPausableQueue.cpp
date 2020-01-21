#include "UscxmlClibPausableQueue.h"

#include "UscxmlClibBase.h"

namespace uscxml {
	
	PausableDelayedEventQueue::PausableDelayedEventQueue(DelayedEventQueueCallbacks* callbacks, ScxmlBase *AScxmlBase) :
		BasicDelayedEventQueue(callbacks), _ScxmlBase(AScxmlBase) {
		_pausedAt.tv_sec = 0;
		_pausedAt.tv_usec = 0;
	}

	std::shared_ptr<DelayedEventQueueImpl> PausableDelayedEventQueue::create(DelayedEventQueueCallbacks* callbacks) {
		std::shared_ptr<PausableDelayedEventQueue> queue(new PausableDelayedEventQueue(callbacks, _ScxmlBase));
		_ScxmlBase->_nestedDelayQueue.push_back(queue);
		
		return queue;
	}

	bool PausableDelayedEventQueue::isPaused() {
		return _pausedAt.tv_sec != 0 || _pausedAt.tv_usec != 0;
	}

	void PausableDelayedEventQueue::pause() {
		if (_pausedAt.tv_sec != 0 || _pausedAt.tv_usec != 0) {
			return; // we are already paused!
		}

		evutil_gettimeofday(&_pausedAt, NULL); // remember when we paused

		{
			// Verbatim copy of stop() without cancelAllDelayed()
			if (_isStarted) {
				_isStarted = false;
				event_base_loopbreak(_eventLoop);
			}
			if (_thread) {
				_thread->join();
				delete _thread;
				_thread = NULL;
			}
		}

		std::lock_guard<std::recursive_mutex> lock(_mutex);

		// remove all events from libevent without deleting them
		for (auto callbackData : _callbackData) {
			Event data = callbackData.second.userData;
			event_del(callbackData.second.event);
		}
	}

	void PausableDelayedEventQueue::resume() {		
		if (_pausedAt.tv_sec != 0 || _pausedAt.tv_usec != 0) {
			struct timeval now;
			struct timeval pausedFor;

			evutil_gettimeofday(&now, NULL);
			evutil_timersub(&now, &_pausedAt, &pausedFor);
			_pausedAt.tv_sec = 0;
			_pausedAt.tv_usec = 0;

			for (auto& callbackData : _callbackData) {
				// add the time we were paused to all due times
				evutil_timeradd(&callbackData.second.due, &pausedFor, &callbackData.second.due);

				struct timeval remain;
				evutil_timersub(&callbackData.second.due, &now, &remain);

				// reenqueue with libevent
				event_add(callbackData.second.event, &remain);
			}
		}
		start();
	}
}
