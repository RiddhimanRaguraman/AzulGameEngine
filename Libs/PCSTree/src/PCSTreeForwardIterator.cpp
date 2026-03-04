//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "PCSTreeForwardIterator.h" 

namespace Azul
{
	PCSTreeForwardIterator::PCSTreeForwardIterator(PCSNode *rootNode)
	{
		this->root = rootNode;
		this->current = rootNode;
	}

	PCSNode *PCSTreeForwardIterator::First()
	{
		this->current = this->root;
		return this->current;
	}


	PCSNode* PCSTreeForwardIterator::Next()
	{
		this->current = this->current->GetForward();
		return this->current;
	}

	//PCSNode* PCSTreeForwardIterator::Next()
	//{
	//	//assert(this->current == nullptr);
	//	PCSNode* tmp = this->current;
	//	PCSNode* back = tmp;

	//	PCSNode* child = tmp->GetChild();
	//	PCSNode* sibling = tmp->GetNextSibling();
	//	PCSNode* parent = tmp->GetParent();

	//	//has a child
	//	if (child != nullptr)
	//	{
	//		tmp = child;
	//	}
	//	//no children
	//	else
	//	{
	//		//has a sibling
	//		if (sibling != nullptr)
	//		{
	//			tmp = sibling;
	//		}

	//		//no siblings
	//		else
	//		{
	//			//find parent sibling
	//			if (parent != nullptr)
	//			{
	//				tmp = parent->GetNextSibling();

	//				//has siblings
	//				if (tmp != nullptr)
	//				{
	//					//found a sibling
	//					
	//				}

	//				//parent has no siblings, go to next parent
	//				else
	//				{
	//					parent = parent->GetParent();
	//				}
	//			}
	//			else
	//			{
	//				tmp = nullptr;
	//			}
	//		}
	//	}

	//	this->current = tmp;

	//	if (tmp == (PCSNode*)0xdddddddddddddddd)
	//	{
	//		this->current = nullptr;
	//		return this->current;
	//	}

	//	if (this->current != nullptr)
	//	{
	//		this->current->SetReverse(back);
	//	}

	//	return  this->current;
	//}

	bool PCSTreeForwardIterator::IsDone()
	{
		if (this->current == nullptr)
			bDone = true;
		return bDone;
	}

	PCSNode *PCSTreeForwardIterator::Current()
	{
		return  this->current;
	}

}

// ---  End of File ---
