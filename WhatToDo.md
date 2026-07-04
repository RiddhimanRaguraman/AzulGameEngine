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
> - Created `Engine/AzulEngine/AzulEngine.vcxproj` (DynamicLibrary, GUID
>   `DBD19CE6-9BD5-4561-B8C1-5304447A007F`, mirrors `Libs/Math` conventions).
> - `EngineDLLInterface.h` (`AZUL_ENGINE_LIBRARY_API` export macro) + `EngineDLLMain.cpp`
>   + first exported symbol `Azul::GetAzulEngineVersion()` (so the import lib is emitted).
> - Registered in `Engine.sln`; app (`Engine.vcxproj`) references it, defines
>   `AZUL_ENGINE_USE_DLL`, adds `..\AzulEngine\include` to its include path.
> - Full solution builds with **zero new errors** (only the pre-existing `PugiXmlTest`
>   "cannot open File.h" failures remain — unrelated to this work).
> - **Next increment:** move the first internals group (`State*` RHI) into the DLL, tag each
>   public class with `AZUL_ENGINE_LIBRARY_API`, add its deps (d3d11, Math/Manager) to the DLL
>   project, build-verify. Then `Buffer*`, then loaders/managers.

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

**Goal:** remove the bridge and the tree; the world is now pure ECS.

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
