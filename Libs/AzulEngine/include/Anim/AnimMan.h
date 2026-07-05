//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ANIM_MAN_H
#define ANIM_MAN_H 

#include "ManBase.h"
#include "DLink.h"
#include "DLinkMan.h"
#include "AnimController.h"
#include "AnimController_TwoAnim.h"
#include "AnimController_OneAnim.h"
#include "Clip.h"
#include "AnimTime.h"
#include "Mesh.h"
#include "TextureObject.h"
#include "Skel.h"
#include "HierarchyTable.h"

#include "EngineDLLInterface.h"

namespace Azul
{
	class GameObjectAnimSkin;

    class AZUL_ENGINE_LIBRARY_API AnimMan : public ManBase
    {
    public:
        enum class Name
        {
            Uninitialized,
            Walk,
            Run,
            HitBack,
            Wave,
            Shotup,
            Dance,
            Gangnam,
            Blend,
            Breakdance,
            Salsa,
            Swing,
            Rumba,
            Shuffle,
            Idle
        };

        // AnimMan is gameplay glue and stays in the app, so it owns its own
        // Name->string conversion (was a StringThis overload before the DLL split).
        static char *NameToString(Name status);

        static void Create(int reserveNum = 0, int reserveGrow = 1);
        static void Destroy();

        static DLink* Add(  Name name, 
                            const char* clipFileName, 
                            Skel::Name skelName, 
                            TextureObject::Name texName, 
                            Mesh::Name meshName, 
                            Vec3 &_pLightColor, 
                            Vec3 &_pLightPos);
		static DLink* Add(Name name,
						  const char* clipFileName,
						  Skel::Name skelName,
						  TextureObject::Name texName,
						  const Mesh::Name* pMeshNames,
						  unsigned int numMeshes,
						  Vec3& _pLightColor,
						  Vec3& _pLightPos);
        static DLink* Add(Name name,
                          const char* clipFileName1,
                          const char* clipFileName2,
                          Skel::Name skelName,
                          TextureObject::Name texName,
                          Mesh::Name meshName,
                          Vec3& _pLightColor,
                          Vec3& _pLightPos);
        static AnimController *Find(Name name);
        static void Update(AnimTime tCurr);
        static void BlendAnimation(AnimTime tDelta);
		static float GetBlendTs();

        static void SetScale(Name name, float sx, float sy, float sz);
        static void SetUniformScale(Name name, float s);
        static void SetPos(Name name, float x, float y, float z);
        static void SetPivotRotX(Name name, float angle);
        static void SetPivotRotY(Name name, float angle);
        static void SetPivotRotZ(Name name, float angle);
        static void SetPivotTotalRot(Name name, const Rot3 mode, float x, float y, float z);
		static void SetPrefabPivot(Name name);
        static void SetBlendTs(Name name, float ts);
        static Clip *GetClip(Name name);

        static void Remove(DLink *pNode);
        static void Dump();

    private:
        static AnimMan *privGetInstance();
        static HierarchyTable::Name privMapToHierarchyName(Skel::Name skelName);
        AnimMan() = delete;
        AnimMan(const AnimMan &) = delete;
        AnimMan &operator = (const AnimMan &) = delete;
        virtual ~AnimMan();

        AnimMan(int reserveNum, int reserveGrow);

    protected:
        DLink *derivedCreateNode() override;

    public:
        class AZUL_ENGINE_LIBRARY_API AnimNode : public DLink
        {
        public:
            AnimNode();
            AnimNode(const AnimNode &) = delete;
            AnimNode &operator=(const AnimNode &) = delete;
            virtual ~AnimNode();

            void Set(Name inName, Clip *pClip, AnimController *pController, GameObjectAnimSkin *pGameSkin);
			void Set(Name inName, Clip *pClip, AnimController *pController, GameObjectAnimSkin *const *pGameSkins, unsigned int numGameSkins);
            AnimController *GetController();
			unsigned int GetNumGameSkins() const;
			GameObjectAnimSkin *GetGameSkin(unsigned int index) const;

            char *GetName() override;
            void Wash() override;
            void Dump() override;

        private:
            void privClear();

        public:
            Name mName;
            Clip *pClip;
            AnimController *pController;
			static const unsigned int MAX_GAME_SKINS = 32;
			GameObjectAnimSkin *pGameSkins[MAX_GAME_SKINS];
			unsigned int numGameSkins;
        };

    private:
        AnimNode *poNodeCompare;
        AnimController_TwoAnim *poBlendTwoAnimController;
		float mBlendTs;
        static AnimMan *posInstance;
        static CompareStrategyBase *posEnumNameCompare;
    };
}

#endif

// --- End of File ---
