#ifndef CAMERA_NULL_H
#define CAMERA_NULL_H
#include "EngineDLLInterface.h"

#include "Camera.h"

namespace Azul
{

	class AZUL_ENGINE_LIBRARY_API CameraNull : public Camera
	{
	public:
		CameraNull();
		CameraNull(const CameraNull &) = delete;
		CameraNull &operator=(const CameraNull &other) = delete;
		virtual ~CameraNull();

		static CameraNull& Instance();


	private:

	};

}

#endif

// --- End of File ---
