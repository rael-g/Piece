# Design Document - Piece.Core (C++)

## 1. Introduction

This document details the architectural design of the Piece.Core of the Piece graphics engine. For an overview of the engine's multi-layered architecture and its guiding philosophy of **Modular Component Architecture**, please refer to the [Piece Engine General Design Document](../OVERVIEW.md).

Implemented in **C++**, this layer will act as a high-performance orchestrator between the low-level C++ backend (WAL/RAL) and the Piece.Framework (C#), where game logic will reside. Its main objective is to abstract the complexity of the backend, provide a rich and optimized API, and manage essential resources for scene rendering, offering it efficiently to the C# layer.

## 2. Responsibilities of the Piece.Core (C++)

*   **RAL Abstraction and Orchestration:** Translate high-level concepts into appropriate calls and sequences of operations for the C++ RAL interfaces (`IGraphicsDevice`, `IRenderContext`, `IVertexBuffer`, etc.), orchestrating the rendering pipeline.
*   **GPU Resource Management:** Allocate, maintain the lifecycle, and release resources such as buffers, textures, shaders, samplers. Implement a robust resource caching system and asynchronous loading, mapping disk assets to `std::unique_ptr`s of RAL resources.
*   **High-Level Rendering Pipeline Management:** Control the rendering flow, including culling, sorting, management of rendering passes (e.g., G-Buffer, light pass, shadow pass, post-processing passes).
*   **Visual Entities and Components Management:** Maintain an optimized representation of renderable objects in the world, their transformations, and their visual properties, linking them to Meshes and Materials.
*   **Implementation of High-Level Concepts (C++):** Provide optimized C++ classes for concepts such as `Material`, `Mesh`, `Model`, `Camera`, `Light`, `RenderSystem`, `PostProcessingManager`, etc., which internally use the RAL interfaces.

## 3. Interaction of the Piece.Core (C++) with the C++ Backend

The Piece.Core, being implemented in C++, interacts directly with the RAL (Render Abstraction Layer) and WAL (Window Abstraction Layer) interfaces of the low-level backends. This interaction is direct and performant, without interop overhead, as both layers are in C++.

However, the *instances* of these low-level backend interfaces (e.g., `IGraphicsDevice`, `IWindow`, `IPhysicsWorld`) are not created directly by the Piece.Core. Instead, they are obtained through the C++ `ServiceLocator`. The `ServiceLocator` provides C++ factory interfaces (e.g., `IGraphicsDeviceFactory`) that have been pre-configured and supplied by the Piece.Framework (C#) via its .NET DI system. This ensures maximum efficiency and control over GPU resources and low-level operations, while centralizing the choice of backend implementation in the C# host.

## 4. Interaction with the Piece.Framework (C# via P/Invoke)

The Piece.Core (C++) will expose an optimized and C-compatible API to be consumed by the Piece.Framework (C#). This C-compatible API will be accessed from C# via P/Invoke.

### 4.0. C++ Service Locator

To enable the flexible injection and configuration of low-level C++ backends from the C# host application, the Piece.Core introduces a C++ `ServiceLocator`. This `ServiceLocator` acts as a central registry for C++ factory interfaces (e.g., `IGraphicsDeviceFactory`, `IWindowFactory`, `IPhysicsWorldFactory`) that are provided by the C# application at engine initialization.

*   **Purpose:** To decouple the core Piece.Core from concrete backend implementations. Instead of directly loading backend DLLs or instantiating specific backend types, the Piece.Core requests factories from the `ServiceLocator` and uses them to create instances of `IGraphicsDevice`, `IWindow`, `IPhysicsWorld`, etc.
*   **Mechanism:** The `ServiceLocator` stores `std::unique_ptr`s to these C++ factory interfaces. The C# host resolves C# wrapper factories (which encapsulate raw C++ factory pointers) via its .NET DI container and uses P/Invoke functions to pass these raw pointers to the C++ `ServiceLocator`. The `ServiceLocator` then takes ownership of these C++ factory instances.
*   **Location:** Defined in `src/cpp/Piece.Intermediate/core/service_locator.h`.

**Conceptual `ServiceLocator` Structure:**
```cpp
#pragma once
#include <memory>
#include "../interfaces/igraphics_device_factory.h"
#include "../interfaces/iwindow_factory.h"
#include "../interfaces/iphysics_world_factory.h"

namespace Piece { namespace Core {
    class ServiceLocator {
    public:
        static ServiceLocator& Get() {
            static ServiceLocator instance;
            return instance;
        }

        void SetGraphicsDeviceFactory(std::unique_ptr<IGraphicsDeviceFactory> factory) {
            graphics_device_factory_ = std::move(factory);
        }
        IGraphicsDeviceFactory* GetGraphicsDeviceFactory() {
            return graphics_device_factory_.get();
        }

        void SetWindowFactory(std::unique_ptr<IWindowFactory> factory) {
            window_factory_ = std::move(factory);
        }
        IWindowFactory* GetWindowFactory() {
            return window_factory_.get();
        }

        void SetPhysicsWorldFactory(std::unique_ptr<IPhysicsWorldFactory> factory) {
            physics_world_factory_ = std::move(factory);
        }
        IPhysicsWorldFactory* GetPhysicsWorldFactory() {
            return physics_world_factory_.get();
        }

    private:
        ServiceLocator() = default;
        ServiceLocator(const ServiceLocator&) = delete;
        ServiceLocator& operator=(const ServiceLocator&) = delete;

        std::unique_ptr<IGraphicsDeviceFactory> graphics_device_factory_;
        std::unique_ptr<IWindowFactory> window_factory_;
        std::unique_ptr<IPhysicsWorldFactory> physics_world_factory_;
    };
}}
```
This C++ Service Locator acts as the crucial bridge for the .NET DI system to configure the C++ layers.

### 4.1. `NativeExports.h` (C++ Header)

*   Will define all exported C++ functions using `extern "C"`.
*   Will use C-compatible data types (e.g., `int`, `float`, pointers, POD structs) for parameter passing.
*   Will return pointers to C++ class instances as `void*` or `IntPtr` (for C#).
*   **Crucially, it will expose functions for the C# layer to set factory instances in the C++ Service Locator.** It will also define a C-compatible struct for passing configuration options.

    **Conceptual `NativeExports.h` Snippet:**
    ```cpp
    #ifndef NATIVE_EXPORTS_H
    #define NATIVE_EXPORTS_H

    // Forward declarations for opaque types and factory interfaces
    struct EngineCore;
    namespace Piece { namespace Core {
        class IGraphicsDeviceFactory;
        class IWindowFactory;
        class IPhysicsWorldFactory;
    }}

    // C-compatible struct for passing configuration options
    struct NativeWindowOptions {
        int initialWindowWidth;
        int initialWindowHeight;
        bool windowResizable;
        // Pointers for strings must be allocated/deallocated correctly across the boundary
        const char* windowTitle;
    };

    struct NativeVulkanOptions {
        bool enableValidationLayers;
        int maxFramesInFlight;
        // Pointers for arrays of strings (e.g., required device extensions) need careful marshaling
    };

    extern "C" {
        // Functions for the C# layer to set C++ factory instances in the Service Locator
        __declspec(dllexport) void PieceCore_SetGraphicsDeviceFactory(void* factoryPtr, const NativeVulkanOptions* options);
        __declspec(dllexport) void PieceCore_SetWindowFactory(void* factoryPtr, const NativeWindowOptions* options);
        __declspec(dllexport) void PieceCore_SetPhysicsWorldFactory(void* factoryPtr, void* options); // Generic options pointer

        // Main engine initialization, now relying on Service Locator
        __declspec(dllexport) EngineCore* Engine_Initialize();
        __declspec(dllexport) void Engine_Destroy(EngineCore* core);

        // ... other engine functions ...
    }

    #endif // NATIVE_EXPORTS_H
    ```

### 4.2. `NativeCalls` (C# Static Class/P/Invoke Wrapper)

*   A static C# class containing all `[DllImport]` declarations for exported C++ functions.
*   Responsible for translating idiomatic C# calls to the C-compatible API exposed by C++.
*   **Will include `[DllImport]` definitions for setting factories and passing configuration options.**

    **Conceptual `NativeCalls` Snippet:**
    ```csharp
    using System;
    using System.Runtime.InteropServices;

    public static class NativeCalls
    {
        // P/Invoke for setting C++ factories in the Service Locator
        [DllImport("PieceCore.dll")]
        public static extern void PieceCore_SetGraphicsDeviceFactory(IntPtr factoryPtr, ref NativeVulkanOptions options);

        [DllImport("PieceCore.dll")]
        public static extern void PieceCore_SetWindowFactory(IntPtr factoryPtr, ref NativeWindowOptions options);

        [DllImport("PieceCore.dll")]
        public static extern void PieceCore_SetPhysicsWorldFactory(IntPtr factoryPtr, IntPtr optionsPtr); // For more complex options

        // Main engine initialization, now relying on Service Locator
        [DllImport("PieceCore.dll")]
        public static extern IntPtr Engine_Initialize();

        [DllImport("PieceCore.dll")]
        public static extern void Engine_Destroy(IntPtr corePtr);

        // Example for other exposed objects and methods
        [DllImport("PieceCore.dll")]
        public static extern IntPtr RenderSystem_Create();

        [DllImport("PieceCore.dll")]
        public static extern void RenderSystem_RenderFrame(IntPtr renderSystemPtr, IntPtr cameraPtr, IntPtr scenePtr);

        // ... and so on for all exposed objects and methods
    }
    ```
    *   **C# `NativeVulkanOptions` and `NativeWindowOptions` structs** (mirroring C++):
        ```csharp
        [StructLayout(LayoutKind.Sequential)]
        public struct NativeWindowOptions {
            public int InitialWindowWidth;
            public int InitialWindowHeight;
            [MarshalAs(UnmanagedType.I1)] // Marshals bool to a 1-byte Boolean
            public bool WindowResizable;
            public IntPtr WindowTitle; // Must be allocated/deallocated from C# side
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct NativeVulkanOptions {
            [MarshalAs(UnmanagedType.I1)]
            public bool EnableValidationLayers;
            public int MaxFramesInFlight;
            // Complex types like string arrays or lists require custom marshalling logic.
        }
        ```
    *   The C# layer will be responsible for allocating unmanaged memory for strings (`Marshal.StringToHGlobalAnsi`) and structs, and then freeing them.
### 4.3. High-Level C# Wrappers

*   C# classes (e.g., `Material`, `Mesh`, `Model`, `Camera`, `RenderSystem`) that mirror the concepts of the Piece.Core.
*   These C# classes will contain an `IntPtr` for the corresponding C++ instance.
*   Their methods will invoke the static functions in `NativeCalls` to interact with the underlying C++ instance.
*   Will implement `IDisposable` to ensure proper release of C++ resources (calling `_Destroy` functions via P/Invoke).
*   **Crucially, C# factory wrappers (e.g., `VulkanGraphicsDeviceFactory`, `GlfwWindowFactory`) will be responsible for:**
    1.  Loading their respective native C++ backend DLLs (e.g., `gfx_vulkan.dll`).
    2.  Calling the C-exported factory creation function within that DLL (e.g., `CreateVulkanGraphicsDeviceFactory()`) to obtain a raw C++ factory pointer (`IntPtr`).
    3.  Holding this `IntPtr` and managing its lifecycle, including calling a C-exported factory destruction function (e.g., `DestroyVulkanGraphicsDeviceFactory()`) when disposed.
    4.  Marshaling C# `Options` objects (e.g., `VulkanOptions`) into C-compatible structs or data blobs to be passed alongside the factory pointer during P/Invoke calls to `PieceIntermediate_SetXxxFactory`.

Example C# Wrapper (`RenderSystem` is shown, but the factory wrappers follow a similar `IntPtr` and `IDisposable` pattern):
```csharp
public class RenderSystem : IDisposable
{
    private IntPtr _nativePtr; // Pointer to the C++ instance

    public RenderSystem()
    {
        _nativePtr = NativeCalls.RenderSystem_Create();
    }

    public void RenderFrame(Camera activeCamera, Scene currentScene)
    {
        NativeCalls.RenderSystem_RenderFrame(_nativePtr, activeCamera.NativePtr, currentScene.NativePtr);
    }

    public void Dispose()
    {
        NativeCalls.RenderSystem_Destroy(_nativePtr);
        _nativePtr = IntPtr.Zero;
    }
}
```

### 4.4. Dynamic Backend Injection (C# DI-driven Plugin Pattern)

The strategy for integrating low-level C++ backends (like graphics APIs or physics engines) has evolved to fully embrace the .NET Dependency Injection system. The Piece.Core *no longer* directly loads backend DLLs (e.g., `gfx_opengl.dll`, `physics_jolt.dll`) via `LoadLibrary` or similar mechanisms. Instead, the responsibility for backend selection, loading, and configuration is orchestrated by the high-level C# application.

**The Workflow:**

1.  **C++ Backend DLLs:** Each low-level backend (graphics, windowing, physics) is compiled into a separate dynamic library (e.g., `gfx_vulkan.dll`, `gfx_glfw.dll`, `physics_jolt.dll`). These DLLs *export C-style factory creation functions* (e.g., `CreateVulkanGraphicsDeviceFactory()`, `CreateGlfwWindowFactory()`) that return raw pointers to concrete C++ factory implementations (e.g., `VulkanGraphicsDeviceFactory*` implementing `IGraphicsDeviceFactory`). These factory functions can also accept C-compatible configuration structs as parameters.
2.  **C# Wrapper NuGet Packages:** For each backend, a C# NuGet package (e.g., `Piece.Vulkan`) is created. This package contains:
    *   The native C++ backend DLL (e.g., `gfx_vulkan.dll`).
    *   A C# wrapper class (e.g., `VulkanGraphicsDeviceFactory`) that implements a corresponding C# factory interface (e.g., `IGraphicsDeviceFactory`).
    *   This C# wrapper class uses P/Invoke to:
        *   Load the native C++ DLL.
        *   Call its exported factory creation function (e.g., `CreateVulkanGraphicsDeviceFactory()`) to get an `IntPtr` (raw C++ factory pointer).
        *   Optionally, pass marshaled C# configuration options to the native factory function.
        *   Manage the lifecycle of the native C++ factory instance (including calling a native destruction function upon C# `Dispose`).
3.  **C# Host Application Configuration:** The user's C# application adds these NuGet packages and configures them via standard .NET Dependency Injection.
    ```csharp
    services.AddPieceVulkan(options => { options.EnableValidationLayers = true; });
    services.AddPieceGlfw(options => { options.InitialWindowWidth = 1280; });
    ```
5.  **C++ Service Locator Population:** During the `GameEngine`'s C# initialization, it resolves the C# factory wrappers from its .NET DI container. It then uses P/Invoke to call `PieceCore_SetXxxFactory()` functions (defined in `NativeExports.h`) on the `PieceCore.dll`, passing the raw C++ factory pointers from the C# wrappers to the C++ `ServiceLocator`. This is also where the marshaled configuration options are passed.
6.  **Piece.Core Consumption:** When the Piece.Core requires an `IGraphicsDevice` or `IWindow`, it queries its internal `ServiceLocator` for the appropriate C++ factory instance and uses it to create the concrete object. The factories themselves use the configuration options they received during their creation.

This refined approach provides unparalleled flexibility and allows developers to manage C++ backend dependencies and configurations entirely within the familiar .NET ecosystem, truly embodying the principles of a modular component architecture.
The conceptual C++ implementation snippet for `Engine_InitializeFromLibraries` is now obsolete and has been removed. The initialization logic for `EngineCore` will instead rely on the C++ `ServiceLocator`.

### 4.5. `EngineCore` Initialization and Service Resolution

The `EngineCore` (or the main initialization routine of the Piece.Core) no longer receives concrete backend instances or library paths directly. Instead, its initialization process involves:

1.  **Accessing the `ServiceLocator`:** Retrieving the singleton instance of the C++ `ServiceLocator`.
2.  **Resolving Factories:** Obtaining the configured C++ factory interfaces (e.g., `IGraphicsDeviceFactory`, `IWindowFactory`, `IPhysicsWorldFactory`) from the `ServiceLocator`. The `ServiceLocator` guarantees that these factories have already been set by the C# application via P/Invoke.
3.  **Creating Concrete Backends:** Using these resolved factories to create the actual `IGraphicsDevice`, `IWindow`, and `IPhysicsWorld` instances. These instances are then managed by the `EngineCore` for the lifetime of the application.
4.  **Passing Configuration:** The configuration options (marshaled from C#) are available to the factories when they are created, allowing the concrete backend implementations to be initialized with specific user-defined settings.

**Conceptual `EngineCore` Initialization Snippet:**
```cpp
// In EngineCore.cpp (simplified)
#include "core/service_locator.h"
#include <memory>

namespace Piece { namespace Core {
    // Forward declarations (assuming RAL/WAL interfaces are in scope)
    // class IWindow;
    // class IGraphicsDevice;
    // class IPhysicsWorld;

    EngineCore::EngineCore() {
        // Retrieve factories from the Service Locator
        IGraphicsDeviceFactory* graphicsFactory = ServiceLocator::Get().GetGraphicsDeviceFactory();
        IWindowFactory* windowFactory = ServiceLocator::Get().GetWindowFactory();
        IPhysicsWorldFactory* physicsFactory = ServiceLocator::Get().GetPhysicsWorldFactory();

        if (!graphicsFactory || !windowFactory || !physicsFactory) {
            // Handle error: factories not set by C# application
            // This indicates a misconfiguration in the C# host DI setup.
            // Log error and potentially throw an exception.
            return;
        }

        // Use factories to create concrete backend instances
        // Note: IWindow might be created first, then passed to IGraphicsDevice.
        // Configuration options (NativeWindowOptions, NativeVulkanOptions) would be
        // passed to the factory's Create methods if they support them.

        // Example: Create window
        auto window_unique_ptr = windowFactory->CreateWindow(WindowOptions); // Assuming CreateWindow takes options
        window_ = std::move(window_unique_ptr);
        window_->Create("Piece Engine", 1280, 720); // Initial window setup

        // Example: Create graphics device
        auto graphics_unique_ptr = graphicsFactory->CreateGraphicsDevice(window_.get(), GraphicsOptions); // Assuming CreateGraphicsDevice takes options
        graphics_device_ = std::move(graphics_unique_ptr);
        graphics_device_->Init(window_.get()); // Initialize device with window context

        // Example: Create physics world
        auto physics_unique_ptr = physicsFactory->CreatePhysicsWorld(PhysicsOptions); // Assuming CreatePhysicsWorld takes options
        physics_world_ = std::move(physics_unique_ptr);
        physics_world_->Init(); // Initialize physics world

        // ... other engine component initializations
    }

    EngineCore::~EngineCore() {
        // Resources are automatically released by unique_ptr
    }
}}
```
This new initialization approach fundamentally changes how the Piece.Core acquires its core backend components, shifting the control to the C# host via DI.

## 5. Main Components of the Piece.Core (C++)

### 5.0. `JobSystem` (Task-Based Multithreading)

*   **Responsibilities:**
    *   Manage a pool of worker threads to process tasks in parallel across all available CPU cores.
    *   Provide an API to submit jobs (small units of work) and manage their dependencies using a task graph.
    *   Implement a "work-stealing" queue to ensure high CPU utilization and efficient load balancing.
    *   This system is fundamental for the performance of other CPU-intensive systems (e.g., culling, physics, asset decompression) and aligns the engine with a modern, data-oriented architecture.
*   **Dependencies:** None. This is a foundational system.

### 5.1. `ResourceManager` (C++ Class)

*   **Responsibilities:**
    *   Manage the lifecycle of RAL resources (`IVertexBuffer`, `ITexture`, `IShaderProgram`, etc.) using `std::unique_ptr`s and ensure they are properly released.
    *   Implement an efficient resource caching system (e.g., `std::unordered_map<std::string, std::unique_ptr<IResource>>`).
    *   Provide methods to load assets from files (models, textures, shaders) and convert them into RAL resources.
    *   **Dynamic Asset Streaming:** Implement a dynamic asset streaming system to load and unload resources (textures, meshes) based on scene visibility, memory budget, and camera proximity. This process will run on background threads managed by the `JobSystem` to avoid blocking the main thread.
*   **Dependencies:** `IGraphicsDevice` (for creating resources), `JobSystem` (for background loading).

### 5.2. `Material` (C++ Class)

*   **Responsibilities:** Abstract the concept of material.
    *   Encapsulate references to `IShaderProgram`, `ITexture`s, `ISampler`s, and `IUniformBuffer`s from the RAL.
    *   Provide methods to set material properties (e.g., `SetFloat("Albedo", 1.0f)`).
    *   Manage the binding of its resources to the `IRenderContext` (via `RenderSystem`).
*   **Dependencies:** `IGraphicsDevice`, `IShaderProgram`, `ITexture`, `ISampler`, `IUniformBuffer`.

### 5.3. `Mesh` (C++ Class)

*   **Responsibilities:** Abstract the geometry of an object.
    *   Encapsulate references to `IVertexBuffer` and `IIndexBuffer` from the RAL.
    *   May contain information about sub-meshes (parts of the mesh that use different materials).
*   **Dependencies:** `IVertexBuffer`, `IIndexBuffer`.

### 5.4. `Model` (C++ Class)

*   **Responsibilities:** Represent a renderable object in the 3D world.
    *   Combines a `Mesh` with one or more `Material`s.
    *   Contains transformation information (position, rotation, scale).
*   **Dependencies:** `Mesh`, `Material`.

### 5.5. `Camera` (C++ Class)

*   **Responsibilities:** Define the rendering perspective.
    *   Store and calculate view and projection matrices.
    *   Properties like FOV, Near/Far Plane, Viewport.
*   **Dependencies:** None direct from RAL, but its data is used by `RenderSystem` to configure the `IRenderContext`.

### 5.6. `Light` (C++ Class)

*   **Responsibilities:** Represent light sources in the scene.
    *   Properties like light type (Directional, Point, Spot), color, intensity.
    *   May include data for shadow mapping.
*   **Dependencies:** None direct from RAL, but its data is used by shaders via `RenderSystem`.

### 5.7. `RenderSystem` (C++ Class)

*   **Responsibilities:** The heart of rendering orchestration.
    *   **Object Collection:** Obtain all `Model`s and `Light`s from the scene that need to be rendered.
    *   **Culling:** Perform Frustum Culling and, potentially, invoke Occlusion Culling. These culling stages will be parallelized as much as possible using the `JobSystem`.
    *   **Sorting:** Order renderable objects for optimization (e.g., opaque front-to-back, transparent back-to-front).
    *   **Command Submission:** Iterate over sorted objects, configure the `IRenderContext` (bind materials, buffers, shaders), and issue drawing calls.
    *   Orchestrate rendering passes (e.g., G-Buffer pass for deferred shading, light pass, shadow pass, post-processing passes).
*   **Dependencies:** `IGraphicsDevice`, `IRenderContext`, `JobSystem`, `Camera`, `Light`, `Model`.

### 5.8. `PhysicsSystemCpp` (C++ Class)

*   **Responsibilities:** Orchestrate the physics simulation.
    *   **Dynamic Backend Loading:** Loads the chosen Physics Abstraction Layer (PAL) backend (e.g., `physics_jolt.dll`) at runtime.
    *   **Simulation Loop:** Manages the `IPhysicsWorld` instance, calling its `Step(deltaTime)` method every frame.
    *   **Synchronization:** After each physics step, reads the updated transforms (position, rotation) from the `IPhysicsBody` objects and synchronizes them with the corresponding `TransformComponent`s in the engine's scene.
    *   **Resource Management:** Manages the creation and destruction of `IPhysicsBody` objects via the PAL.
*   **Dependencies:** `IPhysicsWorld` (PAL interface), `JobSystem` (for potentially parallelizing aspects of physics processing or synchronization), `TransformComponent` (for scene synchronization).

### 5.9. `PostProcessingManager` (C++ Class)

*   **Responsibilities:** Manage and apply post-processing effects.
    *   Maintain a chain of effects (Bloom, TAA, HDR, etc.).
    *   Allocate intermediate `IFrameBuffer`s.
    *   Orchestrate the application of post-processing shaders using `IRenderContext`.
    *   **AI-Powered Upscaling:** Integrate and manage AI-based upscaling techniques (e.g., AMD FSR, NVIDIA DLSS, Intel XeSS) as a final step in the chain. This allows rendering the scene at a lower resolution and intelligently reconstructing it to the target resolution, providing a major performance boost.
*   **Dependencies:** `IRenderContext`, `IFrameBuffer`, `IShaderProgram`, `ITexture`.

## 6. Piece.Framework Rendering Flow (Orchestrated by RenderSystem in Piece.Core)

1.  **`GameLoop` (C#):** Calls `RenderSystem.RenderFrame(Camera activeCamera, Scene currentScene)` on the C# wrapper.
2.  **`RenderSystem` (C++):**
    *   Calls `graphicsDevice->BeginFrame()`.
    *   Gets `renderContext = graphicsDevice->GetImmediateContext()`.
    *   Configures the viewport (`renderContext->SetViewport`).
    *   Configures the camera (passes View/Projection to an `IUniformBuffer` and binds).
    *   **Step 1: G-Buffer/Base Pass (if Deferred):**
        *   For each visible `Model`:
            *   Calls `renderContext->SetShaderProgram(model.Material.ShaderProgram)`.
            *   Calls `renderContext->SetTexture(...)` for material textures.
            *   Calls `renderContext->SetUniformBuffer(...)` for material parameters and model transformations.
            *   Calls `renderContext->SetVertexBuffer(...)`, `renderContext->SetIndexBuffer(...)`.
            *   Calls `renderContext->DrawIndexed(...)`.
    *   **Step 2: Light Pass (if Deferred):**
        *   Renders a full-screen quad.
        *   Lighting shader reads from G-Buffer and applies lights (from `Light`s).
    *   **Step 3: Transparency/Forward:**
        *   Sorts transparent objects.
        *   Renders them, calling `renderContext->SetShaderProgram`, etc.
    *   **Step 4: Post-processing:**
        *   `postProcessingManager->ApplyEffects(renderContext, mainFrameBuffer, finalFrameBuffer)`.
    *   Calls `graphicsDevice->EndFrame()`.

## 7. Resource and Cache Management (ResourceManager in C++)

The `ResourceManager` in C++ is fundamental. Instead of the C# engine user directly creating RAL resources, they would ask the `ResourceManager` through C# wrappers: `ResourceManager.LoadMesh("path/to/mesh.obj")`.

*   `ResourceManager` would call `graphicsDevice->CreateVertexBuffer(...)` (returning `std::unique_ptr`).
*   Would maintain a `std::unordered_map<std::string, std::unique_ptr<IResource>>` for caching.
*   Would be responsible for loading assets from files and creating corresponding RAL resources.
*   Would ensure proper release of RAL resources via `std::unique_ptr` when C++ resources are destroyed.

## 8. Benefits of the Proposed Architecture

The hybrid C++/C# architecture of the Piece.Core, now deeply integrated with the C# .NET Dependency Injection system, offers an ideal balance between performance, control, and flexibility, perfectly aligning with the Piece Engine's philosophy of **Modular Component Architecture**. The Piece.Core embodies the core principles of modularity and extensibility as described in the [Piece Engine General Design Document](../OVERVIEW.md).

In summary, this refined layer ensures:
*   **Optimal Performance:** Critical performance-sensitive components and GPU resource management remain entirely in C++, minimizing P/Invoke overhead in tight loops. (Communication with Piece.Framework)
*   **Low-Level Control:** Leverages C++'s flexibility and memory control for GPU resource management and optimizations, with RAII and `std::unique_ptr`.
*   **Clean and Ergonomic C# API:** Piece.Framework game developers configure and interact with the engine using a familiar, high-level .NET DI pattern, allowing them to focus on game logic.
*   **Memory Safety:** RAL/PAL resources are managed with `std::unique_ptr` in the C++ layer, leveraging RAII to prevent leaks, with C# wrappers managing their corresponding native pointers.
*   **Enhanced Modular Extensibility:** By delegating the selection and configuration of C++ backends and internal components to the Piece.Framework's DI system, the engine achieves an even higher degree of modularity. Developers can easily swap out entire backend implementations (e.g., OpenGL for Vulkan) or specific C++ sub-components simply by adjusting their Piece.Framework application's DI configuration. This truly makes the C++ layers configurable components from the Piece.Framework side, fully aligning with the principles of the Modular Component Architecture.

## 9. Granular Extension Points

The Piece.Core is a critical hub for orchestration and management. Its architecture, now heavily integrated with the C# .NET DI system via the C++ `ServiceLocator`, allows for exceptionally granular extensibility through its adherence to the philosophy of **Modular Component Architecture**. For a complete discussion of this philosophy and extension points across all engine layers, please refer to the [Piece Engine General Design Document](../OVERVIEW.md).

The main specific extension points of this layer now include:

*   **Complete Replacement of the Piece.Core:** Allows the user to provide their own `MyCustomCore.dll` that implements all C-compatible API functions, enabling a total re-engineering of resource management logic and the pipeline. This also implies that such a custom DLL would then internally define and manage its own factory resolution mechanism, or mimic the `ServiceLocator` pattern to integrate with the Piece.Framework's DI.
*   **Partial Replacement / Extension of Internal Components (C# DI-driven):** Through the C++ `ServiceLocator`, whose factories are supplied by the C# application via .NET DI, users can inject their own implementations of core intermediate subsystems. For instance, a user could provide a custom `RenderSystemFactory` or `ResourceManagerFactory` (implemented in C++ and exposed via a C# wrapper and NuGet package). The Piece.Core would then request these custom factories from its `ServiceLocator` and use them to create the desired `RenderSystem` or `ResourceManager`.
*   **Extension of Functionalities via C++ Plugins:** Components can be designed to dynamically load DLLs that provide specific implementations for certain interfaces (e.g., a customized `ICullingStrategy`). This mechanism can also be integrated with the `ServiceLocator` pattern, where the C# host provides a factory that knows how to load and create instances from such specific plugin DLLs.