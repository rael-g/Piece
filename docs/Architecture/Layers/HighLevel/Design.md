# Design Document - Piece.Framework (C# Framework)

## 1. Introduction

This document details the architectural design of the Piece.Framework of the Piece graphics engine. For an overview of the engine's multi-layered architecture and [Piece Engine General Design Document](../OVERVIEW.md).

This C# framework serves as the primary, feature-rich **reference implementation** of the engine's core C# interfaces (e.g., `IEngine`, `IScene`, `INode`). Its main purpose is to provide a very high-level API for game developers, abstracting the complexity of the Piece.Core (C++) and enabling rapid development for PC platforms (Windows, Linux).

By implementing a shared interface contract, it allows tools like the Visual Editor to remain decoupled from the concrete implementation. This ensures that a different Piece.Framework (e.g., a C++/Lua low-level implementation for consoles) could be created in the future and still be managed by the same toolset, fulfilling the engine's core philosophy of modularity.

Its objective is to provide an accessible and powerful high-level API for game developers, with features and capabilities supporting both 2D and 3D development.

## 2. Design Philosophy

The Piece.Framework is the C# developer's "playground," applying the Piece Engine's philosophy of **Modular Component Architecture** with maximum flexibility. This layer embodies the core principles of modularity and extensibility as described in the [Piece Engine General Design Document](../OVERVIEW.md).

In this layer, the key principles are manifested through:
*   **Composition over Inheritance:** Functionality added to game objects via composition of `Component`s.
*   **Node-Based Scene Graph:** Hierarchical organization of the game world.
*   **Data-Driven:** Systems and components configurable via data.
*   **Clean and Idiomatic C# Interface:** Abstraction of C++ complexity to focus on game logic.
## 3. Interaction with the Piece.Core (C++)

The Piece.Framework plays a crucial role in configuring the Piece.Core (C++). Instead of merely consuming a fixed C-compatible API, the C# layer leverages its .NET Dependency Injection (DI) system to resolve C# wrapper factories (e.g., `VulkanGraphicsDeviceFactory`, `GlfwWindowFactory`) for various C++ low-level implementations. These C# wrappers encapsulate `IntPtr`s to actual C++ factory instances (e.g., `IGraphicsDeviceFactory*`, `IWindowFactory*`) residing in the native low-level implementation DLLs. During the engine's initialization, the C# layer uses P/Invoke to pass these C++ factory pointers, along with any relevant configuration options, to a C++ Service Locator in the Piece.Core. This fundamentally shifts the responsibility of low-level implementation selection and configuration from hardcoded C++ logic to the flexible .NET DI system. Once configured, the Piece.Core can then use these provided C++ factories to create concrete `IGraphicsDevice`, `IWindow`, and other low-level C++ objects.

### 3.1. Safe Interop Strategy: Encapsulating Unsafe Code

To maintain the overall safety and robustness of the C# framework, any operations involving raw pointers (`IntPtr`), `unsafe` code blocks, or direct memory manipulation necessary for P/Invoke with the C++ layer will be strictly encapsulated.

*   **Dedicated Interop Projects:** All `unsafe` code and direct P/Invoke calls will reside within dedicated C# projects (e.g., `Piece.Interop`, `Piece.Vulkan.Interop`, `Piece.GLFW.Interop`). These projects will be compiled with the `AllowUnsafeBlocks` flag enabled.
*   **Safe API Exposure:** These dedicated interop projects will expose a **safe, managed API** to the Piece.Framework. This means they will provide C# classes and methods that hide the `IntPtr` handling and `unsafe` operations behind safe abstractions.
*   **Reduced Surface Area:** By confining `unsafe` code to specific, small, and well-defined modules, the surface area for potential memory errors or undefined behavior is significantly reduced. This makes auditing, testing, and maintenance of these critical interop sections much easier.
*   **Framework Safety:** The main C# framework (`Piece.Engine`) and its components will *not* be marked as `unsafe`. They will consume the safe, managed APIs exposed by the interop projects, ensuring that the vast majority of the engine's C# codebase remains type-safe and benefits from .NET's memory safety guarantees.

This strategy ensures that the power and performance of C++ can be leveraged while maintaining a high degree of safety and maintainability within the Piece.Framework.

## 4. Main Components of the Piece.Framework (C#)

### 4.1. `GameEngine`

*   **Responsibilities:**
    *   Main entry point of the game, managing the lifecycle (initialization, update, drawing).
    *   Maintains references to central systems like `InputManager`, `SceneManager`, and `AssetManager`.
    *   **Orchestrates the initialization of the Piece.Core (C++) layer:** This involves resolving C# factory wrappers (e.g., `IGraphicsDeviceFactory`, `IWindowFactory`) from the .NET DI container. It then uses P/Invoke to pass the native C++ factory pointers (obtained from these C# wrappers), along with any associated configuration options, to the C++ Service Locator within the Piece.Core.
    *   Manages the game loop (`Update`, `Draw`).
*   **Dependencies:** `InputManager`, `SceneManager`, `AssetManager`, resolved C# factory wrappers for C++ low-level implementations (e.g., `IGraphicsDeviceFactory`, `IWindowFactory`), C# wrappers for `RenderSystem`, `ResourceManager` (for initialization).

### 4.2. `Scene`

*   **Responsibilities:**
    *   Manages the hierarchy of active nodes (`Node`s) in the game world.
    *   Coordinates the update (`Update`) and drawing (`Draw`) of all its nodes and components.
    *   May contain scene-specific resources (e.g., a `PostProcessingManager`).
*   **Dependencies:** `Node`, `Camera` (for the active camera of the scene), `Light`.

### 4.3. `Node` (Base for GameObject)

*   **Responsibilities:**
    *   Represents an entity in the game world.
    *   Contains a `TransformComponent` (position, rotation, scale).
    *   Can have multiple `Component`s attached.
    *   Can have child `Node`s, forming a hierarchy.
    *   Manages its lifecycle (`OnStart`, `OnUpdate`, `OnDestroy`).
*   **Dependencies:** `TransformComponent`, `Component`.

### 4.4. `Component` (Base for Behaviors)

*   **Responsibilities:**
    *   Abstract base class for all modular functionalities that can be attached to a `Node`.
    *   Provides lifecycle methods (`Start`, `Update`, `Draw`).
*   **Examples:** `TransformComponent`, `MeshRendererComponent`, `CameraComponent`, `LightComponent`, `ScriptComponent` (for custom game logic).
*   **Interaction:** Components communicate via events or by obtaining references to other components in the same or other nodes.

### 4.5. `TransformComponent`

*   **Responsibilities:**
    *   Manage the position, rotation (Quaternion), scale of a `Node`.
    *   Calculate and store local and global transformation matrices.
*   **Interaction:** Sends updated transformation data to the C# `Model` wrapper (which then calls `ModelCpp.SetTransform`).

### 4.6. `InputManager`

*   **Responsibilities:**
    *   Translate raw inputs (keyboard, mouse, gamepad) into abstract game actions.
    *   Can use the WAL's `IWindow` (through a C# wrapper).
*   **Interaction:** The `GameEngine` updates it, and game logic queries it (`Input.IsActionPressed(GameAction.Jump)`).

### 4.7. `AssetManager`

*   **Responsibilities:**
    *   Management of asset loading and caching (C# user-facing).
    *   Encapsulates the C# `ResourceManager` wrapper (which in turn interacts with `ResourceManagerCpp`).
    *   Provides typed methods to load assets (e.g., `Load<Mesh>("path/to/mesh")`).
    *   Manages the lifecycle of C# wrappers and corresponding native resources.
*   **Interaction:** The `GameEngine` initializes it, `Scene`s, `Node`s, and `Component`s request assets from it.

### 4.8. `RenderManager`

*   **Responsibilities:**
    *   Orchestrate the high-level rendering process.
    *   Uses the C# `RenderSystem` wrapper from the Piece.Core.
    *   Collects `RenderableComponent`s from visible nodes, performs high-level culling/sorting, and passes data to `RenderSystemCpp` (via C# wrapper).
*   **Interaction:** Called by `GameEngine.Draw()`.

### 4.9. `CameraComponent`

*   **Responsibilities:**
    *   Attached to a `Node`, representing a camera in the world.
    *   Encapsulates the C# `Camera` wrapper (which interacts with `CameraCpp`).
    *   Manages the view frustum, projection (perspective/orthographic), viewport.
*   **Interaction:** The `RenderManager` uses it to configure rendering.

### 4.10. `LightComponent`

*   **Responsibilities:**
    *   Attached to a `Node`, representing a light source.
    *   Encapsulates the C# `Light` wrapper (which interacts with `LightCpp`).
    *   Defines light properties: type, color, intensity, position, direction.
*   **Interaction:** `RenderManager` collects `LightComponent`s from visible nodes.

### 4.11. `MeshRendererComponent`

*   **Responsibilities:**
    *   Attached to a `Node`, responsible for rendering geometry.
    *   Maintains references to a `Mesh` (C# wrapper for `MeshCpp`) and a `Material` (C# wrapper for `MaterialCpp`).
*   **Interaction:** The `RenderManager` detects it as renderable and calls its `Draw` methods.

### 4.12. `ScriptComponent`

*   **Responsibilities:**
    *   Base component for custom game scripts, written by the game developer.
    *   Provides lifecycle methods (`Start`, `Update`, `OnCollision`, etc.).
*   **Interaction:** Allows the developer to implement specific game logic using the framework's API.

### 4.13. AI Systems and Components

To create rich and believable worlds, the framework will be expanded with a foundation for advanced Artificial Intelligence.

*   **`AISystem` (C# Class):**
    *   **Responsibilities:** A central system responsible for managing and executing AI behaviors throughout the scene. It would discover all AI-enabled components and orchestrate their updates, potentially distributing the workload using the `JobSystem` from the Piece.Core (C++) for high performance with many agents.
    *   **Interaction:** Managed by the `GameEngine` or `Scene`, it operates on components like `BehaviorTreeComponent`.

*   **`BehaviorTreeComponent` (C# Component):**
    *   **Responsibilities:** Attached to a `Node`, this component would execute a Behavior Tree asset. It allows developers to design complex NPC logic (e.g., patrol, attack, flee) in a visual, modular, and reactive way. This moves beyond simple state machines and aligns with modern AI design.
    *   **Interaction:** The `AISystem` would call `Update()` on this component, which would then traverse its behavior tree and execute the appropriate logic.

## 5. Piece.Framework Game Loop Flow (Simplified)

1.  **`GameEngine.Run()`:**
    *   Initializes itself and its systems (`InputManager`, `AssetManager`, `SceneManager`, `AISystem`).
    *   Initializes the Piece.Core (C++) (creates `ResourceManagerCpp`, `RenderSystemCpp`, etc., obtaining their `IntPtr`s).
    *   Enters the main loop:
        *   `GameEngine.Update()`
        *   `GameEngine.Draw()`

2.  **`GameEngine.Update(GameTime gameTime)`:**
    *   `InputManager.Update()` (reads input device state).
    *   `SceneManager.CurrentScene.Update(gameTime)` (updates all nodes and their components, including the `AISystem`).

3.  **`GameEngine.Draw(GameTime gameTime)`:**
    *   Calls `RenderManager.Render(SceneManager.CurrentScene, GameEngine.CurrentCamera)` (passing Piece.Framework C# objects).

4.  **`RenderManager.Render(Scene currentScene, Camera activeCamera)`:**
    *   Collects `MeshRendererComponent`s and `LightComponent`s from visible `Node`s in the `currentScene`.
    *   Performs high-level culling and sorting.
    *   Calls `RenderSystem.RenderFrame(activeCamera, listOfNativeModelPtrs)` (the C# wrapper for `RenderSystemCpp` which makes the P/Invoke call).
    *   Manages post-processing application via `PostProcessingManager`.

## 6. 2D/3D Unification

*   **Node/Component:** The base `Node` and `Component` can be reused. `TransformComponent` can be generic enough (Vector3, Quaternion) for both 2D (using Z=0 for 2D) and 3D, or have specializations (`Transform2DComponent`, `Transform3DComponent`).
*   **RenderManager:** Would contain logic to determine the rendering type (2D or 3D) and dispatch to specific paths in the Piece.Core (C++).
*   **Specific Components:** `SpriteRendererComponent` for 2D, `MeshRendererComponent` for 3D.

## 7. Configuration via .NET Dependency Injection and Options

To provide a clean and idiomatic C# configuration experience for the Piece Engine, especially for its C++ low-level implementations, we leverage the robust .NET Dependency Injection (DI) system in conjunction with the standard .NET Options pattern.

### 7.1. Extension Methods for Low-Level Implementation Registration

Each major module or low-level implementation (e.g., Vulkan renderer, GLFW windowing, Jolt physics) will be distributed as a dedicated C# NuGet package (e.g., `Piece.Vulkan`, `Piece.GLFW`, `Piece.Jolt`). These packages will expose an extension method on `Microsoft.Extensions.DependencyInjection.IServiceCollection` to register their services and C++ factory wrappers.

**Example:**
```csharp
public static class VulkanServiceCollectionExtensions {
    public static IServiceCollection AddPieceVulkan(this IServiceCollection services, Action<VulkanOptions>? configureOptions = null) {
        services.AddSingleton<IGraphicsDeviceFactory, VulkanGraphicsDeviceFactory>();
        // Configure options for Vulkan low-level implementation
        if (configureOptions != null) {
            services.Configure(configureOptions);
        } else {
            services.Configure<VulkanOptions>(options => { /* Apply default Vulkan options */ });
        }
        return services;
    }
}
```

### 7.2. Options Pattern for Configuration

The .NET Options pattern will be used to provide structured, type-safe configuration for each module and low-level implementation.

*   **Options Classes:** For each configurable component, a simple C# POCO (Plain Old C# Object) class will be defined (e.g., `VulkanOptions`, `GlfwOptions`, `EngineOptions`). These classes will contain properties representing the configurable parameters.

    **Example `VulkanOptions.cs`:**
    ```csharp
    public class VulkanOptions {
        public bool EnableValidationLayers { get; set; } = true;
        public string[] RequiredDeviceExtensions { get; set; } = { "VK_KHR_swapchain" };
        public int MaxFramesInFlight { get; set; } = 2;
        // ... other Vulkan-specific settings
    }
    ```

*   **Configuration During DI Registration:** The `configureOptions` delegate in the extension method allows the user to customize these options.

    **User's Application Example:**
    ```csharp
    Host.CreateDefaultBuilder(args)
        .ConfigureServices((hostContext, services) => {
            services.AddPieceCore(options => {
                options.DefaultLogLevel = LogLevel.Information;
                options.AssetRootPath = "Assets/";
            });
            services.AddPieceVulkan(options => {
                options.EnableValidationLayers = hostContext.HostingEnvironment.IsDevelopment();
                options.MaxFramesInFlight = 3;
            });
            services.AddPieceGlfw(options => {
                options.InitialWindowWidth = 1920;
                options.InitialWindowHeight = 1080;
                options.WindowTitle = "My Awesome Piece Engine Game";
            });
            // ...
        })
        .Build()
        .Run();
    ```

### 7.3. Passing Options to C++ Factories

When a C# factory wrapper (e.g., `VulkanGraphicsDeviceFactory`) is instantiated and used to create or configure its native C++ counterpart, the resolved `IOptions<XxxOptions>` instance will be passed.

*   **Serialization/Marshalling:** The C# wrapper will either:
    1.  Serialize the `XxxOptions` object into a C-compatible format (e.g., JSON string, custom binary blob) and pass this data to the C++ factory via P/Invoke. The C++ factory would then deserialize/parse it.
    2.  Marshal individual primitive properties of the `XxxOptions` class into a C-style struct (if the options are simple enough) and pass this struct via P/Invoke.

*   The C++ factory (e.g., `VulkanGraphicsDeviceFactory` in C++) will then use these received options to configure the underlying C++ low-level implementation.

This configuration strategy provides a flexible, type-safe, and idiomatic .NET way to manage the engine's extensive modularity, including the C++ layers.

## 8. Granular Extension Points

The Piece.Framework is the primary environment for game development, offering maximum flexibility and extensibility through its adherence to the philosophy of **Modular Component Architecture**. For a complete discussion of this philosophy and extension points across all engine layers, please refer to the [Piece Engine General Design Document](../OVERVIEW.md).

The main specific extension points of this layer include:

### 9.1. Dependency Injection (DI) with .NET

The Piece.Framework is designed to be fully compatible with the .NET Dependency Injection (DI) system. This is the primary mechanism for configuring and extending the engine, allowing the complete or partial replacement of any core framework subsystem. Crucially, this extends to injecting specific implementations of **C++ low-level implementations** (like graphics APIs or physics engines) into the intermediate C++ layer. By configuring the DI container in the C# host application, developers can seamlessly swap native C++ components without modifying engine source code. This includes passing configuration options to these C++ components as detailed in Section 7.

### 8.2. Extension via Component System

The node and component system is the heart of extensibility for game logic. Developers can create an unlimited number of custom `Component`s to attach to any `Node`, changing or adding functionalities. This includes `ScriptComponent`s for custom game logic.

### 8.3. Visual Editor Extension

The visual editor itself, built in C#, will be extensible, allowing developers to create plugins (new panels, tools, customized inspectors) that integrate with the editor, using the framework's APIs to manipulate scenes and assets.

### 8.4. Multi-Language Support via Embedded Scripting

Further embracing the philosophy of **Modular Component Architecture**, the C# framework supports the use of multiple scripting languages for game logic within the same project. This is achieved by hosting scripting language interpreters within specialized C# components, rather than by replacing the entire Piece.Framework. This approach ensures that the C# object model remains the single source of truth for the Visual Editor, maintaining architectural consistency.

#### The "Bridge Component" Pattern

To use a language like **Lua**, a developer would create a `LuaScriptComponent` in C#.
*   **Role:** This C# component acts as a "bridge" or "proxy". To the rest of the C# engine and the Visual Editor, it's just another component.
*   **Properties:** It would expose properties editable in the Inspector, such as a `string ScriptFile` to specify which `.lua` file to load.
*   **Internal Logic:** Internally, the `LuaScriptComponent` uses a C# library (e.g., NLua, MoonSharp) to instantiate a Lua interpreter.
    *   In its C# `Start()` method, it commands the interpreter to load and execute the specified script file.
    *   In its C# `Update()` method, it calls the corresponding `update()` function within the Lua script.
    *   It securely exposes necessary C# objects (like the parent `Node`) to the Lua environment, allowing the script to manipulate the game world (e.g., `node.transform.position = new_position`).

Because the script's actions are channeled through a C# component, any state change happens within the C# object model, making the process transparent to the Visual Editor and the Piece.Framework.

#### Available Interpreters

The .NET ecosystem (via NuGet) provides a rich selection of interpreters for various languages:
*   **Lua:** `NLua` (a binding to the standard C Lua interpreter) and `MoonSharp` (a pure C# interpreter).
*   **Python:** `IronPython` (a Python implementation on .NET) and `Python.NET` (for embedding the standard CPython).
*   **JavaScript:** `Jint` and `ClearScript` (which can host Google's V8 engine).

#### Performance Considerations

*   **Runtime Cost:** Using an embedded interpreter incurs a runtime performance cost. The scripts are not pre-compiled with the C# project; they are loaded as text and processed by the interpreter every time the game runs.
*   **Interop Overhead:** There is a small but measurable overhead each time code crosses the boundary between C# and the scripted language (e.g., a Lua script accessing a C# object's property). This is called interop cost.
*   **Best Practices:** The performance is excellent for typical game logic, state management, and decision-making. The cost only becomes significant if the script performs thousands of interop calls per frame in a tight loop. Heavy, performance-critical calculations should be kept within C# or C++.