//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

// --------------------------------
// ---      DO NOT MODIFY       ---
// --------------------------------

#include "_UnitTestConfiguration.h"

#include "PCSNode.h"
using namespace Azul;

static PCSNode *pn = nullptr;

//---------------------------------------------------------------------------
// Test Files:
// Single step through, set break points, but do not modify it
//---------------------------------------------------------------------------

TEST_WITH_TEARDOWN(Destructor_Check, TestConfig::ALL)
{
#if Destructor_Check

	pn = new PCSNode();
	PCSNode nodeB;
	PCSNode nodeC;
	PCSNode nodeA;
	PCSNode nodeD;

	CHECK(&nodeA != &nodeB);
	CHECK(&nodeA != &nodeC);
	CHECK(&nodeA != &nodeD);
	CHECK(&nodeB != &nodeC);
	CHECK(&nodeB != &nodeD);
	CHECK(&nodeC != &nodeD);

	CHECK(pn->GetChild() == nullptr);
	CHECK(pn->GetParent() == nullptr);
	CHECK(pn->GetNextSibling() == nullptr);
	CHECK(pn->GetPrevSibling() == nullptr);

	pn->SetParent(&nodeA);
	pn->SetChild(&nodeB);
	pn->SetNextSibling(&nodeC);
	pn->SetPrevSibling(&nodeD);

	CHECK(pn->GetPrevSibling() == &nodeD);
	CHECK(pn->GetNextSibling() == &nodeC);
	CHECK(pn->GetParent() == &nodeA);
	CHECK(pn->GetChild() == &nodeB);

	delete pn;
	pn = nullptr;

#endif
} TEST_END

TEST_TEARDOWN(Destructor_Check)
{
	delete pn;
	pn = nullptr;
} 


// ---  End of File ---
