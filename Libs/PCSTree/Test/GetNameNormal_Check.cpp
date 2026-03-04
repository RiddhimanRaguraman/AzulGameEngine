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

TEST(GetNameNormal_Check, TestConfig::ALL)
{
#if GetNameNormal_Check

	PCSNode pna;

	CHECK(pna.GetChild() == nullptr);
	CHECK(pna.GetParent() == nullptr);
	CHECK(pna.GetNextSibling() == nullptr);
	CHECK(pna.GetPrevSibling() == nullptr);

	pna.SetName("123456789012345");

	char buff[PCSNode::NAME_SIZE];
	PCSNode::Code returnCode;

	returnCode = pna.GetName(buff, PCSNode::NAME_SIZE);

	CHECK(strcmp(buff, "123456789012345") == 0);
	CHECK(returnCode == PCSNode::Code::SUCCESS);

	CHECK(pna.GetChild() == nullptr);
	CHECK(pna.GetParent() == nullptr);
	CHECK(pna.GetNextSibling() == nullptr);
	CHECK(pna.GetPrevSibling() == nullptr);

#endif
} TEST_END

// ---  End of File ---
