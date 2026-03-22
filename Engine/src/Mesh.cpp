//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Engine.h"
#include "Mesh.h"
#include "StringThis.h"
#include "StateDirectXMan.h"
#include "ShaderMappings.h"

namespace Azul
{

	Mesh::Mesh(unsigned int _numVerts, unsigned int _numIndices)
		:
		name{ Azul::Mesh::Name::NOT_INITIALIZED },
		pModelName{ 0 },
		numVerts { _numVerts },
		numIndices { _numIndices }
	{
	}

	Mesh::~Mesh()
	{
	}

	void Mesh::Wash()
	{
	}

	bool Mesh::Compare(DLink *pTarget)
	{
		// This is used in ManBase.Find() 
		assert(pTarget != nullptr);

		Mesh *pDataB = (Mesh *) pTarget;

		bool status = false;

		if (this->name == pDataB->name)
		{
			status = true;
		}

		return status;
	}

	void Mesh::SetName(Mesh::Name _name)
	{
		this->name = _name;
	}

	char *Mesh::GetName()
	{
		// todo - Hack understand why is this needed for print and fix...
		static char pTmp[128];
		strcpy_s(pTmp, 128, StringMe(this->name));
		return pTmp;
	}
	
	void Mesh::ActivateSRVBuffers()
	{
		// Default - false
		assert(false);
	}

	void Mesh::ActivateConstantBuffers()
	{
		// Default - false
		assert(false);
	}

	void Mesh::TransferConstantBuffers()
	{
		// Default - false
		assert(false);
	}

	void Mesh::Initialize_SkinBoneWorldBuffer(size_t _structSize)
	{
		AZUL_UNUSED_VAR(_structSize);
		// Default - false
		assert(false);
	}

	void Mesh::Transfer_SkinBoneWorldBuffer(Mat4* p)
	{
		AZUL_UNUSED_VAR(p);
		// Default - false
		assert(false);
	}

}
// --- End of File ---
