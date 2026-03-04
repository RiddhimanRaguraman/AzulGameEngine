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

TEST(AddNodeRootA_Check, TestConfig::ALL)
{
#if AddNodeRootA_Check

	PCSTree::Info info;
	PCSTree tree;
	PCSNode  *root;
	PCSNode nodeRoot("Root");
	PCSNode nodeA("Node_A");

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

	tree.Insert(&nodeA, &nodeRoot);

	tree.GetInfo(info);
	CHECK(info.maxNumNodes == 2);
	CHECK(info.maxNumLevels == 2);
	CHECK(info.currNumLevels == 2);
	CHECK(info.currNumNodes == 2);

	CHECK(nodeRoot.GetChild() == &nodeA);
	CHECK(nodeRoot.GetParent() == nullptr);
	CHECK(nodeRoot.GetNextSibling() == nullptr);
	CHECK(nodeRoot.GetPrevSibling() == nullptr);

	CHECK(nodeA.GetChild() == nullptr);
	CHECK(nodeA.GetParent() == &nodeRoot);
	CHECK(nodeA.GetNextSibling() == nullptr);
	CHECK(nodeA.GetPrevSibling() == nullptr);

#endif
}TEST_END


// ---  End of File ---
