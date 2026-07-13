# WhatToDo — OOP → ECS migration plan for the Azul engine

> Reference doc for the incremental conversion of the Azul DirectX 11 renderer from a
> deep-inheritance OOP design to an Entity-Component-System (ECS) design **on top of a
> properly layered engine module boundary**.
> Written so it can be followed one phase at a time without a big-bang rewrite.
> The engine stays compilable and runnable after **every** phase.

---

## 0. How to use this document

- Work top-to-bottom, one **phase** at a time (Section 7). Do not start a phase until the
  previous phase's *exit criteria* are green.
- Each phase lists: **goal**, **files touched**, **concrete steps**, **exit criteria**, **risk**.
- **Section 0a is the coding standard. Every line of new/moved code must obey it — no exceptions
  beyond the one listed.**
- Section 3 is the master inventory of every **Entity / Component / System**. When in doubt
  about "where does class X go", look there first.
- Section 4 lists what deliberately **stays OOP**. Do not ECS-ify those.
- Section 5 is the **engine module boundary** (the DLL split) — the layering that lets gameplay
  stay "plug and play" and never reach engine internals.
- Section 2 is the honest performance expectation — read it before promising anyone an FPS number.

**Locked decisions:** `AzulEngine` ships as a **DLL** (same convention as `Libs/Math`,
`Libs/AnimTime`). The ECS registry is **hand-rolled** (own sparse-set), not EnTT.

---

## 0a. Coding standard (MANDATORY for all new and moved code)

This engine follows a deliberately restricted, debuggable C++ style. Match it exactly.

**No containers**
- No STL containers at all — no `std::vector`, `std::list`, `std::set`, `std::map`, `std::array`, etc.
- No automatic/growable containers. Do it the old-fashioned way: raw pointers and
  hand-managed arrays allocated with `new[]` / `malloc`, with explicit counts and capacity.

**Simple C++ (no modern C++)**
- No lambdas, no `auto`, no templates, no range-for-over-STL, no `std::function`, no smart
  pointers (`unique_ptr`/`shared_ptr`), no `<algorithm>`.
- No Boost, no third-party template libraries.

**No streams**
- No `<iostream>`/`<fstream>`/`std::cout`. Use C file I/O: `fopen`, `fread`, `fwrite`,
  `fclose`. (Existing `Trace::out` for logging is fine.)

**No code in macros**
- Macros may define constants/flags only. **Logic lives in `.cpp` files** so it can be seen
  and stepped through in the debugger. No multi-line "do real work" macros.

**Memory ownership (no leaks)**
- If a class allocates with `new`/`malloc`, that same class is responsible for the matching
  `delete`/`free` in its destructor. Ownership is explicit and local. Every `new` has a
  documented owner and a matching `delete`.

**The ONE exception**
- Templates are permitted **only** where the problem is inherently generic and cannot be
  expressed reasonably otherwise (e.g. the ECS component-pool type dispatch). Keep such
  templates minimal, and keep any real logic in `.cpp` where feasible. When you use the
  exception, add a comment saying why.

**File structure (mandatory)**
- **One `.h` + one `.cpp` per class / per component.** Do **not** combine multiple components
  (or multiple classes) into a single file. `TransformComponent` → `TransformComponent.h` +
  `TransformComponent.cpp`; `RenderComponent` → its own pair; and so on.
- Header guards (`#ifndef/#define/#endif`) in the existing style, `namespace Azul`, the
  existing file header comment block, and `// --- End of File ---` footer. Match surrounding code.

---

## 1. Current architecture (what we are migrating from)

- **Build layout today:** `Engine/src/Engine.vcxproj` is `ConfigurationType = Application` — the
  **entire engine (RHI + rendering + gameplay) is one monolithic `.exe`**. The reusable
  foundation lives in `Libs/*` (`Math` = DLL, plus `PCSTree`, `Manager`, `File`, `ProtoBuf`,
  `PugiXml`). There is **no separation between "engine runtime" and "game"** yet — Section 5
  fixes that.
- `GameObject : PCSNode` — abstract base. Owns `poWorld` (Mat4*), `poGraphicsObject`
  (GraphicsObject*), `mDrawEnable`. Virtual `Update()` / `Draw()`.
  - Subclasses: `GameObjectRigidBody`, `GameObjectControlled`,
    `GameObjectAnimSkin : GameObjectControlled`, `GameObjectSprite`, `GameObjectTerrain`.
- `GraphicsObject : GraphicsObject_Abstract` — one subclass **per material/shader combo**:
  `_ColorByVertex`, `_Wireframe`, `_FlatTexture`, `_LightTexture`, `_ConstColor`,
  `_ConstColorLight`, `_Sprite`, `_Null`, `_SkinLightTexture`, `_SkinFlatTexture`.
  Each does `SetState() / SetDataGPU() / Draw() / RestoreState()`. Lighting is hard-coded
  inside `_LightTexture` (`poLightColor`, `poLightPos`).
- `GameObjectMan` — singleton. Owns a `PCSTree` scene graph. `Update()`/`Draw()` walk the
  tree with `PCSTreeForwardIterator`, calling the virtual on each node.
- Behaviors = `Prefab_*` (`Rotate`, `RotateSpin`, `Pendulum`, `Pulse`, `FiboSpiral`,
  `Pivot`, `PrefabAnim`) driven from each object's `Update()`.
- Animation = `Anim` (clip+skeleton) + `AnimController_OneAnim/_TwoAnim` +
  `ComputeBlend_OneAnim/_TwoAnim` (GPU compute skinning) + `Mixer A/B/C` + `Skeleton` +
  `Clip` + `Bone` + `TimerController` + `JointTable` + `HierarchyTable` + `AnimMan`.
- Resource managers (keep these): `MeshNodeMan`, `ShaderObjectNodeMan`, `TexNodeMan`,
  `CameraMan`, `ImageMan`, `GlyphMan`, `AnimMan`, `JointTableMan`, `HierarchyTableMan`.
- DirectX RHI wrapper (keep OOP): `StateDirectXMan`, `StateDevice`, `StateDeviceContext`,
  `StateSwapChain`, `StateRasterizer`, `StateBlend`, `StateDepthStencil*`, `StateViewport`,
  `StateRenderTargetView`, `StateAdapter`, `StateFactory`, `StateDebug`, and all `Buffer*`.

---

## 2. Performance reality check (read before over-promising)

**Is ECS a guaranteed FPS win for this engine? No.**

- Dramatic ECS speedups appear at **thousands-to-millions** of entities. At the hundreds of
  objects this engine renders, the bottleneck is the **GPU and redundant D3D11 state binds**,
  not cache misses over a few hundred `Mat4`s.
- The real, bankable perf win available here is **sorting/batching draws by shader+material**
  and cutting redundant binds. That comes from the `RenderSystem` in **Phase 4** —
  independent of whether the rest of the engine is ECS.
- Skinning is already on the GPU via `ComputeBlend_*`, so animation is not CPU-bound.

**Why do the migration anyway (the actual justification):**
1. **Composition over inheritance** — kills the `GameObjectAnimSkin : GameObjectControlled :
   GameObject` chain and the ~10-way `GraphicsObject_*` explosion.
2. **A real engine/game boundary** (Section 5) — gameplay becomes plug-and-play over handles
   and never touches shader/RHI internals.
3. **Unlocks batching + job-parallel systems** — linear component arrays are trivially
   sortable and parallelizable; virtual `Update()` over a tree is not.

**Bottom line:** migrate for architecture, encapsulation, and to *enable* batching/parallelism.
Treat raw FPS gains as a bonus; if the only goal were speed, do Phase 4 batching and stop.

---

## 3. Master inventory — Entities, Components, Systems

### 3a. Entities (archetypes — just an id + a set of components)

| Today (OOP class) | ECS archetype = entity + these components |
|---|---|
| `GameObjectRigidBody` | Transform, Render, (optional) behavior comp, Hierarchy |
| `GameObjectControlled` | Transform, Render, InputControlled, Hierarchy |
| `GameObjectAnimSkin` | Transform, Render, Skeleton, AnimClip/AnimBlend, GpuSkin, InputControlled, Hierarchy |
| `GameObjectSprite` | Transform2D, Sprite, Render |
| `GameObjectTerrain` | Transform, Render, (Terrain tag) |
| root / null object | Transform, (Root tag) — no Render |

### 3b. Components (plain data, no logic)

| Component | Fields (data only) | Replaces |
|---|---|---|
| `TransformComponent` | `Vec3 pos; Quat rot; Vec3 scale; Mat4 world` | `poWorld`, `poTrans/poQuat/poScale` |
| `HierarchyComponent` | `Entity parent` (children optional) | `PCSNode`/`PCSTree` links |
| `RenderComponent` | `MeshName mesh; ShaderName shader; TextureName tex; enum MaterialKind` | `poGraphicsObject` + `GraphicsObject_*` type |
| `MaterialComponent` | material params (e.g. const color) | per-subclass member data in `GraphicsObject_*` |
| `LightComponent` | `Vec3 color; Vec3 pos` (later: type, range) | hard-coded `poLightColor/poLightPos` in `_LightTexture` |
| `DrawEnabled` (tag) | — | `mDrawEnable` |
| `Sprite2DComponent` | `Rect src; TextureName tex; z` | `GameObjectSprite` / `GraphicsObject_Sprite` |
| `SkeletonComponent` | `Skeleton* / bone array handle` | `Skeleton`, `Bone`, `JointTable`, `HierarchyTable` |
| `AnimClipComponent` | `Clip::Name clip; AnimTime t; TimerController` | `Anim` + `AnimController_OneAnim` |
| `AnimBlendComponent` | `clipA, ratioA, clipB, ratioB, blendTs` | `AnimController_TwoAnim` inputs |
| `GpuSkinComponent` | `ComputeBlend*` handle (compute pipeline + bone SRV/UAV) | `ComputeBlend_OneAnim/_TwoAnim` binding data |
| `InputControlledComponent` | control flags / speeds | `GameObjectControlled::index` + controlled logic |
| behavior comps: `RotateComponent`, `RotateSpinComponent`, `PendulumComponent`, `PulseComponent`, `FiboSpiralComponent`, `PivotComponent` | their per-behavior params (axis, rate, amplitude…) | `Prefab_*` classes |

> **Handle rule:** components hold **ids/handles** (`MeshName`, `Clip::Name`,
> `TextureObject::Name`, `ComputeBlend*`), never own GPU objects. Actual resources stay in the
> resource managers, which live **behind the engine DLL** (Section 4 + 5).

### 3c. Systems (logic that iterates component pools, run in this order each frame)

| System | Reads → writes | Replaces |
|---|---|---|
| `InputSystem` | input state → InputControlled/Transform | `CameraMan::ProcessInput`, controlled `Update()` |
| behavior systems: `RotateSystem` / `PendulumSystem` / `PulseSystem` / `RotateSpinSystem` / `FiboSpiralSystem` / `PivotSystem` | behavior comp + Transform → Transform | `Prefab_*::DerivedUpdate()` |
| `AnimationSystem` | AnimClip + time → skeleton pose | `AnimController_OneAnim::Update` |
| `BlendSystem` | AnimBlend + time → blended pose | `AnimController_TwoAnim::Update`, `Mixer*` |
| `SkinningSystem` | Skeleton pose + GpuSkin → GPU (dispatch compute) | `ComputeBlend_*::Execute/BindWorldBoneArray` |
| `TransformPropagationSystem` | Transform + Hierarchy → world matrix | the implicit world compose in the PCS tree walk |
| `RenderSystem` | Render + Material + Transform (+ Light, Camera) → draw calls, **sorted by shader/material** | `GameObjectMan::Draw` + every `GraphicsObject_*::SetState/SetDataGPU/Draw/RestoreState` |

> **Is the `RenderSystem` "just ECS"?** It is an ECS system, but it is specifically the
> **bridge** between the ECS world (handle-based components) and the OOP RHI. It is the one
> system that legitimately reaches into `State*`/`Buffer*`/`Mesh` internals — that is its job,
> and it lives on the **engine** side of the DLL boundary. Gameplay never sees any of that; it
> only ever writes `RenderComponent` handles. All the other systems are pure data-over-pools.

---

## 4. What deliberately STAYS OOP (do not convert)

These are **service / RHI layers**, not game-world data. The `RenderSystem` and
`SkinningSystem` *call into* them. They all live **inside the engine DLL** (Section 5).

- **DirectX RHI:** `StateDirectXMan`, `StateDevice`, `StateDeviceContext`, `StateSwapChain`,
  `StateRasterizer`, `StateBlend`, `StateDepthStencil*`, `StateViewport`,
  `StateRenderTargetView`, `StateAdapter`, `StateFactory`, `StateDebug`.
- **GPU resource wrappers:** all `Buffer*` (`BufferCBV_*`, `BufferVBV_*`, `BufferIBV_*`,
  `BufferInputLayout_*`, `BufferPixelShader_*`, `BufferVertexShader_*`,
  `BufferComputeShader_*`, `BufferTexture2D`, `BufferSamplerState`, `BufferSRV_*`).
- **Asset loading:** `Converter/*` (mesh/texture/vbo/anim converters), protobuf
  (`Shared/ProtoBuf_lib_items`, `Libs/ProtoBuf`), `PugiXml`, `File`.
- **Resource managers / caches:** `MeshNodeMan`, `ShaderObjectNodeMan`, `TexNodeMan`,
  `ImageMan`, `GlyphMan`, `AnimMan`, `JointTableMan`, `HierarchyTableMan`. (These *become* the
  asset/handle registry that components index into.)
- **Math library** (`Libs/Math`), `Camera` (data-holder; frustum math stays in `Camera`).

Reason: ECS earns its keep by iterating many homogeneous items. None of the above is iterated
per-entity; each is a singleton-ish service with no bulk data to stream.

---

## 5. Engine module boundary (the DLL split) — *set up first, in Phase 0*

**Goal:** two modules with a hard boundary, so gameplay is plug-and-play and can never reach
engine internals (RHI, shader plumbing, loaders).

```
┌─────────────────────────────────────────────┐
│  Game  (Application / .exe)                  │  ← gameplay only
│  - builds scenes: World::Create(),           │
│    World::Add<RenderComponent>{mesh,mat,tex} │
│  - includes ONLY the public facade headers   │
└───────────────────────┬─────────────────────┘
                        │  public facade (small, stable)
                        ▼
┌─────────────────────────────────────────────┐
│  AzulEngine  (DynamicLibrary / .dll)         │  ← everything else
│  PUBLIC headers:  World / ComponentPool /    │
│    System, component structs, handle enums   │
│    (MeshName, ShaderName, TextureName),      │
│    asset-registration calls, Camera          │
│  PRIVATE (not on public include path):       │
│    State* / Buffer* RHI, GraphicsObject*,    │
│    loaders, protobuf, resource managers      │
└─────────────────────────────────────────────┘
```

**What "plug and play" means concretely:** gameplay writes
`RenderComponent{ mesh=Cube, material=LightTextured, tex=Brick }` — an enum + handles. It never
sees `ActivateShader()`, which CBV register, or `TransferWorldViewProj()`. The DLL makes that a
*physical* wall: the internal headers simply are not on the public include path.

**Key caveats (decide/handle in Phase 0):**
- **Templates don't cross a DLL boundary cleanly.** So the split is: ECS core
  (`World`/`ComponentPool`/`System`) + component structs = **shared public headers** (both
  sides compile their own instantiations); RHI + loaders + system *implementations* = **private,
  compiled into the DLL**. This is fine and expected.
- **Keep the boundary API POD-ish** — pass handles, enums, and simple structs across it; avoid
  passing STL containers or ownership across the DLL line.
- **Export macro:** reuse the `__declspec(dllexport/import)` pattern already used by
  `Libs/Math` and `Libs/AnimTime`.
- **Lib fallback:** if DLL export friction bites, a static `AzulEngine.lib` gives the *same*
  public/private header discipline (enforced by not shipping internal headers) with no ABI
  headaches — you lose only the independent-rebuild/runtime-wall benefit. Either satisfies the
  "gameplay can't reach internals" goal.
- **The wall goes up in Phase 0; what hides behind it grows.** `State*`/`Buffer*`/loaders are
  already decoupled from gameplay, so they go private immediately. The **shader-selection**
  internals can only be fully hidden once `GraphicsObject_*` is deleted and `RenderComponent`
  handles are the only surface — that **completes in Phase 4**.

---

## 6. ECS core: hand-rolled registry (decided)

**Decision: roll our own** minimal sparse-set registry — no EnTT. Fits the course-engine style
and the coding standard (Section 0a).

Constraints from Section 0a shape the design:
- No STL, so component storage is **hand-managed `new[]` arrays** with explicit `count`/
  `capacity` (grow by reallocating a bigger raw array and copying — the old-fashioned way),
  mirroring how `Manager`/`DLink`/`PCSTree` already work in this codebase.
- **Templates are the one allowed exception**, used *only* for the per-component-type pool
  dispatch (`ComponentPool<C>`), because that is the inherently-generic part. Keep the template
  surface tiny; put any non-generic logic in `.cpp`. Add a comment at each template explaining
  why the exception applies.

Thin `World` API the rest of this plan assumes (the templated members are the exception):

```cpp
Entity            World::Create();
void              World::Destroy(Entity e);
template<class C> C&   World::Add(Entity e);      // returns ref into the pool array
template<class C> C*   World::TryGet(Entity e);
template<class C> bool World::Has(Entity e);
// iteration is done with an explicit, non-lambda loop over a pool's dense array +
// a Has<>() check for the other required components (no std::function, no views object)
```

Because of the DLL/template caveat (Section 5), the ECS core headers are **public and shared**
(both `AzulEngine.dll` and `Game.exe` compile their own instantiations).

---

## 7. The phases (execute in order)

### Phase 0 — Establish the engine module boundary (the DLL split)

> **Progress (2026-07-04):** Foundation DONE and build-verified.
> - **IMPORTANT: all `.sln`/`.vcxproj`/`.filters` are generated by `premake5.lua` and are
>   gitignored — never hand-edit or hand-create them. Add/modify projects in `premake5.lua`,
>   then regenerate with `UberBuildMe.bat` (runs `premake5 --ws=engine vs2022` and
>   `--ws=converter vs2022`).**
> - Added the `AzulEngine` `SharedLib` (DLL) project to `premake5.lua`; regeneration emits
>   `Engine/AzulEngine/AzulEngine.vcxproj` (kind SharedLib, links `Framework_items`,
>   defines `AZUL_ENGINE_LIBRARY_EXPORTS`/`AZUL_ENGINE_USE_DLL`).
> - Real source files (kept, not generated): `Engine/AzulEngine/include/EngineDLLInterface.h`
>   (`AZUL_ENGINE_LIBRARY_API` macro), `include/EngineInfo.h` + `src/EngineInfo.cpp` (first
>   exported symbol `Azul::GetAzulEngineVersion()` so the import lib is emitted),
>   `src/EngineDLLMain.cpp` (`DllMain`).
> - Wired the Engine app in premake: `links { "AzulEngine" }`, `defines AZUL_ENGINE_USE_DLL`,
>   `includedirs "Engine/AzulEngine/include"`. premake generates the app→DLL ProjectReference.
> - `UberBuildMe.bat` regenerates cleanly; full solution builds **0 errors** (also fixed the
>   long-standing `PugiXmlTest` "cannot open File.h" by adding File include/define/link to that
>   test in premake).
>
> **Increment 1 DONE (2026-07-04): `State*` RHI moved into `AzulEngine.dll`.**
> - Physically `git mv`'d all 30 `State*` files: headers → `Engine/AzulEngine/include/`,
>   sources → `Engine/AzulEngine/src/` (premake's directory globs handle membership; the app
>   still resolves `#include "StateXxx.h"` via its `..\AzulEngine\include` path).
> - Tagged all 15 `State*` classes with `AZUL_ENGINE_LIBRARY_API` (+ `#include
>   "EngineDLLInterface.h"` in each header).
> - Removed two cross-dependencies so the DLL doesn't reach back into the app:
>   (a) deleted an unused `#include "Engine.h"` from `StateDirectXMan.cpp`;
>   (b) refactored `StateRenderTargetView::Initialize()` to use the backbuffer texture directly
>   + `SafeRelease` instead of the `BufferTexture2D` wrapper (behaviorally identical).
> - premake: `AzulEngine` now `links { "Framework_items", "Math", "d3d11", "dxgi",
>   "d3dcompiler" }`, `includedirs Libs/Math/include`, `defines MATH_USE_DLL`.
> - Verified: 15 `State*.obj` in the DLL, 0 in the app; `StateDirectXMan::*` exported from
>   `AzulEngineDebugX64.dll`; full solution builds 0 errors.
> - **Not yet runtime-tested** (DirectX windowed app — needs a real run to confirm the RTV
>   refactor visually; the change follows the standard D3D11 ref-count pattern).
> - **Folder convention (premake `addTreeIncludes`):** `AzulEngine` lives at `Libs/AzulEngine`.
>   Headers sit **next to their `.cpp`** under `Libs/AzulEngine/src/<Feature>/` — there is **no
>   `include/` tree** (flattened 2026-07-06 for h-near-cpp navigation). `addTreeIncludes` auto-
>   adds every non-obj subfolder of `src/` as an include dir for BOTH the DLL and the app, so new
>   feature folders need **no premake edit** — drop files in and regenerate. The engine/game
>   boundary is enforced by **linking** (app imports the DLL's exported symbols), not by
>   include-path privacy.
> **Increment 2 DONE (2026-07-04): `SafeRelease` de-macroed, headers foldered, 18 `Buffer*`
> moved into the DLL.**
> - `SafeRelease` macro replaced by an inline template `Azul::SafeRelease<T>(T*&)` in
>   `include/SafeRelease.h` (the sanctioned generic-template exception; `StateDirectXMan.h`
>   includes it, so no call-site changes across the 40 uses).
> - Header folders now mirror `src/`: `include/State/` (15 State headers), `include/Buffer/`
>   (18 buffer headers), `include/ShaderMappings.h` (shared enum header, was in the app),
>   core headers (`EngineDLLInterface.h`, `EngineInfo.h`, `SafeRelease.h`) at `include/` root.
> - Moved **18 of 21** buffers into `include/Buffer/` + `src/Buffer/`, tagged with
>   `AZUL_ENGINE_LIBRARY_API`. Removed cross-deps: deleted misplaced
>   `assert(... == ShaderObject::GetLatestProfile_*())` debug asserts (+ the `ShaderObject.h`
>   includes) from 5 shader buffers, dropped unused `ShaderObject.h` from the 2 InputLayout
>   buffers and an unused `Mesh.h` from `BufferUAV_cs.h`.
> - **Deferred (3):** the texture cluster `BufferTexture2D`, `BufferSRV_ps`,
>   `BufferTextureSRV_ps` stay in the app for now — they pull `Libs/ProtoBuf/include/textureData.h`,
>   which drags the whole protobuf/abseil dependency. Move them **with `TextureObject`** in a
>   later increment that brings protobuf into the DLL.
> - Verified by clean `Rebuild`: 18 buffer objs in the DLL, 3 (texture cluster) in the app,
>   208 State+Buffer symbols exported, **0 errors**.
> **Increment 3 PARTIAL (2026-07-04): protobuf enabled in the DLL + texture buffers moved.**
> - `AzulEngine` now links the full protobuf stack + `File` + `Manager` (premake: ProtoBuf
>   includes/libs, `FILE_USE_DLL`, `PROTOBUF_USE_DLLS`/`ABSL_CONSUME_DLL`/
>   `PROTOBUF_ENGINE_USE_DLLS`; **`MANAGER_USE_DLL` intentionally NOT set**, same as the app).
> - Moved the deferred texture-buffer cluster (`BufferTexture2D`, `BufferSRV_ps`,
>   `BufferTextureSRV_ps`) into `include/Buffer` + `src/Buffer`. **All 21 buffers now in the DLL.**
>   Proved protobuf-in-DLL works (proto enum crosses the boundary fine). Build 0 errors.
>
> **BLOCKER found — the render/anim subsystems are one coupled cluster, not separable piecemeal:**
> - `Engine/src/StringThis.h` is a hub enum→string utility that `#include`s `Mesh.h`,
>   `ShaderObject.h`, `TextureObject.h`, `Camera.h`, `AnimMan.h`, `JointTable.h`,
>   `HierarchyTable.h`. It's used by 17 files (incl. `TexNodeMan`/`TextureObject::GetName`).
>   It can only compile inside the DLL once ALL those subsystems are in the DLL.
> - Also `ShaderObject` calls `Camera`, so shaders drag `Camera` in.
> - Attempted `TextureObject`/`TexNode`/`TexNodeMan` move → blocked on `StringThis` (needs the
>   not-yet-moved Mesh/Shader/Camera/Anim headers). Reverted that subsystem to stay green;
>   kept the texture buffers + protobuf enablement.
>
> **Decision needed before proceeding (Mesh/Shader/Texture/Camera/Anim):**
>   - **Option A — one big cluster move:** Mesh + ShaderObject(+`_*`) + Texture(+managers) +
>     Camera + Anim/Clip/Skeleton/Mixer/ComputeBlend + `StringThis` all at once. Resolves the
>     hub (everything in the DLL together) but ~80–100 files, high risk, long broken window,
>     and `GraphicsObject_*`/`GameObject`/`Glyph` (staying in app) will import a lot.
>   - **Option B — de-hub `StringThis` first:** give each subsystem its own enum→string so
>     `StringThis` stops including everything; then move subsystems incrementally like State/
>     Buffer. Cleaner + lower per-step risk, but an upfront refactor of `StringThis` + its
>     overloads and 17 consumers.
> - **Still no runtime test of any increment — run the app to confirm rendering.**
>
> **Increment 4 IN PROGRESS (2026-07-04): big cluster move — prep done, green.** Chosen approach
> = move Mesh + Shader + Texture + Camera + Anim(+Clip/Skeleton/Mixer/ComputeBlend/tables) +
> `StringThis` into the DLL together (they mutually reference via `StringThis`, so it's atomic
> at the header level). Decisions/prep completed and build-verified green:
> - **`AnimMan` stays in the app** — it's gameplay glue (creates `GameObjectAnimSkin`,
>   `GraphicsObject_SkinLightTexture`, `Prefab_Pivot`, calls `GameObjectMan::Add`). The pure
>   resources it uses (`Anim`/`Clip`/`ComputeBlend`/`Skeleton`) move to the DLL; `AnimMan`
>   imports them. `AnimManCompareStrategyEnumName` stays with it.
> - **De-hubbed `StringThis`**: removed `#include "AnimMan.h"` + the `StringThis(AnimMan::Name)`
>   overload; added explicit `Clip.h`/`Skel.h` includes (were transitive via AnimMan.h). The
>   `AnimMan::Name`→string map now lives in `AnimMan::NameToString` (app-side); `AnimMan.cpp`'s
>   5 call sites updated. `StringThis` now only depends on subsystems that are moving.
> - Removed unused back-deps: `Engine.h` from `Mesh.cpp`/`MeshProto.cpp`/`ShaderObject.cpp`,
>   `Prefab_Pivot.h` from `Skeleton.cpp`.
> **Boundary decided (user):** DLL = engine (rendering, anim, managers, Camera, Engine base);
> APP keeps the object model it authors with — `GameObject`(+subclasses), `GameObjectMan`,
> `GraphicsObject`(+`_*` variants), `Prefab*`, `AnimMan`, plus the game files (`main`, `Game`,
> `GameMan`, `GameHelper`, `GameSceneContext`, `GameSceneState`).
>
> **Increment 4 DONE (2026-07-04): render/anim cluster moved into the DLL — clean build 0 errors.**
> - Moved ~140 files / 66 tagged classes into `AzulEngine`: `Mesh` (include/src `Mesh/`),
>   `ShaderObject`(+all `_*`) (`Shader/`), `Texture`+`TexNode` (`Texture/`), `Camera`(+Man/
>   Node/Utility) (`Camera/`), and the whole anim primitive set (`Anim/`): `Anim`,
>   `AnimController(+One/Two)`, `Clip`(+Man/Proto), `Skeleton`/`Skel`(+Man/Proto), `Bone`,
>   `Mixer(A/B/C)`, `ComputeBlend(+One/Two)`, `WorldComputeA/C`+`WorldConstant`, `JointTable`
>   /`HierarchyTable`(+Man/Proto), `FrameBucket`, `TimerController`/`AnimTimer`, + all their
>   `*CompareStrategyEnumName`. `StringThis`, `Color`/`Colors` at `include/` root.
> - premake: added `AnimTime` (+`ANIM_TIME_USE_DLL`) and `Engine/shaders/Compiled` to the DLL.
> - **Validated: `Manager`-derived managers export across the DLL boundary fine** (no
>   MANAGER_USE_DLL, compiled locally in the DLL, app imports the static API). This was the
>   main unknown — it works.
> - Fixed a tagging bug: headers with **mixed-case include guards** (e.g.
>   `Tex_NODE_Compare_Strategy_Enum_Name_H`) missed the `EngineDLLInterface.h` insert; added it.
> - Result: DLL = 123 objs (State+Buffer+render/anim), app = 63 objs. Clean `Rebuild` 0 errors.
>
> **KNOWN FOLLOW-UP (robustness):** the HLSL shader compilation (`FxCompile`, model 5.0,
> `->Compiled/*.h`) is still configured on the **Engine app** project, but the `ShaderObject_*`
> that include those generated headers now live in the DLL. It builds today only because
> `Engine/shaders/Compiled/*.h` already exist on disk. On a **fresh checkout** the DLL would
> fail (headers not generated yet, and the DLL builds before the app). **Move the shader
> `FxCompile` build step to the `AzulEngine` project in premake.**
>
> **Remaining for the full option-B boundary:** move `GraphicsObject`(+`_*` material variants)
> and the `Engine` base class into the DLL, and the 2D set (`Glyph`/`GlyphMan`/`FontSprite`/
> `Image`/`ImageMan`/`Sprite`) if it's engine (check for `GameObject` back-deps first). Same
> recipe. **Still no runtime test — run the app to confirm rendering.**

> **Increment 5 DONE (2026-07-05): FULL engine/game split — clean Rebuild 0 errors.**
> User revised the boundary: **only user-authored files stay in the app** — `Game`, `GameMan`,
> `GameHelper`, scenes (`GameSceneContext`/`GameSceneState`, `scene1/2/3`), and the Prefab
> derivatives (`Prefab_Rotate/Pendulum/Pulse/FiboSpiral/RoatateSpin`, `PrefabAnim`). **Everything
> else is engine → `AzulEngine.dll`.**
> - Moved into the DLL (folders `GameObject/`, `Graphics/`, `UI/`, `Prefab/`, root):
>   `GameObject`(+all subclasses)+`GameObjectMan`, `GraphicsObject`(+`_Abstract`+all `_*`),
>   `AnimMan`(+compare), `Glyph`/`GlyphMan`/`GlyphProto`/`FontSprite`/`Image`/`ImageMan`,
>   `Prefab`/`Prefab_Abstract`/`Prefab_Pivot`, `CameraNodeMan`, `Engine` base, `Rect`,
>   `BoundingSphere`, `Ritter`, `WindowColors`.
> - **Decoupled render states from the app:** `GraphicsObject_*` used
>   `GameMan::GetGame()->mBlendState*/mStateRasterizer*` (app). Added `Engine::GetInstance()`
>   (static, set in the `Engine` ctor) and rerouted the 6 `GraphicsObject_*` to it — the states
>   live on the `Engine` base (now in the DLL), so no app dependency. Removed stale `Game.h`
>   includes from `GameObject*.cpp`.
> - **Moved HLSL shader compilation into `AzulEngine`** (premake `addShaderCompilation` helper) —
>   fixes the build-order deadlock (DLL builds before the app, and the `ShaderObject_*` need the
>   generated `*.Px.h/*.Cx.h`). Removed the shader `FxCompile` + `.hlsl` globs from the app.
>   Clean `Rebuild` now regenerates shader headers in the DLL first. **Known follow-up RESOLVED.**
> - **Fixed a real tagging bug: CRLF line endings** broke the `$` anchor for no-base
>   class/struct decls (e.g. `GameObjectMan`, `Rect`), so they weren't exported → app LNK2019.
>   Re-tagged all DLL headers with a CRLF-tolerant, base-optional pattern (also fixed plain
>   `struct`s). premake: added `PCSTree` (for `GameObject : PCSNode`).
> - **Result: DLL = 158 objs (the whole engine), app = 14 source files (the game). Clean
>   Rebuild 0 errors.** DLL 5.4 MB, Engine.exe 1.35 MB.
> - **STILL NOT RUNTIME-TESTED** — everything compiles/links; run the app to confirm rendering
>   (esp. the `Engine::GetInstance()` reroute and the earlier `StateRenderTargetView` refactor).

**Goal:** carve the monolithic `Engine.exe` into `AzulEngine` (DLL, all internals) + `Game`
(the app, gameplay only), with a small public facade. No behavior change, no ECS yet.

**Files/targets:** new `AzulEngine.vcxproj` (`ConfigurationType = DynamicLibrary`); the current
`Engine.vcxproj` (app) is trimmed to gameplay + `Game.cpp` and gains a `ProjectReference` to
`AzulEngine`; add an export macro header (`Engine/Core/EngineAPI.h`, mirroring Math/AnimTime);
create a public include folder (e.g. `Engine/Public/`) vs private (`Engine/Core/`).

**Steps:**
1. Create the `AzulEngine` DLL target. Move the **already-decoupled internals** into it:
   all `State*`, all `Buffer*`, the `Converter/*` loaders + protobuf glue, and the resource
   managers (`MeshNodeMan`, `TexNodeMan`, `ShaderObjectNodeMan`, `AnimMan`, `GlyphMan`,
   `ImageMan`, `JointTableMan`, `HierarchyTableMan`).
2. Decide public vs private headers. **Public (Phase 0):** handle enums (`Mesh::Name`,
   `ShaderObject::Name`, `TextureObject::Name`, `Camera`), asset-registration entry points,
   and the app-facing manager calls currently used by scene setup. **Private:** `State*`,
   `Buffer*`, loaders, protobuf. Put public headers on the app's include path, private ones
   only on the DLL's.
3. Add the `__declspec(dllexport/import)` macro (`AZUL_ENGINE_API`) and tag the public API.
4. Keep `GameObject`/`GraphicsObject_*` in whichever module compiles cleanest for now
   (likely still app-side or a shared layer) — they are *not* the boundary yet; Phase 4 moves
   render internals fully private.
5. Build both targets; run. Output must be byte-for-byte the same game.

**Exit criteria:** two build targets; app links `AzulEngine.dll`; app compiles **without** any
`#include` of `State*`/`Buffer*`/loader headers; game runs identically.

**Risk:** medium (build-system surgery, export macros). Mitigation: move files in small groups,
building after each group; start with `State*`/`Buffer*` (zero gameplay coupling).

---

### Phase 1 — Stand up the ECS core (groundwork, zero gameplay change)

> **DONE (2026-07-06): ECS core built + runtime-verified.** Files in `Libs/AzulEngine/
> {include,src}/ECS/`: `Entity.h` (index+generation handle), `IComponentPool.h/.cpp`
> (type-erased base), `ComponentPool.h` (template sparse-set: dense data + sparse index +
> swap-remove, hand-managed `new[]` arrays, no STL), `World.h/.cpp` (entity create/destroy
> with generation recycling + free list; templated `Add/TryGet/Has/Pool<C>`), `System.h/.cpp`
> (base with `Update(World&, AnimTime)`), `EcsSmokeTest.h/.cpp` (exported `ECS_SmokeTest()`).
> - **Component ids are explicit** (`struct C { static const unsigned int kTypeId; }`), NOT a
>   runtime/template counter — a counter's function-local static would differ between the DLL
>   and app modules and corrupt pool lookup. A compile-time constant is identical everywhere.
> - Template exception (per 0a) used only for `ComponentPool<C>`/`World::Pool<C>`; commented.
> - Builds into the DLL 0 errors. **Runtime-verified** via a standalone harness (compiled the
>   ECS `.cpp` + a test main with cl): all asserts pass incl. generation recycling + a
>   1000-entity add/remove stress. `ECS_SmokeTest()` is also callable from the running app.
> - **Next: Phase 2** — start the `GameObject` bridge (Transform component first).

**Goal:** a working registry (shared public headers) that coexists with the current engine,
proven by a throwaway test, touching no existing game logic.

**Files:** new `Engine/Public/ECS/Entity.h`, `ComponentPool.h`, `World.h`, `System.h`.

**Steps:**
1. Pick registry strategy (Section 6). If rolling your own: `Entity` = 32-bit id + generation;
   `ComponentPool<C>` = sparse-set (dense array + sparse index); `World` owns type-erased pools.
2. Implement the `World` API. Add `System` base (`virtual void Update(World&, AnimTime)`).
3. Write a temporary `ECS_SmokeTest()`: create 2 entities, add a dummy `TransformComponent`,
   iterate a view, print, verify, then remove the call.
4. Capture a **baseline profile** of a representative scene now (for Phase 6 comparison).

**Exit criteria:** compiles/runs as before; smoke test correct; ECS core is header-only and
included by both modules.

**Risk:** very low (additive only).

---

### Phase 2 — Move data into components (the bridge)

> **IN PROGRESS (2026-07-06): Transform bridge landed — builds 0 errors.**
> - New: `Component/ComponentId.h` (central id enum, `COMPONENT_TRANSFORM=0`),
>   `Component/TransformComponent.h` (`Vec3 pos; Quat rot; Vec3 scale; Mat4 world`; only
>   `world` wired so far), `ECS/WorldMan.h/.cpp` (engine-wide `World` singleton, lazy).
> - `GameObject`: replaced `Mat4 *poWorld` with an `Entity mEntity`. Ctor does
>   `mEntity = WorldMan::GetWorld().Create()` + `Add<TransformComponent>` (world=Identity);
>   dtor `Destroy(mEntity)`. `GetWorld()/SetWorld()` now read/write the component (same
>   `Mat4*` contract). Subclasses (`RigidBody`/`AnimSkin`/`Sprite`/`FontSprite`) switched from
>   the removed protected `poWorld` to `GetWorld()`.
> - Bridge is entirely engine-internal (GameObject is in the DLL); the app is untouched.
> - Verified: DLL builds 0 errors; standalone runtime check confirms the pool keeps 16-byte
>   alignment through swap-remove (so SIMD `Vec3`/`Mat4` components are safe). **Full in-app
>   render test still needed** (run `Engine.exe`).
> **pos/rot/scale migration DONE (2026-07-06): builds 0 errors.**
> - Added public `GameObject::GetTransform()` → the entity's `TransformComponent&`;
>   `GetWorld`/`SetWorld` now route through it.
> - `GameObjectRigidBody` + `GameObjectAnimSkin`: removed the heap `poScale`/`poQuat`/`poTrans`
>   (`Vec3*`/`Quat*`) members. Ctors seed `TransformComponent.pos/scale/rot`; `Set*` methods and
>   `Update`/`privUpdate` read/write `GetTransform().pos/rot/scale`; dtors no longer delete them.
>   (Bonus: 3 fewer heap allocations per object, and they're off the leak surface.)
> - `Prefab::SetData(RigidBody&|AnimSkin&)` and `Prefab(RigidBody*)` now read via
>   `GetTransform()` instead of the (removed) friend members. `Prefab` keeps its OWN
>   `poScale/poQuat/poTrans` working copies — those become behavior-component data in Phase 3.
> - **Fixed leaks:** `WorldMan` is now lazy-create + explicit `Destroy()`, called in each scene's
>   unload (after `GameObjectMan::Destroy()`), so `~World` frees the pools/arrays BEFORE the
>   framework leak check. (The reported leaks were the World still alive at check time.)
> **RenderComponent DONE (2026-07-06): builds 0 errors.**
> - `RenderComponent { GraphicsObject* pGraphicsObject; bool drawEnable; }` (id
>   `COMPONENT_RENDER`). Bridge form — still holds the whole `GraphicsObject` bundle; Phase 4
>   decomposes it into mesh/shader/tex handles + a RenderSystem.
> - `GameObject`: removed the `poGraphicsObject` and `mDrawEnable` members. Added protected
>   `GetRender()`; ctor `Add<RenderComponent>`, `GetGraphicsObject`/`Draw`/`DrawEnable`/
>   `DrawDisable` route through it; dtor deletes the GraphicsObject (still GameObject-owned) then
>   destroys the entity. Subclasses (`RigidBody`/`AnimSkin`/`Sprite`/`FontSprite`) use
>   `GetGraphicsObject()` instead of the removed member.
> - **`GameObject` is now a pure shim: its only data member is `Entity mEntity`** — transform +
>   render both live in ECS components.
> **HierarchyComponent DONE (2026-07-07): builds 0 errors.**
> - `HierarchyComponent { Entity parent }` (id `COMPONENT_HIERARCHY`). Added `EntityNull()`/
>   `EntityIsNull()` helpers to `Entity.h`.
> - `GameObject` ctor `Add`s it (parent = `EntityNull()`); new public `GetEntity()` +
>   `SetParent(GameObject*)` and protected `GetHierarchy()`. `GameObjectMan::Add` now calls
>   `pObj->SetParent(pParent)` after the PCS `Insert`, mirroring the parent into the component.
> - **Bridge only:** the `PCSTree` stays authoritative for Update/Draw traversal; the component
>   just records the parent. Phase 5 makes it authoritative + adds `TransformPropagationSystem`.
> - `GameObject` now carries 3 components (Transform, Render, Hierarchy); its only member is
>   still `Entity mEntity`.
> **LightComponent DONE (2026-07-07): builds 0 errors. → PHASE 2 COMPLETE.**
> - `LightComponent { Vec3 color; Vec3 pos }` (id `COMPONENT_LIGHT`).
> - `GraphicsObject_LightTexture`: removed heap `poLightColor`/`poLightPos`; added an
>   `Entity mLightEntity` it owns. Both ctors call `privInitLight` (create light entity +
>   `Add<LightComponent>`); `SetDataGPU` reads the component and `TransferPos/Color(&light.*)`;
>   dtor `Destroy`s the light entity (which is deleted with its owning GameObject during scene
>   unload, before `WorldMan::Destroy` — no leak).
> - **Note:** `GraphicsObject_SkinLightTexture` / `_ConstColorLight` still hold their own light
>   fields — same pattern applies; migrate them when convenient (or fold into Phase 4's
>   RenderSystem).
>
> **PHASE 2 SUMMARY:** GameObject data is fully componentized — `TransformComponent` (pos/rot/
> scale/world), `RenderComponent` (GraphicsObject + drawEnable), `HierarchyComponent` (parent),
> and `LightComponent` (on light-textured materials). `GameObject` is a thin shim over
> `Entity mEntity`. All bridges keep the old OOP behavior; PCSTree still drives traversal.
> **Next: Phase 3 — turn `Update()`/`Prefab_*` into systems over the component pools.**
>
> **Build note:** the generated `Engine.sln` keeps disappearing between builds (it's a
> gitignored artifact) — regenerate with `premake5 --ws=engine vs2022` (or `UberBuildMe.bat`)
> before building if msbuild reports `MSB1009: Project file does not exist`.

**Goal:** relocate object *data* into components while keeping `GameObject` as a thin
forwarding shim so nothing downstream breaks.

**Files:** new `Engine/Public/Component/*` (`TransformComponent.h`, `RenderComponent.h`,
`HierarchyComponent.h`, `LightComponent.h`); edits to `GameObject.*`,
`GameObjectRigidBody.*`, `GameObjectAnimSkin.*`.

**Steps:**
1. Add `Entity id;` to `GameObject`; on construct, `id = World::Create()` +
   `World::Add<TransformComponent>` seeded from `poWorld`.
2. **Transform first.** Make `GetWorld()/SetWorld()` read/write the component; migrate
   `poTrans/poQuat/poScale` on RigidBody and AnimSkin the same way — one field at a time.
3. Add `RenderComponent` seeded from `poGraphicsObject` (mesh/shader/tex handles +
   `MaterialKind` from the concrete type). Keep `poGraphicsObject` alive; Phase 4 removes it.
4. Lift `_LightTexture`'s `poLightColor/poLightPos` into a `LightComponent`; `_LightTexture`
   reads from it. Output stays identical.
5. Mirror the `PCSTree` parent/child into `HierarchyComponent{ parent }` on
   `GameObjectMan::Add`. Tree still authoritative this phase.

**Exit criteria:** identical output; transform/render/light reads all go through components;
`GameObject` owns no raw transform data.

**Risk:** low–medium. Mitigation: migrate one field at a time, compile+run after each.

---

### Phase 3 — Turn Update() into systems (behaviors + animation)

> **IN PROGRESS (2026-07-07): system infrastructure + LocalToWorldSystem landed. Builds 0 errors.**
> - `System/SystemMan.h/.cpp` — ordered list of `System*`, `Create/Destroy/Add/Run`. Owns the
>   systems; `Destroy()` frees them. Created/torn down in `GameObjectMan::Create/Destroy` (so
>   before `WorldMan::Destroy` in scene unload — leak-safe).
> - `System/LocalToWorldSystem.h/.cpp` — iterates the `TransformComponent` **dense pool** in one
>   flat loop (no virtual dispatch, no tree walk) and computes `world = Scale*Rot*Trans`.
> - `GameObjectMan::Update` now runs `SystemMan::Run(WorldMan::GetWorld(), t)` **before** the PCS
>   tree walk. Plain objects get their world from the system; behavior objects (prefab-driven,
>   skinned) still overwrite it in their own `Update`.
> - `GameObjectRigidBody`: removed the base-world else-branch (system does it) and the
>   `setorupdate` flag. **Behavior caveat:** prefab objects now run their prefab from frame 1
>   (was frame-2+); visually negligible. Confirm at runtime.
> **ID SPLIT SETTLED + RotateSystem (2026-07-07): builds 0 errors.**
> - `ComponentId.h` now reserves **engine ids `[0, GAME_COMPONENT_BASE=32)`** and **game ids
>   `[32, 64)`** (World::MAX_COMPONENT_TYPES). `static_assert` guards the engine range. Game
>   components are defined app-side in `Engine/src/GameComponentId.h` (`COMPONENT_ROTATE=32`) so
>   the app adds behaviors without editing the engine, and ids never collide.
> - **App-authored behavior (proves the pattern across the DLL boundary):**
>   `Engine/src/RotateComponent.h` (`angle`,`speed`) + `RotateSystem.h/.cpp` (subclasses the
>   engine's exported `System`; `world = Scale*baseRot*spinZ*Trans`). Registered from each scene
>   via `SystemMan::Add(new RotateSystem())` after `GameObjectMan::Create`. Verified: an
>   app-defined component (id 32) is stored in the DLL's `World`, and the DLL's `SystemMan`
>   virtual-dispatches into the app's `RotateSystem`. Works with the shared /MD runtime.
> - **Reality check:** `Prefab_Rotate` (and `_Pendulum/_Pulse/_FiboSpiral/_RotateSpin`,
>   `PrefabAnim`) are **DEAD CODE** — never instantiated. Only `Prefab_Pivot` is live (AnimMan on
>   skinned objects). So `RotateSystem` is the ready go-forward pattern.
> - **LIVE DEMO — `scene4` (press `4`), RUNTIME-VERIFIED (2026-07-08).** `Engine/src/scene4.h/.cpp`
>   + wired into `GameSceneContext` (enum/ctor/dtor/SetState) and `Game.cpp` (the `4` key). Loads a
>   single `GameObjectRigidBody` `CUBE` (Ward texture, FlatTexture shader, uniform scale 100) with a
>   `RotateComponent` attached (`WorldMan::GetWorld().Add<RotateComponent>(pSpinner->GetEntity())`).
>   Its spin comes **purely from the ECS** (RotateComponent data + RotateSystem logic; no Prefab, no
>   per-object Update). Confirmed spinning in-app. (Terrain/SkyBox scaffolding removed — cube only.)
> - **DEAD `Prefab_*` derivatives DELETED (2026-07-08): builds 0 errors.** Removed all 12 files:
>   `Prefab_Rotate`, `Prefab_RoatateSpin`, `Prefab_Pulse`, `Prefab_Pendulum`, `Prefab_FiboSpiral`,
>   `PrefabAnim` (.h+.cpp). Confirmed repo-wide none were ever `new`'d (only self-includes). Base
>   `Prefab`/`Prefab_Abstract`/`Prefab_Pivot` stay — `Prefab_Pivot` is the one live one (AnimMan on
>   skinned objects). premake regenerates via dir globs (no edit needed).
> - **Reality check on the remaining "live" pieces (2026-07-08 investigation):**
>   - **Input path is DEAD:** `GameObjectControlled` is never instantiated (only in class diagrams);
>     no `InputSystem` conversion needed for it. (`CameraMan::ProcessInput` still drives the camera,
>     but that's a service, not per-object Update — leave as-is / revisit as a `CameraComponent`.)
>   - **Sprite path is NOT a clean `LocalToWorldSystem` fit:** plain `GameObjectSprite` is never used
>     directly — only `FontSprite`. And `FontSprite::Draw()` reuses `GameObjectSprite::privUpdate` to
>     recompute the world **per-glyph** in a single Draw (one entity, N glyph positions). The
>     one-world-per-entity system model can't express that, so its S*R*T is not truly redundant.
>     Leave FontSprite imperative for now (revisit as instanced glyph draw in Phase 4/6).
> - **Next (the one genuinely-live Update piece left): `GameObjectAnimSkin` spin+blend**
>   (`AnimController_*`/`ComputeBlend` → `AnimationSystem`/`BlendSystem`/`SkinningSystem`) — the big
>   remaining Phase 3 conversion. Created via `AnimMan` on skinned objects (uses `Prefab_Pivot`).
>
> **ANIMATION — full flow mapped (2026-07-08), then Increment A landed.**
> The live skinned-anim path (scenes 1 & 2 dancers) runs each frame as:
>   1. `AnimMan::BlendAnimation(tDelta)` — SPACE-key ramps a blend Ts → the two-anim controller.
>   2. `AnimMan::Update(tDelta)` — walks its DLink list, `AnimController::Update(tCurr)` each:
>      OneAnim = timer advance + `pBlend->AnimateMixerA(clip,t)` (CPU sample clip → mixer key bufs);
>      TwoAnim = two timers + two clips + blend ratio.
>   3. `GameObjectMan::Update(tCurr)` → SystemMan::Run, then tree walk → `GameObjectAnimSkin::Update`:
>      world matrix (Prefab_Pivot spin or privUpdate S*Q*Rx*Ry*Rz*T) + `SetWorld` + `pBlend->Execute()`
>      (GPU dispatch: MixerA interpolate → WorldComputeA compose bone-world → copy UAV→SRV).
>   `AnimMan` owns the AnimControllers (drives them via its own list); `GameObjectAnimSkin` owns the
>   `ComputeBlend` and called `Execute()`. `ComputeBlend::Execute` composes bones in MODEL space; the
>   object world matrix is applied later in the vertex shader, so Execute is independent of the
>   transform systems.
> **Increment A DONE (2026-07-08): `SkinningSystem` — builds 0 errors.**
> - New `Component/GpuSkinComponent.h` (`COMPONENT_GPU_SKIN`; non-owning `ComputeBlend* pBlend`),
>   `System/SkinningSystem.h/.cpp` (iterates the GpuSkin pool, calls `pBlend->Execute()`).
> - Registered in `GameObjectMan::Create` after `LocalToWorldSystem` (no-op in scenes with no skins).
> - `GameObjectAnimSkin` ctor now `Add`s a `GpuSkinComponent{pBlend}`; removed the `pBlend->Execute()`
>   from its `Update`. Ordering preserved: `AnimMan::Update` (sample) still precedes
>   `GameObjectMan::Update` (where SystemMan::Run now dispatches skinning). Handle is non-owning —
>   the `AnimController` still deletes the `ComputeBlend`; entity destroy just drops the component.
> - **NEEDS RUNTIME TEST: press 1 or 2 — the dancers must still animate identically.**
> **Increment B DONE (2026-07-08): `AnimationSystem` (one-anim path) — builds 0 errors.**
> - New `Component/AnimClipComponent.h` (`COMPONENT_ANIM_CLIP`; non-owning `AnimController*`),
>   `System/AnimationSystem.h/.cpp` (iterates the pool, `pController->Update(tDelta)`).
> - **CRITICAL time-semantics fix:** the controllers' `TimerController::Update` ACCUMULATES
>   (`tCurr += tDelta`), and scenes drove `AnimMan::Update(tDelta)`. But `SystemMan::Run` was passing
>   the ABSOLUTE `tCurr`. Fixed by threading the frame **delta** through the system runner:
>   `GameObjectMan::Update(tCurr, tDelta)` now calls `SystemMan::Run(world, tDelta)` (tree walk still
>   gets absolute tCurr). No existing system read the arg meaningfully (Rotate uses a fixed increment;
>   LocalToWorld/Skinning ignore it), so the semantic switch is safe. Updated all 4 scene call sites.
> - `AnimMan::Add` (both one-anim variants) parks the AnimMan-owned controller on a **dedicated
>   per-controller entity** via a non-owning `AnimClipComponent` (dedicated, NOT the skin entity, so a
>   controller driving several skin meshes is still sampled exactly once/frame). Bare entities are
>   freed wholesale at `WorldMan::Destroy`.
> - `AnimMan::Update` now drives ONLY `poBlendTwoAnimController` (the one-anim list walk is gone --
>   the system does it). Ownership unchanged: `AnimNode::privClear` still deletes the controller;
>   the component just holds a non-owning handle (same shape as Increment A).
> - System order in `GameObjectMan::Create`: LocalToWorld → **Animation (sample)** → Skinning
>   (dispatch). Two-anim is still sampled by `AnimMan::Update` before `GameObjectMan::Update`.
> - **NEEDS RUNTIME TEST: press 1 (one-anim dancers animate) and 2 (Dance/Gangnam one-anim + Blend
>   two-anim; SPACE still blends). All must look identical to before.**
> - (Also fixed a stray `this->_m14` prefix that had corrupted line 1 of `scene1.cpp`.)
> **ALL ECS COMPONENTS + SYSTEMS NOW LIVE IN THE DLL (2026-07-08): builds 0 errors.**
> User directive: every component and system belongs in `AzulEngine.dll`; the app (`Engine/src`)
> keeps only scenes + game setup. `RotateComponent`/`RotateSystem`/`GameComponentId.h` were the only
> ECS files still app-side (the earlier "prove a game-defined component crosses the boundary" demo).
> - `git mv` `RotateComponent.h` → DLL `Component/`, `RotateSystem.h/.cpp` → DLL `System/`.
> - `RotateComponent` is now an **engine** component: added `COMPONENT_ROTATE` to the engine
>   `ComponentId.h` enum; deleted app-side `GameComponentId.h` (`GAME_COMPONENT_BASE`/[32,64) stays
>   reserved as an extension point, currently unused).
> - `RotateSystem` tagged `AZUL_ENGINE_LIBRARY_API` (+ `EngineDLLInterface.h`) so the exported class
>   links across the boundary.
> - **Registration centralized:** `RotateSystem` is now added in `GameObjectMan::Create` after
>   `LocalToWorldSystem` (order: LocalToWorld → Rotate → Animation → Skinning). Removed the per-scene
>   `SystemMan::Add(new RotateSystem())` + the `SystemMan.h`/`RotateSystem.h` includes from all 4
>   scenes. The app now references NO system/component machinery -- scene4 only `Add`s the
>   `RotateComponent` *data*. Verified: `RotateSystem.obj` builds into `AzulEngine`, not the app.
>
> **Increment C DONE (2026-07-08): `BlendSystem` (two-anim path) — builds 0 errors.**
> - New `Component/AnimBlendComponent.h` (`COMPONENT_ANIM_BLEND`; non-owning `AnimController*`),
>   `System/BlendSystem.h/.cpp` (iterates the pool, `pController->Update(tDelta)`). Same non-owning
>   handle shape as A/B.
> - `AnimMan::Add` (two-anim variant) parks the AnimMan-owned two-anim controller on a dedicated
>   entity via `AnimBlendComponent`. The blend RATIO is still pushed by `AnimMan::BlendAnimation`
>   (SPACE-key ramp -> `pTwoController->SetBlendTs`); the component only drives per-frame sampling.
> - `GameObjectMan::Create` order: LocalToWorld -> Rotate -> Animation -> **Blend** -> Skinning
>   (both anim samplers run before the skinning dispatch).
> - **`AnimMan::Update` fully DELETED (2026-07-08): builds 0 errors.** Removed the method + its
>   declaration in `AnimMan.h` + both scene call sites (scene1/scene2). Blend-ratio input still flows
>   through `AnimMan::BlendAnimation` (unchanged).
> - **NEEDS RUNTIME TEST: press 2 -- Dance/Gangnam (one-anim) + Blend (two-anim); SPACE must still
>   ramp the blend between the two clips exactly as before.**
>
> **PHASE 3 essentially COMPLETE for all LIVE update logic:** behavior (RotateSystem), one-anim
> (AnimationSystem), two-anim (BlendSystem), GPU skinning (SkinningSystem), plain world
> (LocalToWorldSystem) are all data-driven systems over component pools. No `Prefab_*` or
> `AnimController_*` logic runs from any object `Update()`; `AnimMan::Update` is retired.
> **CONTROLLER-DELETION cleanup DONE (2026-07-08): `AnimController`/`_OneAnim`/`_TwoAnim` deleted --
> builds 0 errors.**
> - `git rm` all 6 controller files. Logic moved INTO the systems; data + ownership relocated:
>   - **Components now carry data** (non-owning views): `AnimClipComponent { Anim* pAnim; TimerController*
>     pTimer; ComputeBlend_OneAnim* pBlend; float ratio; }`, `AnimBlendComponent { Anim* pAnimA;
>     TimerController* pTimerA; float ratioA; ...B...; ComputeBlend_TwoAnim* pBlend; }`.
>   - **`AnimationSystem`/`BlendSystem` inline the sample logic** (timer->Update(ratio*dt) +
>     AnimateMixerA[/B/C]) -- exactly the old `AnimController_*::Update` bodies.
>   - **`AnimNode` is now the RAII owner** of `pAnimA/pAnimB/pTimerA/pTimerB/pComputeBlend`
>     (`privClear` deletes them; `ComputeBlend` base ptr has a virtual dtor). Timers are built in
>     `AnimMan::Add` from `Anim::FindMaxTime()` (was done in the controller ctor).
>   - `AnimMan::poBlendTwoAnimController` (controller*) -> `poBlendComputeBlend` (`ComputeBlend_TwoAnim*`)
>     just for `BlendAnimation`'s SPACE-key `SetBlendTs`.
>   - Removed dead `AnimMan::Find` (returned `AnimController*`) and dead `AnimMan::SetBlendTs(Name,float)`
>     + `AnimNode::GetController`.
> - **Lifetime preserved exactly:** privClear deletes the same objects the controller dtors did, at the
>   same time (AnimMan::Destroy, before GameObjectMan/WorldMan::Destroy). No new leak/double-free.
> - **NEEDS RUNTIME TEST: press 1 (one-anim dancers) + 2 (Dance/Gangnam + Blend, SPACE ramps) -- must
>   look identical AND clean-exit with no leak-check failures on scene switch/exit.**
>
> **PHASE 3 COMPLETE.** All motion/behavior/animation is data-driven systems over component pools; no
> `Prefab_*`, `AnimController_*`, or `AnimMan::Update` logic remains. Next: **Phase 4 (RenderSystem)**.

**Goal:** replace per-object virtual `Update()`, `Prefab_*`, and the animation controllers with
data-driven systems.

**Files:** new `Engine/.../System/*` (`RotateSystem`, `PendulumSystem`, `PulseSystem`,
`RotateSpinSystem`, `FiboSpiralSystem`, `PivotSystem`, `InputSystem`, `AnimationSystem`,
`BlendSystem`, `SkinningSystem`); behavior/anim components; edits to `GameObjectMan::Update`.

**Steps:**
1. Start simplest: `Prefab_Rotate::DerivedUpdate()` → `RotateComponent` + `RotateSystem`
   over `ViewOf<RotateComponent, TransformComponent>()`. Repeat per behavior, one per commit.
2. Convert `GameObjectControlled`/`CameraMan::ProcessInput` → `InputControlled` + `InputSystem`.
3. Convert animation: `AnimController_OneAnim` → `AnimationSystem`; `AnimController_TwoAnim`
   + `Mixer*` → `BlendSystem`; `ComputeBlend_*` dispatch → `SkinningSystem`. Components hold
   the `ComputeBlend*`/skeleton handles; systems just drive them.
4. Change `GameObjectMan::Update(t)` to run the ordered system list (Section 3c). Leave the
   tree walk only for not-yet-migrated objects.
5. When the last behavior/anim is migrated, `GameObject::Update()` becomes empty/removable.

**Exit criteria:** all motion/behavior/animation driven by systems over pools; no `Prefab_*` or
`AnimController_*` logic runs from object `Update()`; visuals unchanged.

**Risk:** medium. Mitigation: keep old classes compiled but unused until each system is
verified, then delete.

---

### Phase 4 — Replace Draw() with a RenderSystem, and lock down the render facade  ⚠ highest risk

> **SCOPING (2026-07-08): render path fully surveyed. Key findings below.**
>
> **Current draw path:** `GameObjectMan::Draw()` walks the PCS tree (DFS) → `GameObject::Draw()`
> → if `RenderComponent.drawEnable`, `pGraphicsObject->Render()` → `SetState(); SetDataGPU(); Draw();
> RestoreState()`. `RenderComponent` already exists (Phase 2) but still just wraps `GraphicsObject*`.
> Each `GraphicsObject_*` owns its mesh/shader/tex/material data and hard-codes camera + state.
>
> **Only 4 of the 10 `GraphicsObject_*` are LIVE (instantiated):**
> | Material | Where | Shader | Camera | State set / restore | Per-object transfers | Draw |
> |---|---|---|---|---|---|---|
> | `_Null` | root (GameObjectMan) | NullShader | — | none | none | **no-op** (skip) |
> | `_FlatTexture` | scene3/4 | FlatTexture | 3D persp | tex activate; if alpha→BlendAlpha / restore BlendOff | WVP, UVMatrix | RenderIndexBuffer |
> | `_Sprite` | scene1/2 (FontSprite+sprites) | Sprite | 2D ortho | tex activate; if alpha→BlendAlpha / restore BlendOff | WVP(orig*world*transLL), UV, ColorScale | RenderIndexBuffer |
> | `_SkinLightTexture` | scene1/2 (AnimMan) | SkinLightTexture | 3D persp | tex activate; RasterizerSolidCull / restore BlendOff | WVP, LightPos, LightColor, **BindWorldBoneArray** | ActivateSRVBuffers + RenderIndexBuffer |
>
> **DEAD (never `new`'d) -- delete first, like the Prefab cleanup:** `_LightTexture`,
> `_SkinFlatTexture`, `_ConstColor`, `_ConstColorLight`, `_Wireframe`, `_ColorByVertex` (6 files pairs).
> (`_LightTexture` is the only current `LightComponent` consumer -- deleting it may orphan
> `LightComponent`; keep the component, it's harmless, or revisit.)
>
> **The hard problems (design decisions for the increments):**
> 1. **Sprites/FontSprite do NOT fit one-entity-one-draw.** `FontSprite::Draw()` loops glyphs,
>    mutating the screen rect + color and calling `Render()` **N times per entity** (one per
>    character). A `ViewOf<Render,Transform>` batched loop can't express that. DECISION (locked):
>    a dedicated `SpriteRenderSystem` owns the 2D/UI (sprite+font) pass with its own per-glyph inner
>    loop; the batched `RenderSystem` owns only the 3D materials. 2D pass runs AFTER the 3D pass.
> 2. **Draw ORDER / transparency.** Opaque 3D must draw before alpha-blended UI. Today the PCS-tree
>    DFS order guarantees it. A RenderSystem iterating the RenderComponent **pool** uses insertion
>    order, which is NOT guaranteed to match, and sorting-by-material reorders further -- risking
>    wrong UI layering. Must explicitly order: opaque 3D pass first, then the 2D/UI pass.
> 3. **Skinned draw consumes SkinningSystem output** (`BindWorldBoneArray`) -- RenderSystem runs in
>    the Draw phase (after Update), so ordering is fine, but the component must carry the
>    `ComputeBlend*` handle (already have `GpuSkinComponent`).
> 4. **Camera per material** (3D persp vs 2D ortho) -- RenderSystem picks camera by MaterialKind.
> 5. **`RenderComponent` decomposition** -- from `GraphicsObject*` to
>    `{ MaterialKind; MeshName; ShaderName; TexName; + params (uvMatrix / light / colorScale) }`.
>    The sprite params (per-glyph screen rect) are the messy bit -> reinforces keeping 2D separate.
> 6. **Perf reality (Section 2):** batching gains are marginal at hundreds of objects; the REAL
>    Phase 4 win is encapsulation -- retire `GraphicsObject_*`, hide shader/RHI selection private
>    behind the DLL. Don't over-invest in the sort.
>
> **DECISIONS LOCKED (user, 2026-07-08):** (1) Sprite path -> **build a `SpriteRenderSystem`** (fully
> ECS the 2D/UI path, retire `GraphicsObject_Sprite`), run strictly AFTER the 3D pass. (2) Verification
> bar -> **visual confirmation** per scene (no pixel-diff harness). (3) P4.0 approved + DONE.
>
> **PROPOSED INCREMENT SEQUENCE (each builds + runtime-verified + committed before the next):**
> - **P4.0 DONE (2026-07-08): deleted the 6 dead `GraphicsObject_*`** (`_LightTexture`,
>   `_SkinFlatTexture`, `_ConstColor`, `_ConstColorLight`, `_Wireframe`, `_ColorByVertex`; 12 files).
>   Also removed the 4 dead-variant includes from `GameObject.h` (it only needs base `GraphicsObject*`).
>   `LightComponent` kept (its comment updated). Builds 0 errors; no behavior change. **4 live
>   materials remain: `_Null`, `_FlatTexture`, `_Sprite`, `_SkinLightTexture`.**
> - **P4.1 DONE (2026-07-08): RenderSystem shell, opaque-3D behind a toggle -- builds 0 errors.**
>   - New `Component/MaterialKind.h` (`enum class MaterialKind { Null, FlatTexture, Sprite,
>     SkinLightTexture }` + `MaterialKindIs3D`). `RenderComponent` gained a `MaterialKind kind` field.
>   - `GraphicsObject` got a pure-virtual `GetMaterialKind()`; the 4 live variants return their kind
>     (inline). `GameObject` ctor seeds `render.kind = pGraphicsObject->GetMaterialKind()`.
>   - New `System/RenderSystem.h/.cpp` -- draw-phase class (NOT a SystemMan/Update system); `Draw(World&)`
>     iterates the RenderComponent pool and `Render()`s the 3D kinds. Still calls the existing
>     `GraphicsObject->Render()` (decomposition is P4.2).
>   - `GameObjectMan`: `static bool sUseECSRender` (+ Get/Set), **defaults TRUE**. `Draw()` runs
>     `RenderSystem::Draw` first (3D opaque), then the tree walk. `GameObject::Draw` skips 3D kinds when
>     the toggle is on (so no double-draw); the tree walk still draws Sprite/FontSprite (2D) -- incl.
>     FontSprite's per-glyph `Draw` override. Net: 3D drawn from the pool, 2D on top from the tree ->
>     natural opaque-then-UI layering. Verified only base `GameObject::Draw` + `FontSprite::Draw` exist
>     (no 3D subclass overrides Draw), so the skip is safe.
>   - **NEEDS RUNTIME TEST (toggle defaults ON): press 1/2 (dancers + fonts), 3 (terrain+skybox),
>     4 (cube) -- all identical to before. To A/B compare, set `GameObjectMan::sUseECSRender=false`
>     + rebuild for the original tree-walk path.**
>
> **MATERIALS restored as first-class MaterialKinds (2026-07-08, scene4 demo/test):** ColorByVertex,
> ConstColorLight, and Wireframe brought back (recovered from commit `af302b3`) with `GetMaterialKind()`
> overrides + `MaterialKind` entries (all 3D). **Wired the `ConstColorLight` SHADER** into the pipeline
> (it existed as `ShaderObject_ConstColorLight` + compiled HLSL but was never in the enum): added
> `ShaderObject::Name::ConstColorLight` + factory case + `StringThis` case. Wireframe reuses the
> `ConstColor` shader + wire rasterizer. scene4 now shows 3 equally-spaced spinning cubes
> (ColorByVertex | ConstColorLight | Wireframe), each a 3D kind drawn by the RenderSystem -- proving
> P4.1 handles kinds that didn't exist when it was written. This answers the P4.2 "carry
> Wireframe/ColorByVertex forward?" question: YES, first-class kinds now.
> GRADIENT CUBE DONE: `CubeMesh.m.proto.azul` has NO vertex colors (ColorByVertex rendered it black),
> so added `Mesh/MeshCubeColor` -- a procedural unit cube with a unique RGBA per corner (8 verts, 36
> CCW-from-outside indices to match the engine's CULL_FRONT winding), registered under a new
> `Mesh::Name::CUBE_COLOR` (+ StringThis case) via the existing `MeshNodeMan::Add(Name, Mesh*)`
> overload. scene4's left cube uses it -> interpolated gradient faces. First code-defined (non-proto)
> mesh in the engine.
> - **P4.2 IN PROGRESS -- decompose the 3D materials into RenderSystem branches.** Approach: move each
>   material's `SetState/SetDataGPU/Draw/RestoreState` into a `RenderSystem::privDraw*` branch ONE at a
>   time; read mesh/shader via `GraphicsObject::GetMesh()/GetShader()` (added `GetShader`) + concrete
>   params via a cast to `GraphicsObject_*` (public members), and the world from the entity's
>   `TransformComponent` (not `poWorld`). Un-migrated kinds fall back to `pGraphicsObject->Render()`.
>   Data-in-component + `GraphicsObject_*` deletion is the FINAL consolidated step (after all branches
>   verified). Verify visually per scene.
>   - **DONE (2026-07-08, builds 0 errors): ColorByVertex, FlatTexture, ConstColorLight, Wireframe**
>     branches. `RenderSystem::Draw` switches on `MaterialKind`; each priv* is a faithful copy of the
>     old GraphicsObject_* body. **NEEDS RUNTIME TEST: scene3 (FlatTexture terrain+skybox) + scene4
>     (the 3 cubes) render identically.** scenes 1/2 (SkinLightTexture) still bridge -> unchanged.
>   - **DRAW-ORDER BUG found + fixed (2026-07-08): skybox rendered translucent under the ECS path.**
>     Root cause = hard-problem #2: the RenderSystem iterated the RenderComponent **pool** (creation
>     order: terrain->skybox), but the **PCS tree inserts children at the FRONT**, so the tree DFS
>     order is skybox->terrain (reverse). The alpha-blended skybox (a backdrop) depends on that order,
>     so pool order blended it wrong. **A/B toggle confirmed** it (old tree-walk path = vivid skybox).
>     FIX: RenderSystem no longer iterates the pool -- `RenderSystem::DrawObject(RenderComponent&,Mat4&)`
>     is now driven per-object from `GameObject::Draw` (the tree walk), so 3D renders in the
>     authoritative PCS-tree order. `GameObjectMan::Draw` is a single tree walk again (no separate pool
>     pass). Batched pool iteration + material sorting is deferred to Phase 6 (needs Phase 5 ordering).
>     **NEEDS RUNTIME RE-TEST: scene3 skybox vivid again; scene4 cubes + scenes 1/2 unaffected.**
>   - **DONE (2026-07-08): SkinLightTexture branch -- builds 0 errors.** `privDrawSkinLightTexture`
>     (tex + SolidCull rasterizer + `ActivateMesh`/`ActivateSRVBuffers` + WVP + light + `poComputeBlend
>     ->BindWorldBoneArray` + draw + BlendOff). Consumes the SkinningSystem's bone-world SRV (computed
>     in Update, before Draw). **ALL 5 3D MATERIALS now RenderSystem branches**; the `default:
>     Render()` is just a safety net (unreachable for known kinds). **NEEDS RUNTIME TEST: scenes 1/2
>     dancers animate + render identically.**
> **DATA-MIGRATION CONSOLIDATION IN PROGRESS (2026-07-08): scene4 fully data-path, builds 0 errors.**
> Done material-by-material (snapshot-at-ctor won't work -- e.g. terrain UV is set AFTER ctor -- so
> RenderComponent must be the source of truth, which means construction no longer needs a GraphicsObject).
> - **Foundation:** `RenderComponent` gained the data fields (`pMesh`, `pShader`, `pTex`, `pComputeBlend`,
>   `lightColor`, `lightPos`, `bodyColor`, `uvMatrix`); kept `pGraphicsObject` for un-migrated + 2D/Null.
>   New **data-path ctors** `GameObject(MaterialKind)` / `GameObjectRigidBody(MaterialKind)` (no
>   GraphicsObject; `pGraphicsObject=nullptr`). `GameObject::GetRender()` made **public** so scenes fill
>   handles. `GameObject::Draw` asserts pGraphicsObject only on the Render() path; `RigidBody::Update`
>   null-guards the `SetWorld` push; `RenderSystem::DrawObject` dropped its top pGraphicsObject assert.
> - **MIGRATED + DELETED (3 classes): `GraphicsObject_ColorByVertex`, `_ConstColorLight`, `_Wireframe`.**
>   Their RenderSystem branches now read `r.pMesh/r.pShader` + `r.lightColor/lightPos/bodyColor`. scene4's
>   3 cubes are created via `new GameObjectRigidBody(MaterialKind::X)` + `pCube->GetRender()` populated
>   with mesh/shader/params. **NEEDS RUNTIME TEST: press 4 -- gradient / blue-lit / green-wire cubes
>   identical.**
> - **FlatTexture MIGRATED + DELETED (2026-07-08): scene3 data-path, builds 0 errors.** `GameObjectTerrain`
>   reworked: ctor now takes `Mesh::Name/ShaderObject::Name/TextureObject::Name` (+ sizes), fills
>   RenderComponent handles, and `SetUVRepeat` writes `RenderComponent.uvMatrix` (the late-setter that
>   forced source-of-truth). Skybox = `GameObjectRigidBody(MaterialKind::FlatTexture)` + GetRender().
>   `GameObject(MaterialKind)` ctor now inits uvMatrix=Identity + light/body Vec3s=0. FlatTexture branch
>   reads `r.pMesh/pShader/pTex/uvMatrix`. Removed `GraphicsObject_FlatTexture.h` include from
>   `GameObject.h`. **NEEDS RUNTIME TEST: press 3 -- terrain (with 80x80 UV tiling) + skybox identical.**
>   **4 of 5 3D materials now data-path + deleted** (ColorByVertex, ConstColorLight, Wireframe, FlatTexture).
> - **SkinLightTexture MIGRATED + DELETED (2026-07-08): builds 0 errors.** Added data-path ctors up the
>   chain: `GameObjectControlled(MaterialKind)` and `GameObjectAnimSkin(MaterialKind, ComputeBlend*)`
>   (null-guarded its `GetGraphicsObject()->SetWorld` push). AnimMan's 3 Add variants now create
>   `new GameObjectAnimSkin(MaterialKind::SkinLightTexture, pBlend)` + a `privFillSkinRender` helper that
>   populates RenderComponent (mesh/shader/tex/lightColor/lightPos/pComputeBlend). Branch reads all from
>   RenderComponent (incl. `r.pComputeBlend->BindWorldBoneArray()`). **NEEDS RUNTIME TEST: press 1/2 --
>   dancers animate + render identically.**
>
> **3D DATA-MIGRATION CONSOLIDATION COMPLETE.** ALL 5 3D `GraphicsObject_*` deleted (ColorByVertex,
> ConstColorLight, Wireframe, FlatTexture, SkinLightTexture). Every 3D renderable is now pure ECS data:
> `GameObjectRigidBody/AnimSkin(MaterialKind)` + `RenderComponent` handles, drawn by `RenderSystem`
> branches. Remaining `GraphicsObject_*`: only `_Null` (root) + `_Sprite` (2D). The `RenderSystem`
> `default: Render()` is now dead (unreachable -- all 3D kinds branch).
> **P4.3 DONE (2026-07-08): SpriteRenderSystem -- 2D/UI fully data-path, builds 0 errors.**
> - New `Component/Sprite2DComponent.h` (`COMPONENT_SPRITE2D`: pMesh/pShader/pTexture + uvMatrix +
>   origMatrix + color -- the mutable state that was inside GraphicsObject_Sprite). New
>   `System/SpriteRenderSystem.h/.cpp` (`DrawSprite(Sprite2DComponent&, Mat4& world)` -- the ortho-cam
>   screen-rect/UV/colorScale/alpha logic; was GraphicsObject_Sprite's contract).
> - `GameObjectSprite` reworked to data-path: ctor `(Mesh::Name, ShaderObject::Name, Image::Name, Rect)`
>   fills the Sprite2DComponent; SetImage/SetTexture/SetScreenRect/SetColor write it; `Draw()` calls
>   `SpriteRenderSystem::DrawSprite`; Update just computes world = S*R*T (2D). `FontSprite` data-path
>   ctor; its per-glyph `Draw` loop now configures the Sprite2DComponent via GameObjectSprite methods
>   (SetTexture/SetImage/SetScreenRect) + per-glyph `GameObjectSprite::Update`/`Draw`. `FontSprite::Update`
>   is now a no-op (was pushing world to the GraphicsObject).
> - scenes 1/2: FontSprite created directly `new FontSprite(Mesh::SPRITE, Sprite, Image::GreenBird,
>   Rect(...))` (no GraphicsObject_Sprite); scene2's `->pGraphicsObjectSprite->poColor->Set` calls ->
>   `s_pFontBlend->SetColor(...)`. Deleted `GraphicsObject_Sprite`.
> - **NEEDS RUNTIME TEST (the risky one -- text rendering, can't self-verify): press 1 & 2 -- all UI
>   text labels render correctly (position/color); scene2 SPACE toggles the blend label text+color.**
> **P4.4 DONE (2026-07-08): GraphicsObject hierarchy fully DELETED -- PHASE 4 COMPLETE. Builds 0 errors.**
> - Root is now `new GameObjectRigidBody(MaterialKind::Null)` (no GraphicsObject_Null); `GameObject::Draw`
>   skips `kind==Null` (draws nothing) and dropped the `Render()` fallback + the `useECSRender` toggle
>   (no non-ECS path exists anymore -- removed `sUseECSRender`/Get/Set).
> - Removed `RenderComponent.pGraphicsObject`; removed every `GameObject*(GraphicsObject*)` ctor
>   (GameObject/RigidBody/Controlled/AnimSkin) + `GameObject::GetGraphicsObject()` + the null-guarded
>   `SetWorld` pushes in RigidBody/AnimSkin Update. RenderSystem `default` case -> `assert(false)`.
> - **DELETED `Graphics/` entirely** (folder now empty): `GraphicsObject`, `GraphicsObject_Abstract`,
>   `GraphicsObject_Null` (+ earlier the 5 3D + Sprite = all 10 material classes gone).
> - **NEEDS RUNTIME TEST (broad -- touched root + all ctors + Draw): press 1/2/3/4, switch scenes
>   repeatedly, and exit -- everything renders + clean leak check.**
>
> **=== PHASE 4 COMPLETE ===** `RenderSystem` (3D, per-MaterialKind branches) + `SpriteRenderSystem`
> (2D/UI) own all rendering; `RenderComponent` (3D data) + `Sprite2DComponent` (2D data) are the source
> of truth; the whole `GraphicsObject_*` class explosion is deleted. Gameplay (scenes) describes
> renderables purely as `MaterialKind` + component handles -- it never touches shader/RHI internals
> (encapsulation achieved; the boundary is enforced by the DLL link + the data-only public surface).
> **Next: Phase 5 (delete GameObject/PCSTree scaffolding -> pure ECS) or Phase 6 (profile/optimize:
> batch the RenderSystem by shader/material, archetype storage, job-parallel systems).**
> - **P4.3 -- 2D/UI pass = `SpriteRenderSystem`** (locked). Drives the per-glyph loop from component
>   data, runs strictly AFTER the 3D pass; then delete `_Sprite`.
> - **P4.4 -- flip default, delete `GraphicsObject`/`_Abstract`/`_Null` + the toggle, move render
>   internals onto the DLL's private include path (final encapsulation).**
>
> **OPEN QUESTIONS for the user before P4.1:** (a) OK to delete the 6 dead materials now? (b) Sprite
> path: leave FontSprite imperative (lowest risk) or build a SpriteRenderSystem? (c) How strict is the
> A/B verification bar -- pixel-identical framebuffer diff, or visual confirmation?

**Goal:** one `RenderSystem` that iterates renderables, **sorts by shader/material**, binds
state once per batch — retiring `GraphicsObject_*` and moving all shader-selection internals
**private inside the DLL**. This is where the real perf win and the final encapsulation land.

**Files:** new `.../System/RenderSystem.*`; edits to `Game.cpp` draw path,
`GameObjectMan::Draw`; deletion of `GraphicsObject*`/`GraphicsObject_*`.

**Steps:**
1. Build `RenderSystem` **alongside** the existing `Draw()` path with a runtime toggle
   (`bool useECSRender`) so both can run and be compared.
2. `RenderSystem` iterates `ViewOf<RenderComponent, TransformComponent, DrawEnabled>()`,
   groups by `(shader, material, mesh)`, binds shader/state **once** per group, transfers
   per-object WVP (from `Camera` + `LightComponent`), draws. Reuse `Mesh`/`Buffer*`/`State*`
   unchanged.
3. Move each `GraphicsObject_*::SetState/SetDataGPU/Draw/RestoreState` body into the matching
   `MaterialKind` branch of the `RenderSystem`.
4. **Verify by A/B framebuffer diff** until pixel- (or near-pixel) identical. Then flip the
   default, delete `GraphicsObject_*`, and move the render internals onto the DLL's **private**
   include path — gameplay now literally cannot include them.
5. Skinned materials (`_SkinLightTexture/_SkinFlatTexture`) last — they consume the Phase 3
   `SkinningSystem` output; route them through their own batch until then.

**Exit criteria:** ECS render path is default; `GraphicsObject_*` deleted; draws sorted (fewer
state changes); output matches old path; shader internals are private behind the DLL.

**Risk:** high. Mitigation: dedicated branch, toggle + A/B diff until identical, one
`MaterialKind` at a time.

---

### Phase 5 — Delete the OOP scaffolding

> **SCOPING (2026-07-08): investigated the remaining object model. Findings below.**
>
> **What still runs OOP (driven by GameObjectMan::Update/Draw -> PCS-tree walk -> per-object
> Update()/Draw()):**
> | OOP piece | Still holds / does | ECS destination |
> |---|---|---|
> | `GameObject` (base) | entity id + Update/Draw virtuals | gone (the entity IS the id) |
> | `GameObjectRigidBody` | prefab-driven world in Update | prefab -> system |
> | `GameObjectControlled` | thin base (`int index`) | gone |
> | `GameObjectAnimSkin` | `cur_rot_x/y/z` + `delta_*` + `Prefab_Pivot` + transform setters; Update = pivot/delta world | `PivotComponent` + `PivotSystem` |
> | `GameObjectSprite` | posX/posY/scale/angle -> world (Update); Draw override | 2D-transform system |
> | `FontSprite` | text (message/glyph/x/y/color) + **per-glyph Draw loop** | `TextComponent` + SpriteRenderSystem glyph loop |
> | `GameObjectTerrain` | data-path FlatTexture helper (sizes->scale, UV) | scene sets components (small helper) |
> | `GameObjectMan` | owns PCSTree; Update = SystemMan::Run + tree walk; Draw = tree walk | pure system runner + ordered draw |
> | `PCSTree`/`PCSNode` | **FLAT** (root + all objects as direct children); gives iteration ORDER + lifetime | ordered entity list / draw-order |
> | `AnimMan` -> `GameObjectAnimSkin*` | pokes skins: SetPos/Scale, `SetPrefab(Prefab_Pivot)`, `cur_rot_*`, SetQuat | AnimMan writes components / holds entities |
>
> **Confirmed: the hierarchy is 100% FLAT** -- every `GameObjectMan::Add(obj, GetRoot())`. No object
> parents another. So the PCS tree is NOT used for hierarchy; it's used only for (a) draw/update ORDER
> and (b) lifetime. That means NO `TransformPropagationSystem` is needed (Section 9 open decision #1
> resolves to: flatten to world transforms).
>
> **The hard problems:**
> 1. **DRAW ORDER is the crux.** The flat PCS DFS order = REVERSE creation order (PCS front-insert),
>    and P4 RELIES on it for transparency (skybox-before-terrain; UI-after-3D). The RenderComponent
>    POOL order != that. Removing the tree needs an explicit order: an ordered entity "scene draw list"
>    (preserves current semantics, lowest disruption) OR a layer/z sort key on renderables. The 2D UI
>    pass must still run strictly after the 3D pass.
> 2. **Per-object Update -> systems:** sprite 2D transform (posX/posY->world), anim pivot/delta-rot
>    (`Prefab_Pivot` + `cur_rot_*`). New `PivotComponent`/`PivotSystem`; a sprite-transform system.
> 3. **FontSprite per-glyph:** SpriteRenderSystem must own the glyph loop, reading a `TextComponent`
>    (message/glyph/x/y/color) -- currently `FontSprite::Draw` does it imperatively.
> 4. **AnimMan rework (biggest single piece):** it stores `GameObjectAnimSkin*` in its AnimNode and
>    drives them. Must instead hold entities + write `TransformComponent`/`PivotComponent`. Also owns
>    the anim resources already (post-controller-deletion), so lifetime is partly there.
> 5. **Lifetime:** `GameObjectMan::Destroy` deletes GameObjects (which free `FontSprite::poMessage`,
>    `poPrefab`). Without GameObjects, those heap resources need a new owner (component + explicit free,
>    or move to AnimMan/a manager).
>
> **PROPOSED SEQUENCE (each shippable + runtime-verified):**
> - **P5.0 -- explicit draw order.** Add an ordered entity list ("scene render list") the RenderSystem
>   + SpriteRenderSystem iterate (opaque 3D pass, then 2D pass), replacing the tree-walk draw. Keep
>   GameObject for Update. De-risks ordering FIRST (this is what broke before).
> - **P5.1 -- Sprite/Font -> ECS.** `Sprite2DComponent` already exists; add sprite-transform handling +
>   `TextComponent`; SpriteRenderSystem does the glyph loop. Delete `GameObjectSprite`/`FontSprite`.
> - **P5.2 -- AnimSkin pivot -> ECS.** `PivotComponent` + `PivotSystem`; rework `AnimMan` to drive
>   components (not `GameObjectAnimSkin*`). Delete `GameObjectAnimSkin`/`GameObjectControlled`.
> - **P5.3 -- RigidBody/Terrain -> ECS.** Scenes create entities + components directly (thin helpers).
>   Delete `GameObjectRigidBody`/`GameObjectTerrain`.
> - **P5.4 -- delete `GameObject`, `GameObjectMan`, PCSTree usage.** `Game.cpp`/scenes/AnimMan drive
>   `World` + the system runner + the draw list directly. PCSTree/PCSNode stay in Libs (unused by game).
>
> **Reality check:** Phase 5 is the biggest phase -- it dismantles the object model AND reworks AnimMan
> AND solves ordering, for architecture purity (no measured perf gain by itself). **Phase 6 (batch the
> RenderSystem by shader/material, archetype storage, job-parallel) is the actual PERF win and is
> largely INDEPENDENT of Phase 5.** Worth deciding which to do next.
>
> **DECISIONS LOCKED (user, 2026-07-08):** (a) do **Phase 5 (pure ECS) NEXT**. (b) draw order via a
> **layer/z sort key** on renderables (RenderComponent gets a `layer`; the RenderSystem sorts by it each
> frame; 2D UI runs after the 3D pass). Scenes assign layers (skybox = background layer so it draws
> before the terrain -- that was the transparency dependency). Sort is hand-written (no `<algorithm>`
> per 0a) over a temp array; a few hundred renderables, negligible.
>
> **P5.0 DONE (2026-07-08): layer-sorted 3D draw -- builds 0 errors.**
> - `RenderComponent` gained `int layer` (default 0). `RenderSystem::Draw(World&)` gathers the 3D
>   drawEnable entities into a temp index array, **stable insertion-sorts by layer asc** (hand-written,
>   no `<algorithm>`; n is small), then draws each via `DrawObject` (now private) reading each entity's
>   TransformComponent world. `GameObjectMan::Draw` now = `RenderSystem::Draw` (sorted 3D pass) THEN the
>   tree walk (2D sprites/fonts via their Draw override; base `GameObject::Draw` is now a no-op for
>   3D/Null). scene3 skybox set `r.layer = -1` so it draws before the terrain (the P4 transparency dep).
> - This removes the tree's role in 3D DRAW ORDER (now the layer sort) -- the tree still drives Update +
>   the 2D sprite/font Draw for now. **NEEDS RUNTIME TEST: press 3 (skybox vivid, before terrain), 4
>   (cubes), 1/2 (dancers + UI text) -- all identical.**
> - **NEXT: P5.1 -- Sprite/Font -> ECS** (TextComponent + SpriteRenderSystem glyph loop; a 2D transform
>   handling), so the 2D draw also leaves the tree walk. Then P5.2 (AnimSkin pivot), P5.3 (RigidBody/
>   Terrain), P5.4 (delete GameObject/GameObjectMan/PCSTree).
>
> **P5.1a DONE (2026-07-11): 2D/font draw leaves the tree walk -- builds 0 errors.**
> - New `Component/TextComponent.h` (`const char *pMessage` [non-owning], `Glyph::Name glyphName`,
>   `float x,y`; id `COMPONENT_TEXT`). The color/mesh/shader stay in the same entity's
>   `Sprite2DComponent`. Only `FontSprite` uses the 2D path today (no plain `GameObjectSprite`
>   instances in any scene), so text is the live case.
> - `SpriteRenderSystem` gained the pool pass **`Draw(World&)`** (`DrawSprite`/`DrawText` now
>   private): iterates the `Sprite2DComponent` pool; entities with a `TextComponent` run the
>   per-glyph loop (moved verbatim from `FontSprite::Draw` -- GlyphMan lookup, per-glyph
>   texture/UV/screen-rect into the scratch `Sprite2DComponent`, world = `Trans(x,y)` since text
>   never scales/rotates), the rest draw one quad from their `TransformComponent.world`.
> - `FontSprite`: ctor `Add`s the `TextComponent`; `Set`/`UpdateMessage` mirror message/glyph/x/y
>   into it via `privSyncText` (repointing `pMessage` after each realloc -- FontSprite still owns
>   the `poMessage` buffer + frees it in its dtor). **Removed `FontSprite::Draw` and
>   `GameObjectSprite::Draw` overrides** (the system draws now).
> - `GameObjectMan::Draw` is now **fully pool-driven, no PCS-tree walk**: `RenderSystem::Draw`
>   (3D, layer-sorted) then `SpriteRenderSystem::Draw` (2D, after 3D). The Update tree walk stays
>   (anim/prefab objects) until P5.2/P5.4. `GameObject`/`GameObjectSprite`/`FontSprite` still
>   exist as thin creators/holders -- **deletion is the next P5.1 increment** (needs the scenes to
>   create entity+components directly; scene2 holds `s_pFontBlend` for `UpdateMessage`/`SetColor`,
>   so it needs an entity/handle).
> - **NEEDS RUNTIME TEST:** press 1/2 (UI text: titles, per-dancer labels, scene2 Silly/Gangnam/
>   Blend + the spacebar `UpdateMessage`/`SetColor` toggle) -- all identical to before; 3/4 unchanged.
>
> **P5.1b DONE (2026-07-11): `GameObjectSprite` + `FontSprite` DELETED -- builds 0 errors. → P5.1 COMPLETE.**
> - **Deleted** `GameObject/GameObjectSprite.{h,cpp}` and `UI/FontSprite.{h,cpp}` (the only 2D
>   object-model classes; no plain `GameObjectSprite` ever existed, `FontSprite` was the sole user).
>   Removed the `GameObjectSprite.h` include from `GameObjectMan.h`.
> - **New `UI/Text2D.{h,cpp}`** (exported factory, replaces FontSprite): `Text2D::Add(mesh, shader,
>   image, msg, glyph, x, y, color) -> Entity` creates a **pure ECS text entity** -- `world.Create()`
>   + `Sprite2DComponent` + `TextComponent`, **NOT a GameObject / PCS node**. `SetMessage(e,msg)` /
>   `SetColor(e,color)` mutate it. No per-object Update/Draw, no new/delete: the entity + components
>   live in the World and are freed wholesale by `WorldMan::Destroy` at scene unload.
> - **Message lifetime simplified to non-owning literal pointers.** FontSprite used to deep-copy
>   `poMessage` (heap, freed in its dtor); every scene message is a string literal, so `TextComponent
>   .pMessage` now just points at the caller's literal (stable lifetime, not copied). This dissolves
>   the "who frees poMessage without a GameObject" lifetime problem (scoping pt 5) -- there's no heap
>   to free. (If dynamic strings are ever needed, add a TextMan owner.)
> - Text entities carry **no TransformComponent** -- the text pass computes each glyph's world
>   (`Trans(x,y)`) itself, so no system needs a transform for them.
> - **Scenes rewritten** (scene1: 8 sites; scene2: 6 sites) from `new FontSprite(...)` + `GameObjectMan
>   ::Add` + `->Set(...)` to a single `Text2D::Add(...)`. scene2's mutable label is now
>   `static Entity s_eFontBlend` (was `FontSprite*`): guard `!EntityIsNull(...)`, `Text2D::SetMessage`/
>   `Text2D::SetColor`; the unused `s_pFontSilly`/`s_pFontGangnam` statics were dropped.
> - `GameObject::Draw` base is now fully unused (kept as a no-op until the GameObject shim goes in
>   P5.4). Regenerated premake (globs drop the 4 deleted files, add Text2D); clean build 0 errors.
> - **NEEDS RUNTIME TEST (same as P5.1a):** press 1/2 -- all UI text identical (incl. scene2 spacebar
>   Silly/Gangnam/Blend color+message toggle); 3/4 unchanged. Also watch the framework leak check at
>   scene unload (text entities now freed via `WorldMan::Destroy`, not a GameObject dtor).
>
> **NEXT: P5.2 -- AnimSkin pivot -> ECS** (`PivotComponent` + `PivotSystem`; rework `AnimMan` to drive
> components instead of `GameObjectAnimSkin*`; delete `GameObjectAnimSkin`/`GameObjectControlled`).
>
> **P5.2 DONE (2026-07-11): skins are pure ECS; `GameObjectAnimSkin`/`GameObjectControlled`/`Prefab_Pivot`
> DELETED -- builds 0 errors.**
> - **The pivot path was DEAD, so it was dropped, NOT ported to a PivotComponent/PivotSystem.**
>   Investigation: `AnimMan::SetPrefabPivot`/`SetPivotRot*`/`SetPivotTotalRot`/`SetQuat` and
>   `GameObjectAnimSkin::{cur_rot_*, poPrefab, SetPrefab}` are **never called by any scene/game code**
>   (repo-wide grep) -- so `poPrefab` was always null and `Prefab_Pivot` was never instantiated. With
>   `cur_rot`/delta always 0, `GameObjectAnimSkin::privUpdate` reduced to `world = S*Q*T` -- **exactly
>   what `LocalToWorldSystem` already computes** from the TransformComponent -- and the GPU dispatch
>   already moved to `SkinningSystem` (P3). So `GameObjectAnimSkin::Update` was 100% redundant. Adding a
>   PivotComponent/PivotSystem would have been speculative dead code (cf. the P3 dead-`Prefab_*` purge),
>   so P5.2 drops pivot instead of ECS-ifying it. (Reconciles the old "Prefab_Pivot is live" note --
>   the *capability* existed in AnimMan but nothing ever invoked it.)
> - **Skins are now pure-ECS entities** (like the AnimClip/AnimBlend entities AnimMan already made):
>   new `AnimMan::privCreateSkinEntity` does `World::Create()` + `TransformComponent` (placement; rot
>   identity) + `RenderComponent{SkinLightTexture, mesh/shader/tex/light/pComputeBlend, layer 0}` +
>   `GpuSkinComponent{pBlend}`. **No GameObject, no PCS node.** LocalToWorld (transform) / Skinning
>   (GPU dispatch) / Render (P5.0 layer-sorted 3D pass) systems drive them entirely; the tree walk no
>   longer touches skins. (RenderSystem already drew them from the pool since P5.0, so draw is unchanged.)
> - **AnimMan reworked to hold `Entity` handles**, not `GameObjectAnimSkin*`: `AnimNode` stores
>   `Entity mSkinEntities[MAX_SKIN_ENTITIES]` (`GetSkinEntity`/`GetNumSkinEntities`); the 3 `Add`
>   overloads call `privCreateSkinEntity`; `SetScale`/`SetUniformScale`/`SetPos` write each skin
>   entity's `TransformComponent` via `World::TryGet`. Removed the dead `SetPivotRot*`/`SetPivotTotalRot`/
>   `SetPrefabPivot` API and `privFillSkinRender`.
> - **Deleted** `GameObjectAnimSkin.{h,cpp}`, `GameObjectControlled.{h,cpp}`, `Prefab_Pivot.{h,cpp}`;
>   removed their includes from `GameObjectMan.h`; removed `Prefab::SetData(GameObjectAnimSkin&)` +
>   its include/fwd-decl (Prefab base stays for GameObjectRigidBody until P5.3, though its prefab path
>   is now also dead). Updated GpuSkin/Skinning comments. (`.cd` class-diagram docs still name the old
>   classes -- non-compiled, left as-is.)
> - **Lifetime:** skin entities live in the World, freed by `WorldMan::Destroy` at scene unload (the
>   ComputeBlend is freed first by `AnimMan::Destroy`, same non-owning order as the anim-clip entities).
> - **NEEDS RUNTIME TEST:** press 1 (7 dancers: Ward/Drax/Maw/Pirate/Halo[2-mesh]/Crownboi[multi-mesh]/
>   + labels) and 2 (Silly/Gangnam + spacebar blend) -- all skinned dancers at the right pos/scale,
>   animating, identical to before; 3/4 unchanged. Watch the leak check at unload.
>
> **NEXT: P5.3 -- RigidBody/Terrain -> ECS** (scenes create entities+components directly via thin
> helpers; delete `GameObjectRigidBody`/`GameObjectTerrain` + the now-dead `Prefab` base). Then P5.4
> deletes `GameObject`/`GameObjectMan`/PCSTree usage.
>
> **P5.2 RUNTIME-VERIFIED (2026-07-11, user):** scenes 1/2 dancers + labels + spacebar blend all correct.
>
> **P5.3 DONE (2026-07-11): RigidBody/Terrain -> pure ECS; `GameObjectRigidBody`/`GameObjectTerrain` +
> the whole `Prefab` hierarchy DELETED -- builds 0 errors. Nothing is added to the PCS tree anymore.**
> - New `Factory/Renderable3D.{h,cpp}` (exported; premake auto-globs the new `src/Factory/` folder):
>   `Renderable3D::Add(MaterialKind) -> Entity` creates a **pure-ECS 3D renderable** -- `TransformComponent`
>   (seeded identity) + `RenderComponent` (kind + null handles, same field init as the old GameObject +
>   RigidBody ctors). Accessors `GetTransform`/`GetRender` + `SetTrans`/`SetScale(s)`/`SetScale(sx,sy,sz)`.
>   No GameObject/PCS node; LocalToWorld/Render (+ Rotate) systems drive it (like the P5.2 skins).
> - **`GameObjectTerrain` collapsed into scene3** -- its authored-size->scale + uv-repeat logic was
>   trivial, so scene3 sets it inline: `SetScale(800/200, 100/20, 800/200)` (= 4,5,4, the old
>   SetSize/SetHeight result) and `r.uvMatrix = Scale(80,80,1)` (old SetUVRepeat). Skybox likewise
>   (`r.layer = -1`). **scene4** three cubes -> `Renderable3D::Add(kind)` + `RotateComponent` (unchanged
>   spin). No `GameObjectMan::Add`/`GetRoot`/`SetName` anywhere now.
> - **The PCS-tree root** is now `new GameObject(MaterialKind::Null)` -- `GameObject` was made concrete
>   (pure-virtual `Update` -> no-op body; dropped the `Prefab.h` include; ctor now seeds pos/scale/rot).
>   It is the ONLY live GameObject; the tree holds just the root (nothing is inserted). Removed the
>   `GameObjectRigidBody.h` include from `GameObjectMan.h`.
> - **Deleted** `GameObjectRigidBody.{h,cpp}`, `GameObjectTerrain.{h,cpp}`, and the entire `Prefab/`
>   folder (`Prefab`/`Prefab_Abstract`/`Prefab_Pivot`) -- the Prefab hierarchy was fully dead once the
>   pivot path went (P5.2) and RigidBody's unused `SetPrefab` went with the class.
> - **Lifetime:** the renderable entities live in the World (freed by `WorldMan::Destroy`); only the
>   Null root is a heap GameObject (freed by `GameObjectMan::Destroy` before `WorldMan::Destroy`).
> - **NEEDS RUNTIME TEST:** press 3 (skybox behind the scaled terrain, unchanged) and 4 (three cubes --
>   ColorByVertex/ConstColorLight/Wireframe -- at +/-220 spacing, spinning); 1/2 unchanged. Leak check at unload.
>
> **P5.3 RUNTIME-VERIFIED (2026-07-11, user):** scenes 3/4 (terrain+skybox, spinning cubes) all correct.
>
> **P5.4 DONE (2026-07-11): `GameObject` + `GameObjectMan` + the PCS tree DELETED -- builds 0 errors.
> => PHASE 5 COMPLETE. The game path is now 100% pure ECS (no GameObject, no PCSTree).**
> - **`SystemMan` is now the per-scene ECS frame driver** (absorbed GameObjectMan's remaining role):
>   `Create()` allocates + adds the standard engine systems (LocalToWorld/Rotate/Animation/Blend/
>   Skinning -- moved from GameObjectMan::Create); `Run(world, tDelta)` runs the Update systems (was
>   GameObjectMan::Update minus the tree walk); new `Draw(world)` does `RenderSystem::Draw` then
>   `SpriteRenderSystem::Draw` (was GameObjectMan::Draw). `Destroy()` frees the systems.
> - **Scenes repointed:** `GameObjectMan::Create/Update/Destroy` -> `SystemMan::Create` /
>   `SystemMan::Run(WorldMan::GetWorld(), tDelta)` / `SystemMan::Destroy`; `Game.cpp` `GameObjectMan::Draw()`
>   -> `SystemMan::Draw(WorldMan::GetWorld())`. (`tCurr` is now unused in scene Update -> AZUL_UNUSED_VAR.)
> - **Deleted** `GameObject.{h,cpp}` + `GameObjectMan.{h,cpp}` -- the whole `src/GameObject/` folder is
>   gone. The **PCS tree/root is entirely removed**: no root entity is created (the old Null-root
>   `MeshNull`/`ShaderObject_Null` registration was vestigial -- the root's RenderComponent handles were
>   always null and nothing Finds NULL_MESH/NullShader at runtime; the node managers self-register their
>   own null compare-node). Removed a stale `class GameObject;` fwd-decl from `Skeleton.h`.
> - **Bonus cleanup:** `HierarchyComponent` was orphaned once GameObject went (it was only the PCS-parent
>   mirror; nothing adds/reads it now) -- deleted `HierarchyComponent.h` and its `COMPONENT_HIERARCHY` id.
>   (Section 9 open-decision #1 resolves to **flatten to world transforms** -- scenes are flat, so no
>   `TransformPropagationSystem` was ever needed; the Steps below that call for one are superseded.)
> - **`PCSTree`/`PCSNode` stay in `Libs/` (still linked, now unused by the game)**, per plan.
> - **Lifetime:** all entities live in the World, freed wholesale by `WorldMan::Destroy` at unload; no
>   per-object heap owner remains (no root GameObject). Scene Unload order: `SystemMan::Destroy()` then
>   `WorldMan::Destroy()`.
> - **NEEDS RUNTIME TEST:** all four scenes (1 dancers+labels, 2 blend, 3 terrain+skybox, 4 cubes) and
>   scene switching identical to before; leak check clean at every unload.
>
> **=> Phase 5 complete. NEXT: Phase 6 (optimize -- profile vs baseline, SoA/archetype storage,
> job-parallel systems, instancing) OR stop here (architecture goal met; Phase 6 is the perf-only phase
> and is optional per Section 2).**

**Goal:** remove the bridge and the tree; the world is now pure ECS.

> **NOTE (2026-07-11): the Steps below are the ORIGINAL P5 plan and are now superseded by the P5.0-P5.4
> increment notes above.** In particular Step 2's `HierarchyComponent`-authoritative +
> `TransformPropagationSystem` did NOT happen: the scenes are 100% flat (every object was a direct child
> of the root), so hierarchy flattens to world transforms and no propagation system is needed.

**Files:** delete/retire `GameObject*`, `GameObjectMan`, `PCSTree`/`PCSNode` game usage; edits
to `Game.cpp` and `GameObjectMan` call sites.

**Steps:**
1. Replace remaining `GameObjectMan::Add/Update/Draw` calls with direct `World` +
   system-runner calls.
2. Make `HierarchyComponent` **authoritative**; add `TransformPropagationSystem` that composes
   local→world parent-before-child (keep entities topologically ordered). If scenes are flat,
   store world transforms directly and skip propagation.
3. Delete `GameObject`, subclasses, `GameObjectMan`, and the bridge forwarders.

**Exit criteria:** no `GameObject`/`PCSTree` in the game path; engine runs purely on `World` +
systems; visuals unchanged.

**Risk:** medium (mostly deletion; hierarchy ordering is the subtlety).

---

### Phase 6 — Optimize storage & iteration (only after correctness)

**Goal:** turn the now-clean architecture into *measured* speed.

**Steps:**
1. Profile; compare against the Phase 1 baseline. Confirm where time actually goes.
2. Move hot pools to **archetype / SoA** layout (or lean on EnTT groups).
3. Add explicit system ordering + **job-parallel** dispatch for independent systems (behavior/
   animation parallelize well; render submission stays single-threaded or uses command lists).
4. Add **instancing** for identical mesh+material groups surfaced by the `RenderSystem`.
5. Re-profile; stop when the profile is flat.

**Exit criteria:** measured improvement vs baseline; no visual regressions.

**Risk:** low functionally; easy to over-engineer — stop when the profile is flat.

---

## 8. Cross-cutting rules

- **Ship after every phase.** The engine must build and run at each phase boundary.
- **One concern per commit** (one file moved, one field, one behavior, one material kind).
- **Components hold handles, not GPU objects.** Ownership stays in the RHI/managers (in the DLL).
- **Systems never call each other**; they communicate through components and a fixed run order.
- **Don't ECS-ify the RHI** (Section 4). It stays OOP, private, inside `AzulEngine`.
- **Gameplay includes only public facade headers** (Section 5).
- **Verify visually / by framebuffer diff**, especially Phase 4.

## 9. Open decisions

**Resolved:** `AzulEngine` = **DLL**. Registry = **hand-rolled sparse-set** (no EnTT).

Still open (resolve as you go):
1. Hierarchy: real parent/child propagation vs flatten to world transforms if scenes are
   shallow (Phase 5).
2. Whether to unify the node managers into one `ResourceRegistry` (optional, post-Phase 5).
3. Camera: leave as OOP data-holder, or promote active-camera selection to a `CameraComponent`.

---

## 10. Quick reference — where each existing class ends up

| Existing | Destination |
|---|---|
| `Engine.vcxproj` (monolithic app) | split into `AzulEngine.dll` + `Game.exe` (Phase 0) |
| `State*`, `Buffer*`, `Converter/*`, protobuf, node managers | **private inside `AzulEngine.dll`**, stay OOP (Phase 0) |
| `GameObject`, `GameObjectMan`, `PCSTree`, `PCSNode` (game use) | deleted (Phase 5) |
| `GameObjectRigidBody/Controlled/AnimSkin/Sprite/Terrain` | entity archetypes (3a) |
| `GraphicsObject`, `GraphicsObject_*` | `RenderComponent` + `MaterialComponent` + `RenderSystem`, then deleted + made private (Phase 4) |
| `Prefab_*` | behavior components + systems (Phase 3) |
| `Anim`, `AnimController_*`, `Mixer*` | `AnimClip/AnimBlend` components + `AnimationSystem`/`BlendSystem` (Phase 3) |
| `ComputeBlend_*` | `GpuSkinComponent` + `SkinningSystem` (Phase 3) |
| `Skeleton`, `Bone`, `JointTable`, `HierarchyTable` | `SkeletonComponent` (+ keep resource managers in DLL) |
| light data in `_LightTexture` | `LightComponent` (Phase 2) |
| `Camera`, Math lib | **stay OOP**, in DLL / foundation libs (Section 4) |
| ECS core (`World`/`ComponentPool`/`System`), component structs, handle enums | **public shared headers** (Phase 1) |
