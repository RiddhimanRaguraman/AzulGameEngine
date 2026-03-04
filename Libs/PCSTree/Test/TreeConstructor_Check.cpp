//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

// --------------------------------
// ---      DO NOT MODIFY       ---
// --------------------------------

#include "_UnitTestConfiguration.h"
#include "PCSTree.h"
#include "PCSNode.h"

//---------------------------------------------------------------------------
// Test Files:
// Single step through, set break points, but do not modify it
//---------------------------------------------------------------------------

using namespace Azul;

TEST(TreeConstructor_Check, TestConfig::ALL)
{
#if TreeConstructor_Check

	PCSTree::Info info;
	PCSTree tree;
	PCSNode  *root;

	tree.GetInfo(info);
	root = tree.GetRoot();

	CHECK(root == nullptr);
	CHECK(info.maxNumNodes == 0);
	CHECK(info.currNumLevels == 0);
	CHECK(info.currNumNodes == 0);

#endif
} TEST_END


// ---  End of File ---
