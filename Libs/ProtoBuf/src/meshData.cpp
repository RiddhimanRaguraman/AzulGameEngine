//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------
#include "meshData.h"

namespace Azul
{
	meshData::~meshData()
	{

	}

	meshData::meshData()
		:
		pVersion{ 0 },
		pMeshName{ 0 },
		mode{ meshData::RENDER_MODE::DEFAULT },
		triCount{ 0 },
		vertCount{ 0 },
		vbo_vert(),
		vbo_norm(),
		vbo_uv(),
		vbo_index(),
		vbo_color(),
		vbo_weights(),
		vbo_joints(),
		vbo_invBind()
	{
		strcpy_s(this->pVersion, protoVersion::VERSION_NUM_BYTES, protoVersion::VERSION_STRING);
		assert(strlen(this->pVersion) < protoVersion::VERSION_NUM_BYTES);
	}

	void meshData::Serialize(meshData_proto& out) const
	{
		AZUL_UNUSED_VAR(out);

		std::string sVersion((const char*)this->pVersion, protoVersion::VERSION_NUM_BYTES);
		out.set_pversion(sVersion);

		std::string sName((const char*)this->pMeshName, meshData::FILE_NAME_SIZE);
		out.set_pmeshname(sName);

		out.set_mode((meshData_proto_RENDER_MODE)this->mode);
		out.set_tricount(this->triCount);
		out.set_vertcount(this->vertCount);


		out.set_sphereradius(this->sphereRadius);
		out.set_spherecenter_x(this->sphereCenter[0]);
		out.set_spherecenter_y(this->sphereCenter[1]);
		out.set_spherecenter_z(this->sphereCenter[2]);

		vboData_proto* pVBO_proto;

		pVBO_proto = new vboData_proto();
		this->vbo_vert.Serialize(*pVBO_proto);
		out.set_allocated_vbo_vert(pVBO_proto);

		pVBO_proto = new vboData_proto();
		this->vbo_norm.Serialize(*pVBO_proto);
		out.set_allocated_vbo_norm(pVBO_proto);

		pVBO_proto = new vboData_proto();
		this->vbo_uv.Serialize(*pVBO_proto);
		out.set_allocated_vbo_uv(pVBO_proto);

		pVBO_proto = new vboData_proto();
		this->vbo_index.Serialize(*pVBO_proto);
		out.set_allocated_vbo_index(pVBO_proto);

		pVBO_proto = new vboData_proto();
		this->vbo_color.Serialize(*pVBO_proto);
		out.set_allocated_vbo_color(pVBO_proto);

		// skinned
		pVBO_proto = new vboData_proto();
		this->vbo_weights.Serialize(*pVBO_proto);
		out.set_allocated_vbo_weights(pVBO_proto);

		pVBO_proto = new vboData_proto();
		this->vbo_joints.Serialize(*pVBO_proto);
		out.set_allocated_vbo_joints(pVBO_proto);

		pVBO_proto = new vboData_proto();
		this->vbo_invBind.Serialize(*pVBO_proto);
		out.set_allocated_vbo_invbind(pVBO_proto);
	}

	void meshData::Deserialize(const meshData_proto& in)
	{
		// CHECK the Version
		memcpy_s(this->pVersion, protoVersion::VERSION_NUM_BYTES, in.pversion().data(), protoVersion::VERSION_NUM_BYTES);
		assert(strcmp(this->pVersion, protoVersion::VERSION_STRING) == 0);

		// Load the rest
		memcpy_s(this->pMeshName, meshData::FILE_NAME_SIZE, in.pmeshname().data(), meshData::FILE_NAME_SIZE);

		this->mode = (RENDER_MODE)in.mode();
		this->triCount = in.tricount();
		this->vertCount = in.vertcount();

		this->vbo_vert.Deserialize(in.vbo_vert());
		this->vbo_norm.Deserialize(in.vbo_norm());
		this->vbo_uv.Deserialize(in.vbo_uv());
		this->vbo_index.Deserialize(in.vbo_index());
		this->vbo_color.Deserialize(in.vbo_color());

		// skinned data
		this->vbo_weights.Deserialize(in.vbo_weights());
		this->vbo_joints.Deserialize(in.vbo_joints());
		this->vbo_invBind.Deserialize(in.vbo_invbind());

		this->sphereRadius = in.sphereradius();
		this->sphereCenter[0] = in.spherecenter_x();
		this->sphereCenter[1] = in.spherecenter_y();
		this->sphereCenter[2] = in.spherecenter_z();
	}

	void meshData::Print(const char* const pName) const
	{
		Trace::out("%s: \n", pName);
		Trace::out("\n");

		Trace::out("version: %s \n", this->pVersion);

		this->vbo_vert.Print("vbo_vert");
		this->vbo_norm.Print("vbo_norm");
		this->vbo_uv.Print("vbo_uv");
		this->vbo_index.Print("vbo_index");
		this->vbo_color.Print("vbo_color");
		this->vbo_weights.Print("vbo_weights");
		this->vbo_joints.Print("vbo_joints");
		this->vbo_invBind.Print("vbo_invBlind");

		Trace::out("Sphere Radius = %f\n", this->sphereRadius);
		Trace::out("Sphere Center = (%0.3f,%0.3f,%0.3f)\n", this->sphereCenter[0], this->sphereCenter[1], this->sphereCenter[2]);
	}

}

// --- End of File ---


