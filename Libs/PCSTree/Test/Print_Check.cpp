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

TEST(Print_Check, TestConfig::ALL)
{
#if Print_Check

	PCSNode n;

	PCSNode nodeA;
	PCSNode nodeB;
	PCSNode nodeC;
	PCSNode nodeD;

	nodeA.SetName("Node_A");
	nodeB.SetName("Node_B");
	nodeC.SetName("Node_C");
	nodeD.SetName("Node_D");

	n.SetName("Node_X");

	n.SetParent(&nodeA);
	n.SetNextSibling(&nodeB);
	n.SetPrevSibling(&nodeC);
	n.SetChild(&nodeD);

	CHECK(n.GetChild() == &nodeD);
	CHECK(n.GetParent() == &nodeA);
	CHECK(n.GetNextSibling() == &nodeB);
	CHECK(n.GetPrevSibling() == &nodeC);

	n.PrintNode();

	Trace::out("\n");

	CHECK(n.GetChild() == &nodeD);
	CHECK(n.GetParent() == &nodeA);
	CHECK(n.GetNextSibling() == &nodeB);
	CHECK(n.GetPrevSibling() == &nodeC);

#endif
} TEST_END

// ---  End of File ---
