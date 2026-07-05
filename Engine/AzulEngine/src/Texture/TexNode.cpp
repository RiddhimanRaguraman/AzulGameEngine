//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "TexNode.h"
#include "StringThis.h"

namespace Azul
{

	TexNode::TexNode()
		: DLink(),
		poTextureObject(nullptr)
	{
		this->privClear();
	}

	TexNode::~TexNode()
	{
		this->privClear();
	}

	void TexNode::Set(TextureObject::Name name, TextureObject *pTextureObject)
	{
		assert(pTextureObject);
		this->poTextureObject = pTextureObject;

		this->poTextureObject->SetName(name);
	}

	TextureObject *TexNode::GetTextureObject()
	{
		return this->poTextureObject;
	}

	void TexNode::privClear()
	{
		if(this->poTextureObject)
		{
			delete this->poTextureObject;
		}
		this->poTextureObject = nullptr;
	}

	char *TexNode::GetName()
	{
		char *pName = nullptr;
		if(this->poTextureObject)
		{
			pName = this->poTextureObject->GetName();
		}

		return pName;
	}

	void TexNode::Wash()
	{
		this->privClear();
	}



	void TexNode::Dump()
	{
		Trace::out("      TexNode(%p)\n", this);

		// Data:
		if(this->poTextureObject)
		{
			Trace::out("      TextureObject(%p) \n", this->poTextureObject);
			Trace::out("      Name: %s \n", StringMe(this->poTextureObject->mName));
		}
		else
		{
			Trace::out("      Name: %s \n", "null");
		}

		DLink::Dump();
	}
}


// --- End of File ---
