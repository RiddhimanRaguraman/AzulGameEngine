//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------
#include "SkelEntry.h"

namespace Azul
{
	SkelEntry::~SkelEntry()
	{

	}

	SkelEntry::SkelEntry()
		:nodeIndex(-1),
		parentIndex(-1),
		pNodeName{ 0 }
	{
		strcpy_s(this->pNodeName, SkelEntry::SKELETON_NAME_SIZE, "--Uninitialized--");
		// Make sure version is within the bytes for serialization
		assert(strlen(this->pNodeName) < SkelEntry::SKELETON_NAME_SIZE);
	}

	void SkelEntry::set(int _nodeIndex, int   _parentIndex, char* _pNodeName)
	{
		assert(_pNodeName);
		this->nodeIndex = _nodeIndex;
		this->parentIndex = _parentIndex;
		memcpy_s(this->pNodeName, SkelEntry::SKELETON_NAME_SIZE, _pNodeName, SkelEntry::SKELETON_NAME_SIZE);
	}

	void SkelEntry::Serialize(SkelEntry_proto& out) const
	{
		AZUL_UNUSED_VAR(out);

		out.set_nodeindex(this->nodeIndex);
		out.set_parentindex(this->parentIndex);

		std::string sName((const char*)this->pNodeName, SkelEntry::SKELETON_NAME_SIZE);
		out.set_pnodename(sName);
	}

	void SkelEntry::Deserialize(const SkelEntry_proto& in)
	{
		AZUL_UNUSED_VAR(in);

		this->nodeIndex = in.nodeindex();
		this->parentIndex = in.parentindex();

		memcpy_s(this->pNodeName, SkelEntry::SKELETON_NAME_SIZE, in.pnodename().data(), SkelEntry::SKELETON_NAME_SIZE);
	}

	void SkelEntry::Print(const char* const pName) const
	{
		AZUL_UNUSED_VAR(pName);
		Trace::out("{%2d, %2d, \"%s\" },\n",
			this->nodeIndex,
			this->parentIndex,
			this->pNodeName);
	}
}

// --- End of File ---


