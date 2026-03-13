//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "CameraMan.h"
#include "CameraNull.h"
#include "ManBase.h"
#include "DLinkMan.h"
#include "CameraNodeCompareStrategyEnumName.h"

namespace Azul
{
	CameraMan *CameraMan::posInstance = nullptr;
	CompareStrategyBase *CameraMan::posEnumNameCompare = nullptr;

	//----------------------------------------------------------------------
	// Constructor
	//----------------------------------------------------------------------
	CameraMan::CameraMan(int reserveNum, int reserveGrow)
		: ManBase(new DLinkMan(), new DLinkMan(), reserveNum, reserveGrow)
	{
		// Preload the reserve
		this->proFillReservedPool(reserveNum);

		// initialize derived data here
		Camera *pCam = new CameraNull();
		assert(pCam);
		this->poNodeCompare = new CameraNode();
		assert(this->poNodeCompare);
		this->poNodeCompare->Set(Camera::Name::NullCamera, pCam);
	}

	CameraMan::~CameraMan()
	{
		delete this->poNodeCompare;
		this->poNodeCompare = nullptr;

		// iterate through the list and delete
		Iterator *pIt = this->baseGetActiveIterator();

		DLink *pNode = pIt->First();

		// Walk through the nodes
		while(!pIt->IsDone())
		{
			CameraNode *pDeleteMe = (CameraNode *)pIt->Curr();
			pNode = pIt->Next();
			delete pDeleteMe;
		}

		pIt = this->baseGetReserveIterator();

		pNode = pIt->First();

		// Walk through the nodes
		while(!pIt->IsDone())
		{
			CameraNode *pDeleteMe = (CameraNode *)pIt->Curr();
			pNode = pIt->Next();
			delete pDeleteMe;
		}
	}

// w1(7)
	void CameraMan::SetCurrent(const Camera::Name name, Camera::Type type)
	{
		CameraMan *pCamMan = CameraMan::privGetInstance();
		assert(pCamMan);

		Camera *pCam = CameraMan::Find(name);
		assert(pCam);

		// w1(7)
		switch(type)
		{
		case Camera::Type::PERSPECTIVE_3D:
			assert(pCam->getType() == Camera::Type::PERSPECTIVE_3D);
			pCamMan->pCamPerspective = pCam;
			break;

		case Camera::Type::ORTHOGRAPHIC_2D:
			assert(pCam->getType() == Camera::Type::ORTHOGRAPHIC_2D);
			pCamMan->pCamOrthographic = pCam;
			break;

		default:
			assert(false);
			break;
		}

	}

// w1(7)
	Camera *CameraMan::GetCurrent(Camera::Type type)
	{
		Camera *pCam = nullptr;
		switch(type)
		{
		case Camera::Type::PERSPECTIVE_3D:
			pCam = CameraMan::privGetInstance()->pCamPerspective;
			break;

		case Camera::Type::ORTHOGRAPHIC_2D:
			pCam = CameraMan::privGetInstance()->pCamOrthographic;
			break;

		default:
			assert(false);
			break;
		}

		assert(pCam != nullptr);
		return pCam;
	}

	//----------------------------------------------------------------------
	// Static Methods
	//----------------------------------------------------------------------
	void CameraMan::Create(int reserveNum, int reserveGrow)
	{
		// make sure values are ressonable 
		assert(reserveNum >= 0);
		assert(reserveGrow > 0);

		// initialize the singleton here
		assert(posInstance == nullptr);

		// Do the initialization
		if(posInstance == nullptr)
		{
			posInstance = new CameraMan(reserveNum, reserveGrow);
		}

		if (posEnumNameCompare == nullptr)
		{
			posEnumNameCompare = new CameraNodeCompareStrategyEnumName();
		}
	}

	void CameraMan::Destroy()
	{
		CameraMan *pMan = CameraMan::privGetInstance();
		assert(pMan != nullptr);
		AZUL_UNUSED_VAR(pMan);

		delete CameraMan::posEnumNameCompare;
		CameraMan::posEnumNameCompare = nullptr;

		delete CameraMan::posInstance;
		CameraMan::posInstance = nullptr;
	}

	CameraNode *CameraMan::Add(Camera::Name name, Camera *pCam)
	{
		CameraMan *pMan = CameraMan::privGetInstance();

		assert(pCam);

		CameraNode *pNode = (CameraNode *)pMan->baseAddToFront();
		assert(pNode != nullptr);

		// Initialize the date
		pNode->Set(name, pCam);

		return pNode;
	}

	Camera *CameraMan::Find(Camera::Name _name)
	{
		CameraMan *pMan = CameraMan::privGetInstance();
		assert(pMan != nullptr);

		// set strategy
		pMan->pCompareStrategy = CameraMan::posEnumNameCompare;
		assert(pMan->pCompareStrategy);

		Camera *pCam = nullptr;

		// Compare functions only compares two Nodes

		// So:  Use the Compare Node - as a reference
		//      use in the Compare() function
		Camera *pCamera = pMan->poNodeCompare->GetCamera();
		assert(pCamera);

		pCamera->name = _name;

		CameraNode *pData = (CameraNode *)pMan->baseFind(pMan->poNodeCompare);
		if(pData)
		{
			pCam = pData->GetCamera();
		}

		return pCam;
	}

	void CameraMan::Remove(CameraNode *pNode)
	{
		assert(pNode != nullptr);

		CameraMan *pMan = CameraMan::privGetInstance();
		assert(pMan != nullptr);

		pMan->baseRemove(pNode);
	}

	void CameraMan::Dump()
	{
		CameraMan *pMan = CameraMan::privGetInstance();
		assert(pMan != nullptr);

		pMan->baseDump();
	}

	//----------------------------------------------------------------------
	// Private methods
	//----------------------------------------------------------------------
	CameraMan *CameraMan::privGetInstance()
	{
		// Safety - this forces users to call Create() first before using class
		assert(posInstance != nullptr);

		return posInstance;
	}

	//----------------------------------------------------------------------
	// Override Abstract methods
	//----------------------------------------------------------------------
	DLink *CameraMan::derivedCreateNode()
	{
		DLink *pNodeBase = new CameraNode();
		assert(pNodeBase != nullptr);

		return pNodeBase;
	}

  

    void CameraMan::ProcessInput()
        {
            // grab the one active camera
            // Assuming we control the perspective camera with inputs
            Camera* pCam = this->pCamPerspective;
            if (!pCam)
                return;

            // get its helper data
            Vec3  up, tar, pos;
            Vec3  upNorm, forwardNorm, rightNorm;
            pCam->GetHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

            const float moveSpeed = 0.1f;
            const float zoomSpeed = 0.05f;    // adjust to taste
            bool    moved = false;

            // Mouse input: separate control per button
            static bool  isDragLeft   = false;
            static bool  isDragRight  = false;
            static bool  isDragMiddle = false;
            static POINT lastLeft     = { 0, 0 };
            static POINT lastRight    = { 0, 0 };
            static POINT lastMiddle   = { 0, 0 };

            const float mouseSensitivity = 0.005f;

            // Left button: Y-axis rotation (yaw around up axis) using horizontal movement
            if (GetKeyState(VK_LBUTTON) & 0x8000)
            {
                POINT cur; GetCursorPos(&cur);
                if (!isDragLeft)
                {
                    isDragLeft = true; lastLeft = cur;
                }
                else
                {
                    int deltaX = cur.x - lastLeft.x;
                    if (abs(deltaX) > 0)
                    {
                        float yawDelta = deltaX * mouseSensitivity;
                        Trans  T1(tar);
                        Trans  T2(-tar);
                        Rot    R;  R.set(Axis::AxisAngle, upNorm, -yawDelta);
                        Mat4   M = T2 * R * T1;
                        pos = Vec4(pos, 1.0f) * M;
                        tar = Vec4(tar, 1.0f) * M;
                        up  = Vec4(up, 1.0f) * M;
                        moved = true;
                    }
                    lastLeft = cur;
                }
            }
            else
            {
                isDragLeft = false;
            }

            // Right button: Z-axis rotation (roll around forward axis) using horizontal movement
            if (GetKeyState(VK_RBUTTON) & 0x8000)
            {
                POINT cur; GetCursorPos(&cur);
                if (!isDragRight)
                {
                    isDragRight = true; lastRight = cur;
                }
                else
                {
                    int deltaX = cur.x - lastRight.x;
                    if (abs(deltaX) > 0)
                    {
                        float rollDelta = deltaX * mouseSensitivity;
                        Trans  T1(tar);
                        Trans  T2(-tar);
                        Rot    R;  R.set(Axis::AxisAngle, forwardNorm, -rollDelta);
                        Mat4   M = T2 * R * T1;
                        pos = Vec4(pos, 1.0f) * M;
                        tar = Vec4(tar, 1.0f) * M;
                        up  = Vec4(up, 1.0f) * M;
                        moved = true;
                    }
                    lastRight = cur;
                }
            }
            else
            {
                isDragRight = false;
            }

            // Middle button: X-axis rotation (pitch around right axis) using vertical movement
            if (GetKeyState(VK_MBUTTON) & 0x8000)
            {
                POINT cur; GetCursorPos(&cur);
                if (!isDragMiddle)
                {
                    isDragMiddle = true; lastMiddle = cur;
                }
                else
                {
                    int deltaY = cur.y - lastMiddle.y;
                    if (abs(deltaY) > 0)
                    {
                        float pitchDelta = deltaY * mouseSensitivity;
                        Trans  T1(tar);
                        Trans  T2(-tar);
                        Rot    R;  R.set(Axis::AxisAngle, rightNorm, pitchDelta);
                        Mat4   M = T2 * R * T1;
                        pos = Vec4(pos, 1.0f) * M;
                        tar = Vec4(tar, 1.0f) * M;
                        up  = Vec4(up, 1.0f) * M;
                        moved = true;
                    }
                    lastMiddle = cur;
                }
            }
            else
            {
                isDragMiddle = false;
            }

            // Keyboard input for camera translation (existing code)
            if (GetKeyState('W') & 0x8000)
            {
                Vec3 delta = forwardNorm * moveSpeed;
                pos = pos + delta;
                tar = tar + delta;
                up  = up  + delta;
                moved = true;
            }

            if (GetKeyState('S') & 0x8000)
            {
                Vec3 delta = forwardNorm * moveSpeed;
                pos = pos - delta;
                tar = tar - delta;
                up  = up  - delta;
                moved = true;
            }

            if (GetKeyState('A') & 0x8000)
            {
                Vec3 delta = rightNorm * moveSpeed;
                pos = pos - delta;
                tar = tar - delta;
                up  = up  - delta;
                moved = true;
            }

            if (GetKeyState('D') & 0x8000)
            {
                Vec3 delta = rightNorm * moveSpeed;
                pos = pos + delta;
                tar = tar + delta;
                up  = up  + delta;
                moved = true;
            }

            if (GetKeyState('E') & 0x8000)
            {
                Vec3 delta = upNorm * moveSpeed;
                pos = pos + delta;
                tar = tar + delta;
                up  = up  + delta;
                moved = true;
            }

            if (GetKeyState('Q') & 0x8000)
            {
                Vec3 delta = upNorm * moveSpeed;
                pos = pos - delta;
                tar = tar - delta;
                up  = up  - delta;
                moved = true;
            }
            // zoom (fish-eye)
            if (GetKeyState('C') & 0x8000)
            {
                float fov = pCam->GetFovY();
                pCam->SetFovY(fov - zoomSpeed);
            }
            if (GetKeyState('Z') & 0x8000)
            {
                float fov = pCam->GetFovY();
                pCam->SetFovY(fov + zoomSpeed);
            }

            // if anything changed, push it back into the camera
            if (moved)
            {
                pCam->SetHelper(up, tar, pos);
            }
        }

        void CameraMan::InputUpdate()
        {
            CameraMan* pMan = CameraMan::privGetInstance();
            pMan->ProcessInput();
        }



}

// --- End of File ---





