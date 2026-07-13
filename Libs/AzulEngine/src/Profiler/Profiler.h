#ifndef AZUL_PROFILER_H
#define AZUL_PROFILER_H

#include "AnimTime.h"

#include "EngineDLLInterface.h"

namespace Azul
{
	// Lightweight rolling frame profiler.
	//
	// SystemMan feeds it the per-frame Update (systems) and Draw durations, and
	// the render systems bump a draw-call counter. Every kReportEveryFrames frames
	// EndFrame() prints the ROLLING AVERAGES (update ms, draw ms, draw calls) via
	// Trace::out and resets the accumulators -- so a running build shows a live
	// readout you can compare before/after each optimization (baseline vs batched
	// vs instanced ...). Overhead is two timer reads + a few adds per frame.
	class AZUL_ENGINE_LIBRARY_API Profiler
	{
	public:
		static void AddUpdate(const AnimTime &dt);   // systems (SystemMan::Run) time
		static void AddDraw(const AnimTime &dt);      // draw (SystemMan::Draw) time
		static void CountDraw();                      // one submitted draw call
		static void EndFrame();                       // call once per frame (after Draw)

		// Toggle the console readout (default on). Off zeroes the overhead too.
		static void SetEnabled(bool bEnabled);

	private:
		static const unsigned int kReportEveryFrames = 60;

		static bool sEnabled;
		static unsigned int sFrames;
		static float sUpdateSecSum;
		static float sDrawSecSum;
		static unsigned int sDrawCallSum;
	};
}

#endif

// --- End of File ---
