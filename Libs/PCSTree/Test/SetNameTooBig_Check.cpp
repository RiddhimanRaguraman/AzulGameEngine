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

TEST(SetNameTooBig_Check,TestConfig::ALL)
{
#if SetNameTooBig_Check

	PCSNode pna;

	CHECK(pna.GetChild() == nullptr);
	CHECK(pna.GetParent() == nullptr);
	CHECK(pna.GetNextSibling() == nullptr);
	CHECK(pna.GetPrevSibling() == nullptr);

	PCSNode::Code returnCode;

	returnCode = pna.SetName("Node_A_123456789012345678901234567890123456789");
	CHECK(returnCode == PCSNode::Code::SUCCESS);
	char buff[PCSNode::NAME_SIZE];

	returnCode = pna.GetName(buff, PCSNode::NAME_SIZE);

	CHECK(strcmp(buff, "Node_A_123456789012345678901234") == 0);
	CHECK(returnCode == PCSNode::Code::SUCCESS);

	CHECK(pna.GetChild() == nullptr);
	CHECK(pna.GetParent() == nullptr);
	CHECK(pna.GetNextSibling() == nullptr);
	CHECK(pna.GetPrevSibling() == nullptr);

#endif
} TEST_END


// ---  End of File ---
