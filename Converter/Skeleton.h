//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef SKELETON2_H
#define SKELETON2_H

#include <vector>

#include "tiny_gltf.h"
#include "MathEngine.h"

#include "SkelData.h"

using namespace tinygltf;

namespace Azul
{

	struct skeleton
	{
		skeleton();
		skeleton(const skeleton&) = default;
		skeleton& operator = (const skeleton&) = default;
		~skeleton() = default;

		int          nodeIndex;
		std::string  nodeName;
		int          parentIndex;
		std::string  parentName;
	};

	struct skeleton_table
	{
		//static const unsigned int SKELETON_NAME_SIZE = 64;
		// use SkelData.h

		skeleton_table() = default;
		skeleton_table(const skeleton_table&) = default;
		skeleton_table& operator = (const skeleton_table&) = default;
		~skeleton_table() = default;

		int   nodeIndex;
		int   parentIndex;
		char  nodeName[SkelEntry::SKELETON_NAME_SIZE];
	};

	class SKEL
	{
	public:
		void CreateSkeleton(Model& model, const char* const pTargetName);
		void CreateHierarchy(Model &model, const char *const pTargetName);
	private:
		std::vector<skeleton>      skeletonVector;
		std::vector<skeleton_table> skeletonTable;

	};

}

#endif

// --- End of File ---
