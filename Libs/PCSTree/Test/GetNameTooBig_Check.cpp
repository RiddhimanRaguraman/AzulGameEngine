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

TEST(GetNameTooBig_Check, TestConfig::ALL)
{
#if GetNameTooBig_Check

	PCSNode pna;

	CHECK(pna.GetChild() == nullptr);
	CHECK(pna.GetParent() == nullptr);
	CHECK(pna.GetNextSibling() == nullptr);
	CHECK(pna.GetPrevSibling() == nullptr);

	pna.SetName("123456789012345");

	char buff[80];
	PCSNode::Code returnCode;

	returnCode = pna.GetName(buff, 80);

	CHECK(strcmp(buff, "123456789012345") == 0);

	CHECK(returnCode == PCSNode::Code::SUCCESS);

	CHECK(pna.GetChild() == nullptr);
	CHECK(pna.GetParent() == nullptr);
	CHECK(pna.GetNextSibling() == nullptr);
	CHECK(pna.GetPrevSibling() == nullptr);

#endif
} TEST_END


// ---  End of File ---
