# Azul Game Engine

## Overview
Azul Game Engine is a custom-built C++ game engine designed for high-performance 2D and 3D rendering. It features a robust asset pipeline, skeletal animation system with skinning support, and a flexible rendering architecture that wraps DirectX 11. The engine demonstrates advanced C++ programming techniques and design patterns.

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
    *   Build the `Engine` project.
    *   Run the `Engine` executable.

## Features & Implementation

### 1. Asset Pipeline & Converter
*   **Protobuf Integration**: Implemented a custom asset converter that processes raw assets (GLTF, XML) and serializes them into efficient Google Protocol Buffer (`.proto`) files.
*   **Mesh & Animation Conversion**:
    *   Converted mesh data (vertices, indices, UVs, normals) to `.proto` format.
    *   Extracted and converted skeletal animation data (clips, bones, keyframes) for runtime use.
    *   Handled texture and hierarchy conversion.

### 2. Animation System
*   **Skeletal Animation**: Implemented a complete skeletal animation system supporting hierarchical bone transformations.
*   **Skinning**: Implemented mesh skinning (Linear Blend Skinning) to deform meshes based on bone movements.
    *   Supported weighted vertices for smooth deformation.
*   **Animation Mixing**: System supports blending between different animation clips.

### 3. Rendering System
*   **DirectX Wrapper**: Encapsulated low-level DirectX API calls into high-level abstractions (`GraphicsObject`, `ShaderObject`, `Texture`, `Buffers`) to simplify rendering logic.
*   **Sprite Rendering**: Implemented a 2D sprite rendering system for UI and 2D game elements.
*   **3D Rendering**: Support for various shading models (Wireframe, Flat Texture, Phong, etc.).

### 4. Design Patterns
The engine architecture heavily utilizes industry-standard design patterns:
*   **Singleton Pattern**: Used for centralized resource managers (e.g., `TextureMan`, `ShaderMan`, `MeshNodeMan`, `GameObjectMan`).
*   **Manager Pattern**: Base `ManBase` class provides common functionality for resource management (adding, finding, removing resources).
*   **Factory Pattern**: Implicitly used within Managers to create and manage resource instances.
*   **Bridge/Strategy Pattern**: `GraphicsObject` acts as a bridge, decoupling the game object logic from the specific rendering implementation (Mesh + Shader).
*   **Command Pattern**: Used in the input system and potentially in the playback of animation commands.
*   **Proxy/Adapter Pattern**: Used in the `Proto` classes to adapt the serialized data on disk to the runtime engine structures.
*   **Iterator Pattern**: Used for traversing internal data structures (Linked Lists) within managers.
