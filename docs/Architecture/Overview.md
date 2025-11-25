# Piece Engine: A Modular Component Architecture Game Engine - Designed for Ultimate Modularity and Extensibility

## Overview

The Piece Engine is a multi-layered game development framework designed to offer flexibility and performance. Its architecture is built upon a core philosophy: **Maximum Modularity and Extensibility**. Every component of the engine is designed to be a self-contained, replaceable unit, allowing alternative implementations without modifying the engine's main source code. Our goal is to provide a solid and high-performance foundation while empowering developers to customize every aspect, from the low-level renderer to the high-level game logic.

This document provides a unified and organic view of the Piece Engine's architecture, encompassing everything from the low-level C++ backend to the high-level C# framework.

## The Piece Engine's Modular Component Architecture Philosophy: Dynamic and Modular Extensibility

The essence of the Piece Engine is to be a highly composable and customizable platform for game developers, offering a degree of modularity and extensibility that goes beyond simply replacing entire modules. Our engine allows the **exchange and extension of both fine and coarse parts of its architecture**, facilitating customization at all levels.

This philosophy of **Modular Component Architecture** permeates all layers, promoting:
*   **Modularity:** Independent components with clear interfaces.
*   **Extensibility:** Ease of adding new functionalities or replacing existing ones.
*   **Composition over Inheritance:** Favors building functionality through the combination of components.

This means that:

1.  **Total or Partial Replacement (Respecting Contracts):** If a developer needs to optimize a specific algorithm or functionality, they can write their own implementation and "plug it in" to the engine, replacing only that part. This applies to:
    *   **Graphics Backends (RAL/WAL):** Completely swap an implementation (e.g., OpenGL for Vulkan) or replace only a specific component within a backend (e.g., a new `IVertexBuffer` implementation within the OpenGL backend).
    *   **Intermediate C++ Layer:** Replace the entire `PieceIntermediate.dll` or, more granularly, plug in an optimized `RenderSystem`, a customized `ResourceManager`, or even a specific function, using Dependency Injection (DI) techniques and C++ Plugin Architecture.
    *   **High-Level C# Framework:** Create custom game components, management systems, or tools that integrate seamlessly, leveraging the robust .NET DI system.

2.  **Infinite Possibilities via Flexible Interfaces:** The engine's interfaces are designed to be as generic and non-restrictive as possible. They define contracts that allow the implementation of advanced and future techniques, even those not yet conceived. Our standard implementation can be a starting point, but the architecture *must* allow innovation without forcing the developer into a specific paradigm.

3.  **Ease of Injection and Dynamic Configuration:** The architecture is built to be inherently compatible with Dependency Injection (DI) and Inversion of Control (IoC) patterns.
    *   **For C#:** Components and services can be easily injected and replaced, similar to configuring services with `AddMyServices()` in a .NET host, allowing for the customization of framework and editor functionalities. This includes passing configuration options specific to each component.
    *   **For C++:** Modularity is facilitated by a Service Locator pattern within the intermediate layer. This Service Locator is configured by the high-level C# framework, which resolves C++ factory implementations (e.g., `IGraphicsDeviceFactory`) through its .NET DI system and passes them, along with specific configuration options, to the C++ layer via P/Invoke. This enables dynamic library loading and swapping of low-level C++ backends (like graphics or physics engines) to be orchestrated and configured directly from the C# application's DI setup, eliminating the need for external configuration files for backend selection.

4.  **Dynamic and Agnostic Communication Between Layers:** Communication between layers is designed to be flexible. Dependency injection and interfaces allow interaction to be dynamically configured, ensuring that layers can "talk" without being rigidly coupled to a specific implementation, maintaining technological agnosticism.

## Modular Structure of the Piece Engine

The engine is divided into three main layers, each with its own extension points and distinct responsibilities, with well-defined communication boundaries:

### 1. Low-Level Layer (C++ Backend: WAL/RAL/PAL)
This layer is in direct contact with the GPU and the operating system. It is implemented in C++ for maximum performance and control.
*   **Responsibility:** Provide the lowest-level abstractions for graphics hardware, the operating system, and physical simulation.
*   **Details:** See [Design Document - Low-Level Layer](../Architecture/Layers/LowLevel/DESIGN.md)

### 2. Intermediate Layer (C++: Orchestration and Management)
This layer, implemented in C++, acts as an orchestrator between the C++ backend and the high-level C# framework. It also incorporates a core `Job System` to manage CPU-bound tasks with maximum efficiency, distributing workloads (like physics, AI, culling, and asset decompression) across all available CPU cores using a task-based model, preventing bottlenecks and aligning the engine with Data-Oriented Design (DOD) principles. This layer is also responsible for advanced rendering and asset optimization techniques crucial for modern, high-performance game development.
*   **Responsibility:** Orchestrate the backend, manage GPU resources (caching, lifecycle), implement the high-level rendering pipeline (culling, sorting), and expose a stable C-compatible API.
*   **Details:** See [Design Document - Intermediate Layer](../Architecture/Layers/Intermediate/DESIGN.md)

### 3. High-Level Framework (C#: Game Logic and Tools)
This is the layer where C# game developers interact directly with the engine. It also provides a robust foundation for Intelligent Systems (AI), enabling advanced NPC behaviors and procedural content generation. The engine embraces a Data-Oriented Entity Component System (ECS) approach, moving beyond traditional object-oriented `Node-Component` models towards a data-centric design. This impacts the **C++ Intermediate Layer** by managing flat, contiguous arrays of component types for massive performance boosts through cache-friendly processing by the `JobSystem`.
*   **Responsibility:** Provide a user-friendly and high-level API for the C# game developer. Manage the scene graph, game objects, components, input, UI, and orchestrate rendering through the intermediate C++ layer.
*   **Details:** See [Design Document - High-Level Layer](../Architecture/Layers/HighLevel/DESIGN.md)

### 4. Visual Editor (C#: Development Tool)
The Visual Editor is a C# application built on top of the High-Level Framework. It provides an intuitive graphical interface for interacting with the engine, manipulating scenes, managing assets, and authoring content.
*   **Responsibility:** Provide a user-friendly and extensible environment for game development.
*   **Details:** See [Design Document - Visual Editor](../Architecture/EditorVisual/DESIGN.md)


## Data and Control Flow Across Layers

The engine's lifecycle and data flow follow a top-down and bottom-up pattern across the layers:

1.  **Initialization:** The `GameEngine` (C#) initializes its systems, which in turn initialize the intermediate C++ layer (via P/Invoke for `RenderSystemCpp`, `ResourceManagerCpp`), which then directly interacts with the C++ backend (WAL/RAL) to create devices and contexts.
2.  **Game Loop (`Update`):** The `GameEngine` (C#) calls `Update()` on systems (Input, Scene, etc.). The `InputManager` (C#) can query the `IWindow` (WAL C++ via P/Invoke). `Node`s and `Component`s (C#) update game logic.
3.  **Game Loop (`Draw`):** The `GameEngine` (C#) invokes the `RenderManager` (C#), which translates high-level rendering intentions to the `RenderSystemCpp`. The `RenderSystemCpp` orchestrates the rendering pipeline, interacting with `IGraphicsDevice` and `IRenderContext` (RAL C++) to issue drawing commands. The `IGraphicsDevice` manages `BeginFrame()` and `EndFrame()`.
4.  **Resource Management:** Assets are loaded via `AssetManager` (C#), which delegates to `ResourceManagerCpp`. This, in turn, uses `IGraphicsDevice` (RAL C++) to create GPU resources. C# wrappers hold `IntPtr`s for C++ resources and use `IDisposable` to signal release to the intermediate layer.

## Resource Management Across Layers: Ownership and Lifecycle

Resource management is crucial for the engine's stability and performance:

*   **Low-Level Layer (RAL):** GPU resources (buffers, textures, shaders) are created by `IGraphicsDevice` and returned as `std::unique_ptr`. This ensures that the ownership and lifecycle of these resources are automatically managed by RAII in C++, minimizing leaks.
*   **Intermediate Layer (C++):** The `ResourceManagerCpp` acts as the primary owner and cache for these `std::unique_ptr`s of RAL resources. It is responsible for loading assets from disk and creating corresponding GPU resources. It keeps resources alive as long as they are in use or cached.
*   **High-Level Layer (C#):** C# wrappers (e.g., `Mesh`, `Material`, `Texture`) are not direct owners of C++ resources. Instead, they hold an `IntPtr` to the C++ instance managed in the intermediate layer. The `IDisposable` interface in C# is used to notify the intermediate layer (via P/Invoke `_Destroy` functions) when a C# wrapper resource is no longer needed, allowing the intermediate layer to decide if the underlying C++ resource can be released (e.g., if there are no other references or if it's not in the cache).

## How to Contribute and Extend: Granular Extension Points (The Modular Component Toolbox)

The extensibility of the Piece Engine is granular, allowing you to customize the engine at various levels. This is most powerfully demonstrated by how the graphics backend is selected and loaded at runtime, fully realizing the philosophy of a modular component architecture. Beyond modularity, the engine's design also prioritizes developer workflow and productivity, integrating features like fast compilation times and Hot-Reloading of C++ code to minimize downtime and keep developers in a creative flow.

*   **Low-Level Layer (C++: WAL/RAL/PAL):**
    *   **Backend as a Plugin (C# DI Orchestration):** The core renderers and physics engines are implemented as C++ dynamic libraries (`.dll`, `.so`). Each of these libraries **must** export C-style factory functions (e.g., `CreateVulkanGraphicsDeviceFactory()`) that return instances of C++ factory interfaces (e.g., `IGraphicsDeviceFactory`).
    *   **Runtime Selection & Injection:** At startup, the high-level C# application uses its .NET DI system to select and register C# wrapper factories (e.g., `VulkanGraphicsDeviceFactory`). These C# wrappers then obtain the raw C++ factory pointers from the native backend DLLs and pass them to the intermediate C++ layer's `ServiceLocator` via P/Invoke. This allows the C# application to directly control which C++ backend implementation is injected and used.

*   **Intermediate Layer (C++: Orchestration and Management):**
    *   **Full Replacement:** Develop your own `PieceIntermediate.dll`, implementing the C-compatible API defined in `piece_intermediate_api.h`.
    *   **Partial Replacement/Extension (C# DI-driven):** The intermediate layer uses a C++ Service Locator, which is populated by the C# high-level framework. This means specific internal components (e.g., an optimized `RenderSystem`, a customized `ResourceManager`) can be injected and replaced by resolving their C++ factory implementations via the C# .NET DI system and passing them to the C++ Service Locator. This decouples the intermediate layer from concrete implementations, allowing for flexible customization orchestrated from the C# side.
    *   **Adding New Functionalities:** Create new C++ libraries that export additional C-compatible APIs for new systems (e.g., `MyCustomPhysics.dll`), which the C# layer can then consume.

*   **High-Level Framework (C#: Game Logic and Tools):**
    *   **Dependency Injection (DI .NET):** Use the .NET DI system to replace or extend any framework service (e.g., `IInputManager`, `IAssetManager`, `IRenderManager`) with your own implementations.
    *   **Component and System Creation:** Develop custom `Component`s, `Node`s, and `System`s to extend game logic in C#.
    *   **Embedded Multi-Language Scripting:** Go beyond C# by hosting interpreters for other languages (like Lua, Python, or JavaScript) inside special "bridge" components. This allows developers to write game logic in their preferred scripting language while maintaining a consistent C# project structure that is fully compatible with the visual editor.
    *   **Editor Extension:** Create custom panels, tools, or inspectors for the visual editor, integrating them with the C# framework.
    *   **Asset Pipelines:** Creation of custom asset import and processing pipelines.

## Final Considerations and Future Vision

The multi-layered architecture of the Piece Engine, founded on the principles of modular component architecture, balances performance, control, and flexibility. The clear separation of responsibilities and robust extension mechanisms in each layer empower developers to optimize and adapt the engine to the most diverse project needs.

The future vision includes the expansion of backend implementations (Vulkan, DirectX), the enhancement of editing tools, and the continuous optimization of communication between layers, always maintaining modularity and extensibility as core principles.




