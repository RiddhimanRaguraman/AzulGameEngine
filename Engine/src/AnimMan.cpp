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
#include "Prefab_Pivot.h"
#include "ComputeBlend_OneAnim.h"
#include "ComputeBlend_TwoAnim.h"

#include <Windows.h>

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
        : ManBase(new DLinkMan(), new DLinkMan(), reserveNum, reserveGrow),
        poBlendTwoAnimController(nullptr)
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
            case AnimMan::Name::Gangnam:
                return Clip::Name::Mousey_Gangnam;
            case AnimMan::Name::Run:
                return Clip::Name::Mousey_Run;
            default:
                return Clip::Name::Not_Initialized;
            }
        }
        return Clip::Name::Not_Initialized;
    }

    static Clip::Name privMapToClipNameFromFile(const char* clipFileName, Skel::Name skelName)
    {
        assert(clipFileName);

        if (skelName == Skel::Name::Mousey)
        {
            if (std::strstr(clipFileName, "Gangnam") != nullptr)
            {
                return Clip::Name::Mousey_Gangnam;
            }

            if (std::strstr(clipFileName, "Run") != nullptr)
            {
                return Clip::Name::Mousey_Run;
            }

            if (std::strstr(clipFileName, "Silly") != nullptr || std::strstr(clipFileName, "Dance") != nullptr)
            {
                return Clip::Name::Mousey_Silly_Dance;
            }
        }

        if (skelName == Skel::Name::ChickenBot)
        {
            if (std::strstr(clipFileName, "Walk") != nullptr)
            {
                return Clip::Name::Walk_ChickenBot;
            }
            if (std::strstr(clipFileName, "Run") != nullptr)
            {
                return Clip::Name::Run_ChickenBot;
            }
            if (std::strstr(clipFileName, "HitBack") != nullptr)
            {
                return Clip::Name::HitBack_ChickenBot;
            }
            if (std::strstr(clipFileName, "Shot") != nullptr || std::strstr(clipFileName, "ShotUp") != nullptr)
            {
                return Clip::Name::ShotUp_ChickenBot;
            }
            if (std::strstr(clipFileName, "Idle") != nullptr)
            {
                return Clip::Name::Idle_ChickenBot;
            }
        }

        if (skelName == Skel::Name::DogBot)
        {
            if (std::strstr(clipFileName, "Walk") != nullptr)
            {
                return Clip::Name::Walk_DogBot;
            }
            if (std::strstr(clipFileName, "Run") != nullptr)
            {
                return Clip::Name::Run_DogBot;
            }
            if (std::strstr(clipFileName, "HitBack") != nullptr)
            {
                return Clip::Name::HitBack_DogBot;
            }
            if (std::strstr(clipFileName, "Shot") != nullptr || std::strstr(clipFileName, "ShotUp") != nullptr)
            {
                return Clip::Name::ShotUp_DogBot;
            }
            if (std::strstr(clipFileName, "Idle") != nullptr)
            {
                return Clip::Name::Idle_DogBot;
            }
        }

        if (skelName == Skel::Name::SpiderBot)
        {
            if (std::strstr(clipFileName, "walk") != nullptr || std::strstr(clipFileName, "Walk") != nullptr)
            {
                return Clip::Name::walk_Spiderbot;
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

    DLink *AnimMan::Add(Name name, const char *clipFileName, Skel::Name skelName, TextureObject::Name texName, Mesh::Name meshName, Vec3 &_pLightColor, Vec3 &_pLightPos)
    {
        AnimMan *pMan = AnimMan::privGetInstance();
        assert(pMan != nullptr);

        Clip::Name clipName = privMapToClipName(name, skelName);
        HierarchyTable::Name Hname = privMapToHierarchyName(skelName);

        //HierarchyTable* pHtable = HierarchyTableMan::Find(Hname);

        ClipMan::Add(clipName, clipFileName, skelName, Hname);

        Anim *ptAnim = new Anim(clipName);
        assert(ptAnim);
        ComputeBlend_OneAnim* pBlend = new ComputeBlend_OneAnim(ptAnim);

        AnimController *pController = new AnimController_OneAnim(ptAnim, pBlend, 1.0f);
        assert(pController);

        GraphicsObject_SkinLightTexture* pGraphicsSkin = new GraphicsObject_SkinLightTexture(meshName,
                                                                                             ShaderObject::Name::SkinLightTexture,
                                                                                             texName,
                                                                                             pBlend,
                                                                                             _pLightColor,
                                                                                             _pLightPos);
		assert(pGraphicsSkin);

		GameObjectAnimSkin *pGameSkin = new GameObjectAnimSkin(pGraphicsSkin, pBlend);
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

    DLink* AnimMan::Add(Name name, const char* clipFileName1, const char* clipFileName2, Skel::Name skelName, TextureObject::Name texName, Mesh::Name meshName, Vec3& _pLightColor, Vec3& _pLightPos)
    {
        AnimMan* pMan = AnimMan::privGetInstance();
        assert(pMan != nullptr);

        Clip::Name clipName1 = privMapToClipNameFromFile(clipFileName1, skelName);
        Clip::Name clipName2 = privMapToClipNameFromFile(clipFileName2, skelName);
        assert(clipName1 != Clip::Name::Not_Initialized);
        assert(clipName2 != Clip::Name::Not_Initialized);
        HierarchyTable::Name Hname = privMapToHierarchyName(skelName);

        //HierarchyTable* pHtable = HierarchyTableMan::Find(Hname);

        ClipMan::Add(clipName1, clipFileName1, skelName, Hname);
        ClipMan::Add(clipName2, clipFileName2, skelName, Hname);

        Anim* pAnimA = new Anim(clipName1);
        Anim* pAnimB = new Anim(clipName2);
        ComputeBlend_TwoAnim* pBlend = new ComputeBlend_TwoAnim(pAnimA, pAnimB);

        AnimController_TwoAnim *pTwoController = new AnimController_TwoAnim(pAnimA, 1.0f, pAnimB, 1.0f, pBlend);
        assert(pTwoController);
        AnimController *pController = pTwoController;

        pMan->poBlendTwoAnimController = pTwoController;

        GraphicsObject_SkinLightTexture* pGraphicsSkin = new GraphicsObject_SkinLightTexture(meshName,
            ShaderObject::Name::SkinLightTexture,
            texName,
            pBlend,
            _pLightColor,
            _pLightPos);
        assert(pGraphicsSkin);

        GameObjectAnimSkin* pGameSkin = new GameObjectAnimSkin(pGraphicsSkin, pBlend);
        assert(pGameSkin);
        pGameSkin->SetName(StringMe(name));
        GameObjectMan::Add(pGameSkin, GameObjectMan::GetRoot());

        Clip* pClip = ClipMan::Find(clipName1);
        assert(pClip);

        AnimNode* pNode = (AnimNode*)pMan->baseAddToFront();
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

    void AnimMan::Update(AnimTime tCurr)
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
                c->Update(tCurr);
            }
        }
    }

    void AnimMan::BlendAnimation(AnimTime tDelta)
    {
        static float sBlendTs = 0.0f;
        static bool sBlendOn = false;
        static float sStartTs = 0.0f;
        static float sTargetTs = 0.0f;
        static float sElapsedSec = 1.0f;

        const SHORT spaceState = GetAsyncKeyState(VK_SPACE);
        if (spaceState & 0x0001)
        {
            sBlendOn = !sBlendOn;
            sStartTs = sBlendTs;
            sTargetTs = sBlendOn ? 1.0f : 0.0f;
            sElapsedSec = 0.0f;
        }

        const float dtSec = tDelta / AnimTime(AnimTime::Duration::ONE_SECOND);
        sElapsedSec += dtSec;

        float t = sElapsedSec / 1.0f;
        if (t < 0.0f)
        {
            t = 0.0f;
        }
        else if (t > 1.0f)
        {
            t = 1.0f;
        }

        sBlendTs = sStartTs + (sTargetTs - sStartTs) * t;

        if (sBlendTs < 0.0f)
        {
            sBlendTs = 0.0f;
        }
        else if (sBlendTs > 1.0f)
        {
            sBlendTs = 1.0f;
        }

        AnimMan *pMan = AnimMan::privGetInstance();
        assert(pMan != nullptr);

        if (pMan->poBlendTwoAnimController)
        {
            pMan->poBlendTwoAnimController->SetBlendTs(sBlendTs);
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

	void AnimMan::SetPrefabPivot(Name name)
	{
		AnimMan *pMan = AnimMan::privGetInstance();
		pMan->pCompareStrategy = AnimMan::posEnumNameCompare;
		assert(pMan->pCompareStrategy);

		pMan->poNodeCompare->mName = name;
		AnimNode *pNode = (AnimNode *)pMan->baseFind(pMan->poNodeCompare);
		if (pNode && pNode->pGameSkin)
		{
			pNode->pGameSkin->SetPrefab(new Prefab_Pivot());
		}
	}

    void AnimMan::SetBlendTs(Name name, float ts)
    {
        AnimMan* pMan = AnimMan::privGetInstance();
        pMan->pCompareStrategy = AnimMan::posEnumNameCompare;
        assert(pMan->pCompareStrategy);

        pMan->poNodeCompare->mName = name;
        AnimNode* pNode = (AnimNode*)pMan->baseFind(pMan->poNodeCompare);
        if (pNode && pNode->pController)
        {
            AnimController_TwoAnim* pTwo = dynamic_cast<AnimController_TwoAnim*>(pNode->pController);
            if (pTwo)
            {
                pTwo->SetBlendTs(ts);
            }
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
