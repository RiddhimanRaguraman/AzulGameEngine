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

TEST(SetNameNormal_Check, TestConfig::ALL)
{
#if SetNameNormal_Check

	PCSNode pna;

	CHECK(pna.GetChild() == nullptr);
	CHECK(pna.GetParent() == nullptr);
	CHECK(pna.GetNextSibling() == nullptr);
	CHECK(pna.GetPrevSibling() == nullptr);

	PCSNode::Code returnCode;

	returnCode = pna.SetName("Node_A");

	CHECK(returnCode == PCSNode::Code::SUCCESS);
	char buff[PCSNode::NAME_SIZE];
	 
	returnCode = pna.GetName(buff, PCSNode::NAME_SIZE);

	CHECK(strcmp(buff, "Node_A") == 0);
	CHECK(returnCode == PCSNode::Code::SUCCESS);

	CHECK(pna.GetChild() == nullptr);
	CHECK(pna.GetParent() == nullptr);
	CHECK(pna.GetNextSibling() == nullptr);
	CHECK(pna.GetPrevSibling() == nullptr);

#endif
} TEST_END

// ---  End of File ---
