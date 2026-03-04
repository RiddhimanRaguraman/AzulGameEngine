//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef MESH_MAN_H
#define MESH_MAN_H 

#include "ManBase.h"
#include "MeshNode.h"
#include "Mesh.h"
#include "MeshProto.h"

namespace Azul
{
	class MeshNodeMan : public ManBase
	{
		//----------------------------------------------------------------------
		// Static Methods
		//----------------------------------------------------------------------
	public:
		static void Create(int reserveNum = 0, int reserveGrow = 1);
		static void Destroy();

		static MeshNode *Add(Mesh::Name name, Mesh *pMesh);
		static Mesh *Find(Mesh::Name name);
		static Mesh *Find(const char *pModelName);

		static void Remove(MeshNode *pNode);
		static void Dump();

		//----------------------------------------------------------------------
		// Private methods
		//----------------------------------------------------------------------
	private:
		static MeshNodeMan *privGetInstance();

		MeshNodeMan() = delete;
		MeshNodeMan(const MeshNodeMan &) = delete;
		MeshNodeMan &operator = (const MeshNodeMan &) = delete;

		virtual ~MeshNodeMan();

		MeshNodeMan(int reserveNum, int reserveGrow);

		//----------------------------------------------------------------------
		// Override Abstract methods
		//----------------------------------------------------------------------
	protected:
		DLink *derivedCreateNode() override;

		//----------------------------------------------------------------------
		// Data: unique data for this manager 
		//----------------------------------------------------------------------
	private:
		MeshNode *poNodeCompare;
		static MeshNodeMan *posInstance;
		static CompareStrategyBase *posEnumNameCompare;
		static CompareStrategyBase *posCharNameCompare;

	};
}

#endif

// --- End of File ---
