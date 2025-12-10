# Piece Engine: A Modular Component Architecture Game Engine - Designed for Ultimate Modularity and Extensibility

## Overview

The Piece Engine is a multi-layered game development framework designed to offer flexibility and performance. Its architecture is built upon a core philosophy: **Maximum Modularity and Extensibility**. Every component of the engine is designed to be a self-contained, replaceable unit, allowing alternative implementations without modifying the engine's main source code. Our goal is to provide a solid and high-performance foundation while empowering developers to customize every aspect, from the low-level renderer to the high-level game logic.

Crucially, the engine employs a hybrid C++ and C# architecture where C++ low-level implementations are compiled on-demand via `MSBuild.targets` in NuGet packages, and then seamlessly integrated through Dependency Injection (DI) in C#. This allows the C++ core to remain entirely platform-agnostic and decoupled from specific low-level implementations.

This document provides a unified and organic view of the Piece Engine's architecture, encompassing everything from the low-level C++ implementation to the Piece.Framework (C#).

## The Piece Engine's Modular Component Architecture Philosophy: Dynamic and Modular Extensibility

The essence of the Piece Engine is to be a highly composable and customizable platform for game developers, offering a degree of modularity and extensibility that goes beyond simply replacing entire modules. Our engine allows the **exchange and extension of both fine and coarse parts of its architecture**, facilitating customization at all levels.

This philosophy of **Modular Component Architecture** permeates all layers, promoting:
*   **Modularity:** Independent components with clear interfaces.
*   **Extensibility:** Ease of adding new functionalities or replacing existing ones.
*   **Composition over Inheritance:** Favors building functionality through the combination of components.

This means that:

1.  **Total or Partial Replacement (Respecting Contracts):** If a developer needs to optimize a specific algorithm or functionality, they can write their own implementation and "plug it in" to the engine, replacing only that part. This applies to:
    *   **Graphics Implementations (RAL/WAL):** Completely swap an implementation (e.g., OpenGL for Vulkan) or replace only a specific component within an implementation (e.g., a new `IVertexBuffer` implementation within the OpenGL implementation).
    *   **Piece.Core (C++):** Replace the entire `PieceCore.dll` or, more granularly, plug in an optimized `RenderSystem`, a customized `ResourceManager`, or even a specific function, using Dependency Injection (DI) techniques and C++ Plugin Architecture.
    *   **Piece.Framework (C#):** Create custom game components, management systems, or tools that integrate seamlessly, leveraging the robust .NET DI system.

2.  **Infinite Possibilities via Flexible Interfaces:** The engine's interfaces are designed to be as generic and non-restrictive as possible. They define contracts that allow the implementation of advanced and future techniques, even those not yet conceived. Our standard implementation can be a starting point, but the architecture *must* allow innovation without forcing the developer into a specific paradigm.

3.  **Ease of Injection and Dynamic Configuration:** The architecture is built to be inherently compatible with Dependency Injection (DI) and Inversion of Control (IoC) patterns.
    *   **For C#:** Components and services can be easily injected and replaced, similar to configuring services with `AddMyServices()` in a .NET host, allowing for the customization of framework and editor functionalities. This includes passing configuration options specific to each component.
    *   **For C++ (orchestrated by C#):** Modularity for C++ low-level implementations is facilitated by a Service Locator pattern within the `piece_core`. This Service Locator is configured directly by the C# application. C# wrapper NuGet packages for C++ low-level implementations (e.g., `Piece.Glfw`, `Piece.OpenGL`) provide `Add...()` extension methods for `IServiceCollection`. These methods are responsible for:
        1.  Ensuring their native C++ low-level implementation DLLs are compiled (via `MSBuild.targets`) and available in the application's output directory.
        2.  Invoking a C-style `CreateFactory()` function exported by the native DLL to obtain a pointer to an already created C++ factory implementation (e.g., `IWindowFactory*`). The .NET runtime automatically handles loading the native DLL for this P/Invoke call.
        3.  Passing this factory pointer to the `piece_core` C++ layer via a simple P/Invoke call (e.g., `SetWindowFactory()`), where it's stored in the `ServiceLocator`.
        This approach enables dynamic loading and swapping of low-level C++ implementations (like graphics or physics engines) to be orchestrated and configured directly from the C# application's DI setup, eliminating the need for external configuration files for implementation selection.

4.  **Dynamic and Agnostic Communication Between Layers:** Communication between layers is designed to be flexible. Dependency injection and interfaces allow interaction to be dynamically configured, ensuring that layers can "talk" without being rigidly coupled to a specific implementation, maintaining technological agnosticism.

## Modular Structure of the Piece Engine

The engine is divided into three main layers, each with its own extension points and distinct responsibilities, with well-defined communication boundaries:

### 1. Low-Level Layer (C++ Implementations: WAL/RAL/PAL)
This layer is implemented in C++ for maximum performance and control. Each low-level implementation (e.g., GLFW for windowing, OpenGL for rendering) is packaged as a standalone dynamic library (`.dll`, `.so`) that strictly implements abstract interfaces (like `IWindowFactory` or `IGraphicsDeviceFactory`) and exports a C-style `CreateFactory()` function. These low-level implementations are **completely agnostic** to the `piece_core` C++ and are compiled on-demand via `MSBuild.targets` provided by their respective C# NuGet wrapper packages.
*   **Responsibility:** Provide concrete, platform-specific low-level implementations for low-level abstractions (graphics hardware, operating system interactions, physical simulation) without any direct dependency on the `piece_core` C++.
*   **Details:** See [Design Document - Low-Level Layer](../Architecture/Layers/LowLevel/DESIGN.md)

### 2. Piece.Core (C++: Orchestration and Management)
This layer, implemented in C++, acts as a **passive orchestrator and service container**. It does not initiate low-level implementation loading or configuration. Instead, it exposes a stable C-compatible API to the C# layer for core engine functions and provides a `Service Locator` to receive and store already-created C++ factory implementations (e.g., `IWindowFactory*`, `IGraphicsDeviceFactory*`) passed from the C# layer. It also incorporates a core `Job System` to manage CPU-bound tasks with maximum efficiency, distributing workloads (like physics, AI, culling, and asset decompression) across all available CPU cores using a task-based model, preventing bottlenecks and aligning the engine with Data-Oriented Design (DOD) principles. This layer is also responsible for advanced rendering and asset optimization techniques crucial for modern, high-performance game development.
*   **Responsibility:** Provide a central `Service Locator` for C++ low-level implementation factories, manage GPU resources (caching, lifecycle), implement the high-level rendering pipeline (culling, sorting), and expose a stable C-compatible API for core engine functions.
*   **Details:** See [Design Document - Piece.Core](../Architecture/Layers/Intermediate/DESIGN.md)

### 3. Piece.Framework (C#: Game Logic and Tools)
This is the layer where C# game developers interact directly with the engine. It also provides a robust foundation for Intelligent Systems (AI), enabling advanced NPC behaviors and procedural content generation. The engine embraces a Data-Oriented Entity Component System (ECS) approach, moving beyond traditional object-oriented `Node-Component` models towards a data-centric design. This impacts the **Piece.Core (C++)** by managing flat, contiguous arrays of component types for massive performance boosts through cache-friendly processing by the `JobSystem`.
*   **Responsibility:** Provide a user-friendly and high-level API for the C# game developer. Manage the scene graph, game objects, components, input, UI, and orchestrate rendering through the Piece.Core (C++).
*   **Details:** See [Design Document - Piece.Framework](../Architecture/Layers/HighLevel/DESIGN.md)

### 4. Visual Editor (C#: Development Tool)
The Visual Editor is a C# application built on top of the High-Level Framework. It provides an intuitive graphical interface for interacting with the engine, manipulating scenes, managing assets, and authoring content.
*   **Responsibility:** Provide a user-friendly and extensible environment for game development.
*   **Details:** See [Design Document - Visual Editor](../Architecture/EditorVisual/DESIGN.md)


## Data and Control Flow Across Layers

The engine's lifecycle and data flow follow a top-down and bottom-up pattern across the layers:

1.  **Initialization (Orchestrated by C#):** The C# application (typically through its DI setup) is responsible for ensuring the appropriate native C++ low-level implementation DLLs (e.g., `wal_glfw.dll`, `ral_opengl.dll`) are compiled and available in the application's output directory (via `MSBuild.targets`). It then calls the C-style `CreateFactory()` function exported by each low-level implementation DLL (using P/Invoke) to obtain pointers to `IWindowFactory`, `IGraphicsDeviceFactory`, etc. These factory pointers are then passed to the `piece_core` C++ via P/Invoke (`SetWindowFactory()`, `SetGraphicsDeviceFactory()`) where they are stored in the `ServiceLocator`. Only after all necessary C++ factories are registered does the `GameEngine` (C#) call `Engine_Initialize()` on the `piece_core` C++, which then retrieves the registered factories from the `ServiceLocator` to create devices and contexts.
2.  **Game Loop (`Update`):** The `GameEngine` (C#) calls `Update()` on systems (Input, Scene, etc.). The `InputManager` (C#) can query the `IWindow` (WAL C++ via P/Invoke). `Node`s and `Component`s (C#) update game logic.
3.  **Game Loop (`Draw`):** The `GameEngine` (C#) invokes the `RenderManager` (C#), which translates high-level rendering intentions to the `RenderSystemCpp`. The `RenderSystemCpp` orchestrates the rendering pipeline, interacting with `IGraphicsDevice` and `IRenderContext` (RAL C++) to issue drawing commands. (Through the Piece.Core (C++)) The `IGraphicsDevice` manages `BeginFrame()` and `EndFrame()`.
4.  **Resource Management:** Assets are loaded via `AssetManager` (C#), which delegates to `ResourceManagerCpp`. This, in turn, uses `IGraphicsDevice` (RAL C++) to create GPU resources. C# wrappers hold `IntPtr`s for C++ resources and use `IDisposable` to signal release to the intermediate layer.

## How to Contribute and Extend: Granular Extension Points (The Modular Component Toolbox)

The extensibility of the Piece Engine is granular, allowing you to customize the engine at various levels. This is most powerfully demonstrated by how the graphics low-level implementation is selected and loaded at runtime, fully realizing the philosophy of a modular component architecture. Beyond modularity, the engine's design also prioritizes developer workflow and productivity, integrating features like fast compilation times and Hot-Reloading of C++ code to minimize downtime and keep developers in a creative flow.

*   **Low-Level Layer (C++: WAL/RAL/PAL):**
    *   **Low-Level Implementation as a Plugin (C# DI Orchestration):** The core renderers, windowing systems, and physics engines are implemented as standalone C++ dynamic libraries (`.dll`, `.so`). Each of these libraries **must** implement abstract interfaces (like `IWindowFactory` or `IGraphicsDeviceFactory`) and export a single C-style `CreateFactory()` function that creates and returns a raw pointer to its factory implementation (e.g., `IWindowFactory* CreateFactory() { return new GlfwWindowFactory(); }`). These C++ low-level implementations have **no direct dependency** on the `piece_core` C++.
    *   **Orchestration via C# NuGet Wrappers:** The integration of these C++ low-level implementations is entirely orchestrated by their corresponding C# NuGet wrapper packages (e.g., `Piece.Glfw`, `Piece.OpenGL`).
        1.  These NuGet packages contain the C++ source code and an `MSBuild.targets` file that compiles the native C++ library for the target platform during the C# project's build process.
        2.  The C# code within these NuGet wrappers uses standard .NET P/Invoke to call the `CreateFactory()` function directly from the compiled native DLL, obtaining a raw `IntPtr` to the C++ factory.
        3.  This `IntPtr` is then passed to the `piece_core` C++ layer via a simple P/Invoke call (e.g., `NativeCalls.SetWindowFactory(IntPtr factoryPtr)`), where the `piece_core` assumes ownership and stores it in its `ServiceLocator`.
        This robust approach ensures maximum decoupling, platform-native compilation, and full control over low-level implementation selection and injection directly from the C# application's Dependency Injection setup.

*   **Piece.Core (C++: Orchestration and Management):**
    *   **Full Replacement:** Develop your own `PieceCore.dll`, implementing the C-compatible API defined in `piece_core_api.h`.
    *   **Partial Replacement/Extension (C# DI-driven):** The Piece.Core uses a C++ Service Locator, which is populated by the Piece.Framework (C#). This means specific internal components (e.g., an optimized `RenderSystem`, a customized `ResourceManager`) can be injected and replaced by resolving their C++ factory implementations via the C# .NET DI system and passing them to the C++ Service Locator. This decouples the Piece.Core from concrete implementations, allowing for flexible customization orchestrated from the C# side.
    *   **Adding New Functionalities:** Create new C++ libraries that export additional C-compatible APIs for new systems (e.g., `MyCustomPhysics.dll`), which the C# layer can then consume.

*   **Piece.Framework (C#: Game Logic and Tools):**
    *   **Dependency Injection (DI .NET):** Use the .NET DI system to replace or extend any framework service (e.g., `IInputManager`, `IAssetManager`, `IRenderManager`) with your own implementations.
    *   **Component and System Creation:** Develop custom `Component`s, `Node`s, and `System`s to extend game logic in C#.
    *   **Embedded Multi-Language Scripting:** Go beyond C# by hosting interpreters for other languages (like Lua, Python, or JavaScript) inside special "bridge" components. This allows developers to write game logic in their preferred scripting language while maintaining a consistent C# project structure that is fully compatible with the visual editor.
    *   **Editor Extension:** Create custom panels, tools, or inspectors for the visual editor, integrating them with the C# framework.
    *   **Asset Pipelines:** Creation of custom asset import and processing pipelines.

## Final Considerations and Future Vision

The multi-layered architecture of the Piece Engine, founded on the principles of modular component architecture, balances performance, control, and flexibility. The clear separation of responsibilities and robust extension mechanisms in each layer empower developers to optimize and adapt the engine to the most diverse project needs.

## Multi-Platform Strategy

The Piece Engine's core philosophy of "Maximum Modularity and Extensibility" and its "Modular Component Architecture" provide an ideal foundation for supporting a wide array of platforms, including desktop (Windows, macOS, Linux), consoles (PS5, Xbox Series, Nintendo Switch), mobile (Android, iOS), and web browsers. Our strategy for achieving broad multi-platform support leverages the engine's layered design, the flexible C# orchestration, and the clear separation of concerns inherent in its architecture.

**Strategic Pillars for Multi-Platform Support:**

1.  **Platform-Native Low-Level Implementations (WAL/RAL/PAL) via NuGet/MSBuild.targets:**
    *   **Approach:** Dedicated C++ dynamic libraries (`.dll` or `.so`) are implemented for each target platform to fulfill the `WAL`, `RAL`, and `PAL` interfaces. These C++ low-level implementations are distributed as part of C# NuGet packages (e.g., `Piece.Glfw`, `Piece.OpenGL`).
    *   **Cross-Compilation Orchestration:** Each NuGet package includes an `MSBuild.targets` file that, during the C# project's build process, automatically invokes CMake and Vcpkg to compile the native C++ low-level implementation source code. This ensures that the native libraries are built *on the developer's machine*, for their *specific target platform*, using the correct toolchain (e.g., `clang` for macOS/Linux, MSVC for Windows). This eliminates pre-compiled binary distribution issues and allows for truly flexible multi-platform support.
    *   **Seamless Integration:** The compiled native DLLs are then seamlessly loaded and integrated into the `piece_core` C++ via simple P/Invoke calls orchestrated by the C# application's Dependency Injection system.

2.  **Adaptation of the High-Level C# Framework:**
    *   **Approach:** Ensure the High-Level C# Framework (game logic, UI, asset management) is compatible with target platform runtimes. This may involve integrating with specific .NET implementations (e.g., Mono/Xamarin for mobile, Blazor WebAssembly for web, or console-specific .NET versions).
    *   **Input & System Services:** Adapt and extend the C# input management system to handle platform-specific input mechanisms (e.g., touch gestures, specialized console controller features) and integrate with other platform-specific system services as needed.

**Phased Rollout Strategy:**

The expansion to new platforms will follow a phased approach, prioritizing platforms based on strategic importance and development effort:

*   **Phase 1 (Desktop Expansion):** Enhance existing desktop support to include robust macOS and Linux low-level implementations, leveraging GLFW's cross-platform capabilities where appropriate, or native APIs for deeper integration. The compilation of these low-level implementations will be managed by the NuGet/MSBuild.targets strategy.
*   **Phase 2 (Mobile Platforms):** Develop dedicated WAL/RAL low-level implementations for Android (Vulkan/OpenGL ES) and iOS (Metal) and integrate with their respective build systems and C# runtimes, adhering to the NuGet/MSBuild.targets compilation model.
*   **Phase 3 (Web Browsers):** Target WebAssembly/WebGL via Emscripten for web deployment, ensuring C# framework compatibility (e.g., Blazor WASM) and efficient interop, with C++ compilation for WASM handled by MSBuild.targets.
*   **Phase 4 (Console Platforms):** Initiate development for consoles (PS5, Xbox Series, Nintendo Switch), recognizing the significant effort required for proprietary SDK integration, specialized low-level implementations, and strict certification processes. This phase will be contingent on securing necessary development kits and licenses, with native compilation orchestrated via MSBuild.targets where feasible.

This strategic roadmap ensures a systematic and modular approach to multi-platform support, building upon the engine's architectural strengths to provide developers with extensive deployment options.
