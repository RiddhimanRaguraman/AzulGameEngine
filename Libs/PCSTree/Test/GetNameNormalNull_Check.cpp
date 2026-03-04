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

TEST(GetNameNormalNull_Check, TestConfig::ALL)
{
#if GetNameNormalNull_Check

	PCSNode pna;

	CHECK(pna.GetChild() == nullptr);
	CHECK(pna.GetParent() == nullptr);
	CHECK(pna.GetNextSibling() == nullptr);
	CHECK(pna.GetPrevSibling() == nullptr);

	pna.SetName("123456789012345");

	char *buff_null = 0;

	PCSNode::Code returnCode;

	returnCode = pna.GetName(buff_null, PCSNode::NAME_SIZE);

	CHECK(buff_null == nullptr);
	CHECK(returnCode == PCSNode::Code::FAIL_NULL_PTR);

	CHECK(pna.GetChild() == nullptr);
	CHECK(pna.GetParent() == nullptr);
	CHECK(pna.GetNextSibling() == nullptr);
	CHECK(pna.GetPrevSibling() == nullptr);

#endif
} TEST_END


// ---  End of File ---
