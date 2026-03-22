//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef SHADER_OBJECT_NODE_MAN_H
#define SHADER_OBJECT_NODE_MAN_H 

#include "ManBase.h"
#include "ShaderObjectNode.h"
#include "ShaderObject.h"
#include "ShaderObject_ColorByVertex.h"
#include "ShaderObject_FlatTexture.h"
#include "ShaderObject_SkinFlatTexture.h"
#include "ShaderObject_SkinLightTexture.h"
#include "ShaderObject_WorldA.h"
#include "ShaderObject_WorldC.h"
#include "ShaderObject_LightTexture.h"
#include "ShaderObject_ConstColor.h"
#include "ShaderObject_Sprite.h"
#include "ShaderObject_MixerA.h"
#include "ShaderObject_MixerB.h"
#include "ShaderObject_MixerC.h"

namespace Azul
{

	class ShaderObjectNodeMan : public ManBase
	{
		//----------------------------------------------------------------------
		// Static Methods
		//----------------------------------------------------------------------
	public:
		static void Create(int reserveNum = 0, int reserveGrow = 1);
		static void Destroy();

		static ShaderObject *Add(ShaderObject::Name name);
		static ShaderObjectNode *Add(ShaderObject *pShaderObject);
		static ShaderObject *Find(ShaderObject::Name name);

		static void Remove(ShaderObjectNode *pNode);
		static void Dump();

		//----------------------------------------------------------------------
		// Private methods
		//----------------------------------------------------------------------
	private:
		static ShaderObjectNodeMan *privGetInstance();

		ShaderObjectNodeMan() = delete;
		ShaderObjectNodeMan(const ShaderObjectNodeMan &) = delete;
		ShaderObjectNodeMan &operator = (const ShaderObjectNodeMan &) = delete;
		virtual ~ShaderObjectNodeMan();

		ShaderObjectNodeMan(int reserveNum, int reserveGrow);

		//----------------------------------------------------------------------
		// Override Abstract methods
		//----------------------------------------------------------------------
	protected:
		DLink *derivedCreateNode() override;

		//----------------------------------------------------------------------
		// Data: unique data for this manager 
		//----------------------------------------------------------------------
	private:
		ShaderObjectNode *poNodeCompare;
		static ShaderObjectNodeMan *posInstance;
		static CompareStrategyBase *posEnumNameCompare;

	};
}
#endif

// --- End of File ---
