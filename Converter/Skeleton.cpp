//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Skeleton.h"
#include "File.h"
#include <algorithm>
#include <vector>

namespace Azul
{
	skeleton::skeleton()
		:nodeIndex(-1),
		nodeName(),
		parentIndex(-1),
		parentName("unknown")
	{

	};

	void SKEL::CreateSkeleton(Model& model, const char* const pTargetName)
	{
		assert(pTargetName);

		// Create a skeleton and Fill it
		this->skeletonVector.reserve(model.nodes.size());
		for (size_t i = 0; i < model.nodes.size(); i++)
		{
			skeleton tmpSkeleton;
			this->skeletonVector.push_back(tmpSkeleton);
		}

		//Trace::out("\n\n");

		// Walk through the nodes starting on the node they state
		for (size_t i = 0; i < this->skeletonVector.size(); i++)
		{
			auto pNode = model.nodes[i];
			skeletonVector[i].nodeIndex = (int)i; 

			std::string sNodeName = pNode.name;
			char old_char = '.';
			char new_char = '_';
			std::replace(sNodeName.begin(), sNodeName.end(), old_char, new_char);

			skeletonVector[i].nodeName = sNodeName.c_str();
			//Trace::out("%d: %s \n", i, pNode.name.c_str());
			for (size_t j = 0; j < pNode.children.size(); j++)
			{
				skeletonVector[(size_t)pNode.children[j]].nodeIndex = pNode.children[j];
				skeletonVector[(size_t)pNode.children[j]].nodeName = model.nodes[(size_t)pNode.children[j]].name;
				skeletonVector[(size_t)pNode.children[j]].parentIndex = (int)i;
				skeletonVector[(size_t)pNode.children[j]].parentName = pNode.name;
				//Trace::out("\t child: (%d) %s \n", pNode.children[j], model.nodes[(size_t)pNode.children[j]].name.c_str());
			}
		}

		//Trace::out("\n\n");

		// Fill skeletonTable
		skeletonTable.reserve(skeletonVector.size());
		for (size_t i = 0; i < skeletonVector.size(); i++)
		{
			skeleton_table table;

			table.nodeIndex = skeletonVector[i].nodeIndex;
			table.parentIndex = skeletonVector[i].parentIndex;

			// Copy the string
			memset(table.nodeName, 0, SkelEntry::SKELETON_NAME_SIZE);
			std::string aa(pTargetName);

			std::string cc = aa + "_" + skeletonVector[i].nodeName;

			strncpy_s(table.nodeName,
				SkelEntry::SKELETON_NAME_SIZE,
				cc.c_str(),
				SkelEntry::SKELETON_NAME_SIZE - 1);

			table.nodeIndex = table.nodeIndex;
			table.parentIndex = table.parentIndex;
			if (table.parentIndex < -1)
			{
				table.parentIndex = -1;
			}
			//Trace::out(" %d %d %s\n", table.nodeIndex, table.parentIndex, table.nodeName);
			skeletonTable.push_back(table);
		}

		// put this into the proto
		unsigned int numBones = (uint32_t)skeletonTable.size();
		SkelData* pSkelData = new SkelData(numBones);
		assert(pSkelData);


		for (size_t i = 0; i < numBones; i++)
		{
			pSkelData->poSkelEntry[i].set(skeletonTable[i].nodeIndex,
				skeletonTable[i].parentIndex,
				&skeletonTable[i].nodeName[0]);
		}

		//pSkelData->Print("SkeData");
		SkelData_proto mA_proto;
		pSkelData->Serialize(mA_proto);


		// -------------------------------
		//  Write to file
		//--------------------------------

		File::Handle fh;
		File::Error err;

		// Create output name
		std::string sProtoFileType = "s";
		std::string sBaseName = pTargetName;

		std::string OutputFileName = sBaseName + "." + sProtoFileType + ".proto.azul";
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

		err = File::Open(fh, OutputFileName.c_str(), File::Mode::READ);
		assert(err == File::Error::SUCCESS);

		err = File::Seek(fh, File::Position::END, 0);
		assert(err == File::Error::SUCCESS);

		DWORD FileLength;
		err = File::Tell(fh, FileLength);
		assert(err == File::Error::SUCCESS);

		char* poNewBuff = new char[FileLength]();
		assert(poNewBuff);

		err = File::Seek(fh, File::Position::BEGIN, 0);
		assert(err == File::Error::SUCCESS);

		err = File::Read(fh, poNewBuff, FileLength);
		assert(err == File::Error::SUCCESS);

		err = File::Close(fh);
		assert(err == File::Error::SUCCESS);

		SkelData_proto mB_proto;
		mB_proto.ParseFromArray(poNewBuff, (int)FileLength);

		SkelData mB;
		mB.Deserialize(mB_proto);

		//mB.Print("mB");

		delete[] poNewBuff;
		delete pSkelData;

	};

}

// --- End of File ---
