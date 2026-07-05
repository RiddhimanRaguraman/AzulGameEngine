//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "ShaderObjectNode.h"
#include "StringThis.h"

namespace Azul
{
	ShaderObjectNode::ShaderObjectNode()
		: DLink(),
		poShaderObject(nullptr)
	{
		this->privClear();
	}

	ShaderObjectNode::~ShaderObjectNode()
	{
		this->privClear();
	}

	void ShaderObjectNode::Set(ShaderObject *pShaderObject)
	{
		assert(pShaderObject);
		this->poShaderObject = pShaderObject;

		this->poShaderObject->SetName(pShaderObject->name);
	}

	ShaderObject *ShaderObjectNode::GetShaderObject()
	{
		return this->poShaderObject;
	}

	void ShaderObjectNode::privClear()
	{
		if (this->poShaderObject)
		{
			delete this->poShaderObject;
		}
		this->poShaderObject = nullptr;
	}

	char *ShaderObjectNode::GetName()
	{
		char *pName = nullptr;
		if (this->poShaderObject)
		{
			pName = this->poShaderObject->GetName();
		}

		return pName;
	}

	void ShaderObjectNode::Wash()
	{
		this->privClear();
	}



	void ShaderObjectNode::Dump()
	{
		Trace::out("      ShaderObjectNode(%p)\n", this);

		// Data:
		if (this->poShaderObject)
		{
			Trace::out("      ShaderObject(%p) \n", this->poShaderObject);
			Trace::out("      Name: %s \n", StringMe(this->poShaderObject->name));
		}
		else
		{
			Trace::out("      Name: %s \n", "null");
		}

		DLink::Dump();
	}
}

// --- End of File ---
