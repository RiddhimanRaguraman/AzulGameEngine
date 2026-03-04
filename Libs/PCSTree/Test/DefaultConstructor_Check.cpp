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

TEST(DefaultConstructor_Check, TestConfig::ALL)
{
#if DefaultConstructor_Check

	PCSNode node;

	CHECK(node.GetChild() == nullptr);
	CHECK(node.GetParent() == nullptr);
	CHECK(node.GetNextSibling() == nullptr);
	CHECK(node.GetPrevSibling() == nullptr);

	char buff[PCSNode::NAME_SIZE];
	PCSNode::Code returnCode;
	returnCode = node.GetName(buff, PCSNode::NAME_SIZE);
	char testbuff[PCSNode::NAME_SIZE];
	memset(&testbuff[0], 0x0, PCSNode::NAME_SIZE);

	CHECK(memcmp(buff, testbuff, PCSNode::NAME_SIZE) == 0);
	CHECK(returnCode == PCSNode::Code::SUCCESS);

#endif
} TEST_END

// ---  End of File ---
