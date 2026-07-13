#include "Profiler.h"
#include "Framework.h"   // Trace::out

namespace Azul
{
	bool         Profiler::sEnabled = true;
	unsigned int Profiler::sFrames = 0;
	float        Profiler::sUpdateSecSum = 0.0f;
	float        Profiler::sDrawSecSum = 0.0f;
	unsigned int Profiler::sDrawCallSum = 0;

	void Profiler::SetEnabled(bool bEnabled)
	{
		sEnabled = bEnabled;
	}

	void Profiler::AddUpdate(const AnimTime &dt)
	{
		if (!sEnabled)
		{
			return;
		}
		sUpdateSecSum += dt / AnimTime(AnimTime::Duration::ONE_SECOND);
	}

	void Profiler::AddDraw(const AnimTime &dt)
	{
		if (!sEnabled)
		{
			return;
		}
		sDrawSecSum += dt / AnimTime(AnimTime::Duration::ONE_SECOND);
	}

	void Profiler::CountDraw()
	{
		if (!sEnabled)
		{
			return;
		}
		sDrawCallSum++;
	}

	void Profiler::EndFrame()
	{
		if (!sEnabled)
		{
			return;
		}

		sFrames++;
		if (sFrames < kReportEveryFrames)
		{
			return;
		}

		const float invFrames = 1.0f / (float)sFrames;
		const float updateMs = (sUpdateSecSum * invFrames) * 1000.0f;
		const float drawMs = (sDrawSecSum * invFrames) * 1000.0f;
		const float frameMs = updateMs + drawMs;
		const float drawCalls = (float)sDrawCallSum * invFrames;
		const float fps = (frameMs > 0.0f) ? (1000.0f / frameMs) : 0.0f;

		Trace::out("[PROFILE] frame %.3f ms (~%.0f fps cpu) | update %.3f ms | draw %.3f ms | %.1f draw calls\n",
			frameMs, fps, updateMs, drawMs, drawCalls);

		sFrames = 0;
		sUpdateSecSum = 0.0f;
		sDrawSecSum = 0.0f;
		sDrawCallSum = 0;
	}
}

// --- End of File ---
