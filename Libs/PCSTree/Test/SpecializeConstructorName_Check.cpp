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


TEST(SpecializeConstuctorName_Check, TestConfig::ALL)
{
#if SpecializeConstuctorName_Check

	PCSNode nodeA;
	PCSNode nodeB;
	PCSNode nodeC;
	PCSNode nodeD;

	PCSNode n("Node_X");

	CHECK(n.GetChild() == nullptr);
	CHECK(n.GetParent() == nullptr);
	CHECK(n.GetNextSibling() == nullptr);
	CHECK(n.GetPrevSibling() == nullptr);

	nodeA.SetName("Node_A");
	nodeB.SetName("Node_B");
	nodeC.SetName("Node_C");
	nodeD.SetName("Node_D");

	n.SetParent(&nodeA);
	n.SetNextSibling(&nodeC);
	n.SetPrevSibling(&nodeD);
	n.SetChild(&nodeB);

	CHECK(n.GetChild() == &nodeB);
	CHECK(n.GetParent() == &nodeA);
	CHECK(n.GetNextSibling() == &nodeC);
	CHECK(n.GetPrevSibling() == &nodeD);

	char tmpName[PCSNode::NAME_SIZE];
	n.GetName(tmpName, PCSNode::NAME_SIZE);
	CHECK(strcmp(tmpName, "Node_X") == 0);

#endif
} TEST_END


// ---  End of File ---
