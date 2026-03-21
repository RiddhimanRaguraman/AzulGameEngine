//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ANIM_MAN_H
#define ANIM_MAN_H 

#include "ManBase.h"
#include "DLink.h"
#include "DLinkMan.h"
#include "AnimController.h"
#include "Clip.h"
#include "AnimTime.h"

#include "TextureObject.h"
#include "Skel.h"
#include "HierarchyTable.h"

namespace Azul
{
	class GameObjectAnimSkin;

    class AnimMan : public ManBase
    {
    public:
        enum class Name
        {
            Uninitialized,
            Walk,
            Run,
            HitBack,
            Shotup,
            Dance,
            Gangnam,
            Idle
        };
        
        static void Create(int reserveNum = 0, int reserveGrow = 1);
        static void Destroy();

        static DLink* Add(  Name name, 
                            const char* clipFileName, 
                            AnimTime delta, 
                            Skel::Name skelName, 
                            TextureObject::Name texName, 
                            Mesh::Name meshName, 
                            Vec3 &_pLightColor, 
                            Vec3 &_pLightPos);
        static AnimController *Find(Name name);
        static void Update();

        static void SetScale(Name name, float sx, float sy, float sz);
        static void SetUniformScale(Name name, float s);
        static void SetPos(Name name, float x, float y, float z);
        static void SetPivotRotX(Name name, float angle);
        static void SetPivotRotY(Name name, float angle);
        static void SetPivotRotZ(Name name, float angle);
        static void SetPivotTotalRot(Name name, const Rot3 mode, float x, float y, float z);
        static void SetDelta(Name name, float scale);
		static void SetPrefabPivot(Name name);
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
        class AnimNode : public DLink
        {
        public:
            AnimNode();
            AnimNode(const AnimNode &) = delete;
            AnimNode &operator=(const AnimNode &) = delete;
            virtual ~AnimNode();

            void Set(Name inName, Clip *pClip, AnimController *pController, GameObjectAnimSkin *pGameSkin);
            AnimController *GetController();
			GameObjectAnimSkin *GetGameSkin();

            char *GetName() override;
            void Wash() override;
            void Dump() override;

        private:
            void privClear();

        public:
            Name mName;
            Clip *pClip;
            AnimController *pController;
			GameObjectAnimSkin *pGameSkin;
        };

    private:
        AnimNode *poNodeCompare;
        static AnimMan *posInstance;
        static CompareStrategyBase *posEnumNameCompare;
    };
}

#endif

// --- End of File ---
