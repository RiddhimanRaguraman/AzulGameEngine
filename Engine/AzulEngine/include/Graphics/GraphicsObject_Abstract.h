//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GRAPHICS_OBJECT_ABSTACT_H
#define GRAPHICS_OBJECT_ABSTACT_H

namespace Azul
{
	class GraphicsObject_Abstract
	{
	public:

	protected:
		GraphicsObject_Abstract() = default;
		GraphicsObject_Abstract(const GraphicsObject_Abstract &) = delete;
		GraphicsObject_Abstract &operator = (const GraphicsObject_Abstract &) = delete;
		virtual ~GraphicsObject_Abstract() = default;

		virtual void SetState() = 0;
		virtual void SetDataGPU() = 0;
		virtual void Draw() = 0;
		virtual void RestoreState() = 0;
	};
}

#endif

// --- End of File ---
