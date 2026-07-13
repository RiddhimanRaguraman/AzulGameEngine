#include <string.h>

#include "SpriteRenderSystem.h"
#include "World.h"
#include "Sprite2DComponent.h"
#include "TextComponent.h"
#include "TransformComponent.h"
#include "Mesh.h"
#include "ShaderObject.h"
#include "TextureObject.h"
#include "CameraNodeMan.h"
#include "GlyphMan.h"
#include "Glyph.h"
#include "Rect.h"
#include "Engine.h"
#include "Profiler.h"

namespace Azul
{
	// 2D/UI pool pass, run after the opaque 3D pass. Plain sprites (a
	// Sprite2DComponent with no TextComponent) draw one quad from their world;
	// text entities run the per-glyph loop.
	void SpriteRenderSystem::Draw(World &world)
	{
		ComponentPool<Sprite2DComponent> &pool = world.Pool<Sprite2DComponent>();
		const unsigned int count = pool.GetCount();

		// Hoist the shader/CBV bind across the whole 2D pass (all sprites + every
		// glyph typically share the Sprite shader; rebinding it is idempotent).
		ShaderObject *pLastShader = nullptr;

		for (unsigned int i = 0; i < count; i++)
		{
			Sprite2DComponent &s = pool.GetData(i);
			const Entity &e = pool.GetOwner(i);

			TextComponent *pText = world.TryGet<TextComponent>(e);
			if (pText != nullptr)
			{
				// Text run: the glyph loop fills `s` per glyph and draws each.
				DrawText(*pText, s, pLastShader);
			}
			else
			{
				// Plain sprite: one quad from the entity's world matrix.
				TransformComponent *pT = world.TryGet<TransformComponent>(e);
				assert(pT);
				DrawSprite(s, pT->world, pLastShader);
			}
		}
	}

	// Walk the message, look each glyph
	// up, and re-fill the Sprite2DComponent (texture + UV + screen-rect) before
	// drawing that glyph's quad at its running x position.
	void SpriteRenderSystem::DrawText(TextComponent &t, Sprite2DComponent &s, ShaderObject *&pLastShader)
	{
		assert(t.pMessage);
		size_t len = strlen(t.pMessage);

		Rect screenRect;

		float yTmp = t.y;
		float xEnd = t.x;

		for (size_t i = 0; i < len; i++)
		{
			int key = (int)t.pMessage[i];

			Glyph *pGlyph = GlyphMan::Find(t.glyphName, key);
			assert(pGlyph);

			float xTmp = xEnd + pGlyph->glyphRect.width / 2;
			screenRect.Set(xTmp, yTmp, pGlyph->glyphRect.width, pGlyph->glyphRect.height);

			// Per-glyph sprite state.
			s.pTexture = pGlyph->pText;

			float w = pGlyph->glyphRect.width / (float)pGlyph->pText->width;
			float h = pGlyph->glyphRect.height / (float)pGlyph->pText->height;
			float u = pGlyph->glyphRect.x / (float)pGlyph->pText->width;
			float v = pGlyph->glyphRect.y / (float)pGlyph->pText->height;
			Scale ScaleUV(w, h, 1.0f);
			Trans TransUV(u, v, 0.0f);
			s.uvMatrix = ScaleUV * TransUV;

			Scale ScaleRect(screenRect.width, screenRect.height, 1.0f);
			s.origMatrix = ScaleRect;

			// Glyph world = Trans(x, y) (text never scales/rotates: S = R = I).
			Trans T(xTmp, yTmp, 0.0f);
			Mat4 world = T;

			DrawSprite(s, world, pLastShader);

			// advance to the next character
			xEnd = pGlyph->glyphRect.width / 2 + xTmp;
		}
	}

	void SpriteRenderSystem::DrawSprite(Sprite2DComponent &s, Mat4 &world, ShaderObject *&pLastShader)
	{
		assert(s.pMesh);
		assert(s.pShader);
		assert(s.pTexture);

		Profiler::CountDraw();   // one 2D draw call per sprite/glyph quad

		// SetState
		s.pTexture->ActivateTexture();
		if (s.pTexture->HasAlpha())
		{
			Engine::GetInstance()->mBlendStateAlpha.Activate();
		}

		// SetDataGPU -- bind the shader/CBV only when it changes.
		if (s.pShader != pLastShader)
		{
			s.pShader->ActivateShader();
			s.pShader->ActivateCBV();
			pLastShader = s.pShader;
		}

		Camera *pCam = CameraNodeMan::GetCurrent(Camera::Type::ORTHOGRAPHIC_2D);
		assert(pCam);

		Trans TransToOriginLowerLeft((float)-pCam->getScreenWidth() / 2.0f, (float)-pCam->getScreenHeight() / 2.0f, 0.0f);
		Mat4 tmpMatrix = s.origMatrix * world * TransToOriginLowerLeft;

		s.pShader->TransferWorldViewProj(pCam, &tmpMatrix);
		s.pShader->TransferUVMatrix(&s.uvMatrix);
		s.pShader->TransferColorScale(&s.color);

		s.pMesh->ActivateMesh();

		// Draw
		s.pMesh->RenderIndexBuffer();

		// RestoreState
		Engine::GetInstance()->mBlendStateOff.Activate();
	}
}

// --- End of File ---
