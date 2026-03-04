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

TEST(Version_Check, TestConfig::ALL)
{
#if Version_Check

	CHECK(sizeof(Azul::PCSNode) == 88);
	CHECK(PCSNode::MAJOR_VERSION == 6);
	CHECK(PCSNode::MINOR_VERSION == 0);

#endif
} TEST_END

// ---  End of File ---
