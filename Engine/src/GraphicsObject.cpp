//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "GraphicsObject.h"
#include "MathEngine.h"
#include "Mesh.h"
#include "ShaderObjectNodeMan.h"
#include "MeshNodeMan.h"

using namespace Azul;

GraphicsObject::GraphicsObject(Mesh::Name meshName,
	ShaderObject::Name shaderName)
	: pMesh(MeshNodeMan::Find(meshName)),
	pShaderObj(ShaderObjectNodeMan::Find(shaderName))
{
	this->poWorld = new Mat4(Identity);
	assert(pMesh);
	assert(pShaderObj);
}

GraphicsObject::~GraphicsObject()
{
	delete this->poWorld;
}

void GraphicsObject::Render()
{
	this->SetState();
	this->SetDataGPU();
	this->Draw();
	this->RestoreState();
}

Mesh *GraphicsObject::GetMesh() const
{
	return this->pMesh;
}

Mat4 &GraphicsObject::GetWorld()
{
	return *this->poWorld;
}

void GraphicsObject::SetWorld(Mat4 &_world)
{
	*this->poWorld = _world;
}

// --- End of File ---
