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

TEST(PrintNodeWith0Links_Check, TestConfig::ALL)
{
#if PrintNodeWith0Links_Check

	PCSNode n("Node_X");

	CHECK(n.GetChild() == nullptr);
	CHECK(n.GetParent() == nullptr);
	CHECK(n.GetNextSibling() == nullptr);
	CHECK(n.GetPrevSibling() == nullptr);

	char buff[32];
	CHECK(n.GetName(buff, 32) == PCSNode::Code::SUCCESS);

	CHECK(strcmp(buff, "Node_X") == 0);

	n.PrintNode();

	Trace::out("\n");

#endif
} TEST_END


// ---  End of File ---
