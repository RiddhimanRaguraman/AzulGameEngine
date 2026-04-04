//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "ProceduralMeshes.h"

#include "meshData.h"
#include "GLTF.h"
#include "File.h"
#include "MeshLayout.h"
#include "BoundingSphere.h"
#include "MathEngine.h"

#include "stb_image.h"

#include <vector>
#include <string>

namespace Azul
{
	namespace
	{
		void WriteMesh(const char* const pTargetName, meshData& runModel)
		{
			meshData_proto mA_proto;
			runModel.Serialize(mA_proto);

			File::Handle fh;
			File::Error err;
			bool status;

			const char* pProtoFileType = nullptr;
			status = GLTF::GetAzulProtoType(pProtoFileType, runModel);
			assert(status);

			std::string OutputFileName = std::string(pTargetName) + pProtoFileType + ".proto.azul";
			Trace::out(" --> %+30s\n", OutputFileName.c_str());

			err = File::Open(fh, OutputFileName.c_str(), File::Mode::WRITE);
			assert(err == File::Error::SUCCESS);

			std::string strOut;
			mA_proto.SerializeToString(&strOut);

			File::Write(fh, strOut.data(), (DWORD)strOut.length());
			assert(err == File::Error::SUCCESS);

			err = File::Close(fh);
			assert(err == File::Error::SUCCESS);
		}

		void SetBoundingSphere(meshData& runModel, const std::vector<VertexPos>& positions)
		{
			assert(!positions.empty());

			std::vector<Vec3> pts;
			pts.reserve(positions.size());
			for (const auto& vp : positions)
			{
				Vec3 p;
				p.set(vp.Position.x, vp.Position.y, vp.Position.z);
				pts.push_back(p);
			}

			Sphere s;
			RitterSphere(s, pts.data(), (int)pts.size());

			runModel.sphereRadius = s.rad;
			runModel.sphereCenter[0] = s.cntr[x];
			runModel.sphereCenter[1] = s.cntr[y];
			runModel.sphereCenter[2] = s.cntr[z];
		}

		bool ReadFileToBuffer(const char* const pFileName, std::vector<unsigned char>& out)
		{
			File::Handle fh;
			File::Error err;

			err = File::Open(fh, pFileName, File::Mode::READ);
			if (err != File::Error::SUCCESS)
			{
				return false;
			}

			err = File::Seek(fh, File::Position::END, 0);
			if (err != File::Error::SUCCESS)
			{
				File::Close(fh);
				return false;
			}

			DWORD length = 0;
			err = File::Tell(fh, length);
			if (err != File::Error::SUCCESS || length == 0)
			{
				File::Close(fh);
				return false;
			}

			err = File::Seek(fh, File::Position::BEGIN, 0);
			if (err != File::Error::SUCCESS)
			{
				File::Close(fh);
				return false;
			}

			out.resize(length);
			err = File::Read(fh, out.data(), length);
			File::Close(fh);

			return err == File::Error::SUCCESS;
		}
	}

	void CreateFlatPlane(const char* const pTargetName, float len, float urep, float vrep)
	{
		assert(pTargetName);

		meshData runModel;
		Trace::out("%-25s", pTargetName);

		const float half = 0.5f * len;

		std::vector<VertexPos> pos(4);
		std::vector<VertexNorm> norm(4);
		std::vector<VertexTexCoord> uv(4);

		pos[0].Position = Vec3f(-half, 0.0f, -half);
		pos[1].Position = Vec3f(-half, 0.0f, half);
		pos[2].Position = Vec3f(half, 0.0f, half);
		pos[3].Position = Vec3f(half, 0.0f, -half);

		for (auto& n : norm)
		{
			n.Norm = Vec3f(0.0f, 1.0f, 0.0f);
		}

		uv[0].TexCoord = Vec2f(urep, vrep);
		uv[1].TexCoord = Vec2f(urep, 0.0f);
		uv[2].TexCoord = Vec2f(0.0f, 0.0f);
		uv[3].TexCoord = Vec2f(0.0f, vrep);

		uint32_t indices[] = { 0,1,2, 0,2,3 };

		memcpy_s(runModel.pMeshName, meshData::FILE_NAME_SIZE, pTargetName, strlen(pTargetName));

		GLTF::SetCustomVBO(runModel.vbo_vert,
			pos.data(),
			(unsigned int)(pos.size() * sizeof(VertexPos)),
			(unsigned int)pos.size(),
			vboData::VBO_COMPONENT::FLOAT,
			vboData::VBO_TYPE::VEC3,
			vboData::VBO_TARGET::ARRAY_BUFFER);

		GLTF::SetCustomVBO(runModel.vbo_norm,
			norm.data(),
			(unsigned int)(norm.size() * sizeof(VertexNorm)),
			(unsigned int)norm.size(),
			vboData::VBO_COMPONENT::FLOAT,
			vboData::VBO_TYPE::VEC3,
			vboData::VBO_TARGET::ARRAY_BUFFER);

		GLTF::SetCustomVBO(runModel.vbo_uv,
			uv.data(),
			(unsigned int)(uv.size() * sizeof(VertexTexCoord)),
			(unsigned int)uv.size(),
			vboData::VBO_COMPONENT::FLOAT,
			vboData::VBO_TYPE::VEC2,
			vboData::VBO_TARGET::ARRAY_BUFFER);

		GLTF::SetCustomVBO_index(runModel.vbo_index,
			indices,
			(unsigned int)sizeof(indices),
			(unsigned int)(sizeof(indices) / sizeof(indices[0])),
			vboData::VBO_COMPONENT::UNSIGNED_INT,
			vboData::VBO_TYPE::SCALAR,
			vboData::VBO_TARGET::ELEMENT_ARRAY_BUFFER);

		runModel.triCount = 2;
		runModel.vertCount = (unsigned int)pos.size();
		SetBoundingSphere(runModel, pos);

		WriteMesh(pTargetName, runModel);
	}

	void CreateSkyBox(const char* const pTargetName, float len, float urep, float vrep)
	{
		assert(pTargetName);

		meshData runModel;
		Trace::out("%-25s", pTargetName);

		const float half = 0.5f * len;

		std::vector<VertexPos> pos(24);
		std::vector<VertexNorm> norm(24);
		std::vector<VertexTexCoord> uv(24);

		const Vec3f up(0.0f, 1.0f, 0.0f);
		const Vec3f down(0.0f, -1.0f, 0.0f);
		const Vec3f left(-1.0f, 0.0f, 0.0f);
		const Vec3f right(1.0f, 0.0f, 0.0f);
		const Vec3f front(0.0f, 0.0f, 1.0f);
		const Vec3f back(0.0f, 0.0f, -1.0f);

		pos[0].Position = Vec3f(-half, -half, -half);  uv[0].TexCoord = Vec2f(0.25f * urep, 1.0f);        norm[0].Norm = down;
		pos[1].Position = Vec3f(-half, -half, half);   uv[1].TexCoord = Vec2f(0.5f * urep, 1.0f);         norm[1].Norm = down;
		pos[2].Position = Vec3f(half, -half, half);    uv[2].TexCoord = Vec2f(0.5f * urep, 0.65f);        norm[2].Norm = down;
		pos[3].Position = Vec3f(half, -half, -half);   uv[3].TexCoord = Vec2f(0.25f * urep, 0.65f);       norm[3].Norm = down;

		pos[4].Position = Vec3f(-half, half, -half);   uv[4].TexCoord = Vec2f(0.5f * urep, 0.0f * vrep);  norm[4].Norm = up;
		pos[5].Position = Vec3f(-half, half, half);    uv[5].TexCoord = Vec2f(0.5f * urep, 0.3334f * vrep); norm[5].Norm = up;
		pos[6].Position = Vec3f(half, half, half);     uv[6].TexCoord = Vec2f(0.25f * urep, 0.3334f * vrep); norm[6].Norm = up;
		pos[7].Position = Vec3f(half, half, -half);    uv[7].TexCoord = Vec2f(0.25f * urep, 0.0f * vrep); norm[7].Norm = up;

		pos[8].Position = Vec3f(-half, -half, -half);  uv[8].TexCoord = Vec2f(0.75f * urep, 0.65f * vrep); norm[8].Norm = left;
		pos[9].Position = Vec3f(-half, half, -half);   uv[9].TexCoord = Vec2f(0.75f * urep, 0.3334f * vrep); norm[9].Norm = left;
		pos[10].Position = Vec3f(-half, half, half);   uv[10].TexCoord = Vec2f(0.5f * urep, 0.3334f * vrep); norm[10].Norm = left;
		pos[11].Position = Vec3f(-half, -half, half);  uv[11].TexCoord = Vec2f(0.5f * urep, 0.65f * vrep); norm[11].Norm = left;

		pos[12].Position = Vec3f(half, -half, -half);  uv[12].TexCoord = Vec2f(0.0f * urep, 0.65f * vrep); norm[12].Norm = right;
		pos[13].Position = Vec3f(half, half, -half);   uv[13].TexCoord = Vec2f(0.0f * urep, 0.3334f * vrep); norm[13].Norm = right;
		pos[14].Position = Vec3f(half, half, half);    uv[14].TexCoord = Vec2f(0.25f * urep, 0.3334f * vrep); norm[14].Norm = right;
		pos[15].Position = Vec3f(half, -half, half);   uv[15].TexCoord = Vec2f(0.25f * urep, 0.65f * vrep); norm[15].Norm = right;

		pos[16].Position = Vec3f(-half, -half, half);  uv[16].TexCoord = Vec2f(0.5f * urep, 0.65f * vrep); norm[16].Norm = front;
		pos[17].Position = Vec3f(-half, half, half);   uv[17].TexCoord = Vec2f(0.5f * urep, 0.3334f * vrep); norm[17].Norm = front;
		pos[18].Position = Vec3f(half, half, half);    uv[18].TexCoord = Vec2f(0.25f * urep, 0.3334f * vrep); norm[18].Norm = front;
		pos[19].Position = Vec3f(half, -half, half);   uv[19].TexCoord = Vec2f(0.25f * urep, 0.65f * vrep); norm[19].Norm = front;

		pos[20].Position = Vec3f(-half, -half, -half); uv[20].TexCoord = Vec2f(0.75f * urep, 0.65f * vrep); norm[20].Norm = back;
		pos[21].Position = Vec3f(-half, half, -half);  uv[21].TexCoord = Vec2f(0.75f * urep, 0.3334f * vrep); norm[21].Norm = back;
		pos[22].Position = Vec3f(half, half, -half);   uv[22].TexCoord = Vec2f(1.0f * urep, 0.3334f * vrep); norm[22].Norm = back;
		pos[23].Position = Vec3f(half, -half, -half);  uv[23].TexCoord = Vec2f(1.0f * urep, 0.65f * vrep); norm[23].Norm = back;

		uint32_t indices[] =
		{
			0,1,2, 0,2,3,
			4,6,5, 4,7,6,
			8,9,10, 8,10,11,
			12,15,14, 12,14,13,
			16,17,18, 16,18,19,
			20,23,22, 20,22,21
		};

		memcpy_s(runModel.pMeshName, meshData::FILE_NAME_SIZE, pTargetName, strlen(pTargetName));

		GLTF::SetCustomVBO(runModel.vbo_vert,
			pos.data(),
			(unsigned int)(pos.size() * sizeof(VertexPos)),
			(unsigned int)pos.size(),
			vboData::VBO_COMPONENT::FLOAT,
			vboData::VBO_TYPE::VEC3,
			vboData::VBO_TARGET::ARRAY_BUFFER);

		GLTF::SetCustomVBO(runModel.vbo_norm,
			norm.data(),
			(unsigned int)(norm.size() * sizeof(VertexNorm)),
			(unsigned int)norm.size(),
			vboData::VBO_COMPONENT::FLOAT,
			vboData::VBO_TYPE::VEC3,
			vboData::VBO_TARGET::ARRAY_BUFFER);

		GLTF::SetCustomVBO(runModel.vbo_uv,
			uv.data(),
			(unsigned int)(uv.size() * sizeof(VertexTexCoord)),
			(unsigned int)uv.size(),
			vboData::VBO_COMPONENT::FLOAT,
			vboData::VBO_TYPE::VEC2,
			vboData::VBO_TARGET::ARRAY_BUFFER);

		GLTF::SetCustomVBO_index(runModel.vbo_index,
			indices,
			(unsigned int)sizeof(indices),
			(unsigned int)(sizeof(indices) / sizeof(indices[0])),
			vboData::VBO_COMPONENT::UNSIGNED_INT,
			vboData::VBO_TYPE::SCALAR,
			vboData::VBO_TARGET::ELEMENT_ARRAY_BUFFER);

		runModel.triCount = 12;
		runModel.vertCount = (unsigned int)pos.size();
		SetBoundingSphere(runModel, pos);

		WriteMesh(pTargetName, runModel);
	}

	void CreateTerrain(const char* const pHeightMapFile, const char* const pTargetName, float len, float maxHeight, float yTrans, int repeatU, int repeatV)
	{
		assert(pTargetName);

		(void)repeatU;
		(void)repeatV;

		meshData runModel;
		Trace::out("%-25s", pTargetName);

		int width = 0;
		int height = 0;
		int comp = 0;

		std::vector<unsigned char> fileBytes;
		unsigned char* pixels = nullptr;

		if (pHeightMapFile != nullptr)
		{
			if (ReadFileToBuffer(pHeightMapFile, fileBytes))
			{
				pixels = stbi_load_from_memory(fileBytes.data(), (int)fileBytes.size(), &width, &height, &comp, 4);
			}
		}

		if (pixels == nullptr || width <= 1 || height <= 1)
		{
			Trace::out(" --> %+30s\n", "heightmap missing (flat fallback)");
			width = 64;
			height = 64;
		}

		auto ComputeRawHeightAt = [&](int col, int row) -> float
		{
			if (col < 0 || col >= width || row < 0 || row >= height)
			{
				return 0.0f;
			}

			unsigned char h = 0;
			if (pixels != nullptr)
			{
				const size_t idx = ((size_t)row * (size_t)width + (size_t)col) * 4u;
				h = pixels[idx + 0];
			}

			return ((float)h / 255.0f) * maxHeight;
		};

		const float cellSizeX = len / (float)(width - 1);
		const float cellSizeZ = len / (float)(height - 1);

		const float h00 = ComputeRawHeightAt(0, 0);
		const float h10 = ComputeRawHeightAt(width - 1, 0);
		const float h01 = ComputeRawHeightAt(0, height - 1);
		const float h11 = ComputeRawHeightAt(width - 1, height - 1);
		const float hAvgCorner = 0.25f * (h00 + h10 + h01 + h11);

		auto ComputeHeightAt = [&](int col, int row) -> float
		{
			const float raw = ComputeRawHeightAt(col, row);

			const float u = (width > 1) ? ((float)col / (float)(width - 1)) : 0.0f;
			const float v = (height > 1) ? ((float)row / (float)(height - 1)) : 0.0f;

			const float hTop = h00 + (h10 - h00) * u;
			const float hBot = h01 + (h11 - h01) * u;
			const float hPlane = hTop + (hBot - hTop) * v;

			return (raw - hPlane) + hAvgCorner + yTrans;
		};

		std::vector<VertexPos> pos;
		std::vector<VertexNorm> norm;
		std::vector<VertexTexCoord> uv;
		pos.resize((size_t)width * (size_t)height);
		norm.resize((size_t)width * (size_t)height);
		uv.resize((size_t)width * (size_t)height);

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				const int index = i * width + j;
				const float xPos = -0.5f * len + (float)j * cellSizeX;
				const float zPos = -0.5f * len + (float)i * cellSizeZ;
				const float h = ComputeHeightAt(j, i);

				const float hL = ComputeHeightAt(j - 1, i);
				const float hR = ComputeHeightAt(j + 1, i);
				const float hD = ComputeHeightAt(j, i - 1);
				const float hU = ComputeHeightAt(j, i + 1);

				Vec3 n;
				const float denomX = (cellSizeX > 0.0f) ? (2.0f * cellSizeX) : 1.0f;
				const float denomZ = (cellSizeZ > 0.0f) ? (2.0f * cellSizeZ) : 1.0f;
				const float dHdX = (hR - hL) / denomX;
				const float dHdZ = (hU - hD) / denomZ;
				n.set(-dHdX, 1.0f, -dHdZ);
				n.norm();

				pos[index].Position = Vec3f(xPos, h, zPos);
				norm[index].Norm = Vec3f(n[x], n[y], n[z]);
				uv[index].TexCoord = Vec2f((float)j / (float)(width - 1),
					(float)i / (float)(height - 1));
			}
		}

		const unsigned int triCount = (unsigned int)((width - 1) * (height - 1) * 2);
		std::vector<uint32_t> indices;
		indices.resize((size_t)triCount * 3u);

		size_t k = 0;
		for (int i = 0; i < height - 1; i++)
		{
			for (int j = 0; j < width - 1; j++)
			{
				const uint32_t idx = (uint32_t)(i * width + j);
				indices[k++] = idx;
				indices[k++] = idx + (uint32_t)width;
				indices[k++] = idx + 1u;

				indices[k++] = idx + 1u;
				indices[k++] = idx + (uint32_t)width;
				indices[k++] = idx + (uint32_t)width + 1u;
			}
		}

		if (pixels != nullptr)
		{
			stbi_image_free(pixels);
			pixels = nullptr;
		}

		memcpy_s(runModel.pMeshName, meshData::FILE_NAME_SIZE, pTargetName, strlen(pTargetName));

		GLTF::SetCustomVBO(runModel.vbo_vert,
			pos.data(),
			(unsigned int)(pos.size() * sizeof(VertexPos)),
			(unsigned int)pos.size(),
			vboData::VBO_COMPONENT::FLOAT,
			vboData::VBO_TYPE::VEC3,
			vboData::VBO_TARGET::ARRAY_BUFFER);

		GLTF::SetCustomVBO(runModel.vbo_norm,
			norm.data(),
			(unsigned int)(norm.size() * sizeof(VertexNorm)),
			(unsigned int)norm.size(),
			vboData::VBO_COMPONENT::FLOAT,
			vboData::VBO_TYPE::VEC3,
			vboData::VBO_TARGET::ARRAY_BUFFER);

		GLTF::SetCustomVBO(runModel.vbo_uv,
			uv.data(),
			(unsigned int)(uv.size() * sizeof(VertexTexCoord)),
			(unsigned int)uv.size(),
			vboData::VBO_COMPONENT::FLOAT,
			vboData::VBO_TYPE::VEC2,
			vboData::VBO_TARGET::ARRAY_BUFFER);

		GLTF::SetCustomVBO_index(runModel.vbo_index,
			indices.data(),
			(unsigned int)(indices.size() * sizeof(uint32_t)),
			(unsigned int)indices.size(),
			vboData::VBO_COMPONENT::UNSIGNED_INT,
			vboData::VBO_TYPE::SCALAR,
			vboData::VBO_TARGET::ELEMENT_ARRAY_BUFFER);

		runModel.triCount = triCount;
		runModel.vertCount = (unsigned int)pos.size();
		SetBoundingSphere(runModel, pos);

		WriteMesh(pTargetName, runModel);
	}

	void createFlatplane(const char* const pTargetName, float len, float urep, float vrep)
	{
		CreateFlatPlane(pTargetName, len, urep, vrep);
	}

	void createSkyBox(const char* const pTargetName, float len, float urep, float vrep)
	{
		CreateSkyBox(pTargetName, len, urep, vrep);
	}

	void createterrain(const char* const pHeightMapFile, const char* const pTargetName, float len, float maxHeight, float yTrans, int repeatU, int repeatV)
	{
		CreateTerrain(pHeightMapFile, pTargetName, len, maxHeight, yTrans, repeatU, repeatV);
	}
}

// --- End of File ---
