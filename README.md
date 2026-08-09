# Azul Game Engine

## Overview
Azul Game Engine is a custom-built C++ game engine designed for high-performance 2D and 3D rendering. It features a robust asset pipeline, a skeletal animation system with GPU compute skinning, and a flexible rendering architecture that wraps DirectX 11.

The engine has been **re-architected around a hand-rolled Entity-Component-System**. The previous object-oriented model — a `PCSTree` of `GameObject` subclasses, each with virtual `Update()`/`Draw()` and a `GraphicsObject` bridge — has been replaced by entities, plain-data components stored in packed arrays, and systems that sweep those arrays once per frame. The engine also now builds as a **DLL** (`AzulEngine`) with the game as a thin client on top.

> Deep dive: [`ECS_Setup.md`](ECS_Setup.md) walks the whole ECS from `Entity` → components → systems → GPU buffer, and maps the draw step onto Vulkan.


## Click here for the demo

[![Watch the demo](https://img.youtube.com/vi/Rj3A31DF2R4/maxresdefault.jpg)](https://youtu.be/Rj3A31DF2R4)

## Prerequisites
*   **Visual Studio 2022** (or newer)
*   **C++14.4** (MSVC Toolset)
*   **Windows SDK**

## How to Run

1.  **Asset Conversion**:
    *   Open the solution file `Converter.sln`.
    *   Build the `Converter` project (Release/Debug).
    *   Run the `Converter` executable first. This process converts raw assets (GLTF models, textures, animations) into the engine's custom Protobuf-based binary format.
    *   Ensure the converted files are generated in the `Data/` directory.

2.  **Run the Engine**:
    *   Open the solution file `Engine.sln`.
    *   Build the `Engine` project (this builds the `AzulEngine` DLL and the game executable that links it).
    *   Run the `Engine` executable.

## Project Layout

| Path | Contents |
|---|---|
| `Libs/AzulEngine/src/` | The engine, built as a DLL. ECS core, systems, components, renderer, animation. |
| `Engine/src/` | The game: `main`, `Game`, the scene state machine, and `scene1`–`scene5`. |
| `Converter/` | Offline asset pipeline (GLTF/XML → Protobuf). |
| `Libs/` | Supporting libraries: `Math`, `AnimTime`, `File`, `PCSTree`, `Manager`, `ProtoBuf`, `PugiXml`. |

Everything the game needs crosses the DLL boundary through `AZUL_ENGINE_LIBRARY_API`. `Engine/src` is now ~10 files; it was 200+ before the split.

---

## Architecture: Entity-Component-System

### The three pieces

*   **Entity** — an id only: a slot `index` plus a `generation` counter. No data, no behavior. The generation is bumped when a slot is recycled, so a stale handle to a destroyed entity is detectable.
*   **Component** — a plain `struct` of data with no logic, stored in a tightly packed array.
*   **System** — logic that sweeps one component array each frame.

```
Entity 42  ─┬─ TransformComponent { pos, rot, scale, world }
            ├─ RenderComponent    { kind, mesh, shader, tex, ... }
            └─ RotateComponent    { angle, speed }
```

### Storage: sparse sets

Each component type gets its own `ComponentPool<C>` (`ECS/ComponentPool.h`), a sparse set built from three hand-managed arrays:

```
sparse[entity.index] -> dense slot, or ECS_INVALID_INDEX
denseData[slot]      -> the component
denseOwner[slot]     -> the owning Entity (parallel to denseData)
```

Live components stay contiguous, so "iterate every transform" is a linear walk over memory instead of chasing pointers through a tree. Removal is an O(1) swap-with-last, and the `denseOwner` generation check rejects stale handles.

The `World` (`ECS/World.h`) owns the entity slots and one pool per component type, indexed by that type's id. **Ids are explicit compile-time constants**, not a runtime counter — a templated counter's function-local static would be instantiated separately in the DLL and the app and could hand back different ids for the same type, corrupting the pool lookup.

### Component registry

Ids live in `Component/ComponentId.h`. 64 slots total, split so game code can add its own without colliding with the engine:

| Range | Owner |
|---|---|
| `[0, 32)` | Engine components |
| `[32, 64)` | Reserved for game/app components |

Engine components: `Transform`, `Render`, `Light`, `GpuSkin`, `AnimClip`, `AnimBlend`, `Rotate`, `Sprite2D`, `Text`.

### Frame flow

`SystemMan` (`System/SystemMan.h`) is the per-scene frame driver. Update systems run in a fixed order, then the two draw passes:

```
SystemMan::Run(world, dt)
  1. LocalToWorldSystem   world = S * R * T over the transform pool
  2. RotateSystem         spins RotateComponent entities
  3. AnimationSystem      samples clips into the mixer buffers
  4. BlendSystem          blends between clips
  5. SkinningSystem       dispatches GPU compute skinning

SystemMan::Draw(world)
  6. RenderSystem         3D pass: layer-sorted, per-MaterialKind
  7. SpriteRenderSystem   2D/UI pass: sprites + text
```

Each system is a no-op in scenes without the matching components, so a scene only pays for what it uses.

### Rendering

`RenderSystem::Draw` gathers the draw-enabled 3D entities from the `RenderComponent` pool, stable insertion-sorts them by `layer`, then draws each through a `MaterialKind` switch (`ColorByVertex`, `FlatTexture`, `ConstColorLight`, `Wireframe`, `SkinLightTexture`). The shader and its constant buffer are bound **once per contiguous run of same-shader draws**, which drops the redundant `ActivateShader`/`ActivateCBV` binds the old per-object path issued.

`Factory/Renderable3D` is the convenience constructor: `Renderable3D::Add(kind)` creates the entity with a seeded identity transform and a `RenderComponent`, and the caller fills in the mesh/shader/texture handles.

`Instance/InstanceRenderer` is kept as a **GPU-instancing reference** — it documents how a single `DrawIndexedInstanced` over the dense transform array was wired. It is excluded from the build in `premake5.lua`.

### Coding constraints

*   **No STL.** Every array is a raw `new[]`/`delete[]` block with an explicit count and capacity, grown by allocate-bigger-and-copy.
*   **Templates only for component storage.** `ComponentPool<C>` and `World::Add/TryGet/Has/Pool` are header-only because templates can't be DLL-exported; all non-generic bookkeeping lives in `World.cpp` behind the export macro.
*   **No lambdas or view objects.** Systems iterate a pool's dense array with an explicit loop and ask the `World` for whatever else they need.

---

## Features & Implementation

### 1. Asset Pipeline & Converter
*   **Protobuf Integration**: A custom asset converter processes raw assets (GLTF, XML) and serializes them into efficient Google Protocol Buffer (`.proto`) files.
*   **Mesh & Animation Conversion**:
    *   Mesh data (vertices, indices, UVs, normals) converted to `.proto` format.
    *   Skeletal animation data (clips, bones, keyframes) extracted and converted for runtime use.
    *   Texture and hierarchy conversion.

### 2. Animation System
*   **Skeletal Animation**: Hierarchical bone transformations driven by `AnimClipComponent` / `AnimBlendComponent` and the Animation/Blend systems.
*   **Skinning**: Linear Blend Skinning with weighted vertices, dispatched as a **GPU compute pass** (`ComputeBlend`) by `SkinningSystem`. `RenderSystem` then binds the bone-world SRV that pass produced.
*   **Animation Mixing**: Blending between clips via the `Mixer` chain.

### 3. Rendering System
*   **DirectX Wrapper**: Low-level DirectX calls encapsulated behind `ShaderObject`, `TextureObject`, `Mesh`, and the `Buffer*` / `State*` families.
*   **3D Rendering**: Five material kinds — wireframe, flat texture, per-vertex color, constant-color-plus-light, and skinned-lit.
*   **Sprite & Text Rendering**: 2D pass for UI, driven by `Sprite2DComponent` and `TextComponent`.

### 4. Profiling
`Profiler` (`Profiler/Profiler.h`) is a rolling frame profiler wired into `SystemMan`. Every 60 frames it prints averaged **update ms**, **draw ms**, and **draw-call count** via `Trace::out`:

```
[PROFILE] frame 8.412 ms (~119 fps cpu) | update 1.203 ms | draw 7.209 ms | 27000.0 draw calls
```

Update and draw are timed separately on purpose. At high object counts the frame is dominated by D3D11 draw submission, so **FPS alone will not show the ECS difference** — the update column is where the architecture change is visible. Toggle the whole thing with `Profiler::SetEnabled(false)`, which also zeroes the overhead.

### 5. Scenes
| Scene | Demonstrates |
|---|---|
| 1–2 | Skeletal animation, blending, multiple characters, 2D text overlay |
| 3 | Basic 3D renderables through the ECS factory |
| 4 | Mixed material kinds (`ConstColorLight`, `Wireframe`) with rotation |
| 5 | Stress test: a 30³ grid — **27,000 individually-drawn rotating cubes**, one entity each, no instancing |

---

## Design Patterns

*   **Entity-Component-System**: The core architecture. Replaces the inheritance hierarchy (`GameObject` → `GameObjectRigidBody` / `GameObjectAnimSkin` / …) with composition over packed data.
*   **Sparse Set**: Component storage — O(1) add/remove/lookup with a dense, cache-friendly iteration array.
*   **Handle + Generation**: Entities are versioned indices rather than raw pointers, making dangling references detectable instead of undefined.
*   **Singleton Pattern**: Centralized managers — `WorldMan`, `SystemMan`, `MeshNodeMan`, `TexNodeMan`, `ShaderObjectNodeMan`, `CameraNodeMan`.
*   **Manager Pattern**: Base `ManBase` class provides common resource management (add, find, remove).
*   **Factory Pattern**: `Renderable3D` and `Text2D` assemble the correct component sets for an entity.
*   **Strategy Pattern**: `CompareStrategy` classes parameterize how each manager's lookup matches.
*   **Proxy/Adapter Pattern**: The `*Proto` classes adapt on-disk serialized data to runtime engine structures.
*   **Iterator Pattern**: Used for traversing manager-internal linked lists.

### What the ECS replaced

| Old (OO) | New (ECS) |
|---|---|
| `GameObjectMan` walking a `PCSTree` with `PCSTreeForwardIterator` | `SystemMan` sweeping packed component arrays |
| Virtual `GameObject::Update()` / `Draw()` per node | Systems iterating one pool, no virtual dispatch per object |
| `GraphicsObject` bridge, one subclass per material | `RenderComponent` + a `MaterialKind` switch |
| ~6 heap allocations per object (`Mat4`, `Vec3`×2, `Quat`, node, graphics object) | 2 array slots, 0 per-object allocations |
| Adding a material = 3 new classes | Adding a material = 1 enum value + 1 branch |
