//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "PCSTree.h"
#include "PCSNode.h"

namespace Azul
{
	// constructor
	PCSNode::PCSNode()
		: pParent(nullptr),
		pChild(nullptr),
		pNextSibling(nullptr),
		pPrevSibling(nullptr),
		pForward(nullptr),
		pReverse(nullptr)
	{
		memset(this->pName, 0, PCSNode::NAME_SIZE);
	}

	// copy constructor
	PCSNode::PCSNode(const PCSNode& in)
	{
		this->pParent = in.pParent;
		this->pChild = in.pChild;
		this->pNextSibling = in.pNextSibling;
		this->pPrevSibling = in.pPrevSibling;
		this->pForward = in.pForward;
		this->pReverse = in.pReverse;

		// Copy all NAME_SIZE bytes of the name buffer
		std::memcpy(this->pName, in.pName, PCSNode::NAME_SIZE);
	}

	// Specialize Constructor
	PCSNode::PCSNode(PCSNode* const pInParent, PCSNode* const pInChild, PCSNode* const pInNextSibling, PCSNode* const pInPrevSibling, const char* const pInName)
		: pParent(pInParent),
		pChild(pInChild),
		pNextSibling(pInNextSibling),
		pPrevSibling(pInPrevSibling),
		pForward(nullptr),
		pReverse(nullptr)
	{
		SetName(pInName);
	}

	PCSNode::PCSNode(const char* const pInName)
		: pParent(nullptr), pChild(nullptr), pNextSibling(nullptr), pPrevSibling(nullptr)
	{
		SetName(pInName);
	}

	// destructor
	PCSNode::~PCSNode()
	{
		this->pParent = nullptr;
		this->pChild = nullptr;
		this->pNextSibling = nullptr;
		this->pPrevSibling = nullptr;
		this->pForward = nullptr;
		this->pReverse = nullptr;
	}

	// assignment operator
	PCSNode & PCSNode::operator = (const PCSNode & in)
	{
		if (this != &in)
		{
			this->pParent = in.pParent;
			this->pChild = in.pChild;
			this->pNextSibling = in.pNextSibling;
			this->pPrevSibling = in.pPrevSibling;
			this->pForward = in.pForward;
			this->pReverse = in.pReverse;
			strncpy_s(this->pName, PCSNode::NAME_SIZE, in.pName, PCSNode::NAME_SIZE - 1);
		}
		return *this;
	}

	void PCSNode::SetParent(PCSNode * const pIn)
	{
		this->pParent = pIn;
	}

	void PCSNode::SetChild(PCSNode * const pIn)
	{
		this->pChild = pIn;
	}

	void PCSNode::SetNextSibling(PCSNode * const pIn)
	{
		this->pNextSibling = pIn;
	}

	void PCSNode::SetPrevSibling(PCSNode * const pIn)
	{
		this->pPrevSibling = pIn; //
	}

	void PCSNode::SetForward(PCSNode* const pIn)
	{
		this->pForward = pIn;
	}

	void PCSNode::SetReverse(PCSNode* const pIn)
	{
		this->pReverse = pIn; //
	}


	PCSNode* PCSNode::GetParent(void) const
	{
		return this->pParent;
	}

	PCSNode* PCSNode::GetChild(void) const
	{
		return this->pChild;
	}

	PCSNode* PCSNode::GetNextSibling(void) const
	{
		return this->pNextSibling;
	}

	PCSNode* PCSNode::GetPrevSibling(void) const
	{
		return this->pPrevSibling;
	}

	PCSNode* PCSNode::GetForward(void) const
	{
		return this->pForward;
	}

	PCSNode* PCSNode::GetReverse(void) const
	{
		return this->pReverse;
	}

	PCSNode::Code PCSNode::SetName(const char * const pInName)
	{
		if (pInName == NULL)
		{
			return Code::FAIL_NULL_PTR;
		}

		for (int i = 0; i < NAME_SIZE - 1; i++)
		{
			this->pName[i] = pInName[i];
		}
		return Code::SUCCESS;
	}

	PCSNode::Code PCSNode::GetName(char * const pOutBuffer, unsigned int sizeOutBuffer) const
	{
		if (pOutBuffer == NULL)
		{
			return Code::FAIL_NULL_PTR;
		}

		if (sizeOutBuffer < NAME_SIZE)
		{
			for (unsigned int i = 0; i < sizeOutBuffer; i++)
			{
				if (i == sizeOutBuffer - 1)
				{
					pOutBuffer[i] = NULL;
					return Code::SUCCESS;
				}
				pOutBuffer[i] = this->pName[i]; 
			}
		}
		for (unsigned int i = 0; i < sizeOutBuffer; i++)
		{
			pOutBuffer[i] = this->pName[i];
		}
		return Code::SUCCESS;
	}

	void PCSNode::PrintNode() const
	{
		char buffer[PCSNode::NAME_SIZE];
		this->GetName(buffer, PCSNode::NAME_SIZE);
		Trace::out("Node_%s\n", buffer);
	}

	void PCSNode::PrintChildren() const
	{
		const PCSNode* pTmp = this->pChild;
		while (pTmp)
		{
			pTmp->PrintNode();
			pTmp = pTmp->pNextSibling;
		}
	}

	void PCSNode::PrintSiblings() const
	{
		// Start from the leftmost sibling
		const PCSNode* pTmp = this;

		while (pTmp->pPrevSibling != nullptr)
		{
			pTmp = pTmp->pPrevSibling;
		}

		// Print from left to right
		while (pTmp != nullptr)
		{
			pTmp->PrintNode();  // Assumes PrintNode prints the node's name
			pTmp = pTmp->pNextSibling;
		}
	}

	int PCSNode::GetNumSiblings() const
	{
		int count = 1;
		const PCSNode* pTmp = this->pNextSibling;
		while (pTmp)
		{
			count++;
			pTmp = pTmp->pNextSibling;
		}
		pTmp = this->pPrevSibling;
		while (pTmp)
		{
			count++;
			pTmp = pTmp->pPrevSibling;
		}
		return count;
	}

	int PCSNode::GetNumChildren() const
	{
		int count = 0;
		PCSNode* pTmp = this->pChild;
		while (pTmp)
		{
			count++;
			pTmp = pTmp->pNextSibling;
		}
		return count;
	}
}

// ---  End of File ---
