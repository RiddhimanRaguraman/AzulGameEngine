#ifndef TIMER_CONTROLLER_H
#define TIMER_CONTROLLER_H
#include "EngineDLLInterface.h"

#include "AnimTimer.h"

namespace Azul
{
	class AZUL_ENGINE_LIBRARY_API TimerController
	{
	public:
		TimerController(AnimTime Min, AnimTime Max);

		TimerController() = delete;
		TimerController(const TimerController &) = delete;
		TimerController &operator = (const TimerController &) = delete;
		virtual ~TimerController() = default;

		void Update(AnimTime tDelta);

		void SetCurrTime(const AnimTime time);
		AnimTime GetCurrTime() const;

		void SetMaxTime(const AnimTime time);
		AnimTime GetMaxTime() const;

		void SetMinTime(const AnimTime time);
		AnimTime GetMinTime() const;

	private:
		AnimTime tCurr;
		AnimTime tMax;
		AnimTime tMin;
	};
}

#endif

// --- End of File ---

