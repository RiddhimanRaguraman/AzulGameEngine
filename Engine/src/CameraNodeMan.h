//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef CAMERA_NODE_MAN_H
#define CAMERA_NODE_MAN_H 

#include "ManBase.h"
#include "CameraNode.h"
#include "Camera.h"

namespace Azul
{

	class CameraNodeMan : public ManBase
	{
	public:
		// public interface --------------------------------------------------
		static void         Create(int reserveNum = 0, int reserveGrow = 1);
		static void         Destroy();

		static CameraNode* Add(Camera::Name _name, Camera* pCamera);
		static Camera* Find(Camera::Name _name);
		static void         Remove(CameraNode* pNode);
		static void         Update();                 // call once per frame
		static void         Dump();

		// big four deleted - manager lives through singleton ----------------
		CameraNodeMan(const CameraNodeMan&) = delete;
		CameraNodeMan& operator=(const CameraNodeMan&) = delete;
		virtual ~CameraNodeMan();

		static void         SetCurrent(Camera::Name name, Camera::Type type);
		static Camera*		GetCurrent(Camera::Type type);

	private:            // only singleton can create/destroy itself
		CameraNodeMan(int reserveNum, int reserveGrow);
		CameraNodeMan() = delete;
		
		static CameraNodeMan* privGetInstance();

		void ProcessInput();

		// ManBase override --------------------------------------------------
		virtual DLink* derivedCreateNode() override;

	private:
		CameraNode* poNodeCompare;
		static CameraNodeMan* posInstance;
		static CompareStrategyBase *posEnumNameCompare;

		Camera* pCamOrthographic;
		Camera* pCamPerspective;
	};
}

#endif

// --- End of File ---
