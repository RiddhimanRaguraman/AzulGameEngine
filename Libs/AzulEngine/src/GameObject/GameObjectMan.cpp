//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "MeshNull.h"
#include "GraphicsObject_Null.h"
#include "ShaderObject_Null.h"
#include "GameObject.h"
#include "GameObjectMan.h"
#include "PCSTreeForwardIterator.h"
#include "MeshNodeMan.h"
#include "ShaderObjectNodeMan.h"
#include "WorldMan.h"
#include "World.h"
#include "SystemMan.h"
#include "LocalToWorldSystem.h"
#include "RotateSystem.h"
#include "AnimationSystem.h"
#include "BlendSystem.h"
#include "SkinningSystem.h"
#include "RenderSystem.h"

namespace Azul
{
	// ECS render path ON by default. Flip to false to A/B compare against the
	// pure GraphicsObject::Render() path (both now run in the same PCS-tree order).
	bool GameObjectMan::sUseECSRender = true;

	bool GameObjectMan::GetUseECSRender()
	{
		return sUseECSRender;
	}

	void GameObjectMan::SetUseECSRender(bool b)
	{
		sUseECSRender = b;
	}

	void GameObjectMan::Add(GameObject *pObj, GameObject *pParent)
	{
		assert(pObj);
		assert(pParent);

		// Get singleton
		GameObjectMan *pGOM = GameObjectMan::privGetInstance();

		// insert object to root
		pGOM->poRootTree->Insert(pObj, pParent);

		// Bridge: mirror the parent into the child's HierarchyComponent (the
		// PCSTree stays authoritative for traversal until Phase 5).
		pObj->SetParent(pParent);
	}

	PCSTree *GameObjectMan::GetPCSTree()
	{
		// Get singleton
		GameObjectMan *pGOM = GameObjectMan::privGetInstance();
		assert(pGOM);

		return pGOM->poRootTree;
	}

	GameObject *GameObjectMan::GetRoot()
	{
		// Get singleton
		GameObjectMan *pGOM = GameObjectMan::privGetInstance();
		assert(pGOM);

		GameObject *pGameObj = (GameObject *)pGOM->poRootTree->GetRoot();
		assert(pGameObj);

		return pGameObj;
	}

	void GameObjectMan::Create()
	{
		GameObjectMan *pGOM = GameObjectMan::privGetInstance();
		assert(pGOM);

		if (pGOM->poRootTree == nullptr)
		{
			MeshNull *pMesh = new MeshNull();
			assert(pMesh);

			MeshNodeMan::Add(Mesh::Name::NULL_MESH, pMesh);

			ShaderObject *pShader = new ShaderObject_Null(ShaderObject::Name::NullShader);
			assert(pShader);
			ShaderObjectNodeMan::Add(pShader);

			GraphicsObject_Null *pGraphicsObject = new GraphicsObject_Null(Mesh::Name::NULL_MESH, ShaderObject::Name::NullShader);
			GameObjectRigidBody *pGameRoot = new GameObjectRigidBody(pGraphicsObject);
			pGameRoot->SetName("GameObject_Root");

			pGOM->poRootTree = new PCSTree();
			assert(pGOM->poRootTree);

			pGOM->poRootTree->Insert(pGameRoot, pGOM->poRootTree->GetRoot());

			// Phase 3: engine systems that run over the component pools each
			// frame, in order. LocalToWorldSystem computes world = S*R*T;
			// RotateSystem spins entities with a RotateComponent (overwrites the
			// plain world, so it runs after LocalToWorld); AnimationSystem samples
			// single-clip anims and BlendSystem samples two-clip blends into the
			// mixer buffers; SkinningSystem then dispatches GPU compute skinning
			// that consumes them. Each is a no-op in scenes without the matching
			// components.
			SystemMan::Create();
			SystemMan::Add(new LocalToWorldSystem());
			SystemMan::Add(new RotateSystem());
			SystemMan::Add(new AnimationSystem());
			SystemMan::Add(new BlendSystem());
			SystemMan::Add(new SkinningSystem());
		}
	}

	void GameObjectMan::Destroy()
	{
		// Get singleton
		GameObjectMan *pGOM = GameObjectMan::privGetInstance();
		assert(pGOM);

		PCSTree *pTree = pGOM->poRootTree;
		if (pTree == nullptr)
		{
			return;
		}

		// Free the systems (before WorldMan::Destroy in the scene unload).
		SystemMan::Destroy();

		PCSNode *pNode = nullptr;

		PCSTreeForwardIterator pForIter(pTree->GetRoot());
		pNode = pForIter.First();
		PCSNode *pTmp = nullptr;

		// todo: create an Erase() like SE456
		while (!pForIter.IsDone())
		{
			pTmp = pForIter.Current();

			pNode = pForIter.Next();
			delete pTmp;
		}

		delete pGOM->poRootTree;
		pGOM->poRootTree = nullptr;
	}

	void GameObjectMan::Update(AnimTime currentTime, AnimTime tDelta)
	{
		//Trace::out("\n");
		//Trace::out("------ GameObjectMan::Update() -----------------\n");
		//Trace::out("\n");

		GameObjectMan *pGOM = GameObjectMan::privGetInstance();
		assert(pGOM);

		// Phase 3: systems run first over the component pools (LocalToWorldSystem
		// fills world = S*R*T), then the tree walk lets behavior objects
		// (prefab-driven / skinned) overwrite world and do per-object work.
		// Systems are driven with the frame DELTA (time-based systems -- e.g.
		// AnimationSystem's timer -- accumulate it); the tree walk keeps absolute.
		SystemMan::Run(WorldMan::GetWorld(), tDelta);

		PCSNode *pRootNode = pGOM->poRootTree->GetRoot();
		assert(pRootNode);

		PCSTreeForwardIterator pIt(pRootNode);

		GameObject *pGameObj = nullptr;

		for (pIt.First(); !pIt.IsDone(); pIt.Next())
		{
			pGameObj = (GameObject *) pIt.Current();
			//pGameObj->PrintNode();
			pGameObj->Update(currentTime);
		}
	}

	void GameObjectMan::Draw()
	{
		//Trace::out("\n");
		//Trace::out("--- GameObjectMan::Draw() ----\n");
		GameObjectMan *pGOM = GameObjectMan::privGetInstance();
		assert(pGOM);

		// P4.2: single PCS-tree walk (authoritative draw order). When the ECS path
		// is on, GameObject::Draw routes 3D materials through the RenderSystem's
		// per-MaterialKind branches (in this tree order); 2D/UI stay on the
		// GraphicsObject path. When off, everything draws via GraphicsObject::Render.
		PCSNode *pRootNode = pGOM->poRootTree->GetRoot();
		assert(pRootNode);

		PCSTreeForwardIterator pIt(pRootNode);

		GameObject *pGameObj = nullptr;

		for (pIt.First(); !pIt.IsDone(); pIt.Next())
		{
			pGameObj = (GameObject *) pIt.Current();
			//pGameObj->PrintNode();
			pGameObj->Draw();
		}
	}

	GameObjectMan::GameObjectMan()
		: poRootTree(nullptr)
	{
	}

	GameObjectMan::~GameObjectMan()
	{
		delete this->poRootTree;
	}

	GameObjectMan *GameObjectMan::privGetInstance(void)
	{
		// This is where its actually stored (BSS section)
		static GameObjectMan gom;
		return &gom;
	}

}

// --- End of File ---
