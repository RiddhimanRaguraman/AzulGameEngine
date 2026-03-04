//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "Camera.h"
#include "StateDirectXMan.h"
#include "StringThis.h"

namespace Azul
{

	Camera::~Camera()
	{

	}

	void Camera::GetHelper(Vec3 &up, Vec3 &tar, Vec3 &pos,
						   Vec3 &upNorm, Vec3 &forwardNorm, Vec3 &pRightNorm)
	{
		this->getPos(pos);
		this->getLookAt(tar);
		this->getUp(upNorm);
		up = pos + upNorm;

		forwardNorm = tar - pos;
		forwardNorm.norm();

		this->getRight(pRightNorm);
	}

	void Camera::SetHelper(Vec3 &up_pt, Vec3 &tar_pt, Vec3 &pos_pt)
	{
		Vec3 upVect = up_pt - pos_pt;
		this->setOrientAndPosition(upVect, tar_pt, pos_pt);
	}

	Camera::Camera(Camera::Type _camType)
		: aspectRatio(0), farDist(0), fovy(0), nearDist(0)
	{
		//out("Camera(): ---------\n");
		this->name = Camera::Name::NOT_INITIALIZED;
		this->camType = _camType;
	}


	Camera::Type Camera::getType() const
	{
		return this->camType;
	}

	char *Camera::GetName() const
	{
		static char pTmp[128];
		strcpy_s(pTmp, 128, StringMe(this->name));
		return pTmp;
	}
	void Camera::SetName(Camera::Name inName)
	{
		this->name = inName;
	}

	// critical informational knobs for the perspective matrix
	// Field of View Y is in degrees (copying lame OpenGL)
	void Camera::setPerspective(const float Fovy, const float Aspect, const float NearDist, const float FarDist)
	{
		this->aspectRatio = Aspect;
		this->fovy = MATH_PI_180 * Fovy;
		this->nearDist = NearDist;
		this->farDist = FarDist;
	}

	void Camera::setOrthographic(const float _xMin, const float _xMax, const float _yMin, const float _yMax, const float _zMin, const float _zMax)
	{
		assert(this->camType == Camera::Type::ORTHOGRAPHIC_2D);

		this->xMin = _xMin;
		this->yMin = _yMin;
		this->zMin = _zMin;

		this->xMax = _xMax;
		this->yMax = _yMax;
		this->zMax = _zMax;

		this->far_height = this->yMax - this->yMin;
		this->far_width = this->xMax - this->xMin;
		this->near_width = this->yMax - this->yMin;
		this->nearDist = this->zMin;
		this->farDist = this->zMax;
	}

	int Camera::getScreenWidth() const
	{
		return this->viewport_width;
	}

	int Camera::getScreenHeight() const
	{
		return this->viewport_height;
	}

	// Just a pass through to setup the view port screen sub window
	void Camera::setViewport(const int inX, const int inY, const int width, const int height)
	{
		this->viewport_x = inX;
		this->viewport_y = inY;
		this->viewport_width = width;
		this->viewport_height = height;

		this->privSetViewState();
	};

	// Simple wrapper
	void Camera::privSetViewState(void)
	{
		D3D11_VIEWPORT tmp;

		tmp.Width = (float)this->viewport_width;
		tmp.Height = (float)this->viewport_height;
		tmp.TopLeftX = (float)this->viewport_x;
		tmp.TopLeftY = (float)this->viewport_y;
		tmp.MinDepth = 0.0f;
		tmp.MaxDepth = 1.0f;

		StateDirectXMan::GetContext()->RSSetViewports(1, &tmp);
	};

	// Goal, calculate the near height / width, same for far plane 
	void Camera::privCalcPlaneHeightWidth(void)
	{

		this->near_height = 2.0f * tanf((this->fovy * MATH_PI / 180.0f) * .5f) * this->nearDist;
		this->near_width = this->near_height * this->aspectRatio;

		this->far_height = 2.0f * tanf((this->fovy * MATH_PI / 180.0f) * .5f) * this->farDist;
		this->far_width = this->far_height * this->aspectRatio;

	};

	void Camera::setOrientAndPosition(const Vec3 &inUp, const Vec3 &inLookAt, const Vec3 &inPos)
	{
		// Remember the up, lookAt and right are unit length, and are perpendicular.
		// Treat lookAt as king, find Right vect, then correct Up to insure perpendiculare.
		// Make sure that all vectors are unit vectors.

		this->vLookAt = inLookAt;
		this->vDir = -(inLookAt - inPos); // Right-Hand camera: vDir is flipped
		this->vDir.norm();

		// Clean up the vectors (Right hand rule)
		this->vRight = inUp.cross(this->vDir);
		this->vRight.norm();

		this->vUp = this->vDir.cross(this->vRight);
		this->vUp.norm();

		this->vPos = inPos;
	};

	void Camera::privCalcFrustumVerts(void)
	{
		// Top Left corner and so forth.  In this form to see the pattern
		// Might be confusing (remember the picture) vDir goes from screen into your EYE
		// so distance from the eye is "negative" vDir
		this->nearTopLeft = this->vPos + this->vDir * this->nearDist + this->vUp * this->near_height * 0.5f - this->vRight * this->near_width * 0.5f;
		this->nearTopRight = this->vPos + this->vDir * this->nearDist + this->vUp * this->near_height * 0.5f + this->vRight * this->near_width * 0.5f;
		this->nearBottomLeft = this->vPos + this->vDir * this->nearDist - this->vUp * this->near_height * 0.5f - this->vRight * this->near_width * 0.5f;
		this->nearBottomRight = this->vPos + this->vDir * this->nearDist - this->vUp * this->near_height * 0.5f + this->vRight * this->near_width * 0.5f;
		this->farTopLeft = this->vPos + this->vDir * this->farDist + this->vUp * this->far_height * 0.5f - this->vRight * this->far_width * 0.5f;
		this->farTopRight = this->vPos + this->vDir * this->farDist + this->vUp * this->far_height * 0.5f + this->vRight * this->far_width * 0.5f;
		this->farBottomLeft = this->vPos + this->vDir * this->farDist - this->vUp * this->far_height * 0.5f - this->vRight * this->far_width * 0.5f;
		this->farBottomRight = this->vPos + this->vDir * this->farDist - this->vUp * this->far_height * 0.5f + this->vRight * this->far_width * 0.5f;
	};

	void Camera::privCalcFrustumCollisionNormals(void)
	{
		// Normals of the frustum around nearTopLeft
		Vec3 A = this->nearBottomLeft - this->nearTopLeft;
		Vec3 B = this->nearTopRight - this->nearTopLeft;
		Vec3 C = this->farTopLeft - this->nearTopLeft;

		this->frontNorm = A.cross(B);
		this->frontNorm.norm();

		this->leftNorm = C.cross(A);
		this->leftNorm.norm();

		this->topNorm = B.cross(C);
		this->topNorm.norm();

		// Normals of the frustum around farBottomRight
		A = this->farBottomLeft - this->farBottomRight;
		B = this->farTopRight - this->farBottomRight;
		C = this->nearBottomRight - this->farBottomRight;

		this->backNorm = A.cross(B);
		this->backNorm.norm();

		this->rightNorm = B.cross(C);
		this->rightNorm.norm();

		this->bottomNorm = C.cross(A);
		this->bottomNorm.norm();
	};

	// The projection matrix (note it's invertable)
	void Camera::privUpdateProjectionMatrix(void)
	{
		if (this->camType == Camera::Type::PERSPECTIVE_3D)
		{
			float d = 1.0f / tanf(fovy / 2.0f);

			this->projMatrix[m0] = d / aspectRatio;
			this->projMatrix[m1] = 0.0f;
			this->projMatrix[m2] = 0.0f;
			this->projMatrix[m3] = 0.0f;

			this->projMatrix[m4] = 0.0f;
			this->projMatrix[m5] = d;
			this->projMatrix[m6] = 0.0f;
			this->projMatrix[m7] = 0.0f;

			this->projMatrix[m8] = 0.0f;
			this->projMatrix[m9] = 0.0f;
			this->projMatrix[m10] = -(this->farDist + this->nearDist) / (this->farDist - this->nearDist);
			this->projMatrix[m11] = -1.0f;

			this->projMatrix[m12] = 0.0f;
			this->projMatrix[m13] = 0.0f;
			this->projMatrix[m14] = (-2.0f * this->farDist * this->nearDist) / (this->farDist - this->nearDist);
			this->projMatrix[m15] = 0.0f;

			// Converting from OpenGL-style NDC of [-1,1] to DX's [0,1].  See note: https://anteru.net/blog/2011/12/27/1830/
			// (Note: NDCConvert should be precomputed once and stored for reuse)
			Trans B(0.0f, 0.0f, 1.0f);
			Scale S(1.0f, 1.0f, 0.5f);

			projMatrix = projMatrix * B * S;
		}
		else
		{
			this->projMatrix[m0] = 2.0f / (xMax - xMin);
			this->projMatrix[m1] = 0.0f;
			this->projMatrix[m2] = 0.0f;
			this->projMatrix[m3] = 0.0f;

			this->projMatrix[m4] = 0.0f;
			this->projMatrix[m5] = 2.0f / (yMax - yMin);
			this->projMatrix[m6] = 0.0f;
			this->projMatrix[m7] = 0.0f;

			this->projMatrix[m8] = 0.0f;
			this->projMatrix[m9] = 0.0f;
			this->projMatrix[m10] = -1.0f / (zMax - zMin); // -2.0f / (zMax - zMin);
			this->projMatrix[m11] = 0.0f;

			this->projMatrix[m12] = 0;
			this->projMatrix[m13] = 0;
			this->projMatrix[m14] = -(zMin) / (zMax - zMin); //-(zMax + zMin) / (zMax - zMin);
			this->projMatrix[m15] = 1.0f;
		}
	};

	void Camera::privUpdateViewMatrix(void)
	{
		// This functions assumes the your vUp, vRight, vDir are still unit
		// And perpendicular to each other

		// Set for DX Right-handed space
		this->viewMatrix[m0] = this->vRight[x];
		this->viewMatrix[m1] = this->vUp[x];
		this->viewMatrix[m2] = this->vDir[x];
		this->viewMatrix[m3] = 0.0f;

		this->viewMatrix[m4] = this->vRight[y];
		this->viewMatrix[m5] = this->vUp[y];
		this->viewMatrix[m6] = this->vDir[y];
		this->viewMatrix[m7] = 0.0f;

		this->viewMatrix[m8] = this->vRight[z];
		this->viewMatrix[m9] = this->vUp[z];
		this->viewMatrix[m10] = this->vDir[z];
		this->viewMatrix[m11] = 0.0f;

		// Apply R^t( -Pos ). Use dot-product with the basis vectors
		this->viewMatrix[m12] = -vPos.dot(vRight);
		this->viewMatrix[m13] = -vPos.dot(vUp);
		this->viewMatrix[m14] = -vPos.dot(vDir);
		this->viewMatrix[m15] = 1.0f;
	};

	// Update everything (make sure it's consistent)
	void Camera::updateCamera(void)
	{
		// First find the near height/width, far height/width
		this->privCalcPlaneHeightWidth();

		// Find the frustum physical verts
		this->privCalcFrustumVerts();

		// find the frustum collision normals
		this->privCalcFrustumCollisionNormals();

		// update the projection matrix
		this->privUpdateProjectionMatrix();

		// update the view matrix
		this->privUpdateViewMatrix();
	}

	// Accessor mess:
	Mat4 &Camera::getViewMatrix(void)
	{
		return this->viewMatrix;
	}

	Mat4 &Camera::getProjMatrix(void)
	{
		return this->projMatrix;
	}

	void Camera::getPos(Vec3 &outPos) const
	{
		outPos = this->vPos;
	}

	void  Camera::getDir(Vec3 &outDir) const
	{
		outDir = this->vDir;
	}

	void  Camera::getUp(Vec3 &outUp) const
	{
		outUp = this->vUp;
	}

	void Camera::getLookAt(Vec3 &outLookAt) const
	{
		outLookAt = this->vLookAt;
	}

	void Camera::getRight(Vec3 &outRight) const
	{
		outRight = this->vRight;
	}

	void Camera::getFieldOfView(float &Value) const
	{
		Value = this->aspectRatio;
	}

	void Camera::setFieldOfView(const float Value)
	{
		this->aspectRatio = Value;
	}

	void Camera::getNearDist(float &Value) const
	{
		Value = this->nearDist;
	}

	void Camera::setNearDist(const float Value)
	{
		this->nearDist = Value;
	}

	void Camera::getNearTopLeft(Vec3 &vOut) const
	{
		vOut = this->nearTopLeft;
	}

	void Camera::getNearTopRight(Vec3 &vOut) const
	{
		vOut = this->nearTopRight;
	}

	void Camera::getNearBottomLeft(Vec3 &vOut)const
	{
		vOut = this->nearBottomLeft;
	}

	void Camera::getNearBottomRight(Vec3 &vOut) const
	{
		vOut = this->nearBottomRight;
	}

	void Camera::getFarTopLeft(Vec3 &vOut) const
	{
		vOut = this->farTopLeft;
	}

	void Camera::getFarTopRight(Vec3 &vOut) const
	{
		vOut = this->farTopRight;
	}

	void Camera::getFarBottomLeft(Vec3 &vOut) const
	{
		vOut = this->farBottomLeft;
	}

	void Camera::getFarBottomRight(Vec3 &vOut)const
	{
		vOut = this->farBottomRight;
	}
}

// --- End of File ---
