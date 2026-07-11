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
#include "GameObjectAnimSkin.h"
#include "Skeleton.h"
#include "ShaderObject.h"
#include "MeshNodeMan.h"
#include "ShaderObjectNodeMan.h"
#include "TexNodeMan.h"
#include "RenderComponent.h"
#include "MathEngine.h"
#include "HierarchyTableMan.h"
#include "Prefab_Pivot.h"
#include "ComputeBlend_OneAnim.h"
#include "ComputeBlend_TwoAnim.h"
#include "TimerController.h"
#include "PCSNode.h"
#include "WorldMan.h"
#include "World.h"
#include "AnimClipComponent.h"
#include "AnimBlendComponent.h"


namespace Azul
{

    AnimMan *AnimMan::posInstance = nullptr;
    CompareStrategyBase *AnimMan::posEnumNameCompare = nullptr;

    AnimMan::AnimNode::AnimNode()
        : DLink(), mName(Name::Uninitialized), pClip(nullptr),
          pAnimA(nullptr), pAnimB(nullptr), pTimerA(nullptr), pTimerB(nullptr),
          pComputeBlend(nullptr), pGameSkins{ nullptr }, numGameSkins(0)
    {
        this->privClear();
    }

    AnimMan::AnimNode::~AnimNode()
    {
        this->privClear();
    }

    void AnimMan::AnimNode::privClear()
    {
        // Own the animation resources the deleted AnimController_* used to own.
        // (ComputeBlend has a virtual dtor, so the base pointer deletes the
        // concrete _OneAnim/_TwoAnim correctly.)
        delete this->pAnimA;
        delete this->pAnimB;
        delete this->pTimerA;
        delete this->pTimerB;
        delete this->pComputeBlend;
        this->pAnimA = nullptr;
        this->pAnimB = nullptr;
        this->pTimerA = nullptr;
        this->pTimerB = nullptr;
        this->pComputeBlend = nullptr;

        pClip = nullptr;
		for (unsigned int i = 0; i < AnimNode::MAX_GAME_SKINS; i++)
		{
			this->pGameSkins[i] = nullptr;
		}
		this->numGameSkins = 0;
        mName = Name::Uninitialized;
    }

    void AnimMan::AnimNode::Set(Name inName, Clip *clip, GameObjectAnimSkin *gameSkin)
    {
        this->privClear();
        this->mName = inName;
        this->pClip = clip;
		if (gameSkin != nullptr)
		{
			this->pGameSkins[0] = gameSkin;
			this->numGameSkins = 1;
		}
    }

	void AnimMan::AnimNode::Set(Name inName, Clip *clip, GameObjectAnimSkin *const *pSkins, unsigned int numSkins)
	{
		this->privClear();
		this->mName = inName;
		this->pClip = clip;

		assert(pSkins != nullptr);
		assert(numSkins <= AnimNode::MAX_GAME_SKINS);

		unsigned int outCount = 0;
		for (unsigned int i = 0; i < numSkins; i++)
		{
			if (pSkins[i] != nullptr)
			{
				this->pGameSkins[outCount] = pSkins[i];
				outCount++;
			}
		}
		this->numGameSkins = outCount;
	}

	unsigned int AnimMan::AnimNode::GetNumGameSkins() const
	{
		return this->numGameSkins;
	}

	GameObjectAnimSkin *AnimMan::AnimNode::GetGameSkin(unsigned int index) const
	{
		assert(index < this->numGameSkins);
		return this->pGameSkins[index];
	}

    char *AnimMan::NameToString(AnimMan::Name status)
    {
        static char buffer[64];
        switch (status)
        {
        case AnimMan::Name::Gangnam:       strcpy_s(buffer, 64, "Gangnam");       break;
        case AnimMan::Name::Walk:          strcpy_s(buffer, 64, "Walk");          break;
        case AnimMan::Name::Run:           strcpy_s(buffer, 64, "Run");           break;
        case AnimMan::Name::HitBack:       strcpy_s(buffer, 64, "HitBack");       break;
        case AnimMan::Name::Shotup:        strcpy_s(buffer, 64, "Shotup");        break;
        case AnimMan::Name::Dance:         strcpy_s(buffer, 64, "Dance");         break;
        case AnimMan::Name::Rumba:         strcpy_s(buffer, 64, "Rumba");         break;
        case AnimMan::Name::Shuffle:       strcpy_s(buffer, 64, "Shuffle");       break;
        case AnimMan::Name::Swing:         strcpy_s(buffer, 64, "Swing");         break;
        case AnimMan::Name::Salsa:         strcpy_s(buffer, 64, "Salsa");         break;
        case AnimMan::Name::Blend:         strcpy_s(buffer, 64, "Blend");         break;
        case AnimMan::Name::Breakdance:    strcpy_s(buffer, 64, "Breakdance");    break;
        case AnimMan::Name::Wave:          strcpy_s(buffer, 64, "Wave");          break;
        case AnimMan::Name::Idle:          strcpy_s(buffer, 64, "Idle");          break;
        case AnimMan::Name::Uninitialized: strcpy_s(buffer, 64, "Uninitialized"); break;
        default:
            assert(false);
        }
        return buffer;
    }

    char *AnimMan::AnimNode::GetName()
    {
        static char pTmp[128];
        strcpy_s(pTmp, 128, AnimMan::NameToString(this->mName));
        return pTmp;
    }

    void AnimMan::AnimNode::Wash()
    {
        this->privClear();
    }

    void AnimMan::AnimNode::Dump()
    {
        Trace::out("      AnimNode(%p)\n", this);
        Trace::out("      Name: %s \n", AnimMan::NameToString(this->mName));
        DLink::Dump();
    }

    AnimMan::AnimMan(int reserveNum, int reserveGrow)
        : ManBase(new DLinkMan(), new DLinkMan(), reserveNum, reserveGrow),
        poBlendComputeBlend(nullptr),
		mBlendTs(0.0f)
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
        else if (skelName == Skel::Name::Halo)
        {
            switch (name)
            {
            case AnimMan::Name::Shuffle:
                return Clip::Name::Halo_Shuffle;
            default:
                return Clip::Name::Not_Initialized;
            }
        }
        else if (skelName == Skel::Name::Crownboi)
        {
            switch (name)
            {
            case AnimMan::Name::Rumba:
                return Clip::Name::Crownboi_Rumba;
            default:
                return Clip::Name::Not_Initialized;
            }
        }
        else if (skelName == Skel::Name::Drax)
        {
            switch (name)
            {
            case AnimMan::Name::Swing:
                return Clip::Name::Drax_Swing;
            default:
                return Clip::Name::Not_Initialized;
            }
        }
        else if (skelName == Skel::Name::Maw)
        {
            switch (name)
            {
            case AnimMan::Name::Breakdance:
                return Clip::Name::Maw_Breakdance;
            default:
                return Clip::Name::Not_Initialized;
            }
        }
        else if (skelName == Skel::Name::Pirate)
        {
            switch (name)
            {
            case AnimMan::Name::Salsa:
                return Clip::Name::Pirate_Salsa;
            default:
                return Clip::Name::Not_Initialized;
            }
        }
        else if (skelName == Skel::Name::Ward)
        {
            switch (name)
            {
            case AnimMan::Name::Wave:
                return Clip::Name::Ward_Wave;
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
        case Skel::Name::Halo:
            return HierarchyTable::Name::Halo;
        case Skel::Name::Crownboi:
            return HierarchyTable::Name::Crownboi;
        case Skel::Name::Drax:
            return HierarchyTable::Name::Drax;
        case Skel::Name::Maw:
            return HierarchyTable::Name::Maw;
        case Skel::Name::Pirate:
            return HierarchyTable::Name::Pirate;
        case Skel::Name::Ward:
            return HierarchyTable::Name::Ward;
        default:
            return HierarchyTable::Name::Not_Initialized;


        }
    }

    void AnimMan::privFillSkinRender(GameObjectAnimSkin *pSkin, Mesh::Name mesh,
        TextureObject::Name tex, ComputeBlend *pBlend, Vec3 &lightColor, Vec3 &lightPos)
    {
        assert(pSkin);
        RenderComponent &rc = pSkin->GetRender();
        rc.pMesh = MeshNodeMan::Find(mesh);
        rc.pShader = ShaderObjectNodeMan::Find(ShaderObject::Name::SkinLightTexture);
        rc.pTex = TexNodeMan::Find(tex);
        rc.lightColor = lightColor;
        rc.lightPos = lightPos;
        rc.pComputeBlend = pBlend;
        assert(rc.pMesh);
        assert(rc.pShader);
        assert(rc.pTex);
    }

    DLink *AnimMan::Add(Name name, const char *clipFileName, Skel::Name skelName, TextureObject::Name texName, Mesh::Name meshName, Vec3 &_pLightColor, Vec3 &_pLightPos)
    {
        AnimMan *pMan = AnimMan::privGetInstance();
        assert(pMan != nullptr);

        Clip::Name clipName = privMapToClipName(name, skelName);
        HierarchyTable::Name Hname = privMapToHierarchyName(skelName);

        ClipMan::Add(clipName, clipFileName, skelName, Hname);

        Anim *ptAnim = new Anim(clipName);
        assert(ptAnim);
        ComputeBlend_OneAnim* pBlend = new ComputeBlend_OneAnim(ptAnim);

        // Timer over the clip's play range (was created inside AnimController_OneAnim).
        TimerController* pTimer = new TimerController(AnimTime(AnimTime::Duration::ZERO), ptAnim->FindMaxTime());
        assert(pTimer);

        // Phase 3: the single-clip anim is sampled by the AnimationSystem. Its
        // data lives on a dedicated entity via a non-owning AnimClipComponent;
        // the AnimNode (below) owns ptAnim/pTimer/pBlend and frees them.
        {
            World& w = WorldMan::GetWorld();
            Entity animEnt = w.Create();
            AnimClipComponent& ac = w.Add<AnimClipComponent>(animEnt);
            ac.pAnim = ptAnim;
            ac.pTimer = pTimer;
            ac.pBlend = pBlend;
            ac.ratio = 1.0f;
        }

		// Data-path SkinLightTexture: no GraphicsObject; fill the RenderComponent.
		GameObjectAnimSkin *pGameSkin = new GameObjectAnimSkin(MaterialKind::SkinLightTexture, pBlend);
		assert(pGameSkin);
		privFillSkinRender(pGameSkin, meshName, texName, pBlend, _pLightColor, _pLightPos);
		pGameSkin->SetName(AnimMan::NameToString(name));
		GameObjectMan::Add(pGameSkin, GameObjectMan::GetRoot());

        Clip *pClip = ClipMan::Find(clipName);
        assert(pClip);

        AnimNode *pNode = (AnimNode *)pMan->baseAddToFront();
        assert(pNode);
        pNode->Set(name, pClip, pGameSkin);
        pNode->pAnimA = ptAnim;
        pNode->pTimerA = pTimer;
        pNode->pComputeBlend = pBlend;
        return pNode;
    }

	DLink *AnimMan::Add(Name name,
						const char *clipFileName,
						Skel::Name skelName,
						TextureObject::Name texName,
						const Mesh::Name *pMeshNames,
						unsigned int numMeshes,
						Vec3 &_pLightColor,
						Vec3 &_pLightPos)
	{
		assert(pMeshNames != nullptr);
		assert(numMeshes > 0);
		assert(numMeshes <= AnimNode::MAX_GAME_SKINS);

		AnimMan *pMan = AnimMan::privGetInstance();
		assert(pMan != nullptr);

		Clip::Name clipName = privMapToClipName(name, skelName);
		HierarchyTable::Name Hname = privMapToHierarchyName(skelName);

		ClipMan::Add(clipName, clipFileName, skelName, Hname);

		Anim *ptAnim = new Anim(clipName);
		assert(ptAnim);
		ComputeBlend_OneAnim *pBlend = new ComputeBlend_OneAnim(ptAnim);

		// Timer over the clip's play range (was created inside AnimController_OneAnim).
		TimerController *pTimer = new TimerController(AnimTime(AnimTime::Duration::ZERO), ptAnim->FindMaxTime());
		assert(pTimer);

		// Phase 3: one anim drives every skin mesh below, but it must be sampled
		// ONCE per frame -- its data lives on a single dedicated entity via a
		// non-owning AnimClipComponent that the AnimationSystem drives. The
		// AnimNode (below) owns ptAnim/pTimer/pBlend and frees them.
		{
			World& w = WorldMan::GetWorld();
			Entity animEnt = w.Create();
			AnimClipComponent& ac = w.Add<AnimClipComponent>(animEnt);
			ac.pAnim = ptAnim;
			ac.pTimer = pTimer;
			ac.pBlend = pBlend;
			ac.ratio = 1.0f;
		}

		GameObjectAnimSkin *pSkins[AnimNode::MAX_GAME_SKINS]{ nullptr };

		for (unsigned int i = 0; i < numMeshes; i++)
		{
			GameObjectAnimSkin *pGameSkin = new GameObjectAnimSkin(MaterialKind::SkinLightTexture, pBlend);
			assert(pGameSkin);
			privFillSkinRender(pGameSkin, pMeshNames[i], texName, pBlend, _pLightColor, _pLightPos);

			char goName[PCSNode::NAME_SIZE]{ 0 };
			strcpy_s(goName, PCSNode::NAME_SIZE, AnimMan::NameToString(name));
			char suffix[12]{ 0 };
			sprintf_s(suffix, sizeof(suffix), "_%u", i);
			strcat_s(goName, PCSNode::NAME_SIZE, suffix);
			pGameSkin->SetName(goName);

			GameObjectMan::Add(pGameSkin, GameObjectMan::GetRoot());
			pSkins[i] = pGameSkin;
		}

		Clip *pClip = ClipMan::Find(clipName);
		assert(pClip);

		AnimNode *pNode = (AnimNode *)pMan->baseAddToFront();
		assert(pNode);
		pNode->Set(name, pClip, pSkins, numMeshes);
		pNode->pAnimA = ptAnim;
		pNode->pTimerA = pTimer;
		pNode->pComputeBlend = pBlend;
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

        // Timers over each clip's play range (were created inside AnimController_TwoAnim).
        TimerController* pTimerA = new TimerController(AnimTime(AnimTime::Duration::ZERO), pAnimA->FindMaxTime());
        TimerController* pTimerB = new TimerController(AnimTime(AnimTime::Duration::ZERO), pAnimB->FindMaxTime());
        assert(pTimerA);
        assert(pTimerB);

        // Kept so BlendAnimation can push the SPACE-key blend ratio.
        pMan->poBlendComputeBlend = pBlend;

        // Phase 3: the two-clip blend is sampled by the BlendSystem. Its data lives
        // on a dedicated entity via a non-owning AnimBlendComponent; the AnimNode
        // (below) owns the anims/timers/blend and frees them.
        {
            World& w = WorldMan::GetWorld();
            Entity blendEnt = w.Create();
            AnimBlendComponent& bc = w.Add<AnimBlendComponent>(blendEnt);
            bc.pAnimA = pAnimA;
            bc.pTimerA = pTimerA;
            bc.ratioA = 1.0f;
            bc.pAnimB = pAnimB;
            bc.pTimerB = pTimerB;
            bc.ratioB = 1.0f;
            bc.pBlend = pBlend;
        }

        GameObjectAnimSkin* pGameSkin = new GameObjectAnimSkin(MaterialKind::SkinLightTexture, pBlend);
        assert(pGameSkin);
        privFillSkinRender(pGameSkin, meshName, texName, pBlend, _pLightColor, _pLightPos);
        pGameSkin->SetName(AnimMan::NameToString(name));
        GameObjectMan::Add(pGameSkin, GameObjectMan::GetRoot());

        Clip* pClip = ClipMan::Find(clipName1);
        assert(pClip);

        AnimNode* pNode = (AnimNode*)pMan->baseAddToFront();
        assert(pNode);
        pNode->Set(name, pClip, pGameSkin);
        pNode->pAnimA = pAnimA;
        pNode->pAnimB = pAnimB;
        pNode->pTimerA = pTimerA;
        pNode->pTimerB = pTimerB;
        pNode->pComputeBlend = pBlend;
        return pNode;
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

        const float dtSec = tDelta / AnimTime( AnimTime::Duration::ONE_SECOND );
        sElapsedSec += dtSec;

        float t = sElapsedSec / 3.0f;
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

		pMan->mBlendTs = sBlendTs;

        if (pMan->poBlendComputeBlend)
        {
            pMan->poBlendComputeBlend->SetBlendTs(sBlendTs);
        }
    }

	float AnimMan::GetBlendTs()
	{
		AnimMan* pMan = AnimMan::privGetInstance();
		assert(pMan != nullptr);
		return pMan->mBlendTs;
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
		if (pNode)
		{
			for (unsigned int i = 0; i < pNode->GetNumGameSkins(); i++)
			{
				GameObjectAnimSkin *pSkin = pNode->GetGameSkin(i);
				if (pSkin)
				{
					pSkin->SetScale(sx, sy, sz);
				}
			}
		}
    }

    void AnimMan::SetUniformScale(Name name, float s)
    {
		AnimMan *pMan = AnimMan::privGetInstance();
		pMan->pCompareStrategy = AnimMan::posEnumNameCompare;
		assert(pMan->pCompareStrategy);

		pMan->poNodeCompare->mName = name;
		AnimNode *pNode = (AnimNode *)pMan->baseFind(pMan->poNodeCompare);
		if (pNode)
		{
			for (unsigned int i = 0; i < pNode->GetNumGameSkins(); i++)
			{
				GameObjectAnimSkin *pSkin = pNode->GetGameSkin(i);
				if (pSkin)
				{
					pSkin->SetScale(s, s, s);
				}
			}
		}
    }

    void AnimMan::SetPos(Name name, float x, float y, float z)
    {
		AnimMan *pMan = AnimMan::privGetInstance();
		pMan->pCompareStrategy = AnimMan::posEnumNameCompare;
		assert(pMan->pCompareStrategy);

		pMan->poNodeCompare->mName = name;
		AnimNode *pNode = (AnimNode *)pMan->baseFind(pMan->poNodeCompare);
		if (pNode)
		{
			for (unsigned int i = 0; i < pNode->GetNumGameSkins(); i++)
			{
				GameObjectAnimSkin *pSkin = pNode->GetGameSkin(i);
				if (pSkin)
				{
					pSkin->SetTrans(x, y, z);
				}
			}
		}
    }

	void AnimMan::SetPrefabPivot(Name name)
	{
		AnimMan *pMan = AnimMan::privGetInstance();
		pMan->pCompareStrategy = AnimMan::posEnumNameCompare;
		assert(pMan->pCompareStrategy);

		pMan->poNodeCompare->mName = name;
		AnimNode *pNode = (AnimNode *)pMan->baseFind(pMan->poNodeCompare);
		if (pNode)
		{
			for (unsigned int i = 0; i < pNode->GetNumGameSkins(); i++)
			{
				GameObjectAnimSkin *pSkin = pNode->GetGameSkin(i);
				if (pSkin)
				{
					pSkin->SetPrefab(new Prefab_Pivot());
				}
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
		if (pNode)
		{
			for (unsigned int i = 0; i < pNode->GetNumGameSkins(); i++)
			{
				GameObjectAnimSkin *pSkin = pNode->GetGameSkin(i);
				if (pSkin)
				{
					pSkin->cur_rot_x = angle;
				}
			}
		}
    }

    void AnimMan::SetPivotRotY(Name name, float angle)
    {
		AnimMan *pMan = AnimMan::privGetInstance();
		pMan->pCompareStrategy = AnimMan::posEnumNameCompare;
		assert(pMan->pCompareStrategy);

		pMan->poNodeCompare->mName = name;
		AnimNode *pNode = (AnimNode *)pMan->baseFind(pMan->poNodeCompare);
		if (pNode)
		{
			for (unsigned int i = 0; i < pNode->GetNumGameSkins(); i++)
			{
				GameObjectAnimSkin *pSkin = pNode->GetGameSkin(i);
				if (pSkin)
				{
					pSkin->cur_rot_y = angle;
				}
			}
		}
    }

    void AnimMan::SetPivotRotZ(Name name, float angle)
    {
		AnimMan *pMan = AnimMan::privGetInstance();
		pMan->pCompareStrategy = AnimMan::posEnumNameCompare;
		assert(pMan->pCompareStrategy);

		pMan->poNodeCompare->mName = name;
		AnimNode *pNode = (AnimNode *)pMan->baseFind(pMan->poNodeCompare);
		if (pNode)
		{
			for (unsigned int i = 0; i < pNode->GetNumGameSkins(); i++)
			{
				GameObjectAnimSkin *pSkin = pNode->GetGameSkin(i);
				if (pSkin)
				{
					pSkin->cur_rot_z = angle;
				}
			}
		}
    }

    void AnimMan::SetPivotTotalRot(Name name, const Rot3 mode, float x, float y, float z)
    {
		AnimMan *pMan = AnimMan::privGetInstance();
		pMan->pCompareStrategy = AnimMan::posEnumNameCompare;
		assert(pMan->pCompareStrategy);

		pMan->poNodeCompare->mName = name;
		AnimNode *pNode = (AnimNode *)pMan->baseFind(pMan->poNodeCompare);
		if (pNode)
		{
			Quat q(mode, x, y, z);
			for (unsigned int i = 0; i < pNode->GetNumGameSkins(); i++)
			{
				GameObjectAnimSkin *pSkin = pNode->GetGameSkin(i);
				if (pSkin)
				{
					pSkin->SetQuat(q);
					pSkin->cur_rot_x = 0.0f;
					pSkin->cur_rot_y = 0.0f;
					pSkin->cur_rot_z = 0.0f;
				}
			}
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
