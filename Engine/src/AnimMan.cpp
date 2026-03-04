//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "AnimMan.h"
#include "StringThis.h"
#include "ClipMan.h"
#include "SkelMan.h"
#include "Anim.h"
#include "AnimManCompareStrategyEnumName.h"

namespace Azul
{

    AnimMan *AnimMan::posInstance = nullptr;
    CompareStrategyBase *AnimMan::posEnumNameCompare = nullptr;

    AnimMan::AnimNode::AnimNode()
        : DLink(), mName(Name::Uninitialized), pClip(nullptr), pController(nullptr)
    {
        this->privClear();
    }

    AnimMan::AnimNode::~AnimNode()
    {
        this->privClear();
    }

    void AnimMan::AnimNode::privClear()
    {
        if (pController)
        {
            delete pController;
        }
        pController = nullptr;
        pClip = nullptr;
        mName = Name::Uninitialized;
    }

    void AnimMan::AnimNode::Set(Name inName, Clip *clip, AnimController *controller)
    {
        this->privClear();
        this->mName = inName;
        this->pClip = clip;
        this->pController = controller;
    }

    AnimController *AnimMan::AnimNode::GetController()
    {
        return this->pController;
    }

    char *AnimMan::AnimNode::GetName()
    {
        static char pTmp[128];
        strcpy_s(pTmp, 128, StringMe(this->mName));
        return pTmp;
    }

    void AnimMan::AnimNode::Wash()
    {
        this->privClear();
    }

    void AnimMan::AnimNode::Dump()
    {
        Trace::out("      AnimNode(%p)\n", this);
        Trace::out("      Name: %s \n", StringMe(this->mName));
        DLink::Dump();
    }

    AnimMan::AnimMan(int reserveNum, int reserveGrow)
        : ManBase(new DLinkMan(), new DLinkMan(), reserveNum, reserveGrow)
    {
        this->proFillReservedPool(reserveNum);
        this->poNodeCompare = new AnimNode();
    }

    AnimMan::~AnimMan()
    {
        delete this->poNodeCompare;
        this->poNodeCompare = nullptr;

        Iterator *pIt = this->baseGetActiveIterator();
        DLink *pNode = pIt->First();
        while (!pIt->IsDone())
        {
            AnimNode *pDeleteMe = (AnimNode *)pIt->Curr();
            pNode = pIt->Next();
            delete pDeleteMe;
        }

        pIt = this->baseGetReserveIterator();
        pNode = pIt->First();
        while (!pIt->IsDone())
        {
            AnimNode *pDeleteMe = (AnimNode *)pIt->Curr();
            pNode = pIt->Next();
            delete pDeleteMe;
        }
    }

    void AnimMan::Create(int reserveNum, int reserveGrow)
    {
        assert(posInstance == nullptr);
        posInstance = new AnimMan(reserveNum, reserveGrow);
        assert(posInstance != nullptr);

        if (posEnumNameCompare == nullptr)
        {
            posEnumNameCompare = new AnimManCompareStrategyEnumName();
        }
    }

    void AnimMan::Destroy()
    {
        AnimMan *pMan = AnimMan::privGetInstance();
        assert(pMan != nullptr);

        delete AnimMan::posEnumNameCompare;
        AnimMan::posEnumNameCompare = nullptr;

        delete pMan;
        posInstance = nullptr; 
    }

    static Clip::Name privMapToClipName(AnimMan::Name name, Skel::Name skelName)
    {
        if (skelName == Skel::Name::ChickenBot)
        {
            switch (name)
            {
            case AnimMan::Name::Walk:
                return Clip::Name::Walk_ChickenBot;
            case AnimMan::Name::Run:
                return Clip::Name::Run_ChickenBot;
            case AnimMan::Name::HitBack:
                return Clip::Name::HitBack_ChickenBot;
            case AnimMan::Name::Shotup:
                return Clip::Name::ShotUp_ChickenBot;
            case AnimMan::Name::Idle:
                return Clip::Name::Idle_ChickenBot;
            default:
                return Clip::Name::Not_Initialized;
            }
        }
        else if (skelName == Skel::Name::DogBot)
        {
            switch (name)
            {
            case AnimMan::Name::Walk:
                return Clip::Name::Walk_DogBot;
            case AnimMan::Name::Run:
                return Clip::Name::Run_DogBot;
            case AnimMan::Name::HitBack:
                return Clip::Name::HitBack_DogBot;
            case AnimMan::Name::Shotup:
                return Clip::Name::ShotUp_DogBot;
            case AnimMan::Name::Idle:
                return Clip::Name::Idle_DogBot;
            default:
                return Clip::Name::Not_Initialized;
            }
        }
        else if (skelName == Skel::Name::SpiderBot)
        {
            switch (name)
            {
            case AnimMan::Name::Walk:
                return Clip::Name::walk_Spiderbot;
            default:
                return Clip::Name::Not_Initialized;
            }
        }
        else if (skelName == Skel::Name::Mousey)
        {
            switch (name)
            {
            case AnimMan::Name::Dance:
                return Clip::Name::Mousey_Silly_Dance;
            default:
                return Clip::Name::Not_Initialized;
            }
        }
        return Clip::Name::Not_Initialized;
    }

    DLink *AnimMan::Add(Name name, const char *clipFileName, AnimTime delta, Skel::Name skelName, TextureObject::Name texName, Mesh::Name meshName, JointTable* pJointTable)
    {
        AnimMan *pMan = AnimMan::privGetInstance();
        assert(pMan != nullptr);

        Clip::Name clipName = privMapToClipName(name, skelName);

        ClipMan::Add(clipFileName, clipName, skelName);

        Anim *pAnim = new Anim(clipName, texName, meshName, pJointTable);
        assert(pAnim);

        AnimController *pController = new AnimController(pAnim, delta);
        assert(pController);

        Clip *pClip = ClipMan::Find(clipName);
        assert(pClip);

        AnimNode *pNode = (AnimNode *)pMan->baseAddToFront();
        assert(pNode);
        pNode->Set(name, pClip, pController);
        return pNode;
    }

    AnimController *AnimMan::Find(Name name)
    {
        AnimMan *pMan = AnimMan::privGetInstance();
        assert(pMan != nullptr);

        pMan->pCompareStrategy = AnimMan::posEnumNameCompare;
        assert(pMan->pCompareStrategy);

        pMan->poNodeCompare->mName = name;
        AnimNode *pData = (AnimNode *)pMan->baseFind(pMan->poNodeCompare);
        return pData ? pData->GetController() : nullptr;
    }

    void AnimMan::Update()
    {
        AnimMan *pMan = AnimMan::privGetInstance();
        assert(pMan != nullptr);

        Iterator *pIt = pMan->baseGetActiveIterator();
        for (DLink *p = pIt->First(); !pIt->IsDone(); p = pIt->Next())
        {
            AnimNode *n = (AnimNode *)p;
            AnimController *c = n->GetController();
            if (c)
            {
                c->Update();
            }
        }
    }

    void AnimMan::Remove(DLink *pNode)
    {
        assert(pNode != nullptr);
        AnimMan *pMan = AnimMan::privGetInstance();
        assert(pMan != nullptr);
        pMan->baseRemove(pNode);
    }

    void AnimMan::Dump()
    {
        AnimMan *pMan = AnimMan::privGetInstance();
        assert(pMan != nullptr);
        pMan->baseDump();
    }

    void AnimMan::SetScale(Name name, float sx, float sy, float sz)
    {
        AnimController *pCtrl = AnimMan::Find(name);
        if (pCtrl)
        {
            pCtrl->SetPivotScale(sx, sy, sz);
        }
    }

    void AnimMan::SetUniformScale(Name name, float s)
    {
        AnimController* pCtrl = AnimMan::Find(name);
        if (pCtrl)
        {
            pCtrl->SetPivotUniformScale(s);
        }
    }

    void AnimMan::SetPos(Name name, float x, float y, float z)
    {
        AnimController *pCtrl = AnimMan::Find(name);
        if (pCtrl)
        {
            pCtrl->SetPivotTrans(x, y, z);
        }
    }

    void AnimMan::SetDelta(Name name, float scale)
    {
        AnimController *pCtrl = AnimMan::Find(name);
        if (pCtrl)
        {
            pCtrl->SetDelta(scale);
        }
    }

    void AnimMan::SetPivotRotX(Name name, float angle)
    {
        AnimController* pCtrl = AnimMan::Find(name);
        if (pCtrl)
            pCtrl->SetPivotRotX(angle);
    }

    void AnimMan::SetPivotRotY(Name name, float angle)
    {
        AnimController* pCtrl = AnimMan::Find(name);
        if (pCtrl)
            pCtrl->SetPivotRotY(angle);
    }

    void AnimMan::SetPivotRotZ(Name name, float angle)
    {
        AnimController* pCtrl = AnimMan::Find(name);
        if (pCtrl)
            pCtrl->SetPivotRotZ(angle);
    }

    void AnimMan::SetPivotTotalRot(Name name, const Rot3 mode, float x, float y, float z)
    {
        AnimController* pCtrl = AnimMan::Find(name);
        if (pCtrl)
            pCtrl->SetPivotTotalRot(mode, x, y, z);
    }

    Clip *AnimMan::GetClip(Name name)
    {
        AnimMan *pMan = AnimMan::privGetInstance();
        assert(pMan != nullptr);

        AnimNode *pNode = pMan->poNodeCompare;
        pNode->mName = name;
        DLink *pLink = pMan->baseFind(pNode);
        if (pLink)
        {
            AnimNode *pAnimNode = (AnimNode *)pLink;
            return pAnimNode->pClip;
        }
        return nullptr;
    }

    AnimMan *AnimMan::privGetInstance()
    {
        assert(posInstance != nullptr);
        return posInstance;
    }

    DLink *AnimMan::derivedCreateNode()
    {
        DLink *pNodeBase = new AnimNode();
        assert(pNodeBase != nullptr);
        return pNodeBase;
    }
}

// --- End of File ---
