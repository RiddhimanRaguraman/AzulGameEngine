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

TEST(AddNodeRootAB_Check, TestConfig::ALL)
{
#if AddNodeRootAB_Check

	PCSTree::Info info;
	PCSTree tree;
	PCSNode  *root;
	PCSNode nodeRoot("Root");
	PCSNode nodeA("Node_A");
	PCSNode nodeB("Node_B");

	root = tree.GetRoot();
	tree.Insert(&nodeRoot, root);
	tree.Insert(&nodeB, &nodeRoot);

	tree.GetInfo(info);
	CHECK(info.maxNumNodes == 2);
	CHECK(info.maxNumLevels == 2);
	CHECK(info.currNumLevels == 2);
	CHECK(info.currNumNodes == 2);

	CHECK(nodeRoot.GetChild() == &nodeB);
	CHECK(nodeRoot.GetParent() == nullptr);
	CHECK(nodeRoot.GetNextSibling() == nullptr);
	CHECK(nodeRoot.GetPrevSibling() == nullptr);

	CHECK(nodeB.GetChild() == nullptr);
	CHECK(nodeB.GetParent() == &nodeRoot);
	CHECK(nodeB.GetNextSibling() == nullptr);
	CHECK(nodeB.GetPrevSibling() == nullptr);

	tree.Insert(&nodeA, &nodeRoot);

	tree.GetInfo(info);
	CHECK(info.maxNumNodes == 3);
	CHECK(info.maxNumLevels == 2);
	CHECK(info.currNumLevels == 2);
	CHECK(info.currNumNodes == 3);

	CHECK(nodeRoot.GetChild() == &nodeA);
	CHECK(nodeRoot.GetParent() == nullptr);
	CHECK(nodeRoot.GetNextSibling() == nullptr);
	CHECK(nodeRoot.GetPrevSibling() == nullptr);

	CHECK(nodeB.GetChild() == nullptr);
	CHECK(nodeB.GetParent() == &nodeRoot);
	CHECK(nodeB.GetNextSibling() == nullptr);
	CHECK(nodeB.GetPrevSibling() == &nodeA);

	CHECK(nodeA.GetChild() == nullptr);
	CHECK(nodeA.GetParent() == &nodeRoot);
	CHECK(nodeA.GetNextSibling() == &nodeB);
	CHECK(nodeA.GetPrevSibling() == nullptr);

#endif
}TEST_END


// ---  End of File ---
