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

TEST(SpecializeConstuctor_Check, TestConfig::ALL)
{
#if SpecializeConstuctor_Check

	PCSNode nodeA;
	PCSNode nodeB;
	PCSNode nodeC;
	PCSNode nodeD;

	PCSNode n(&nodeA, &nodeB, &nodeC, &nodeD, "Node_Z");

	nodeA.SetName("Node_A");
	nodeB.SetName("Node_B");
	nodeC.SetName("Node_C");
	nodeD.SetName("Node_D");

	CHECK(n.GetChild() == &nodeB);
	CHECK(n.GetParent() == &nodeA);
	CHECK(n.GetNextSibling() == &nodeC);
	CHECK(n.GetPrevSibling() == &nodeD);

	char tmpName[PCSNode::NAME_SIZE];

	n.GetName(tmpName, PCSNode::NAME_SIZE);

	CHECK(strcmp(tmpName, "Node_Z") == 0);

	CHECK(n.GetChild() == &nodeB);
	CHECK(n.GetParent() == &nodeA);
	CHECK(n.GetNextSibling() == &nodeC);
	CHECK(n.GetPrevSibling() == &nodeD);

#endif
} TEST_END

// ---  End of File ---
