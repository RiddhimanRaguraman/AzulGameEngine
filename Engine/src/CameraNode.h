//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef CAMERA_NODE_H
#define CAMERA_NODE_H

#include "DLink.h"
#include "Camera.h"

namespace Azul
{

	class CameraNode : public DLink
	{
	public:
		CameraNode();
		CameraNode(const CameraNode &) = delete;
		CameraNode &operator = (const CameraNode &) = delete;
		virtual ~CameraNode();

		void Set(Camera::Name name, Camera *pCam);
		void SetName(Camera::Name _name);
		Camera *GetCamera();

		virtual char *GetName() override;
		virtual void Wash() override;

		virtual void Dump() override;

	private:
		void privClear();

		//--------------------------------------------------
		// Data
		//--------------------------------------------------
		Camera *poCamera;
	};

}

#endif

// --- End of File ---
