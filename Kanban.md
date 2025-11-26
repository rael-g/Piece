# Piece Engine Development Kanban Board

## Backlog (Post v1.0.0 Features & Enhancements)

### Implement Advanced Rendering Pipelines
  - tags: [rendering, advanced]
  - priority: medium
  - steps:
      - [ ] Deferred Shading
      - [ ] Shadow Mapping (Cascaded Shadow Maps, VSM)
      - [ ] Global Illumination (SSAO, Ray-Traced GI)
      - [ ] Reflections (SSR, Ray-Traced Reflections)
      - [ ] Physically Based Rendering (PBR) Materials
      - [ ] Compute Shader integration for various effects (e.g., Culling, Particle Systems)
      - [ ] GPU-Driven Rendering (Indirect Draws, Mesh Shaders)
      - [ ] Ray Tracing integration (RTX, Vulkan Ray Tracing)
      - [ ] Variable Rate Shading (VRS)
  ```md
  Implement a comprehensive suite of advanced rendering techniques to ensure state-of-the-art visual quality and performance. This includes various lighting models, shadow algorithms, global illumination, and modern rendering APIs features.
  ```

### Integrate Full Physics Engine (PAL)
  - tags: [physics, pal]
  - priority: medium
  - steps:
      - [ ] Jolt Physics integration
      - [ ] Physics Body creation and manipulation
      - [ ] Collision detection and response
      - [ ] Joints and constraints
  ```md
  Integrate a robust Physics Abstraction Layer (PAL) with a chosen physics engine (e.g., Jolt Physics) to handle realistic object interactions, collisions, and physical simulations. (Beyond minimal 1.0.0 PAL)
  ```

### Develop Advanced AI System
  - tags: [ai, scripting]
  - priority: medium
  - steps:
      - [ ] Behavior Tree Editor and Runtime
      - [ ] Navigation Mesh (NavMesh) generation and pathfinding
      - [ ] Flocking, Boids, and other crowd simulations
  ```md
  Develop a flexible and powerful AI system to enable complex NPC behaviors, pathfinding, and procedural content generation. This includes tools for AI authoring and integration with game logic.
  ```

### Scripting Language Integration (C# Bridge Components)
  - tags: [scripting, csharp]
  - priority: low
  - steps:
      - [ ] Lua Scripting (via NLua/MoonSharp)
      - [ ] Python Scripting (via IronPython/Python.NET)
  ```md
  Integrate support for popular scripting languages (Lua, Python) through C# bridge components, allowing game developers to write logic in their preferred language while leveraging the C# framework and editor.
  ```

### Develop Full-Featured Visual Editor (C#)
  - tags: [editor, ui, csharp]
  - priority: medium
  - steps:
      - [ ] Advanced Asset Browser with previews and metadata editing
      - [ ] Comprehensive Scene Graph manipulation (grouping, locking, advanced selection)
      - [ ] Custom property editors for diverse component types
      - [ ] Undo/Redo System for all editor operations
      - [ ] Integrated debugging tools
  ```md
  Evolve the minimal visual editor into a full-featured development environment, including advanced asset management, scene manipulation, and debugging capabilities. (Beyond minimal 1.0.0 Editor)
  ```

### Dynamic Asset Streaming
  - tags: [performance, assets]
  - priority: medium
  - steps:
      - [ ] Implement asset prioritization based on camera proximity/visibility
      - [ ] Integrate background loading and unloading of assets
      - [ ] Manage memory budgets for streamed assets
  ```md
  Implement a system for dynamically streaming assets into and out of memory based on runtime needs, optimizing memory usage and load times for large scenes.
  ```

### Hot-Reloading of C++ Code
  - tags: [workflow, c++, live-coding]
  - priority: low
  - steps:
      - [ ] Research dynamic library loading/unloading mechanisms for C++
      - [ ] Implement live-reloading for C++ components in development
  ```md
  Enable hot-reloading of C++ code during development to accelerate iteration times and improve developer workflow.
  ```

### Implement Multi-Platform Strategy
  - tags: [multi-platform, core, infrastructure, strategy]
  - priority: high
  - steps:
      - [ ] Phase 1 (Desktop Expansion): Implement macOS and Linux WAL/RAL backends.
      - [ ] Phase 2 (Mobile Platforms): Implement Android (Vulkan/OpenGL ES) and iOS (Metal) WAL/RAL backends, integrate toolchains/runtimes.
      - [ ] Phase 3 (Web Browsers): Implement WebAssembly/WebGL WAL/RAL backends via Emscripten, integrate C# runtime (e.g., Blazor WASM).
      - [ ] Phase 4 (Console Platforms): Research and initiate development for PS5, Xbox Series, Nintendo Switch WAL/RAL backends (requires SDKs/licenses).
      - [ ] Extend CMake build system for cross-compilation across all target platforms.
      - [ ] Adapt C# Framework (input, system services, runtime) for each target platform.
      - [ ] Implement platform-specific performance profiling and optimization.
  ```md
  Execute the defined Multi-Platform Strategy to expand the Piece Engine's support to a broad range of platforms including desktop, mobile, web, and consoles. This involves developing platform-specific low-level backends, adapting the build system, and integrating appropriate C# runtimes.
  ```

## To Do (Goals for v1.0.0 - Minimal Viable Engine)

### Phase 0: Logging Integration Setup
  - tags: [v1.0.0, logging, cross-cutting]
  - priority: critical
  - steps:
      - [ ] Implement robust cross-language logging setup for C++ (`spdlog`) and C# (`Serilog`).
      - [ ] **Test:** Verify logging system works correctly across C++ and C# with various log levels and sinks.
      - [x] Create `docs/Development/LoggingStrategy.md` outlining the logging strategy.
      - [x] Add reference to `LoggingStrategy.md` in `docs/Architecture/Overview.md`.
  ```md
  Establish the foundational logging infrastructure for both C++ and C# components, ensuring consistent, high-performance, and cross-language log capture.
  ```

### Phase 0: Logging Integration Setup
  - tags: [v1.0.0, logging, cross-cutting]
  - priority: critical
  - steps:
      - [ ] Implement robust cross-language logging setup for C++ (`spdlog`) and C# (`Serilog`).
      - [ ] **Test:** Verify logging system works correctly across C++ and C# with various log levels and sinks.
      - [x] Create `docs/Development/LoggingStrategy.md` outlining the logging strategy.
      - [x] Add reference to `LoggingStrategy.md` in `docs/Architecture/Overview.md`.
  ```md
  Establish the foundational logging infrastructure for both C++ and C# components, ensuring consistent, high-performance, and cross-language log capture.
  ```

### Phase 1: Foundation & Build System
  - tags: [v1.0.0, setup, foundation, build]
  - priority: high
  - steps:
      - [x] Finalize root `CMakeLists.txt` for C++ project structure.
      - [x] Finalize `CMakePresets.json` for standardized build configurations (Windows, Linux, Debug, Release).
      - [x] Finalize `vcpkg.json` for C++ dependencies (e.g., GLFW, GLM).
      - [x] Set up initial `src/csharp/Piece.sln` with `Piece.Engine` and `Piece.Editor` projects.
      - [x] Establish `Piece.Intermediate.Abstractions` C# project for P/Invoke interfaces and factory definitions.
      - [-] Implement `Safe Interop Encapsulation` for all P/Invoke boundaries in dedicated C# interop projects.
      - [x] Implement basic CI/CD pipeline (GitHub Actions) for automated builds and tests on PRs and `main` branch.
      - [ ] Implement Logging for Foundation & Build System.
      - [ ] Implement Tests for Foundation & Build System.
  ```md
  Establish the complete foundational build system for both C++ and C# components, ensuring proper project structure, dependency management, interop safety, and automated build processes.
  ```

### Phase 2: Core C++ Layer Implementations
  - tags: [v1.0.0, c++, low-level, intermediate, wal, ral, pal]
  - priority: high
  - steps:
      - [x] Implement C++ `ServiceLocator` (singleton) for backend factory injection.
      - [x] Write unit tests for `ServiceLocator`.
      - [x] Define C-compatible API in `NativeExports.h` for core engine functions to be consumed by C#.
      - [x] Implement `EngineCore` C++ class, relying on `ServiceLocator` for backend instantiation.
      - [x] Write integration tests for `EngineCore` and `ServiceLocator`.
      - [ ] **WAL (GLFW Backend):**
          - [x] Fully implement `IWindow` interface.
          - [x] Implement `GlfwWindow` class.
          - [x] Write integration tests for `GlfwWindow`.
          - [x] Implement `IWindowFactory` and `GlfwWindowFactory`.
          - [x] Write unit tests for `GlfwWindowFactory`.
          - [x] Export `CreateGlfwWindowFactory()` C-style function from `gfx_glfw` DLL.
          - [x] Write integration tests for `CreateGlfwWindowFactory` export.
      - [ ] **RAL (OpenGL Backend):**
          - [ ] Fully implement `IGraphicsDevice`, `IRenderContext` interfaces.
          - [ ] Implement `OpenGLGraphicsDevice` and `OpenGLRenderContext` classes.
          - [ ] Implement core RAL resource interfaces: `IVertexBuffer`, `IIndexBuffer`, `IShader`, `IShaderProgram`.
          - [ ] Implement OpenGL-specific resource classes.
          - [ ] Implement `IGraphicsDeviceFactory` and `OpenGLGraphicsDeviceFactory`.
          - [ ] Export `CreateOpenGLGraphicsDeviceFactory()` C-style function from `gfx_opengl` DLL.
      - [ ] **PAL (Minimal Backend):**
          - [x] Define `IPhysicsWorld` and `IPhysicsBody` interfaces.
          - [ ] Write tests for PAL interfaces through a minimal backend implementation.
          - [ ] Implement a minimal physics backend (e.g., a basic collision detection placeholder or simple AABB physics).
          - [ ] Implement `IPhysicsWorldFactory` and its minimal backend implementation.
          - [ ] Export C-style factory function for the minimal PAL backend.
      - [ ] Implement a basic `JobSystem` (thread pool) for future asynchronous tasks.
      - [ ] Implement a minimal `ResourceManager` for loading basic mesh, texture, and shader assets.
      - [ ] Implement core `Material`, `Mesh`, `Model`, `Camera`, `Light` C++ classes that utilize RAL resources.
      - [ ] Implement a functional `RenderSystem` to draw simple `Model`s with a basic camera and light.
      - [ ] Implement `PhysicsSystemCpp` to manage the minimal PAL backend.
      - [ ] Implement Logging for Core C++ Layer Implementations.
      - [ ] Implement Tests for Core C++ Layer Implementations.
  ```md
  Develop the core C++ engine components, including minimal but functional implementations of the Window, Render, and Physics Abstraction Layers (WAL/RAL/PAL) with at least one backend each. Establish the Intermediate C++ layer with its Service Locator, P/Invoke interface, and managers for resources, rendering, and physics.
  ```

### Phase 3: High-Level C# Framework
  - tags: [v1.0.0, c#, high-level]
  - priority: high
  - steps:
      - [ ] Implement `GameEngine` lifecycle (Initialization, Update, Draw loops).
      - [ ] Implement `Scene`, `Node`, `Component` base classes for the scene graph.
      - [ ] Implement `TransformComponent` (position, rotation, scale).
      - [ ] Implement `MeshRendererComponent` for rendering.
      - [ ] Implement `CameraComponent` and `LightComponent`.
      - [ ] Implement `InputManager` using C# WAL wrapper.
      - [ ] Implement `AssetManager` (C# wrapper for C++ `ResourceManager`).
      - [ ] Implement `RenderManager` (C# wrapper for C++ `RenderSystem`).
      - [ ] Implement C# wrappers (`IntPtr` + `IDisposable`) for all core C++ classes (Camera, Light, Material, Mesh, Model, etc.).
      - [ ] Develop C# wrapper NuGet packages for core C++ backends (GLFW, OpenGL, Minimal PAL), integrating with .NET DI.
      - [ ] Configure `Piece.Engine` to use .NET DI to resolve and configure these C# factory wrappers, populating the C++ `ServiceLocator`.
      - [ ] Implement Logging for High-Level C# Framework.
      - [ ] Implement Tests for High-Level C# Framework.
  ```md
  Develop the High-Level C# Framework, providing a user-friendly API for game developers. This includes the core game loop, scene graph components, input and asset management, rendering orchestration, and robust C# wrappers for all underlying C++ engine functionalities, orchestrated via .NET Dependency Injection.
  ```

### Phase 4: Minimal Visual Editor
  - tags: [v1.0.0, editor, c#]
  - priority: high
  - steps:
      - [ ] Implement `EditorApplication` to host the `GameEngine`.
      - [ ] Implement `EditorState` to manage global editor state.
      - [ ] Implement a basic `ViewportPanel` to display the game scene rendered by `GameEngine`.
      - [ ] Implement a basic `HierarchyPanel` to list `Node`s in the `ActiveScene`.
      - [ ] Implement a basic `InspectorPanel` to display and allow editing of simple properties of selected `Node`s/`Component`s.
      - [ ] Implement basic scene serialization/deserialization for saving/loading editor state and scenes.
      - [ ] Implement Logging for the Minimal Visual Editor
      - [ ] Implement Tests for the Minimal Visual Editor
  ```md
  Develop a minimal but functional Visual Editor in C# that can host the engine's game loop, display the scene in a viewport, and allow basic inspection and manipulation of scene elements.
  ```

### Phase 5: Core Examples & Testing
  - tags: [v1.0.0, edd, testing, quality]
  - priority: high
  - steps:
      - [ ] Create a "Getting Started" guide in `README.md` for the v1.0.0 engine.
      - [ ] Implement "Example: Basic Window Creation" following EDD.
      - [ ] Implement "Example: Clear Screen" following EDD.
      - [ ] Implement "Example: Draw a Basic Triangle" following EDD.
      - [ ] Write comprehensive unit tests for all implemented core C++ and C# components.
      - [ ] Write integration tests to verify interactions between C++ and C# layers and backend plugins.
      - [ ] Ensure all project code adheres to Conventional Commits and passes CI checks.
      - [ ] Implement Logging for Core Examples & Testing.
      - [ ] Implement Tests for Core Examples & Testing.
  ```md
  Validate the core engine functionality through Example-Driven Development (EDD) examples, ensuring comprehensive unit and integration test coverage across all layers and adherence to development best practices.
  ```

## In Progress

## Done