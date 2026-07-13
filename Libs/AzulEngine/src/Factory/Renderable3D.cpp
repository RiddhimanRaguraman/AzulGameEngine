#include "Renderable3D.h"

#include "MathEngine.h"
#include "WorldMan.h"
#include "World.h"
#include "TransformComponent.h"
#include "RenderComponent.h"

namespace Azul
{
	Entity Renderable3D::Add(MaterialKind kind)
	{
		World &w = WorldMan::GetWorld();
		Entity e = w.Create();

		// Placement transform; LocalToWorldSystem folds pos/rot/scale into world.
		TransformComponent &t = w.Add<TransformComponent>(e);
		t.pos.set(0.0f, 0.0f, 0.0f);
		t.scale.set(1.0f, 1.0f, 1.0f);
		t.rot = Quat(Identity);
		t.world = Mat4(Identity);

		// Render data: superset across the 3D kinds. The caller fills the handles it needs.
		RenderComponent &r = w.Add<RenderComponent>(e);
		r.kind = kind;
		r.drawEnable = true;
		r.layer = 0;
		r.pMesh = nullptr;
		r.pShader = nullptr;
		r.pTex = nullptr;
		r.pComputeBlend = nullptr;
		r.lightColor.set(0.0f, 0.0f, 0.0f);
		r.lightPos.set(0.0f, 0.0f, 0.0f);
		r.bodyColor.set(0.0f, 0.0f, 0.0f);
		r.uvMatrix = Mat4(Identity);

		return e;
	}

	TransformComponent &Renderable3D::GetTransform(const Entity &e)
	{
		TransformComponent *pT = WorldMan::GetWorld().TryGet<TransformComponent>(e);
		assert(pT);
		return *pT;
	}

	RenderComponent &Renderable3D::GetRender(const Entity &e)
	{
		RenderComponent *pR = WorldMan::GetWorld().TryGet<RenderComponent>(e);
		assert(pR);
		return *pR;
	}

	void Renderable3D::SetTrans(const Entity &e, float x, float y, float z)
	{
		Renderable3D::GetTransform(e).pos.set(x, y, z);
	}

	void Renderable3D::SetScale(const Entity &e, float s)
	{
		Renderable3D::GetTransform(e).scale.set(s, s, s);
	}

	void Renderable3D::SetScale(const Entity &e, float sx, float sy, float sz)
	{
		Renderable3D::GetTransform(e).scale.set(sx, sy, sz);
	}
}

// --- End of File ---
