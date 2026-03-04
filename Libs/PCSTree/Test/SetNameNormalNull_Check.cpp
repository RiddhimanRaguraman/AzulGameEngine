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

TEST(SetNameNormalNull_Check, TestConfig::ALL)
{
#if SetNameNormalNull_Check

	PCSNode pna;

	CHECK(pna.GetChild() == nullptr);
	CHECK(pna.GetParent() == nullptr);
	CHECK(pna.GetNextSibling() == nullptr);
	CHECK(pna.GetPrevSibling() == nullptr);

	char *buff_null = 0;
	PCSNode::Code returnCode;

	returnCode = pna.SetName(buff_null);
	CHECK(returnCode == PCSNode::Code::FAIL_NULL_PTR);

	char buff[PCSNode::NAME_SIZE];

	returnCode = pna.GetName(buff, PCSNode::NAME_SIZE);

	CHECK(returnCode == PCSNode::Code::SUCCESS);

	CHECK(pna.GetChild() == nullptr);
	CHECK(pna.GetParent() == nullptr);
	CHECK(pna.GetNextSibling() == nullptr);
	CHECK(pna.GetPrevSibling() == nullptr);

#endif
} TEST_END

// ---  End of File ---
