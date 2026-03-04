//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

// --------------------------------
// ---      DO NOT MODIFY       ---
// --------------------------------

#include "_UnitTestConfiguration.h"

#include "PCSNode.h"
using namespace Azul;

//---------------------------------------------------------------------------
// Test Files:
// Single step through, set break points, but do not modify it
//---------------------------------------------------------------------------

TEST(AssignOperator_Check, TestConfig::ALL)
{
#if AssignOperator_Check

	PCSNode nodeA("node_A");
	PCSNode nodeB("node_B");
	PCSNode nodeC("node_C");
	PCSNode nodeD("node_D");
	PCSNode node("TestNode");

	CHECK(&nodeA != &nodeB);
	CHECK(&nodeA != &nodeC);
	CHECK(&nodeA != &nodeD);
	CHECK(&nodeB != &nodeC);
	CHECK(&nodeB != &nodeD);
	CHECK(&nodeC != &nodeD);

	CHECK(node.GetChild() == nullptr);
	CHECK(node.GetParent() == nullptr);
	CHECK(node.GetNextSibling() == nullptr);
	CHECK(node.GetPrevSibling() == nullptr);

	node.SetParent(&nodeA);
	node.SetChild(&nodeB);
	node.SetNextSibling(&nodeC);
	node.SetPrevSibling(&nodeD);

	CHECK(node.GetPrevSibling() == &nodeD);
	CHECK(node.GetNextSibling() == &nodeC);
	CHECK(node.GetParent() == &nodeA);
	CHECK(node.GetChild() == &nodeB);

	CHECK(nodeA.GetPrevSibling() == nullptr);
	CHECK(nodeA.GetNextSibling() == nullptr);
	CHECK(nodeA.GetParent() == nullptr);
	CHECK(nodeA.GetChild() == nullptr);

	CHECK(nodeB.GetPrevSibling() == nullptr);
	CHECK(nodeB.GetNextSibling() == nullptr);
	CHECK(nodeB.GetParent() == nullptr);
	CHECK(nodeB.GetChild() == nullptr);

	CHECK(nodeC.GetPrevSibling() == nullptr);
	CHECK(nodeC.GetNextSibling() == nullptr);
	CHECK(nodeC.GetParent() == nullptr);
	CHECK(nodeC.GetChild() == nullptr);

	CHECK(nodeD.GetPrevSibling() == nullptr);
	CHECK(nodeD.GetNextSibling() == nullptr);
	CHECK(nodeD.GetParent() == nullptr);
	CHECK(nodeD.GetChild() == nullptr);

	PCSNode copyNode;
	// assignment
	copyNode = node;

	CHECK(node.GetPrevSibling() == copyNode.GetPrevSibling());
	CHECK(node.GetNextSibling() == copyNode.GetNextSibling());
	CHECK(node.GetParent() == copyNode.GetParent());
	CHECK(node.GetChild() == copyNode.GetChild());

	char buff[PCSNode::NAME_SIZE];
	PCSNode::Code returnCode;
	returnCode = copyNode.GetName(buff, PCSNode::NAME_SIZE);
	CHECK(strcmp(buff, "TestNode") == 0);
	CHECK(returnCode == PCSNode::Code::SUCCESS);

	CHECK(nodeA.GetPrevSibling() == nullptr);
	CHECK(nodeA.GetNextSibling() == nullptr);
	CHECK(nodeA.GetParent() == nullptr);
	CHECK(nodeA.GetChild() == nullptr);

	CHECK(nodeB.GetPrevSibling() == nullptr);
	CHECK(nodeB.GetNextSibling() == nullptr);
	CHECK(nodeB.GetParent() == nullptr);
	CHECK(nodeB.GetChild() == nullptr);

	CHECK(nodeC.GetPrevSibling() == nullptr);
	CHECK(nodeC.GetNextSibling() == nullptr);
	CHECK(nodeC.GetParent() == nullptr);
	CHECK(nodeC.GetChild() == nullptr);

	CHECK(nodeD.GetPrevSibling() == nullptr);
	CHECK(nodeD.GetNextSibling() == nullptr);
	CHECK(nodeD.GetParent() == nullptr);
	CHECK(nodeD.GetChild() == nullptr);

#endif
} TEST_END

// ---  End of File ---
