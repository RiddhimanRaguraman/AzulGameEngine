//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "GLTF.h"

#include "ConvertJoint.h"
#include "json.hpp"
#include "File.h"
#include "MathEngine.h"
#include "JointData.h"

using namespace tinygltf;
using json = nlohmann::json;

namespace Azul
{
	void ConvertJoint(const char* const pFileName, const char* const pTargetName)
	{
		bool status;
		tinygltf::Model gltfModel;

		// runtime model
		meshData  runModel;

		Trace::out("%-25s", pFileName);

		// Strip the extension .tga off the name
		unsigned int len = (uint32_t)strlen(pFileName);
		char* pTmp = new char[len + 1]();
		memset(pTmp, 0, len);
		memcpy(pTmp, pFileName, len - strlen(".glb"));

		// base name
		std::string BaseName = pTargetName;
		delete[] pTmp;

		// Load the gltfModel
		status = GLTF::Load(gltfModel, pFileName);
		assert(status);

		// Read glb into memory (raw buffer)
		char* poBuff = nullptr;
		unsigned int BuffSize(0);

		status = GLTF::GetGLBRawBuffer(poBuff, BuffSize, pFileName);
		assert(status);

		// Get the Binary Buffer Address
		char* pBinaryBuff = nullptr;
		unsigned int BinaryBuffSize = 0;
		status = GLTF::GetBinaryBuffPtr(pBinaryBuff, BinaryBuffSize, poBuff, BuffSize);
		assert(status);

		// --------------------------------------------------
		//  Fill: Joint Data
		// --------------------------------------------------

		size_t jointAccessorIndex = (size_t)gltfModel.meshes[0].primitives[0].attributes.find("JOINTS_0")->second;

		//unsigned char *pBuff = (unsigned char *)&gltfModel.buffers[0].data[0];

		// Joint
		auto JointAccessor = gltfModel.accessors[jointAccessorIndex];
		auto JointBuffView = gltfModel.bufferViews[(size_t)JointAccessor.bufferView];

		//unsigned char *pJointBuff = pBuff + JointBuffView.byteOffset;
		assert(JointAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT);
		assert(JointAccessor.type == TINYGLTF_TYPE_VEC4);

		// -------------------------------------------------
		//  Joint table
		// -------------------------------------------------

		auto data = gltfModel.skins[0].joints;

		JointData mA((unsigned int)data.size(), (unsigned int*)&data[0]);


		//mA.Print("input");
		JointData_proto mA_proto;
		mA.Serialize(mA_proto);


		//Trace::out("\n");
		//Trace::out("int JointTable_Size = %d; \n", data.size());
		//Trace::out("int JointTable[] = { \n");
		//for(size_t i = 0; i < data.size(); i++)
		//{
		//	Trace::out("\t\t\t\t%d, \n", data[i]);
		//}
		//Trace::out("\n");



		//Trace::out("\n");
		//Trace::out("message size: %d \n", mA_proto.ByteSizeLong());
		//Trace::out("\n");

		// -------------------------------
		//  Write to file
		//--------------------------------

		File::Handle fh;
		File::Error err;

		const char* pProtoFileType = ".j";

		std::string OutputFileName = BaseName + pProtoFileType + ".proto.azul";
		Trace::out(" --> %+30s\n", OutputFileName.c_str());

		err = File::Open(fh, OutputFileName.c_str(), File::Mode::WRITE);
		assert(err == File::Error::SUCCESS);

		std::string strOut;
		mA_proto.SerializeToString(&strOut);

		File::Write(fh, strOut.data(), (uint32_t)strOut.length());
		assert(err == File::Error::SUCCESS);

		err = File::Close(fh);
		assert(err == File::Error::SUCCESS);

		// -------------------------------
		// Read and recreate model data
		// -------------------------------
		unsigned char* poNewBuff;
		unsigned int newBuffSize;

		err = File::GetFileAsBuffer(OutputFileName.c_str(), poNewBuff, newBuffSize);
		assert(err == File::Error::SUCCESS);

		std::string strIn((const char*)poNewBuff, newBuffSize);
		JointData_proto mB_proto;

		mB_proto.ParseFromString(strIn);

		JointData mB;
		mB.Deserialize(mB_proto);
		//mB.Print("output");

		delete[] poBuff;
		delete[] poNewBuff;

	}
}

// --- End of File ---