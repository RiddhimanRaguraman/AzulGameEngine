//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "ShaderObjectNodeMan.h"
#include "ShaderObject_Null.h"
#include "ManBase.h"
#include "DLinkMan.h"
#include "ShaderObjectNodeCompareStrategyEnumName.h"

namespace Azul
{
	ShaderObjectNodeMan *ShaderObjectNodeMan::posInstance = nullptr;
	CompareStrategyBase *ShaderObjectNodeMan::posEnumNameCompare = nullptr;

	//----------------------------------------------------------------------
	// Constructor
	//----------------------------------------------------------------------
	ShaderObjectNodeMan::ShaderObjectNodeMan(int reserveNum, int reserveGrow)
		: ManBase(new DLinkMan(), new DLinkMan(), reserveNum, reserveGrow)
	{
		// Preload the reserve
		this->proFillReservedPool(reserveNum);

		// initialize derived data here
		ShaderObject *pShaderObject = new ShaderObject_Null(ShaderObject::Name::NullShader);
		assert(pShaderObject);

		this->poNodeCompare = new ShaderObjectNode();
		assert(this->poNodeCompare);

		this->poNodeCompare->Set(pShaderObject);
	}

	ShaderObjectNodeMan::~ShaderObjectNodeMan()
	{
		delete this->poNodeCompare;
		this->poNodeCompare = nullptr;

		// iterate through the list and delete
		Iterator *pIt = this->baseGetActiveIterator();

		DLink *pNode = pIt->First();

		// Walk through the nodes
		while (!pIt->IsDone())
		{
			ShaderObjectNode *pDeleteMe = (ShaderObjectNode *) pIt->Curr();
			pNode = pIt->Next();
			delete pDeleteMe;
		}

		pIt = this->baseGetReserveIterator();

		pNode = pIt->First();

		// Walk through the nodes
		while (!pIt->IsDone())
		{
			ShaderObjectNode *pDeleteMe = (ShaderObjectNode *) pIt->Curr();
			pNode = pIt->Next();
			delete pDeleteMe;
		}
	}

	//----------------------------------------------------------------------
	// Static Methods
	//----------------------------------------------------------------------
	void ShaderObjectNodeMan::Create(int reserveNum, int reserveGrow)
	{
		// make sure values are ressonable 
		assert(reserveNum >= 0);
		assert(reserveGrow > 0);

		// initialize the singleton here
		assert(posInstance == nullptr);

		// Do the initialization
		if (posInstance == nullptr)
		{
			posInstance = new ShaderObjectNodeMan(reserveNum, reserveGrow);
		}

		if (posEnumNameCompare == nullptr)
		{
			posEnumNameCompare = new ShaderObjectNodeCompareStrategyEnumName();
		}
	}

	void ShaderObjectNodeMan::Destroy()
	{
		ShaderObjectNodeMan *pMan = ShaderObjectNodeMan::privGetInstance();
		assert(pMan != nullptr);
		AZUL_UNUSED_VAR(pMan);

		delete ShaderObjectNodeMan::posEnumNameCompare;
		ShaderObjectNodeMan::posEnumNameCompare = nullptr;

		delete ShaderObjectNodeMan::posInstance;
		ShaderObjectNodeMan::posInstance = nullptr;
	}

	ShaderObject *ShaderObjectNodeMan::Add(ShaderObject::Name name)
	{
		ShaderObject *pShader = nullptr;
		switch (name)
		{
			case ShaderObject::Name::NullShader:
				pShader = new ShaderObject_Null(ShaderObject::Name::NullShader);
				break;
			case ShaderObject::Name::ColorByVertex:
				pShader = new ShaderObject_ColorByVertex(ShaderObject::Name::ColorByVertex);
				break;
			case ShaderObject::Name::FlatTexture:
				pShader = new ShaderObject_FlatTexture(ShaderObject::Name::FlatTexture);
				break;
			case ShaderObject::Name::LightTexture:
				pShader = new ShaderObject_LightTexture(ShaderObject::Name::LightTexture);
				break;
			case ShaderObject::Name::ConstColor:
				pShader = new ShaderObject_ConstColor(ShaderObject::Name::ConstColor);
				break;
			case ShaderObject::Name::SkinFlatTexture:
				pShader = new ShaderObject_SkinFlatTexture(ShaderObject::Name::SkinFlatTexture);
				break;
			case ShaderObject::Name::SkinLightTexture:
				pShader = new ShaderObject_SkinLightTexture(ShaderObject::Name::SkinLightTexture);
				break;
			case ShaderObject::Name::MixerACompute:
				pShader = new ShaderObject_MixerA(ShaderObject::Name::MixerACompute);
				break;
			case ShaderObject::Name::MixerBCompute:
				pShader = new ShaderObject_MixerB(ShaderObject::Name::MixerBCompute);
				break;
			case ShaderObject::Name::MixerCCompute:
				pShader = new ShaderObject_MixerC(ShaderObject::Name::MixerCCompute);
				break;
			case ShaderObject::Name::WorldComputeA:
				pShader = new ShaderObject_WorldA(ShaderObject::Name::WorldComputeA);
				break;
			case ShaderObject::Name::WorldComputeC:
				pShader = new ShaderObject_WorldC(ShaderObject::Name::WorldComputeC);
				break;
			case ShaderObject::Name::Sprite:
				pShader = new ShaderObject_Sprite(ShaderObject::Name::Sprite);
				break;
			case ShaderObject::Name::Uninitialized:
				assert(false);
				break;

			default:
				assert(false);
				break;
		}

		assert(pShader);
		ShaderObjectNodeMan::Add(pShader);
		return pShader;
	}

	ShaderObjectNode *ShaderObjectNodeMan::Add(ShaderObject *pShader)
	{
		ShaderObjectNodeMan *pMan = ShaderObjectNodeMan::privGetInstance();

		assert(pShader);

		ShaderObjectNode *pNode = (ShaderObjectNode *) pMan->baseAddToFront();
		assert(pNode != nullptr);

		// Initialize the date
		pNode->Set(pShader);

		return pNode;
	}

	ShaderObject *ShaderObjectNodeMan::Find(ShaderObject::Name _name)
	{
		ShaderObjectNodeMan *pMan = ShaderObjectNodeMan::privGetInstance();
		assert(pMan != nullptr);

		// set strategy
		pMan->pCompareStrategy = ShaderObjectNodeMan::posEnumNameCompare;
		assert(pMan->pCompareStrategy);

		// Compare functions only compares two Nodes

		// So:  Use the Compare Node - as a reference
		//      use in the Compare() function
		ShaderObject *pShaderObject = pMan->poNodeCompare->GetShaderObject();
		assert(pShaderObject);

		pShaderObject->name = _name;

		ShaderObjectNode *pData = (ShaderObjectNode *) pMan->baseFind(pMan->poNodeCompare);
		return pData->GetShaderObject();
	}

	void ShaderObjectNodeMan::Remove(ShaderObjectNode *pNode)
	{
		assert(pNode != nullptr);

		ShaderObjectNodeMan *pMan = ShaderObjectNodeMan::privGetInstance();
		assert(pMan != nullptr);

		pMan->baseRemove(pNode);
	}

	void ShaderObjectNodeMan::Dump()
	{
		ShaderObjectNodeMan *pMan = ShaderObjectNodeMan::privGetInstance();
		assert(pMan != nullptr);

		pMan->baseDump();
	}

	//----------------------------------------------------------------------
	// Private methods
	//----------------------------------------------------------------------
	ShaderObjectNodeMan *ShaderObjectNodeMan::privGetInstance()
	{
		// Safety - this forces users to call Create() first before using class
		assert(posInstance != nullptr);

		return posInstance;
	}

	//----------------------------------------------------------------------
	// Override Abstract methods
	//----------------------------------------------------------------------
	DLink *ShaderObjectNodeMan::derivedCreateNode()
	{
		DLink *pNodeBase = new ShaderObjectNode();
		assert(pNodeBase != nullptr);

		return pNodeBase;
	}

}

// --- End of File ---




