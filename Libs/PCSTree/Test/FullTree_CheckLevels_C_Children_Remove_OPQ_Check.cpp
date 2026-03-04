//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

// --------------------------------
// ---      DO NOT MODIFY       ---
// --------------------------------

#include "_UnitTestConfiguration.h"
#include "PCSTree.h"
#include "PCSNode.h"

//---------------------------------------------------------------------------
// Test Files:
// Single step through, set break points, but do not modify it
//---------------------------------------------------------------------------

using namespace Azul;

TEST(FullTree_CheckLevels_C_Children_Remove_OPQ_Check, TestConfig::ALL)
{
#if FullTree_CheckLevels_C_Children_Remove_OPQ_Check

	PCSTree::Info info;
	PCSTree tree;
	PCSNode  *root;

	PCSNode nodeRoot("Root");
	PCSNode nodeA("Node_A");
	PCSNode nodeB("Node_B");
	PCSNode nodeC("Node_C");
	PCSNode nodeD("Node_D");
	PCSNode nodeE("Node_E");
	PCSNode nodeI("Node_I");
	PCSNode nodeK("Node_K");
	PCSNode nodeL("Node_L");
	PCSNode nodeR("Node_R");
	PCSNode nodeF("Node_F");
	PCSNode nodeG("Node_G");
	PCSNode nodeJ("Node_J");
	PCSNode nodeM("Node_M");
	PCSNode nodeN("Node_N");
	PCSNode nodeO("Node_O");
	PCSNode nodeP("Node_P");
	PCSNode nodeQ("Node_Q");
	PCSNode nodeH("Node_H");
	PCSNode nodeS("Node_S");
	PCSNode nodeT("Node_T");
	PCSNode nodeU("Node_U");
	PCSNode nodeV("Node_V");
	PCSNode nodeW("Node_W");

	root = tree.GetRoot();
	tree.Insert(&nodeRoot, root);

	tree.Insert(&nodeC, &nodeRoot);
	tree.Insert(&nodeB, &nodeRoot);
	tree.Insert(&nodeA, &nodeRoot);
	tree.Insert(&nodeI, &nodeA);
	tree.Insert(&nodeE, &nodeA);
	tree.Insert(&nodeD, &nodeA);
	tree.Insert(&nodeL, &nodeB);
	tree.Insert(&nodeK, &nodeB);
	tree.Insert(&nodeR, &nodeC);
	tree.Insert(&nodeF, &nodeD);
	tree.Insert(&nodeJ, &nodeI);
	tree.Insert(&nodeG, &nodeI);
	tree.Insert(&nodeN, &nodeL);
	tree.Insert(&nodeM, &nodeL);
	tree.Insert(&nodeQ, &nodeR);
	tree.Insert(&nodeP, &nodeR);
	tree.Insert(&nodeO, &nodeR);
	tree.Insert(&nodeH, &nodeF);
	tree.Insert(&nodeS, &nodeP);
	tree.Insert(&nodeW, &nodeS);
	tree.Insert(&nodeV, &nodeS);
	tree.Insert(&nodeU, &nodeS);
	tree.Insert(&nodeT, &nodeS);

	tree.GetInfo(info);

	CHECK(info.maxNumNodes == 24);
	CHECK(info.maxNumLevels == 6);
	CHECK(info.currNumLevels == 6);
	CHECK(info.currNumNodes == 24);

	CHECK(nodeRoot.GetChild() == &nodeA);
	CHECK(nodeRoot.GetParent() == nullptr);
	CHECK(nodeRoot.GetNextSibling() == nullptr);
	CHECK(nodeRoot.GetPrevSibling() == nullptr);

	CHECK(nodeA.GetChild() == &nodeD);
	CHECK(nodeA.GetParent() == &nodeRoot);
	CHECK(nodeA.GetNextSibling() == &nodeB);
	CHECK(nodeA.GetPrevSibling() == nullptr);

	CHECK(nodeB.GetChild() == &nodeK);
	CHECK(nodeB.GetParent() == &nodeRoot);
	CHECK(nodeB.GetNextSibling() == &nodeC);
	CHECK(nodeB.GetPrevSibling() == &nodeA);

	CHECK(nodeC.GetChild() == &nodeR);
	CHECK(nodeC.GetParent() == &nodeRoot);
	CHECK(nodeC.GetNextSibling() == nullptr);
	CHECK(nodeC.GetPrevSibling() == &nodeB);

	CHECK(nodeD.GetChild() == &nodeF);
	CHECK(nodeD.GetParent() == &nodeA);
	CHECK(nodeD.GetNextSibling() == &nodeE);
	CHECK(nodeD.GetPrevSibling() == nullptr);

	CHECK(nodeE.GetChild() == nullptr);
	CHECK(nodeE.GetParent() == &nodeA);
	CHECK(nodeE.GetNextSibling() == &nodeI);
	CHECK(nodeE.GetPrevSibling() == &nodeD);

	CHECK(nodeI.GetChild() == &nodeG);
	CHECK(nodeI.GetParent() == &nodeA);
	CHECK(nodeI.GetNextSibling() == nullptr);
	CHECK(nodeI.GetPrevSibling() == &nodeE);

	CHECK(nodeK.GetChild() == nullptr);
	CHECK(nodeK.GetParent() == &nodeB);
	CHECK(nodeK.GetNextSibling() == &nodeL);
	CHECK(nodeK.GetPrevSibling() == nullptr);

	CHECK(nodeL.GetChild() == &nodeM);
	CHECK(nodeL.GetParent() == &nodeB);
	CHECK(nodeL.GetNextSibling() == nullptr);
	CHECK(nodeL.GetPrevSibling() == &nodeK);

	CHECK(nodeR.GetChild() == &nodeO);
	CHECK(nodeR.GetParent() == &nodeC);
	CHECK(nodeR.GetNextSibling() == nullptr);
	CHECK(nodeR.GetPrevSibling() == nullptr);

	CHECK(nodeF.GetChild() == &nodeH);
	CHECK(nodeF.GetParent() == &nodeD);
	CHECK(nodeF.GetNextSibling() == nullptr);
	CHECK(nodeF.GetPrevSibling() == nullptr);

	CHECK(nodeG.GetChild() == nullptr);
	CHECK(nodeG.GetParent() == &nodeI);
	CHECK(nodeG.GetNextSibling() == &nodeJ);
	CHECK(nodeG.GetPrevSibling() == nullptr);

	CHECK(nodeJ.GetChild() == nullptr);
	CHECK(nodeJ.GetParent() == &nodeI);
	CHECK(nodeJ.GetNextSibling() == nullptr);
	CHECK(nodeJ.GetPrevSibling() == &nodeG);

	CHECK(nodeM.GetChild() == nullptr);
	CHECK(nodeM.GetParent() == &nodeL);
	CHECK(nodeM.GetNextSibling() == &nodeN);
	CHECK(nodeM.GetPrevSibling() == nullptr);

	CHECK(nodeN.GetChild() == nullptr);
	CHECK(nodeN.GetParent() == &nodeL);
	CHECK(nodeN.GetNextSibling() == nullptr);
	CHECK(nodeN.GetPrevSibling() == &nodeM);

	CHECK(nodeO.GetChild() == nullptr);
	CHECK(nodeO.GetParent() == &nodeR);
	CHECK(nodeO.GetNextSibling() == &nodeP);
	CHECK(nodeO.GetPrevSibling() == nullptr);

	CHECK(nodeP.GetChild() == &nodeS);
	CHECK(nodeP.GetParent() == &nodeR);
	CHECK(nodeP.GetNextSibling() == &nodeQ);
	CHECK(nodeP.GetPrevSibling() == &nodeO);

	CHECK(nodeQ.GetChild() == nullptr);
	CHECK(nodeQ.GetParent() == &nodeR);
	CHECK(nodeQ.GetNextSibling() == nullptr);
	CHECK(nodeQ.GetPrevSibling() == &nodeP);

	CHECK(nodeH.GetChild() == nullptr);
	CHECK(nodeH.GetParent() == &nodeF);
	CHECK(nodeH.GetNextSibling() == nullptr);
	CHECK(nodeH.GetPrevSibling() == nullptr);

	CHECK(nodeS.GetChild() == &nodeT);
	CHECK(nodeS.GetParent() == &nodeP);
	CHECK(nodeS.GetNextSibling() == nullptr);
	CHECK(nodeS.GetPrevSibling() == nullptr);

	CHECK(nodeT.GetChild() == nullptr);
	CHECK(nodeT.GetParent() == &nodeS);
	CHECK(nodeT.GetNextSibling() == &nodeU);
	CHECK(nodeT.GetPrevSibling() == nullptr);

	CHECK(nodeU.GetChild() == nullptr);
	CHECK(nodeU.GetParent() == &nodeS);
	CHECK(nodeU.GetNextSibling() == &nodeV);
	CHECK(nodeU.GetPrevSibling() == &nodeT);

	CHECK(nodeV.GetChild() == nullptr);
	CHECK(nodeV.GetParent() == &nodeS);
	CHECK(nodeV.GetNextSibling() == &nodeW);
	CHECK(nodeV.GetPrevSibling() == &nodeU);

	CHECK(nodeW.GetChild() == nullptr);
	CHECK(nodeW.GetParent() == &nodeS);
	CHECK(nodeW.GetNextSibling() == nullptr);
	CHECK(nodeW.GetPrevSibling() == &nodeV);

	tree.Remove(&nodeA);
	tree.Remove(&nodeB);
	tree.Remove(&nodeO);
	tree.Remove(&nodeP);
	tree.Remove(&nodeQ);

	tree.GetInfo(info);

	CHECK(info.maxNumNodes == 24);
	CHECK(info.maxNumLevels == 6);
	CHECK(info.currNumLevels == 3);
	CHECK(info.currNumNodes == 3);

	CHECK(nodeRoot.GetChild() == &nodeC);
	CHECK(nodeRoot.GetParent() == nullptr);
	CHECK(nodeRoot.GetNextSibling() == nullptr);
	CHECK(nodeRoot.GetPrevSibling() == nullptr);

	CHECK(nodeA.GetChild() == nullptr);
	CHECK(nodeA.GetParent() == nullptr);
	CHECK(nodeA.GetNextSibling() == nullptr);
	CHECK(nodeA.GetPrevSibling() == nullptr);

	CHECK(nodeB.GetChild() == nullptr);
	CHECK(nodeB.GetParent() == nullptr);
	CHECK(nodeB.GetNextSibling() == nullptr);
	CHECK(nodeB.GetPrevSibling() == nullptr);

	CHECK(nodeC.GetChild() == &nodeR);
	CHECK(nodeC.GetParent() == &nodeRoot);
	CHECK(nodeC.GetNextSibling() == nullptr);
	CHECK(nodeC.GetPrevSibling() == nullptr);

	CHECK(nodeD.GetChild() == nullptr);
	CHECK(nodeD.GetParent() == nullptr);
	CHECK(nodeD.GetNextSibling() == nullptr);
	CHECK(nodeD.GetPrevSibling() == nullptr);

	CHECK(nodeE.GetChild() == nullptr);
	CHECK(nodeE.GetParent() == nullptr);
	CHECK(nodeE.GetNextSibling() == nullptr);
	CHECK(nodeE.GetPrevSibling() == nullptr);

	CHECK(nodeI.GetChild() == nullptr);
	CHECK(nodeI.GetParent() == nullptr);
	CHECK(nodeI.GetNextSibling() == nullptr);
	CHECK(nodeI.GetPrevSibling() == nullptr);

	CHECK(nodeK.GetChild() == nullptr);
	CHECK(nodeK.GetParent() == nullptr);
	CHECK(nodeK.GetNextSibling() == nullptr);
	CHECK(nodeK.GetPrevSibling() == nullptr);

	CHECK(nodeL.GetChild() == nullptr);
	CHECK(nodeL.GetParent() == nullptr);
	CHECK(nodeL.GetNextSibling() == nullptr);
	CHECK(nodeL.GetPrevSibling() == nullptr);

	CHECK(nodeR.GetChild() == nullptr);
	CHECK(nodeR.GetParent() == &nodeC);
	CHECK(nodeR.GetNextSibling() == nullptr);
	CHECK(nodeR.GetPrevSibling() == nullptr);

	CHECK(nodeF.GetChild() == nullptr);
	CHECK(nodeF.GetParent() == nullptr);
	CHECK(nodeF.GetNextSibling() == nullptr);
	CHECK(nodeF.GetPrevSibling() == nullptr);

	CHECK(nodeG.GetChild() == nullptr);
	CHECK(nodeG.GetParent() == nullptr);
	CHECK(nodeG.GetNextSibling() == nullptr);
	CHECK(nodeG.GetPrevSibling() == nullptr);

	CHECK(nodeJ.GetChild() == nullptr);
	CHECK(nodeJ.GetParent() == nullptr);
	CHECK(nodeJ.GetNextSibling() == nullptr);
	CHECK(nodeJ.GetPrevSibling() == nullptr);

	CHECK(nodeM.GetChild() == nullptr);
	CHECK(nodeM.GetParent() == nullptr);
	CHECK(nodeM.GetNextSibling() == nullptr);
	CHECK(nodeM.GetPrevSibling() == nullptr);

	CHECK(nodeN.GetChild() == nullptr);
	CHECK(nodeN.GetParent() == nullptr);
	CHECK(nodeN.GetNextSibling() == nullptr);
	CHECK(nodeN.GetPrevSibling() == nullptr);

	CHECK(nodeO.GetChild() == nullptr);
	CHECK(nodeO.GetParent() == nullptr);
	CHECK(nodeO.GetNextSibling() == nullptr);
	CHECK(nodeO.GetPrevSibling() == nullptr);

	CHECK(nodeP.GetChild() == nullptr);
	CHECK(nodeP.GetParent() == nullptr);
	CHECK(nodeP.GetNextSibling() == nullptr);
	CHECK(nodeP.GetPrevSibling() == nullptr);

	CHECK(nodeQ.GetChild() == nullptr);
	CHECK(nodeQ.GetParent() == nullptr);
	CHECK(nodeQ.GetNextSibling() == nullptr);
	CHECK(nodeQ.GetPrevSibling() == nullptr);

	CHECK(nodeH.GetChild() == nullptr);
	CHECK(nodeH.GetParent() == nullptr);
	CHECK(nodeH.GetNextSibling() == nullptr);
	CHECK(nodeH.GetPrevSibling() == nullptr);

	CHECK(nodeS.GetChild() == nullptr);
	CHECK(nodeS.GetParent() == nullptr);
	CHECK(nodeS.GetNextSibling() == nullptr);
	CHECK(nodeS.GetPrevSibling() == nullptr);

	CHECK(nodeT.GetChild() == nullptr);
	CHECK(nodeT.GetParent() == nullptr);
	CHECK(nodeT.GetNextSibling() == nullptr);
	CHECK(nodeT.GetPrevSibling() == nullptr);

	CHECK(nodeU.GetChild() == nullptr);
	CHECK(nodeU.GetParent() == nullptr);
	CHECK(nodeU.GetNextSibling() == nullptr);
	CHECK(nodeU.GetPrevSibling() == nullptr);

	CHECK(nodeV.GetChild() == nullptr);
	CHECK(nodeV.GetParent() == nullptr);
	CHECK(nodeV.GetNextSibling() == nullptr);
	CHECK(nodeV.GetPrevSibling() == nullptr);

	CHECK(nodeW.GetChild() == nullptr);
	CHECK(nodeW.GetParent() == nullptr);
	CHECK(nodeW.GetNextSibling() == nullptr);
	CHECK(nodeW.GetPrevSibling() == nullptr);

#endif
}TEST_END


// ---  End of File ---
