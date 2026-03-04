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

TEST(AddNodeRoot_Check,TestConfig::ALL)
{
#if AddNodeRoot_Check

	PCSTree::Info info;
	PCSTree tree;
	PCSNode  *root;
	PCSNode nodeRoot("Root");

	CHECK(nodeRoot.GetChild() == nullptr);
	CHECK(nodeRoot.GetParent() == nullptr);
	CHECK(nodeRoot.GetNextSibling() == nullptr);
	CHECK(nodeRoot.GetPrevSibling() == nullptr);

	root = tree.GetRoot();
	tree.Insert(&nodeRoot, root);

	CHECK(nodeRoot.GetChild() == nullptr);
	CHECK(nodeRoot.GetParent() == nullptr);
	CHECK(nodeRoot.GetNextSibling() == nullptr);
	CHECK(nodeRoot.GetPrevSibling() == nullptr);

	root = tree.GetRoot();
	CHECK(root == &nodeRoot);

	tree.GetInfo(info);

	CHECK(info.maxNumNodes == 1);
	CHECK(info.maxNumLevels == 1);
	CHECK(info.currNumLevels == 1);
	CHECK(info.currNumNodes == 1);

#endif
}TEST_END

// ---  End of File ---
