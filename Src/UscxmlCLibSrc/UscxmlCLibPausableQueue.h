#pragma once

#include <list>

#include "uscxml/Interpreter.h"
#include "uscxml/interpreter/BasicEventQueue.h"
#include "uscxml/interpreter/BasicDelayedEventQueue.h"

class ScxmlBase;

namespace uscxml {

	class PausableDelayedEventQueue : public BasicDelayedEventQueue {
	public:
		PausableDelayedEventQueue(DelayedEventQueueCallbacks* callbacks, ScxmlBase *AScxmlBase);

		virtual std::shared_ptr<DelayedEventQueueImpl> create(DelayedEventQueueCallbacks* callbacks) override;

		void pause();

		void resume();

		bool isPaused();

	protected:
		timeval _pausedAt;
		ScxmlBase *_ScxmlBase;
	};
}

