//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Skeleton.h"
#include "File.h"
#include "HierarchyData.h"
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

	}
	void SKEL::CreateHierarchy(Model &model, const char *const pTargetName)
	{
		assert(pTargetName);

		// Create a skeleton and Fill it
		this->skeletonVector.reserve(model.nodes.size());
		for (size_t i = 0; i < model.nodes.size(); i++)
		{
			skeleton tmpSkeleton;
			this->skeletonVector.push_back(tmpSkeleton);
		}

		// Walk through the nodes starting on the node they state
		size_t StartNodeIndex = 0;

		for (size_t i = StartNodeIndex; i < StartNodeIndex + this->skeletonVector.size(); i++)
		{
			auto pNode = model.nodes[i];
			skeletonVector[i - StartNodeIndex].nodeIndex = (int)i;
			skeletonVector[i - StartNodeIndex].nodeName = pNode.name;
			//	Trace::out("%d: %s \n", i, pNode.name.c_str());
			for (size_t j = 0; j < pNode.children.size(); j++)
			{
				skeletonVector[(size_t)pNode.children[j] - StartNodeIndex].nodeIndex = pNode.children[j];
				skeletonVector[(size_t)pNode.children[j] - StartNodeIndex].nodeName = model.nodes[(size_t)pNode.children[j]].name;
				skeletonVector[(size_t)pNode.children[j] - StartNodeIndex].parentIndex = (int)i;
				skeletonVector[(size_t)pNode.children[j] - StartNodeIndex].parentName = pNode.name;
				//		Trace::out("\t child: (%d) %s \n", pNode.children[j], model.nodes[(size_t)pNode.children[j]].name.c_str());
			}
		}

		// Fill skeletonTable
		skeletonTable.reserve(skeletonVector.size());

		//	Trace::out("\n");
		for (size_t i = 0; i < skeletonVector.size(); i++)
		{
			skeleton_table table;

			table.nodeIndex = skeletonVector[i].nodeIndex;
			table.parentIndex = skeletonVector[i].parentIndex;

			// Copy the string
			memset(table.nodeName, 0, SkelEntry::SKELETON_NAME_SIZE);
			strncpy_s(table.nodeName,
				SkelEntry::SKELETON_NAME_SIZE,
				skeletonVector[i].nodeName.c_str(),
				SkelEntry::SKELETON_NAME_SIZE - 1);

			//	Trace::out("%i c:%d p:%d   name:%s\n", i, table.nodeIndex, table.parentIndex, table.nodeName);

			skeletonTable.push_back(table);
		}

		// ---------------------------------
		// Find max Depth
		// ---------------------------------

		int MaxDepth = -1;
		for (size_t i = 0; i < skeletonTable.size(); i++)
		{
			int start = (int)i;
			int curr = start;
			int count = 0;
			while (curr != -1)
			{
				//		Trace::out("curr: %d  %s\n",
				//			skeletonTable[(size_t)curr].nodeIndex,
				//			skeletonTable[(size_t)curr].nodeName);
				curr = (int)skeletonTable[(size_t)curr].parentIndex;
				count++;
			}

			if (count > MaxDepth)
			{
				MaxDepth = count;
			}
			//	Trace::out("curr: %d  depth: %d  max:%d\n", start, count, MaxDepth);
		}

		std::vector<std::vector<int>> TableA;
		for (size_t i = 0; i < skeletonTable.size(); i++)
		{
			std::vector<int> entry;

			int start = (int)i;
			int curr = start;
			int count = 0;
			while (curr != -1)
			{
				entry.push_back(skeletonTable[(size_t)curr].nodeIndex);

				curr = (int)skeletonTable[(size_t)curr].parentIndex;
				count++;
			}

			std::reverse(entry.begin(), entry.end());

			size_t extra = ((size_t)MaxDepth - entry.size());
			for (size_t j = 0; j < extra; j++)
			{
				// fill with -1 or anything
				//entry.push_back((int)skeletonTable.size());
				entry.push_back(-1);
			}

			TableA.push_back(entry);
		}

		Trace::out("\n");
		// -----------------------------------------------
		// Hierarchy Table
		// -----------------------------------------------

		// complete table
		//for(size_t i = 0; i < TableA.size(); i++)
		//{
		//	Trace::out("%2d: ", i);
		//	for(size_t j = 0; j < TableA[i].size(); j++)
		//	{
		//		Trace::out("%2d ", TableA[i][j]);
		//	}
		//	Trace::out("\n");
		//}

		//Trace::out("\n");
		//Trace::out("--- Reworked --- \n");
		//Trace::out("int JointTable[] = { \n");

		std::vector<std::vector<int>> CorrectedTableA;

		for (size_t k = 0; k < model.skins[0].joints.size(); k++)
		{
			//Trace::out("%d: %d, \n", k, model.skins[0].joints[k]);
			size_t i = (unsigned int)model.skins[0].joints[k];
			CorrectedTableA.push_back(TableA[i]);
		}


		// -----------------------------------------------
		// put into a proto
		// -----------------------------------------------

		HierarchyData hData((unsigned int)CorrectedTableA.size(), (unsigned int)MaxDepth);
		size_t k = 0;
		for (size_t i = 0; i < CorrectedTableA.size(); i++)
		{
			for (size_t j = 0; j < CorrectedTableA[i].size(); j++)
			{
				hData.poData[k++] = (unsigned int)CorrectedTableA[i][j];
			}
		}

		//hData.Print("proto");

		HierarchyData_proto hA_proto;
		hData.Serialize(hA_proto);

		HierarchyData hB;
		hB.Deserialize(hA_proto);
		hB.Print("hb");

		// -------------------------------
		//  Write to file
		//--------------------------------

		File::Handle fh;
		File::Error err;

		// Create output name
		std::string sProtoFileType = "h";
		std::string sBaseName = pTargetName;

		std::string OutputFileName = sBaseName + "." + sProtoFileType + ".proto.azul";
		Trace::out(" --> %+30s\n", OutputFileName.c_str());
		err = File::Open(fh, OutputFileName.c_str(), File::Mode::WRITE);
		assert(err == File::Error::SUCCESS);

		std::string strOut;
		hA_proto.SerializeToString(&strOut);

		File::Write(fh, strOut.data(), (DWORD)strOut.length());
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

		char *poNewBuff = new char[FileLength]();
		assert(poNewBuff);

		err = File::Seek(fh, File::Position::BEGIN, 0);
		assert(err == File::Error::SUCCESS);

		err = File::Read(fh, poNewBuff, FileLength);
		assert(err == File::Error::SUCCESS);

		err = File::Close(fh);
		assert(err == File::Error::SUCCESS);


		delete[] poNewBuff;


	};

}

// --- End of File ---
