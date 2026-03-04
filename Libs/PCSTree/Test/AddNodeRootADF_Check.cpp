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

TEST(AddNodeRootADF_Check, TestConfig::ALL)
{
#if AddNodeRootADF_Check

	PCSTree::Info info;
	PCSTree tree;
	PCSNode  *root;
	PCSNode nodeRoot("Root");
	PCSNode nodeA("Node_A");
	PCSNode nodeD("Node_D");
	PCSNode nodeF("Node_F");

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

	tree.Insert(&nodeD, &nodeA);

	tree.GetInfo(info);
	CHECK(info.maxNumNodes == 3);
	CHECK(info.maxNumLevels == 3);
	CHECK(info.currNumLevels == 3);
	CHECK(info.currNumNodes == 3);

	CHECK(nodeRoot.GetChild() == &nodeA);
	CHECK(nodeRoot.GetParent() == nullptr);
	CHECK(nodeRoot.GetNextSibling() == nullptr);
	CHECK(nodeRoot.GetPrevSibling() == nullptr);

	CHECK(nodeA.GetChild() == &nodeD);
	CHECK(nodeA.GetParent() == &nodeRoot);
	CHECK(nodeA.GetNextSibling() == nullptr);
	CHECK(nodeA.GetPrevSibling() == nullptr);

	CHECK(nodeD.GetChild() == nullptr);
	CHECK(nodeD.GetParent() == &nodeA);
	CHECK(nodeD.GetNextSibling() == nullptr);
	CHECK(nodeD.GetPrevSibling() == nullptr);

	tree.Insert(&nodeF, &nodeD);

	tree.GetInfo(info);
	CHECK(info.maxNumNodes == 4);
	CHECK(info.maxNumLevels == 4);
	CHECK(info.currNumLevels == 4);
	CHECK(info.currNumNodes == 4);

	CHECK(nodeRoot.GetChild() == &nodeA);
	CHECK(nodeRoot.GetParent() == nullptr);
	CHECK(nodeRoot.GetNextSibling() == nullptr);
	CHECK(nodeRoot.GetPrevSibling() == nullptr);

	CHECK(nodeA.GetChild() == &nodeD);
	CHECK(nodeA.GetParent() == &nodeRoot);
	CHECK(nodeA.GetNextSibling() == nullptr);
	CHECK(nodeA.GetPrevSibling() == nullptr);

	CHECK(nodeD.GetChild() == &nodeF);
	CHECK(nodeD.GetParent() == &nodeA);
	CHECK(nodeD.GetNextSibling() == nullptr);
	CHECK(nodeD.GetPrevSibling() == nullptr);

	CHECK(nodeF.GetChild() == nullptr);
	CHECK(nodeF.GetParent() == &nodeD);
	CHECK(nodeF.GetNextSibling() == nullptr);
	CHECK(nodeF.GetPrevSibling() == nullptr);

#endif
}TEST_END


// ---  End of File ---
