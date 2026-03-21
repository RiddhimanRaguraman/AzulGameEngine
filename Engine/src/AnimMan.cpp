//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "AnimMan.h"
#include "StringThis.h"
#include "ClipMan.h"
#include "SkelMan.h"
#include "Anim.h"
#include "AnimManCompareStrategyEnumName.h"
#include "GameObjectMan.h"
#include "GraphicsObject_SkinLightTexture.h"
#include "GameObjectAnimSkin.h"
#include "Skeleton.h"
#include "ShaderObject.h"
#include "MathEngine.h"
#include "HierarchyTableMan.h"

namespace Azul
{

    AnimMan *AnimMan::posInstance = nullptr;
    CompareStrategyBase *AnimMan::posEnumNameCompare = nullptr;

    AnimMan::AnimNode::AnimNode()
        : DLink(), mName(Name::Uninitialized), pClip(nullptr), pController(nullptr), pGameSkin(nullptr)
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
		pGameSkin = nullptr;
        mName = Name::Uninitialized;
    }

    void AnimMan::AnimNode::Set(Name inName, Clip *clip, AnimController *controller, GameObjectAnimSkin *gameSkin)
    {
        this->privClear();
        this->mName = inName;
        this->pClip = clip;
        this->pController = controller;
		this->pGameSkin = gameSkin;
    }

    AnimController *AnimMan::AnimNode::GetController()
    {
        return this->pController;
    }

	GameObjectAnimSkin *AnimMan::AnimNode::GetGameSkin()
	{
		return this->pGameSkin;
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

    HierarchyTable::Name AnimMan::privMapToHierarchyName( Skel::Name skelName)
    {
        switch (skelName)
        {
        case Skel::Name::ChickenBot:
            return HierarchyTable::Name::ChickenBot;
        case Skel::Name::DogBot:
            return HierarchyTable::Name::DogBot;
        case Skel::Name::SpiderBot:
            return HierarchyTable::Name::SpiderBot;
        case Skel::Name::Mousey:
            return HierarchyTable::Name::Mousey;
        default:
            return HierarchyTable::Name::Not_Initialized;


        }
    }

    DLink *AnimMan::Add(Name name, const char *clipFileName, AnimTime delta, Skel::Name skelName, TextureObject::Name texName, Mesh::Name meshName, Vec3 &_pLightColor, Vec3 &_pLightPos)
    {
        AnimMan *pMan = AnimMan::privGetInstance();
        assert(pMan != nullptr);

        Clip::Name clipName = privMapToClipName(name, skelName);

        ClipMan::Add(clipName, skelName, clipFileName  );

		Skeleton *ptSkeleton = new Skeleton(clipName);
		assert(ptSkeleton);

        Mixer *ptMixer = new Mixer(ptSkeleton->GetClip());
        assert(ptMixer);

        HierarchyTable::Name Hname = privMapToHierarchyName(skelName);
        HierarchyTable *pHierarchyTable = HierarchyTableMan::Find(Hname);

        WorldCompute *ptWorldCompute = new WorldCompute(ptMixer, pHierarchyTable);
        assert(ptWorldCompute);

        Anim *ptAnim = new Anim(ptSkeleton, ptMixer);
        assert(ptAnim);

        AnimController *pController = new AnimController(ptAnim, delta);
        assert(pController);

		GraphicsObject_SkinLightTexture *pGraphicsSkin = new GraphicsObject_SkinLightTexture(meshName,
			                                                                                 ShaderObject::Name::SkinLightTexture,
			                                                                                 texName,
                                                                                             ptMixer,
                                                                                             ptWorldCompute,
                                                                                             _pLightColor,
                                                                                             _pLightPos);
		assert(pGraphicsSkin);

		GameObjectAnimSkin *pGameSkin = new GameObjectAnimSkin(pGraphicsSkin, ptMixer, ptWorldCompute);
		assert(pGameSkin);
		pGameSkin->SetName(StringMe(name));
		GameObjectMan::Add(pGameSkin, GameObjectMan::GetRoot());

        Clip *pClip = ClipMan::Find(clipName);
        assert(pClip);

        AnimNode *pNode = (AnimNode *)pMan->baseAddToFront();
        assert(pNode);
        pNode->Set(name, pClip, pController, pGameSkin);
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
		AnimMan *pMan = AnimMan::privGetInstance();
		pMan->pCompareStrategy = AnimMan::posEnumNameCompare;
		assert(pMan->pCompareStrategy);

		pMan->poNodeCompare->mName = name;
		AnimNode *pNode = (AnimNode *)pMan->baseFind(pMan->poNodeCompare);
		if (pNode && pNode->pGameSkin)
		{
			pNode->pGameSkin->SetScale(sx, sy, sz);
		}
    }

    void AnimMan::SetUniformScale(Name name, float s)
    {
		AnimMan *pMan = AnimMan::privGetInstance();
		pMan->pCompareStrategy = AnimMan::posEnumNameCompare;
		assert(pMan->pCompareStrategy);

		pMan->poNodeCompare->mName = name;
		AnimNode *pNode = (AnimNode *)pMan->baseFind(pMan->poNodeCompare);
		if (pNode && pNode->pGameSkin)
		{
			pNode->pGameSkin->SetScale(s, s, s);
		}
    }

    void AnimMan::SetPos(Name name, float x, float y, float z)
    {
		AnimMan *pMan = AnimMan::privGetInstance();
		pMan->pCompareStrategy = AnimMan::posEnumNameCompare;
		assert(pMan->pCompareStrategy);

		pMan->poNodeCompare->mName = name;
		AnimNode *pNode = (AnimNode *)pMan->baseFind(pMan->poNodeCompare);
		if (pNode && pNode->pGameSkin)
		{
			pNode->pGameSkin->SetTrans(x, y, z);
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
		AnimMan *pMan = AnimMan::privGetInstance();
		pMan->pCompareStrategy = AnimMan::posEnumNameCompare;
		assert(pMan->pCompareStrategy);

		pMan->poNodeCompare->mName = name;
		AnimNode *pNode = (AnimNode *)pMan->baseFind(pMan->poNodeCompare);
		if (pNode && pNode->pGameSkin)
		{
			pNode->pGameSkin->cur_rot_x = angle;
		}
    }

    void AnimMan::SetPivotRotY(Name name, float angle)
    {
		AnimMan *pMan = AnimMan::privGetInstance();
		pMan->pCompareStrategy = AnimMan::posEnumNameCompare;
		assert(pMan->pCompareStrategy);

		pMan->poNodeCompare->mName = name;
		AnimNode *pNode = (AnimNode *)pMan->baseFind(pMan->poNodeCompare);
		if (pNode && pNode->pGameSkin)
		{
			pNode->pGameSkin->cur_rot_y = angle;
		}
    }

    void AnimMan::SetPivotRotZ(Name name, float angle)
    {
		AnimMan *pMan = AnimMan::privGetInstance();
		pMan->pCompareStrategy = AnimMan::posEnumNameCompare;
		assert(pMan->pCompareStrategy);

		pMan->poNodeCompare->mName = name;
		AnimNode *pNode = (AnimNode *)pMan->baseFind(pMan->poNodeCompare);
		if (pNode && pNode->pGameSkin)
		{
			pNode->pGameSkin->cur_rot_z = angle;
		}
    }

    void AnimMan::SetPivotTotalRot(Name name, const Rot3 mode, float x, float y, float z)
    {
		AnimMan *pMan = AnimMan::privGetInstance();
		pMan->pCompareStrategy = AnimMan::posEnumNameCompare;
		assert(pMan->pCompareStrategy);

		pMan->poNodeCompare->mName = name;
		AnimNode *pNode = (AnimNode *)pMan->baseFind(pMan->poNodeCompare);
		if (pNode && pNode->pGameSkin)
		{
			Quat q(mode, x, y, z);
			pNode->pGameSkin->SetQuat(q);
			pNode->pGameSkin->cur_rot_x = 0.0f;
			pNode->pGameSkin->cur_rot_y = 0.0f;
			pNode->pGameSkin->cur_rot_z = 0.0f;
		}
    }

    Clip *AnimMan::GetClip(Name name)
    {
        AnimMan *pMan = AnimMan::privGetInstance();
        assert(pMan != nullptr);

		pMan->pCompareStrategy = AnimMan::posEnumNameCompare;
		assert(pMan->pCompareStrategy);

		pMan->poNodeCompare->mName = name;
		AnimNode *pNode = (AnimNode *)pMan->baseFind(pMan->poNodeCompare);
		return pNode ? pNode->pClip : nullptr;
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
