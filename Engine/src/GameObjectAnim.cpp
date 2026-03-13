//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Game.h"
#include "MathEngine.h"
#include "GameObjectAnim.h"
#include "GraphicsObject.h"
#include "Mesh.h"
#include "Anim.h"

namespace Azul
{

	GameObjectAnim::GameObjectAnim(GraphicsObject *pGraphicsObject, Bone *_pBoneResult)
		: GameObjectControlled(pGraphicsObject)
	{
		assert(pGraphicsObject);
		this->poTrans = new Vec3(0.0f, 0.0f, 0.0f);
		this->poScale = new Vec3(1.0f, 1.0f, 1.0f);
		this->poQuat = new Quat(0.0f, 0.0f, 0.0f, 1.0f);
		this->poLocal = new Mat4(Identity);
		this->poBoneOrientation = new Mat4(Identity);
		this->pBoneResult = _pBoneResult;
		assert(_pBoneResult);
		assert(poBoneOrientation);
		assert(this->poTrans);
		assert(this->poScale);
		assert(this->poQuat);
		assert(this->poLocal);
	}

	GameObjectAnim::~GameObjectAnim()
	{
		delete this->poTrans;
		delete this->poScale;
		delete this->poQuat;
		delete this->poLocal;
		delete this->poBoneOrientation;
	}

	void GameObjectAnim::privUpdate(AnimTime currentTime)
	{
		AZUL_UNUSED_VAR(currentTime);

		//Trace::out("index: %d \n", index);

		Trans T;
		Scale S;
		Quat  Q;

		// Get the parent matrix... if root, set to identity
		GameObjectAnim *pParentObject = (GameObjectAnim *)this->GetParent();
		Mat4 matParent(Identity);
		if(pParentObject)
		{
			matParent = *pParentObject->poWorld;

			// REMEMBER this is for Animation and hierachy, you need to handle models differently
			// Get the result bone array, from there make the matrix

			// Fix: Force root bone (index 0) to have no translation for in-place animation
			if (index == 0)
			{
				T.set(0, 0, 0);
			}
			else
			{
				T.set(this->pBoneResult[index].T);
			}
			
			S.set(this->pBoneResult[index].S);
			Q = this->pBoneResult[index].Q;
		}
		else
		{
			T.set(0, 0, 0);
			S.set(1, 1, 1);
			Q.set(Identity);
		}

		// world matrix
		*this->poLocal = S * Q * T;
		*this->poWorld = *poLocal * matParent;

	}

	void GameObjectAnim::Update(AnimTime currentTime)
	{
		// Goal: update the world matrix
		this->privUpdate(currentTime);
		Anim::PoseSkeleton(this);
		// update the bounding volume based on world matrix
		this->poGraphicsObject->SetWorld(*this->poWorld);
	}

	void GameObjectAnim::SetQuat(float qx, float qy, float qz, float qw)
	{
		this->poQuat->set(qx, qy, qz, qw);
	}

	void GameObjectAnim::SetScale(float sx, float sy, float sz)
	{
		this->poScale->set(sx, sy, sz);
	}

	void GameObjectAnim::SetTrans(float x, float y, float z)
	{
		this->poTrans->set(x, y, z);
	}

	void GameObjectAnim::SetIndex(int i)
	{
		this->index = i;
	}

	void GameObjectAnim::SetScale(Vec3 &r)
	{
		*this->poScale = r;
	}

	void GameObjectAnim::SetQuat(Quat &r)
	{
		*this->poQuat = r;
	}

	void GameObjectAnim::SetTrans(Vec3 &r)
	{
		*this->poTrans = r;
	}

	void GameObjectAnim::SetBoneOrientation(const Mat4 &tmp)
	{
		*this->poBoneOrientation = tmp;
	}

	Mat4 GameObjectAnim::GetBoneOrientation(void) const
	{
		// return a copy...
		return Mat4(*this->poBoneOrientation);
	}
}

// --- End of File ---
