#ifndef SHADER_OBJECT_NODE_H
#define SHADER_OBJECT_NODE_H
#include "EngineDLLInterface.h"

#include "DLink.h"
#include "ShaderObject.h"

namespace Azul
{

	class AZUL_ENGINE_LIBRARY_API ShaderObjectNode : public DLink
	{
	public:
		ShaderObjectNode();
		ShaderObjectNode(const ShaderObject &) = delete;
		ShaderObjectNode &operator = (const ShaderObject &) = delete;
		virtual ~ShaderObjectNode();

		void Set(ShaderObject *pShaderObject);
		ShaderObject *GetShaderObject();

		virtual char *GetName() override;
		virtual void Wash() override;

		virtual void Dump() override;

	private:
		void privClear();

		//--------------------------------------------------
		// Data
		//--------------------------------------------------
		ShaderObject *poShaderObject;
	};
}

#endif

// --- End of File ---
