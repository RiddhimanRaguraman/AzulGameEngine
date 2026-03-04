//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "PCSTree.h"
#include "PCSNode.h"

namespace Azul
{

	// constructor
	PCSTree::PCSTree()
	{
		mInfo.maxNumNodes = 0;
		mInfo.currNumNodes = 0;
		mInfo.currNumLevels = 0;
		mInfo.maxNumLevels = 0;
		pRoot = NULL;
	};

	// destructor
	PCSTree::~PCSTree()
	{
		
	};

	// get Root
	PCSNode * PCSTree::GetRoot() const
	{
		return this->pRoot;
	}

	// insert
	void PCSTree::Insert(PCSNode* pInNode, PCSNode* pParent)
	{
		// assert(pInNode)
		assert(pInNode != nullptr);

		// ------------------------------------------------------------
		// Case 1: inserting root (pParent == nullptr)
		// ------------------------------------------------------------
		if (pParent == nullptr)
		{
			this->pRoot = pInNode;

			pInNode->SetChild(nullptr);
			pInNode->SetParent(nullptr);
			pInNode->SetNextSibling(nullptr);
			pInNode->SetPrevSibling(nullptr);
			pInNode->SetForward(nullptr);
			pInNode->SetReverse(nullptr);

			this->privInfoAddNode();
			this->privInfoAddNewLevel(pInNode);
			return;
		}

		// ------------------------------------------------------------
		// Case 2: parent has no child yet -> pInNode becomes first child
		// ------------------------------------------------------------
		if (pParent->GetChild() == nullptr)
		{
			// link as parent's child
			pParent->SetChild(pInNode);
			pInNode->SetParent(pParent);

			// new node is a leaf at insertion time
			pInNode->SetChild(nullptr);
			pInNode->SetNextSibling(nullptr);
			pInNode->SetPrevSibling(nullptr);

			// maintain forward/reverse chain
			PCSNode* pFwd = pParent->GetForward();

			pParent->SetForward(pInNode);
			pInNode->SetReverse(pParent);

			if (pFwd != nullptr)
			{
				pInNode->SetForward(pFwd);
				pFwd->SetReverse(pInNode);
			}
			else
			{
				// parent had no forward; hook root's reverse to this node
				this->pRoot->SetReverse(pInNode);
			}

			// if parent is root, set root forward and ensure reverse points back
			if (pParent == this->pRoot)
			{
				this->pRoot->SetForward(pInNode);
				pInNode->SetReverse(this->pRoot);
			}

			this->privInfoAddNode();
			this->privInfoAddNewLevel(pInNode);
			return;
		}

		// ------------------------------------------------------------
		// Case 3: parent already has a child -> insert as new first child
		// ------------------------------------------------------------
		PCSNode* pOldChild = pParent->GetChild();

		// old child becomes next sibling; old child's prev sibling becomes new node
		pOldChild->SetPrevSibling(pInNode);

		// parent/child/sibling wiring
		pInNode->SetParent(pParent);
		pInNode->SetChild(nullptr);
		pInNode->SetNextSibling(pOldChild);
		pInNode->SetPrevSibling(nullptr);

		// parent now points to new first child
		pParent->SetChild(pInNode);

		// forward/reverse chain updates
		pInNode->SetForward(pParent->GetForward());
		pParent->SetForward(pInNode);

		pOldChild->SetReverse(pInNode);
		pInNode->SetReverse(pParent);

		// NOTE: disassembly only calls privInfoAddNode() in this branch (no AddNewLevel)
		this->privInfoAddNode();
	}

	// Remove - recursively removes node and all its children
	void PCSTree::Remove(PCSNode* const pInNode)
	{
		// assert(pInNode)
		assert(pInNode != nullptr);

		// ------------------------------------------------------------
		// If node has a child, remove child first (recursive), then self
		// ------------------------------------------------------------
		if (pInNode->GetChild() != nullptr)
		{
			PCSNode* tmp = pInNode->GetChild();
			assert(tmp != nullptr);

			this->Remove(tmp);
			this->Remove(pInNode);
			return;
		}

		// ------------------------------------------------------------
		// Leaf node cases
		// ------------------------------------------------------------
		if (pInNode->GetNextSibling() == nullptr)
		{
			PCSNode* parent = pInNode->GetParent();

			// If no parent, this is root leaf
			if (parent == nullptr)
			{
				this->pRoot = nullptr;

				// jump to common cleanup (no privInfoRemoveNewLevel in this path)
				this->privRemoveNode(pInNode);
				return;
			}

			// binary redundantly asserts parent != nullptr
			assert(parent != nullptr);

			PCSNode* tmp = parent->GetChild();
			assert(tmp != nullptr);

			// If parent's child has no next sibling -> only child, clear parent's child
			if (tmp->GetNextSibling() == nullptr)
			{
				parent->SetChild(nullptr);
			}
			else
			{
				// Otherwise detach by setting prevSibling->nextSibling = nullptr
				PCSNode* prev = pInNode->GetPrevSibling();
				prev->SetNextSibling(nullptr);
			}

			// Forward/Reverse fix-up (first time, matches disassembly ordering)
			PCSNode* fwd = pInNode->GetForward();
			PCSNode* rev = pInNode->GetReverse();

			if (rev != nullptr)
			{
				rev->SetForward(fwd);
			}
			if (fwd != nullptr)
			{
				fwd->SetReverse(rev);
			}

			// If root's reverse points at the node being removed, update it
			if (this->pRoot != nullptr)
			{
				if (this->pRoot->GetReverse() == pInNode)
				{
					this->pRoot->SetReverse(rev);
				}
			}

			// remove/adjust level info (binary does this only in this path)
			this->privInfoRemoveNewLevel();

			// then fall into common cleanup
			this->privRemoveNode(pInNode);
			return;
		}
		else
		{
			// Has a next sibling
			PCSNode* parent = pInNode->GetParent();
			assert(parent != nullptr);

			PCSNode* tmp = parent->GetChild();
			assert(tmp != nullptr);

			// If removing the first child of parent
			if (tmp == pInNode)
			{
				parent->SetChild(pInNode->GetNextSibling());

				PCSNode* ns = pInNode->GetNextSibling();
				if (ns != nullptr)
				{
					ns->SetPrevSibling(nullptr);
				}

				this->privRemoveNode(pInNode);
				return;
			}
			else
			{
				PCSNode* prev = pInNode->GetPrevSibling();
				prev->SetNextSibling(pInNode->GetNextSibling());

				PCSNode* ns = pInNode->GetNextSibling();
				if (ns != nullptr)
				{
					ns->SetPrevSibling(prev);
				}

				this->privRemoveNode(pInNode);
				return;
			}
		}
	}

	void PCSTree::privRemoveNode(PCSNode* const pInNode)
	{
		// Clear structural tree links (binary does NOT clear forward/reverse here)
		pInNode->SetChild(nullptr);
		pInNode->SetParent(nullptr);
		pInNode->SetNextSibling(nullptr);
		pInNode->SetPrevSibling(nullptr);

		// Forward/Reverse fix-up (second time, idempotent, matches disassembly)
		PCSNode* fwd2 = pInNode->GetForward();
		PCSNode* rev2 = pInNode->GetReverse();

		if (rev2 != nullptr)
		{
			rev2->SetForward(fwd2);
		}
		if (fwd2 != nullptr)
		{
			fwd2->SetReverse(rev2);
		}

		// Update node count info
		this->privInfoRemoveNode();
	}

	void PCSTree::GetInfo(Info& info)
	{
		this->privInfoRemoveNewLevel();

		info.currNumNodes = this->mInfo.currNumNodes;
		info.maxNumNodes = this->mInfo.maxNumNodes;
		info.currNumLevels = this->mInfo.currNumLevels;
		info.maxNumLevels = this->mInfo.maxNumLevels;
	}


	void PCSTree::Print() const
	{
		Trace::out("\n");
		Trace::out("printTree () ------------------------------\n");

		if (this->pRoot != nullptr)
		{
			this->privDumpTreeDepthFirst(this->pRoot);
		}
	}

	void PCSTree::privDepthFirst(PCSNode* tmp)
	{
		char buffer[PCSNode::NAME_SIZE];
		tmp->GetName(buffer, PCSNode::NAME_SIZE);
		printf("->>> depthFirst( %s )\n", buffer);

		PCSNode* child = tmp->GetChild();
		while (child != nullptr)
		{
			// recurse
			this->privDepthFirst(child);

			// advance across siblings
			child = child->GetNextSibling();
		}
	}


	void PCSTree::privDumpTreeDepthFirst(PCSNode* node) const
	{
		// Note: disassembly calls PrintNode() unconditionally (no null-check).
		node->PrintNode();

		PCSNode* child = node->GetChild();
		while (child != nullptr)
		{
			this->privDumpTreeDepthFirst(child);
			child = child->GetNextSibling();
		}
	}

	void PCSTree::privRecalculateLevel(PCSNode* node)
	{
		if (node == nullptr)
		{
			return;
		}

		// compute this node's level relative to the root (root == level 1)
		PCSNode* pWalk = node;
		int level = 1;

		while (pWalk != this->pRoot)
		{
			pWalk = pWalk->GetParent();
			level++;
		}

		// update current max levels if this node is deeper
		if (level > this->mInfo.currNumLevels)
		{
			this->mInfo.currNumLevels = level;
		}

		// recurse across children and siblings
		PCSNode* child = node->GetChild();
		while (child != nullptr)
		{
			this->privRecalculateLevel(child);
			child = child->GetNextSibling();
		}
	}

	void PCSTree::privInfoAddNewLevel(PCSNode* const inNode)
	{
		PCSNode* pWalk = inNode;
		int level = 1;

		// walk up until we hit the root, counting levels (root == level 1)
		while (pWalk != this->pRoot)
		{
			pWalk = pWalk->GetParent();
			level++;
		}

		// update current levels if this insertion increased depth
		if (level > this->mInfo.currNumLevels)
		{
			this->mInfo.currNumLevels = level;
		}

		// update max levels if current exceeds max
		if (this->mInfo.currNumLevels > this->mInfo.maxNumLevels)
		{
			this->mInfo.maxNumLevels = this->mInfo.currNumLevels;
		}
	}

	void PCSTree::privInfoRemoveNewLevel()
	{
		// reset and recompute from the root
		this->mInfo.currNumLevels = 0;
		this->privRecalculateLevel(this->pRoot);
	}

	void PCSTree::privInfoAddNode()
	{
		this->mInfo.currNumNodes++;

		if (this->mInfo.currNumNodes > this->mInfo.maxNumNodes)
		{
			this->mInfo.maxNumNodes++;
		}
	}

	void PCSTree::privInfoRemoveNode()
	{
		this->mInfo.currNumNodes--;
	}
}

// ---  End of File ---
