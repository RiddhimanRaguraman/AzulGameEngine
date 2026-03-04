//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef CAMERA_H
#define CAMERA_H

#include "MathEngine.h"

namespace Azul
{

	class Camera : public Align16
	{
	public:
		enum class Type
		{
			PERSPECTIVE_3D,
			ORTHOGRAPHIC_2D
		};

		enum class Name
		{
			CAMERA_0,
			CAMERA_1,
			CAMERA_2,
			NOT_INITIALIZED,
			NullCamera
		};

	public:
		// Default constructor
		Camera() = delete;
		Camera(const Camera &) = delete;
		Camera &operator = (const Camera &) = delete;
		virtual ~Camera();
		Camera(Camera::Type _camType);

		// Setup on single camera
		void setPerspective(const float FieldOfView_Degs, const float AspectRatio, const float NearDist, const float FarDist);
		// 2D Camera
		void setOrthographic(const float xMin, const float xMax, const float yMin, const float yMax, const float zMin, const float zMax);

		
		void setViewport(const int inX, const int inY, const int width, const int height);
		void setOrientAndPosition(const Vec3 &Up_vect, const Vec3 &inLookAt_pt, const Vec3 &pos_pt);

		// update camera system
		void updateCamera(void);

		// Get the matrices for rendering
		Mat4 &getViewMatrix();
		Mat4 &getProjMatrix();

		// accessors
		void getPos(Vec3 &outPos) const;
		void getDir(Vec3 &outDir) const;
		void getUp(Vec3 &outUp) const;
		void getLookAt(Vec3 &outLookAt) const;
		void getRight(Vec3 &outRight) const;

		int getScreenWidth() const;
		int getScreenHeight() const;

		// Why no SETS?  Pos,Dir,Up,LookAt, Right
		//   They have to be adjust together in setOrientAndPosition()

		void getFieldOfView(float &Value) const;
		void setFieldOfView(const float Value);

		void getNearDist(float &Value) const;
		void setNearDist(const float Value);

		// accessors

		Camera::Type getType() const;

		char *GetName() const;
		void SetName(Camera::Name name);

		// helper functions
		void GetHelper(Vec3 &up, Vec3 &tar, Vec3 &pos, Vec3 &upNorm, Vec3 &forwardNorm, Vec3 &rightNorm);
		void SetHelper(Vec3 &up, Vec3 &tar, Vec3 &pos);

		void getNearTopLeft(Vec3 &vOut) const;
		void getNearTopRight(Vec3 &vOut) const;
		void getNearBottomLeft(Vec3 &vOut) const;
		void getNearBottomRight(Vec3 &vOut) const;
		void getFarTopLeft(Vec3 &vOut) const;
		void getFarTopRight(Vec3 &vOut) const;
		void getFarBottomLeft(Vec3 &vOut) const;
		void getFarBottomRight(Vec3 &vOut) const;
		float GetFovY() const
		{
			return this->fovy;
		}
		void SetFovY(float f)
		{
			this->fovy = f;
		}

	private:  // methods should never be public
		void privSetViewState(void);
		void privCalcPlaneHeightWidth(void);
		void privCalcFrustumVerts(void);
		void privCalcFrustumCollisionNormals(void);
		void privUpdateProjectionMatrix(void);
		void privUpdateViewMatrix(void);

	private:  // data  (Keep it private)

		char vPtrPad[8]; // virtual ptr adjustment to keep aligned

		// -------------------------------------------------------
		// 16-bit aligned data below this point:
		// -------------------------------------------------------

		// Projection Matrix
		Mat4	projMatrix;
		Mat4	viewMatrix;

		// camera unit vectors (up, dir, right)
		Vec3	vDir;
		Vec3	vRight;  // derived by up and dir


		Vec3	vUp;      // Up vector
		Vec3	vPos;     // pos pt
		Vec3	vLookAt;  // target pt

		// world space coords for viewing frustum
		Vec3	nearTopLeft;
		Vec3	nearTopRight;
		Vec3	nearBottomLeft;

		Vec3	nearBottomRight;
		Vec3	farTopLeft;
		Vec3	farTopRight;
		Vec3	farBottomLeft;

		Vec3	farBottomRight;

		// Normals of the frustum
		Vec3	frontNorm;
		Vec3	backNorm;
		Vec3	rightNorm;

		Vec3	leftNorm;
		Vec3	topNorm;
		Vec3	bottomNorm;

		// -------------------------------------------------------
		// unaligned data below this point:
		// -------------------------------------------------------

		Camera::Type camType;

		// Define the frustum inputs
		float	nearDist;
		float	farDist;
		float	fovy;  // aka view angle along y axis in radians
		float	aspectRatio;

		// Screen size in world space
		float	near_height;
		float	near_width;
		float	far_height;
		float	far_width;

		// Ortho hacks 
		float	xMin;
		float	yMin;
		float	zMin;
		float	xMax;
		float	yMax;
		float	zMax;

		// viewports
		int		viewport_x;
		int		viewport_y;
		int		viewport_width;
		int		viewport_height;

		// Name
	public:
		Camera::Name name;
		char pad[12];
	};

}


/*

void CameraNodeMan::ProcessInput()
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


*/

#endif

// --- End of File ---
