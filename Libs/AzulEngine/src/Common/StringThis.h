#ifndef STRING_THIS_H
#define STRING_THIS_H
#include "EngineDLLInterface.h"

#include "Mesh.h"
#include "ShaderObject.h"
#include "TextureObject.h"
#include "Camera.h"
#include "Clip.h"
#include "Skel.h"
#include "JointTable.h"
#include "HierarchyTable.h"

namespace Azul
{
	class AZUL_ENGINE_LIBRARY_API StringThis
	{
	public:
		static const unsigned int BUFFER_SIZE = 64;
	public:

		StringThis(JointTable::Name status);
		StringThis(Mesh::Name);
		StringThis(ShaderObject::Name);
		StringThis(TextureObject::Name);
		StringThis(Camera::Name);
		StringThis(Skel::Name);
		StringThis(Clip::Name);
		StringThis(HierarchyTable::Name);

		operator char *();

		// data:
		char buffer[BUFFER_SIZE];
	};

#define StringMe(x)  ((char *)StringThis(x)) 

}

#endif

// --- End of File ---
