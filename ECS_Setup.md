# ECS Setup — how the Azul engine's Entity-Component-System works

A ground-up walkthrough of the hand-rolled **sparse-set ECS** in this engine, traced
through the creation and rendering of a single cube (scene4 style), from
`Entity` → components → systems → GPU buffer.

Written so it can be re-applied to another engine (e.g. a Vulkan renderer): the ECS
core is 100% graphics-API-agnostic; only the final *draw* step touches D3D11, and the
last section maps that onto Vulkan.

---

## 0. The big picture

Instead of an object model where each game object is a heap-allocated class in a
tree (`GameObject` → `Update()`/`Draw()` virtuals), everything is:

- **Entity** — just an id (a handle). No data, no methods.
- **Component** — plain data (a `struct`), no logic. Stored in tightly-packed arrays.
- **System** — logic that sweeps one component array each frame.

```
Entity 42  ─┬─ TransformComponent { pos, rot, scale, world }
            ├─ RenderComponent    { mesh, shader, material, ... }
            └─ RotateComponent    { angle, speed }
```

The whole world is a handful of flat arrays. Iterating "every transform" is a linear
walk over contiguous memory (cache-friendly, trivially parallelizable) instead of
chasing pointers down a tree and dispatching a virtual per node.

**Coding-standard note:** this engine forbids STL. So every "array" below is a raw
`new[]`/`delete[]` block with an explicit `count` and `capacity`, grown by
allocate-bigger-and-copy. Templates are used *only* for the per-type component pool.

Core files (`Libs/AzulEngine/src/`):

| File | Role |
|---|---|
| `ECS/Entity.h` | the entity handle |
| `ECS/ComponentPool.h` | the sparse set (one per component type) |
| `ECS/IComponentPool.h` | type-erased base so the World can hold pools of mixed types |
| `ECS/World.h` / `World.cpp` | owns entities + all pools |
| `ECS/WorldMan.h` / `.cpp` | singleton access to the one World |
| `ECS/System.h`, `System/SystemMan.*` | system base + the per-frame runner |
| `Component/*.h` | the component structs + the id registry |
| `Factory/Renderable3D.*` | helper that builds a renderable entity |
| `System/LocalToWorldSystem`, `RotateSystem`, `RenderSystem`, ... | the systems |

---

## 1. The Entity — an id with a generation

```cpp
// ECS/Entity.h
struct Entity
{
    unsigned int index;        // which slot
    unsigned int generation;   // how many times that slot has been reused
};
```

An entity is **not** a pointer — it's an index plus a *generation counter*. When an
entity is destroyed its slot is recycled, and the slot's generation is bumped. Any
old copy of the handle still holds the *previous* generation, so a stale handle can
be detected (`IsAlive` compares generations) instead of dereferencing freed memory.
This is the classic "generational index / handle" pattern — do this in Vulkan too;
it makes use-after-free a silent no-op instead of a crash.

`EntityNull()` / `EntityIsNull()` give a "no entity" sentinel (`index = 0xFFFFFFFF`).

---

## 2. Components — plain data + a compile-time type id

A component is a `struct` of data with **one** special member: a static id.

```cpp
// Component/TransformComponent.h
struct TransformComponent
{
    Vec3 pos;
    Quat rot;
    Vec3 scale;
    Mat4 world;                    // filled by LocalToWorldSystem each frame

    static const unsigned int kTypeId = COMPONENT_TRANSFORM;   // 0
};

// Component/RenderComponent.h  (superset across 3D material kinds)
struct RenderComponent
{
    MaterialKind kind;             // ColorByVertex / FlatTexture / ConstColorLight / ...
    bool drawEnable;
    int  layer;                    // draw-order key (sorted ascending)
    Mesh          *pMesh;          // handles into the resource managers -- NOT owned
    ShaderObject  *pShader;
    TextureObject *pTex;
    ComputeBlend  *pComputeBlend;
    Vec3 lightColor, lightPos, bodyColor;
    Mat4 uvMatrix;
    static const unsigned int kTypeId = COMPONENT_RENDER;      // 1
};

// Component/RotateComponent.h
struct RotateComponent
{
    float angle;   // accumulated radians
    float speed;   // radians / frame
    static const unsigned int kTypeId = COMPONENT_ROTATE;      // 7
};
```

The ids live in one central registry:

```cpp
// Component/ComponentId.h
enum ComponentId : unsigned int
{
    COMPONENT_TRANSFORM = 0,
    COMPONENT_RENDER,
    COMPONENT_LIGHT,
    COMPONENT_GPU_SKIN,
    COMPONENT_ANIM_CLIP,
    COMPONENT_ANIM_BLEND,
    COMPONENT_ROTATE,
    COMPONENT_SPRITE2D,
    COMPONENT_TEXT,
    ENGINE_COMPONENT_COUNT,
    GAME_COMPONENT_BASE = 32       // [0,32) engine ids, [32,64) reserved for game
};
```

**Why explicit constants and not a runtime type counter?** The World stores its pools
in an array indexed by `kTypeId`. If the id came from a `static int counter++` inside
a template, that function-local static would be instantiated *separately* in the DLL
and in the app and could hand back different numbers for the same type — corrupting
the lookup. A compile-time constant is identical in every module. (In a single-binary
Vulkan engine you can get away with a counter, but constants are still safer.)

**Components hold handles, never GPU objects.** `RenderComponent.pMesh` is a
non-owning pointer into a resource manager. The actual vertex/index buffers live in
the `Mesh`; the component just references it. Keep this rule — it's what lets you sort,
copy, and stream components freely.

---

## 3. The sparse set — `ComponentPool<C>` (the heart of it)

One `ComponentPool<C>` stores all components of type `C`. It's a **sparse set**: two
arrays that let you do O(1) add / lookup / remove *and* iterate the live components
packed contiguously.

```cpp
// ECS/ComponentPool.h   (condensed)
template <typename C>
class ComponentPool : public IComponentPool
{
    unsigned int *sparse;      // sparse[entity.index] -> dense slot, or INVALID
    unsigned int  sparseCap;

    C            *denseData;   // packed array of components  (the payload)
    Entity       *denseOwner;  // packed array, parallel: which entity owns slot i
    unsigned int  denseCount;  // number of live components
    unsigned int  denseCap;
};
```

Two index spaces:
- **sparse**, indexed by `entity.index` — big and holey. `sparse[e.index]` tells you
  where that entity's component sits in the dense array (or `INVALID`).
- **dense**, `0 .. denseCount-1` — packed, no holes. This is what systems iterate.
  `denseData[i]` is the component, `denseOwner[i]` is the entity that owns it.

```
entity.index:   0    1    2    3    4    5
sparse:       [ 2 |  X |  0 |  X |  1 |  X ]     (X = INVALID)
                │         │         │
dense slot:     └──►2     └──►0     └──►1
denseData:    [ cmpOf#2 | cmpOf#4 | cmpOf#0 ]    (packed, iterate this)
denseOwner:   [   #2    |   #4    |   #0    ]
```

### Add
```cpp
C &Add(const Entity &e)
{
    privEnsureSparse(e.index);                 // grow sparse[] to cover e.index
    assert(sparse[e.index] == ECS_INVALID_INDEX);
    privEnsureDense(denseCount + 1);           // grow dense arrays if full
    unsigned int slot = denseCount;
    sparse[e.index]  = slot;                    // point entity -> new dense slot
    denseOwner[slot] = e;                       // remember owner (with generation)
    denseCount++;
    return denseData[slot];                     // caller fills the fields
}
```

### Has / TryGet  (with the stale-handle guard)
```cpp
bool Has(const Entity &e) const
{
    if (e.index >= sparseCap) return false;
    unsigned int slot = sparse[e.index];
    if (slot == ECS_INVALID_INDEX) return false;
    return denseOwner[slot].generation == e.generation;   // reject stale handles
}
C *TryGet(const Entity &e) { return Has(e) ? &denseData[sparse[e.index]] : nullptr; }
```

### Remove — swap-remove keeps the dense array packed
```cpp
void Remove(const Entity &e)
{
    unsigned int slot = sparse[e.index];
    if (slot == ECS_INVALID_INDEX) return;
    unsigned int last = denseCount - 1;
    denseData[slot]  = denseData[last];         // move the last element into the hole
    denseOwner[slot] = denseOwner[last];
    sparse[denseOwner[slot].index] = slot;      // fix the moved element's sparse entry
    sparse[e.index] = ECS_INVALID_INDEX;
    denseCount--;
}
```
Swap-remove is O(1) and never leaves gaps, so iteration stays a tight `for` loop.
(Order isn't preserved — fine, because draw order comes from an explicit `layer` sort,
not pool order.)

### Growth (no STL)
`privEnsureDense` / `privEnsureSparse` double the capacity, `new[]` a bigger block,
copy the old contents, `delete[]` the old. Exactly how `std::vector` grows, done by hand.

### Iteration surface for systems
```cpp
unsigned int GetCount() const { return denseCount; }
C            &GetData(unsigned int slot)       { return denseData[slot]; }
const Entity &GetOwner(unsigned int slot) const { return denseOwner[slot]; }
```

A system iterating "component C + component D" does: loop the (smaller) C pool's dense
array, and for each, `world.TryGet<D>(owner)` to fetch the paired component. No views,
no lambdas — just a `for` loop and a lookup.

---

## 4. The World — entities + one pool per type

```cpp
// ECS/World.h  (condensed)
class World
{
    static const unsigned int MAX_COMPONENT_TYPES = 64;

    // entity slots
    unsigned int *poGeneration;   // generation per slot
    unsigned int  mSlotCount;     // high-water: slots ever handed out
    unsigned int *poFreeList;     // recycled slot indices
    unsigned int  mFreeCount;

    // one pool per component type, indexed by C::kTypeId
    IComponentPool *poPools[MAX_COMPONENT_TYPES];

public:
    Entity Create();
    void   Destroy(const Entity &e);
    bool   IsAlive(const Entity &e) const;

    template<class C> C&                Add(const Entity&e){ return Pool<C>().Add(e); }
    template<class C> C*                TryGet(const Entity&e){ return Pool<C>().TryGet(e);}
    template<class C> bool              Has(const Entity&e){ return Pool<C>().Has(e); }
    template<class C> ComponentPool<C>& Pool();
};
```

Pools are created lazily, keyed by the component's id:

```cpp
template<class C> ComponentPool<C>& World::Pool()
{
    const unsigned int id = C::kTypeId;
    assert(id < MAX_COMPONENT_TYPES);
    if (poPools[id] == nullptr)
        poPools[id] = new ComponentPool<C>();          // first Add<C> creates the pool
    return *static_cast<ComponentPool<C>*>(poPools[id]);
}
```

`poPools` holds `IComponentPool*` (a type-erased base with a single virtual,
`OnEntityDestroyed`). That's the only reason the base exists: so `World::Destroy` can
tell *every* pool to drop a dead entity without knowing the concrete types.

### Entity lifecycle
```cpp
Entity World::Create()
{
    if (mFreeCount > 0) {                          // reuse a recycled slot
        unsigned int slot = poFreeList[--mFreeCount];
        return { slot, poGeneration[slot] };       // its generation was bumped on Destroy
    }
    if (mSlotCount >= mSlotCap) privGrowGenerations();
    poGeneration[mSlotCount] = 0;
    return { mSlotCount++, 0 };                     // brand-new slot
}

void World::Destroy(const Entity &e)
{
    if (!IsAlive(e)) return;
    for (unsigned int i = 0; i < MAX_COMPONENT_TYPES; i++)   // drop from every pool
        if (poPools[i]) poPools[i]->OnEntityDestroyed(e);
    poGeneration[e.index]++;                        // invalidate old handles
    privPushFree(e.index);                          // recycle the slot
}

bool World::IsAlive(const Entity &e) const
{ return e.index < mSlotCount && poGeneration[e.index] == e.generation; }
```

There is exactly one World, reached through a tiny singleton:

```cpp
// ECS/WorldMan.cpp
World &WorldMan::GetWorld() {
    if (posWorld == nullptr) posWorld = new World();   // lazy
    return *posWorld;
}
void WorldMan::Destroy() { delete posWorld; posWorld = nullptr; }   // scene unload
```

---

## 5. Systems — logic over the pools, in a fixed order

```cpp
// ECS/System.h
class System
{
public:
    virtual void Update(World &world, AnimTime tDelta) = 0;
};
```

`SystemMan` owns an ordered list of systems and drives the frame:

```cpp
// System/SystemMan.cpp
void SystemMan::Create()                         // once per scene
{
    posInstance = new SystemMan();
    Add(new LocalToWorldSystem());   // 1. world = S*R*T for every transform
    Add(new RotateSystem());         // 2. spins RotateComponent entities (overwrites world)
    Add(new AnimationSystem());      // 3. sample single-clip anims
    Add(new BlendSystem());          // 4. sample two-clip blends
    Add(new SkinningSystem());       // 5. dispatch GPU compute skinning
}

void SystemMan::Run(World &world, AnimTime tDelta)   // called every frame (Update)
{
    for (unsigned int i = 0; i < mCount; i++)
        poSystems[i]->Update(world, tDelta);
}

void SystemMan::Draw(World &world)                   // called every frame (Render)
{
    RenderSystem::Draw(world);        // 3D pass (layer-sorted)
    SpriteRenderSystem::Draw(world);  // 2D/UI pass
}
```

**Order matters and is explicit** (no dependency graph — just the array order).
`LocalToWorldSystem` composes the base world matrix; `RotateSystem` runs after it and
*overwrites* the world for spinning entities; the anim systems feed the skinning
dispatch. Each system is a no-op in scenes that don't have the matching component.

A representative system — this is the entire behavior loop for a spinning object:

```cpp
// System/RotateSystem.cpp
void RotateSystem::Update(World &world, AnimTime)
{
    ComponentPool<RotateComponent> &pool = world.Pool<RotateComponent>();
    for (unsigned int i = 0; i < pool.GetCount(); i++)     // linear sweep, packed
    {
        RotateComponent &rc = pool.GetData(i);
        const Entity    &e  = pool.GetOwner(i);
        TransformComponent *pT = world.TryGet<TransformComponent>(e);
        if (!pT) continue;

        rc.angle += rc.speed;
        Scale S(pT->scale.x(), pT->scale.y(), pT->scale.z());
        Rot   baseRot(pT->rot);
        Rot   spin(Rot1::Z, rc.angle);
        Trans T(pT->pos.x(), pT->pos.y(), pT->pos.z());
        pT->world = S * baseRot * spin * T;                // write the world matrix
    }
}
```

---

## 6. Full walkthrough — creating and drawing a cube (scene4)

### Step 1 — the scene creates the entity

Scenes use the `Renderable3D` factory instead of touching the World directly:

```cpp
// Factory/Renderable3D.cpp
Entity Renderable3D::Add(MaterialKind kind)
{
    World &w = WorldMan::GetWorld();
    Entity e = w.Create();                          // (A) new entity id

    TransformComponent &t = w.Add<TransformComponent>(e);   // (B) add + seed transform
    t.pos.set(0,0,0);  t.scale.set(1,1,1);  t.rot = Quat(Identity);  t.world = Mat4(Identity);

    RenderComponent &r = w.Add<RenderComponent>(e);         // (C) add render data (empty handles)
    r.kind = kind;  r.drawEnable = true;  r.layer = 0;
    r.pMesh = nullptr;  r.pShader = nullptr;  r.pTex = nullptr;  /* ... */
    return e;
}
```

Then scene4 fills it in:

```cpp
// Engine/src/scene4.cpp  (one cube)
Entity cube = Renderable3D::Add(MaterialKind::ColorByVertex);   // (A)(B)(C)
Renderable3D::SetTrans(cube, -220.0f, 0.0f, 0.0f);              // writes t.pos
Renderable3D::SetScale(cube, 52.0f);                            // writes t.scale

RenderComponent &r = Renderable3D::GetRender(cube);             // (D) fill the handles
r.pMesh   = MeshNodeMan::Find(Mesh::Name::CUBE);                //     shared cube mesh
r.pShader = ShaderObjectNodeMan::Find(ShaderObject::Name::ColorByVertex);

RotateComponent &rc = WorldMan::GetWorld().Add<RotateComponent>(cube);   // (E) behavior
rc.angle = 0.0f;  rc.speed = 0.01f;
```

**What is in memory now** (say this is entity `#5`):
```
World.poGeneration[5] = 0
TransformComponent pool:  sparse[5] -> denseData[k]  = { pos=(-220,0,0), scale=52, rot=I, world=? }
RenderComponent    pool:  sparse[5] -> denseData[m]  = { ColorByVertex, CUBE, ColorByVertex shader, ... }
RotateComponent    pool:  sparse[5] -> denseData[n]  = { angle=0, speed=0.01 }
```
No `new GameObject`, no tree node — just three entries appended to three packed arrays.

### Step 2 — per-frame update (the systems)

Each frame `Game::Update()` → the scene's `Update` → `SystemMan::Run(world, dt)`:

```
LocalToWorldSystem : for every TransformComponent -> world = Scale*Rot*Trans
RotateSystem       : for our cube (has RotateComponent) -> angle+=speed;
                     world = Scale * baseRot * spin(Z,angle) * Trans   (overwrites)
Animation/Blend/Skinning : no-op (cube has none of those components)
```

After this, `TransformComponent.world` for the cube holds the final model→world matrix
for this frame. Nothing has been drawn yet.

### Step 3 — per-frame draw (RenderSystem → GPU)

`Game::Render()` → `SystemMan::Draw(world)` → `RenderSystem::Draw(world)`:

```cpp
// System/RenderSystem.cpp  (condensed)
void RenderSystem::Draw(World &world)
{
    ComponentPool<RenderComponent> &pool = world.Pool<RenderComponent>();
    unsigned int *order = new unsigned int[pool.GetCount()];      // draw-order indices
    unsigned int n = 0;
    for (i in pool)                                              // 1. gather drawable 3D
        if (r.drawEnable && MaterialKindIs3D(r.kind)) order[n++] = i;

    insertion_sort(order, by ascending r.layer);                // 2. sort by layer

    ShaderObject *pLastShader = nullptr;
    for (k in [0,n))                                            // 3. draw each
    {
        RenderComponent    &r  = pool.GetData(order[k]);
        TransformComponent *pT = world.TryGet<TransformComponent>(pool.GetOwner(order[k]));

        if (r.pShader != pLastShader) {                         // bind shader once per run
            r.pShader->ActivateShader();
            r.pShader->ActivateCBV();
            pLastShader = r.pShader;
        }
        DrawObject(r, pT->world);                               // -> per-material branch
    }
    delete[] order;
}
```

`DrawObject` switches on `r.kind` to the right material function. For our cube:

```cpp
// the ColorByVertex branch -- reads only the RenderComponent handles + the world matrix
void RenderSystem::privDrawColorByVertex(RenderComponent &r, Mat4 &world)
{
    r.pMesh->ActivateMesh();                                          // bind vertex buffers
    r.pShader->TransferWorldViewProj(currentCamera, &world);          // upload matrices (CBV)
    r.pMesh->RenderIndexBuffer();                                     // bind index buffer + draw
}
```

### Step 4 — how it reaches the GPU buffer (the RHI)

This is the only API-specific part. The `Mesh` and `ShaderObject` own thin D3D11
wrappers; `StateDirectXMan::GetContext()` returns the raw `ID3D11DeviceContext*`.

```cpp
// Mesh::ActivateMesh()  -> bind vertex buffers to input slots
VBVBuffer_pos.SetActive(VertexSlot::Position);     // ID3D11: IASetVertexBuffers(slot 0, posVB)
VBVBuffer_color.SetActive(VertexSlot::Color);      //         IASetVertexBuffers(slot 1, colorVB)

// ShaderObject::TransferWorldViewProj(cam, &world)  -> upload the 3 matrices into constant buffers
ConstantBuff_World.Transfer(&world);               // ID3D11: map/update a constant buffer
ConstantBuff_View.Transfer(&cam->getViewMatrix());
ConstantBuff_Projection.Transfer(&cam->getProjMatrix());
// (ActivateShader/ActivateCBV earlier bound VS+PS+InputLayout and the CBV slots)

// Mesh::RenderIndexBuffer()  -> bind index buffer + issue the draw
IBVBuffer.SetActive();                                             // IASetIndexBuffer
ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
ctx->DrawIndexed(numIndices, 0, 0);                               // <-- the draw call
```

The vertex shader (HLSL) reads the vertex position + the three matrices from constant
buffers and outputs the clip-space position:
```hlsl
matrix Mat = mul(mul(vsWorldMatrix, vsViewMatrix), vsProjectionMatrix);
outValue.position = mul(float4(inPos.pos, 1.0f), Mat);
outValue.color    = float4(inColor.color, 1.0f);
```

That's the whole path: **entity id → components (data) → systems (compute the world
matrix) → RenderSystem (per-object) → Mesh/Shader RHI wrappers → `DrawIndexed`.**

---

## 7. The frame, end to end

```
Game::Update()                          Game::Render()
  scene->Update(dt)                       SystemMan::Draw(world)
    CameraNodeMan::Update()                 RenderSystem::Draw(world)      // 3D, layer-sorted
    SystemMan::Run(world, dt)                 gather RenderComponent pool
      LocalToWorldSystem   (world=S*R*T)      sort by layer
      RotateSystem         (spin)             per object: bind shader, DrawObject
      AnimationSystem                           -> Mesh/Shader -> DrawIndexed
      BlendSystem                             SpriteRenderSystem::Draw(world)  // 2D/UI
      SkinningSystem  (GPU compute dispatch)
```

Scene load calls `SystemMan::Create()` (builds the systems) and creates entities.
Scene unload calls `SystemMan::Destroy()` then `WorldMan::Destroy()` — the World's
destructor frees every pool (and each pool frees its own arrays), so everything is
released in one shot.

---

## 8. Why this layout is fast

- **Linear iteration.** A system walks a pool's dense array front-to-back — contiguous
  memory, hardware prefetch friendly, no pointer chasing, no vtable dispatch per object.
- **Data, not objects.** Components are POD-ish structs packed together, so a hot loop
  (e.g. 1,000,000 world-matrix updates) streams straight through cache.
- **Trivially parallelizable.** A pool sweep is a flat `for` loop with independent
  iterations — split the range across threads and it just works (the OOP tree walk
  can't).
- **O(1) everything.** Add / has / get / remove are all O(1) via the sparse↔dense
  mapping; remove is swap-remove so the array never fragments.

Where it does *not* help: **draw-call submission**. Drawing N objects is N graphics-API
calls regardless of ECS or OOP — that's a GPU/driver cost, not an object-model cost.
ECS wins on *managing* and *updating* huge object counts; to draw huge counts you still
need culling / instancing / batching on top.

---

## 9. Porting this to a Vulkan engine

**Keep verbatim (API-agnostic):**
- `Entity` (generational index), `ComponentPool<C>` (sparse set), `World` (pools by
  type id + entity lifecycle), the component structs, `System`/`SystemMan`, and every
  behavior/transform system (`LocalToWorldSystem`, `RotateSystem`, animation sampling).
  None of that knows what a GPU is.

**Rewrite only the render/RHI layer** (Section 6 step 4). The mapping:

| This engine (D3D11) | Vulkan equivalent |
|---|---|
| `RenderComponent { Mesh*, Shader*, ... }` handles | same — components still hold handles into your Vulkan resource caches |
| `RenderSystem::Draw` gather + layer sort | same idea; record into a `VkCommandBuffer` instead of immediate calls |
| `ShaderObject::ActivateShader` (bind VS/PS/InputLayout) | `vkCmdBindPipeline` (a `VkPipeline` per material) |
| `ConstantBuff_*::Transfer` + `ActivateCBV` (per-object matrices) | **push constants** for the small per-object world matrix, or a dynamic UBO / a big matrix SSBO indexed by draw; bind via `vkCmdBindDescriptorSets` |
| `Mesh::ActivateMesh` (`IASetVertexBuffers`) | `vkCmdBindVertexBuffers` |
| `Mesh::RenderIndexBuffer` (`IASetIndexBuffer` + `DrawIndexed`) | `vkCmdBindIndexBuffer` + `vkCmdDrawIndexed` |
| `StateDirectXMan::GetContext()` (immediate context) | a recorded `VkCommandBuffer` submitted to a `VkQueue` |
| shader-bind hoisting (bind once per run) | sort draws by pipeline/material to minimize `vkCmdBindPipeline`/descriptor rebinds |

Notes / upgrades that fit naturally once the ECS is in place:
- **Per-object data:** instead of updating a constant buffer per draw (the slow part
  here), put all world matrices in one SSBO and index it with `gl_InstanceIndex` /
  push-constant offset. The ECS already stores them contiguously in the transform pool,
  so you can memcpy the whole pool into the SSBO in one shot.
- **Culling as a system:** add a `FrustumCullSystem` that sweeps the transform pool and
  writes a visible flag / compacted draw list — a flat parallel loop the ECS makes easy.
- **Instancing / indirect:** group the render pool by mesh+material and emit
  `vkCmdDrawIndexedInstanced` or `vkCmdDrawIndexedIndirect` per group.
- **Multithreading:** record several secondary command buffers in parallel, one per
  slice of the render pool.

The takeaway: the ECS is the *CPU-side data architecture*; the graphics API is a
swappable back end reached only at the final "record a draw" step.
