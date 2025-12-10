# Design Document - Low-Level Layer of the Piece Engine (C++)

This document describes the architecture of the low-level implementations of the Piece graphics engine, implemented in C++. It aligns with the engine's philosophy of **Modular Component Architecture** by being modular, extensible, and agnostic concerning graphics and windowing APIs. This low-level implementation provides the essential abstractions for interacting with the GPU and the operating system in a performant manner.

## 1. General Low-Level Implementation Architecture

The low-level implementation architecture is composed of two main abstraction layers:

1.  **Window Abstraction Layer (WAL):** Provides a generic interface for managing windows, graphics contexts, and user inputs.
2.  **Render Abstraction Layer (RAL):** Defines a common set of interfaces for low-level rendering resources and commands, regardless of the underlying graphics API (OpenGL, Vulkan, DirectX).

This low-level implementation is the foundation upon which the Piece.Core (C++) and, subsequently, the Piece.Framework (C#) will be built. Communication with the Piece.Core will be direct, without interop overhead, as both are in C++.

### Proposed Directory Structure for the Low-Level Implementations (inside `src/`)

```
src/
├── wal/                        # Window Abstraction Layer (Interfaces)
│   └── iwindow.h               # IWindow Interface (Window Abstraction)
├── ral/                        # Render Abstraction Layer (Interfaces)
│   ├── ral_types.h             # Common Enums and Structs for RAL
│   ├── iacceleration_structure.h # IAccelerationStructure Interface
│   ├── icompute_buffer.h       # IComputeBuffer Interface
│   ├── iframebuffer.h          # IFrameBuffer Interface
│   ├── iindirect_draw_buffer.h # IIndirectDrawBuffer Interface
│   ├── iindex_buffer.h         # IIndexBuffer Interface
│   ├── irender_context.h       # IRenderContext Interface
│   ├── isampler.h              # ISampler Interface
│   ├── ishader.h               # IShader Interface (Shader Module)
│   ├── ishader_program.h       # IShaderProgram Interface (Linked Shader Program)
│   ├── itexture.h              # ITexture Interface (Texture Data)
│   ├── iuniform_buffer.h       # IUniformBuffer Interface
│   └── ivertex_buffer.h        # IVertexBuffer Interface
├── gfx_glfw/                   # WAL Implementation with GLFW
├── gfx_opengl/                 # RAL Implementation with OpenGL
├── gfx_vulkan/                 # RAL Implementation with Vulkan (future)
├── gfx_directx/                # RAL Implementation with DirectX (future)
└── app_example/                # Example application using the backend
```

## 2. Window Abstraction Layer (WAL)

The WAL is defined by the `IWindow` interface, which encapsulates interaction with the windowing system and input processing.

**Interface (Conceptual):**
```cpp
// KeyCode enum definition and other input enums would be in a common WAL file
enum class KeyCode : int { /* ... */ };

class IWindow {
public:
    virtual ~IWindow() = default;
    // The actual creation and initial setup of the window should occur in the factory or constructor
    // The Init method can be used for post-construction setup that requires the window to exist.
    virtual void Init(const NativeWindowOptions& options) = 0; // Assuming a NativeWindowOptions struct is defined
    virtual void PollEvents() = 0;
    virtual void SwapBuffers() = 0;
    virtual bool ShouldClose() const = 0;
    virtual void* GetNativeWindow() const = 0; // For integration with the graphics API

    // Input Methods
    virtual bool IsKeyPressed(KeyCode keycode) const = 0;
    virtual bool IsMouseButtonPressed(KeyCode button) const = 0;
    virtual std::pair<float, float> GetMousePosition() const = 0;
    virtual float GetMouseX() const = 0;
    virtual float GetMouseY() const = 0;
};
```
**Note:** The `IWindow` interface provides a polling model for input, sufficient for the low-level layer. A more robust event system could be built in higher layers.

The `NativeWindowOptions` struct, defined in the Piece.Core's `NativeExports.h`, would be marshaled from C# and passed to the factory and eventually to the window's constructor/Init method.

## 3. Render Abstraction Layer (RAL)

The RAL is the graphical heart of the engine, providing interfaces for device management, rendering contexts, and GPU resources. The RAL aims to be graphics API-agnostic, allowing concrete implementations (OpenGL, Vulkan, DirectX) to be swapped.

### 3.1. `IGraphicsDevice` (Graphics Device Manager)

*   **Responsibilities:** Graphics API initialization and destruction, management of GPU resource lifecycle (returning `std::unique_ptr` for automatic memory management), and provision of rendering contexts.

**Interface (Conceptual):**
```cpp
// TextureFormat enum definition would be in ral_types.h
enum class TextureFormat { /* ... */ };

class IGraphicsDevice {
public:
    virtual ~IGraphicsDevice() = default;
    virtual void Init(IWindow* window, const NativeVulkanOptions& options) = 0; // Assuming NativeVulkanOptions struct is defined
    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;
    virtual IRenderContext* GetImmediateContext() = 0;

    // Resource Creation Methods (returning unique_ptr for lifetime management)
    virtual std::unique_ptr<IVertexBuffer> CreateVertexBuffer(const void* data, uint32_t size, const VertexLayout& layout) = 0;
    virtual std::unique_ptr<IIndexBuffer> CreateIndexBuffer(const uint32_t* data, uint32_t count) = 0;
    virtual std::unique_ptr<IShader> CreateShaderModule(ShaderStage stage, const std::string& source) = 0;
    virtual std::unique_ptr<IShaderProgram> CreateShaderProgram(const std::vector<IShader*>& shaderModules) = 0;
    virtual std::unique_ptr<ITexture> CreateTexture(TextureType type, uint32_t width, uint32_t height, uint32_t depth, TextureFormat format, const void* data = nullptr): unique_ptr<ITexture>
    virtual std::unique_ptr<ISampler> CreateSampler(TextureFilter minFilter, TextureFilter magFilter, TextureWrap sWrap, TextureWrap tWrap): unique_ptr<ISampler>
    virtual std::unique_ptr<IComputeBuffer> CreateComputeBuffer(uint32_t size, BufferUsage usage, const void* data = nullptr): unique_ptr<IComputeBuffer>
    virtual std::unique_ptr<IIndirectDrawBuffer> CreateIndirectDrawBuffer(uint32_t size, const void* data = nullptr): unique_ptr<IIndirectDrawBuffer>
    virtual std::unique_ptr<IAccelerationStructure> CreateAccelerationStructure(info: const AccelerationStructureBuildInfo&): unique_ptr<IAccelerationStructure>
    virtual std::unique_ptr<IFrameBuffer> CreateFrameBuffer(uint32_t width, uint32_t height) = 0;
    virtual std::unique_ptr<IUniformBuffer> CreateUniformBuffer(uint32_t size, const void* data = nullptr) = 0;
};
```
**Note:** Factory methods return `std::unique_ptr` to ensure ownership and lifecycle of C++ resources in a safe and idiomatic way. The `NativeVulkanOptions` struct would be defined in the Piece.Core's `NativeExports.h` and marshaled from C# for configuration.

### 3.2. `IRenderContext` (Rendering Context)

*   **Responsibilities:** Record and submit drawing commands and state changes to the GPU. Implementations for modern APIs (Vulkan/DX12) will translate these calls into Pipeline State Objects (PSOs) and Command Buffer management.

**Interface (Conceptual):**
```cpp
// Definitions for DepthFunc, BlendMode, CullMode, TextureFilter, TextureWrap enums would be in ral_types.h

class IRenderContext {
public:
    virtual ~IRenderContext() = default;

    virtual void Clear(float r, float g, float b, float a) = 0;
    virtual void SetViewport(int x, int y, int width, int height) = 0;
    virtual void BindFrameBuffer(IFrameBuffer* framebuffer) = 0;

    // Resource Binding
    virtual void SetVertexBuffer(IVertexBuffer* vertexBuffer) = 0;
    virtual void SetIndexBuffer(IIndexBuffer* indexBuffer) = 0;
    virtual void SetShaderProgram(IShaderProgram* shaderProgram) = 0;
    virtual void SetTexture(ITexture* texture, uint32_t slot) = 0;
    virtual void SetSampler(ISampler* sampler, uint32_t slot) = 0;
    virtual void SetUniformBuffer(IUniformBuffer* uniformBuffer, uint32_t bindingPoint) = 0;
    virtual void SetStorageBuffer(IComputeBuffer* computeBuffer, uint32_t bindingPoint) = 0;
    virtual void SetAccelerationStructure(IAccelerationStructure* as) = 0;

    // Render State Commands
    virtual void SetDepthTest(bool enable) = 0;
    virtual void SetDepthFunc(DepthFunc func) = 0;
    virtual void SetDepthMask(bool enable) = 0;
    virtual void SetBlendMode(BlendMode mode) = 0;
    virtual void SetCullMode(CullMode mode) = 0;

    // Drawing Commands
    virtual void DrawIndexed(uint32_t indexCount, uint32_t startIndex, int32_t baseVertex) = 0;
    virtual void DrawArrays(uint32_t vertexCount, uint32_t startVertex) = 0;
    virtual void DrawIndexedInstanced(uint32_t indexCount, uint32_t instanceCount, uint32_t startIndex, int32_t baseVertex, uint32_t baseInstance) = 0;
    virtual void DrawArraysInstanced(uint32_t vertexCount, uint32_t instanceCount, uint32_t startVertex, uint32_t baseInstance) = 0;
    virtual void DispatchCompute(uint32_t groupX, uint32_t groupY, uint32_t groupZ) = 0;
    virtual void DispatchMesh(uint32_t groupX, uint32_t groupY, uint32_t groupZ) = 0;
    virtual void TraceRays(uint32_t width, uint32_t height, uint32_t depth = 1) = 0;
    virtual void SetVariableRateShading(const VRSSettings& settings) = 0;
};
```
**Note:** Individual state methods are maintained for abstraction simplicity, even if implementations for modern APIs translate these calls into internal PSO management and swapping.

### 3.3. `IVertexBuffer` (Vertex Buffer)

*   **Responsibilities:** Store vertex data on the GPU.

**Interface (Conceptual):**
```cpp
// Definitions of VertexDataType, VertexLayoutElement, VertexLayout would be in ral_types.h
class IVertexBuffer {
public:
    virtual ~IVertexBuffer() = default;
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual void SetData(const void* data, uint32_t size) = 0;
    virtual uint32_t GetSize() const = 0;
    virtual const VertexLayout& GetLayout() const = 0;
};
```

### 3.4. `IIndexBuffer` (Index Buffer)

*   **Responsibilities:** Store indices for vertices on the GPU.

**Interface (Conceptual):**
```cpp
class IIndexBuffer {
public:
    virtual ~IIndexBuffer() = default;
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual void SetData(const uint32_t* data, uint32_t count) = 0;
    virtual uint32_t GetCount() const = 0;
};
```

### 3.5. `IShader` (Shader Module)

*   **Responsibilities:** Represent a single compiled shader stage (e.g., Vertex, Fragment, Compute).

**Interface (Conceptual):**
```cpp
class IShader {
public:
    virtual ~IShader() = default;
};
```
**Note:** This interface is now a pure container for a compiled module. Binding and uniform manipulation methods have been moved to `IShaderProgram`.

### 3.6. `IShaderProgram` (Linked Shader Program)

*   **Responsibilities:** Abstract the complete linked shader program and manage the definition of its uniforms.

**Interface (Conceptual):**
```cpp
class IShaderProgram {
public:
    virtual ~IShaderProgram() = default;
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual void SetUniformInt(const std::string& name, int value) = 0;
    virtual void SetUniformFloat(const std::string& name, float value) = 0;
    virtual void SetUniformFloat2(const std::string& name, float v0, float v1) = 0;
    virtual void SetUniformFloat3(const std::string& name, float v0, float v1, float v2) = 0;
    virtual void SetUniformFloat4(const std::string& name, float v0, float v1, float v2, float v3) = 0;
    virtual void SetUniformMat4(name: const std::string&, matrix: const float*): void
};
```
**Note:** This interface now contains the `SetUniform...` and `Bind`/`Unbind` methods that were previously in `IShader`.

### 3.7. `ITexture` (Texture Data)

*   **Responsibilities:** Store image data on the GPU. Does not manage sampling states.

**Interface (Conceptual):
```cpp
class ITexture {
public:
    virtual ~ITexture() = default;
    virtual void Bind(uint32_t slot = 0) const = 0;
    virtual void Unbind() const = 0;
    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;
    virtual void* GetNativeID() const = 0;
};
```
**Note:** `SetFilter` and `SetWrap` methods have been removed, and their responsibility transferred to `ISampler`.

### 3.8. `IFrameBuffer` (Framebuffer Object)

*   **Responsibilities:** Manage off-screen rendering targets.

**Interface (Conceptual):**
```cpp
class IFrameBuffer {
public:
    virtual ~IFrameBuffer() = default;
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual ITexture* GetColorAttachment(uint32_t index = 0) const = 0;
    virtual ITexture* GetDepthAttachment() const = 0;
    virtual void Resize(uint32_t width, uint32_t height) = 0;
};
```

### 3.9. `IUniformBuffer` (Uniform Buffer)

*   **Responsibilities:** Provide a block of data for shaders that can be updated efficiently.

**Interface (Conceptual):**
```cpp
class IUniformBuffer {
public:
    virtual ~IUniformBuffer() = default;
    virtual void SetData(const void* data, uint32_t size) = 0;
    virtual uint32_t GetSize() const = 0;
};
```

### 3.10. `ISampler` (Texture Sampling State)

*   **Responsibilities:** Define how a texture is sampled by shaders (filtering, wrap modes).

**Interface (Conceptual):**
```cpp
// Definitions for TextureFilter and TextureWrap enums would be in ral_types.h
class ISampler {
public:
    virtual ~ISampler() = default;
    virtual void Bind(uint32_t slot = 0) const = 0;
    virtual void Unbind() const = 0;
};
```
**Note:** This interface was introduced to separate sampling state from texture data, a modern GPU practice.

### 3.11. `IComputeBuffer` (Compute/Storage Buffer)

*   **Responsibilities:** Manage GPU memory buffers that can be read and written by compute shaders.

**Interface (Conceptual):**
```cpp
class IComputeBuffer {
public:
    virtual ~IComputeBuffer() = default;
    virtual void SetData(const void* data, uint32_t size) = 0;
    virtual uint32_t GetSize() const = 0;
};
```

### 3.12. `IIndirectDrawBuffer` (Indirect Draw Buffer)

*   **Responsibilities:** Store parameters for drawing calls that are issued by the GPU (GPU-Driven Rendering).

**Interface (Conceptual):**
```cpp
class IIndirectDrawBuffer {
public:
    virtual ~IIndirectDrawBuffer() = default;
    virtual void SetData(const void* data, uint32_t size) = 0;
    virtual uint32_t GetSize() const = 0;
};
```

### 3.13. `IAccelerationStructure` (Acceleration Structure for Ray Tracing)

*   **Responsibilities:** Abstract hierarchical data structures (e.g., BVH) to accelerate Ray Tracing queries.

**Interface (Conceptual):**
```cpp
// Definition of AccelerationStructureBuildInfo would be in ral_types.h
struct AccelerationStructureBuildInfo { /* ... */ };

class IAccelerationStructure {
public:
    virtual ~IAccelerationStructure() = default;
    virtual void Build(const AccelerationStructureBuildInfo& info) = 0;
    virtual void Update(const AccelerationStructureBuildInfo& info) = 0;
};
```

### 3.14. Common RAL Types and Enums (in `ral_types.h`)

*   **`ShaderStage`:** Enum for shader stages (Vertex, Fragment, Compute, Task, Mesh, RayGen, etc.).
*   **`TextureType`:** Enum for texture types (1D, 2D, 3D, Cube, etc.).
*   **`BufferUsage`:** Flags to indicate buffer usage (Vertex, Index, Uniform, Storage, Indirect, Read, Write).
*   **`VertexDataType`, `VertexLayoutElement`, `VertexLayout`:** Structs to describe vertex layout.
*   **`GeometryData`, `AccelerationStructureBuildInfo`:** Structs for AS build information.
*   **`DepthFunc`, `BlendMode`, `CullMode`:** Enums for render states (previously in `IRenderContext`).
*   **`TextureFilter`, `TextureWrap`:** Enums for texture sampling states (previously in `ITexture`/`ISampler`).
*   **`VRSSettings`, `VRSShadingRate`, `VRSCombiner`:** Struct and enums for Variable Rate Shading configuration.

## 4. Physics Abstraction Layer (PAL)

The PAL defines a common set of C++ interfaces for physics simulation, allowing the engine to integrate various physics engines as pluggable implementations.

### 4.1. `IPhysicsWorld` (Physics World Manager)

*   **Responsibilities:** Manages the physics simulation environment (gravity, broad-phase/narrow-phase settings), acts as a factory for physics objects, and controls the simulation step.

**Interface (Conceptual):**
```cpp
// Forward declarations for other PAL interfaces
class IPhysicsBody;
class IColliderShape;
class IPhysicsMaterial;
class IJoint;

class IPhysicsWorld {
public:
    virtual ~IPhysicsWorld() = default;
    virtual void Init(const NativePhysicsOptions& options) = 0; // Assuming NativePhysicsOptions struct is defined
    virtual void Step(float deltaTime) = 0;
    virtual void SetGravity(float x, float y, float z) = 0;

    // Factory Methods for Physics Objects (returning unique_ptr)
    virtual std::unique_ptr<IPhysicsBody> CreateRigidBody(const RigidBodyCreationInfo& info) = 0;
    virtual std::unique_ptr<IColliderShape> CreateBoxShape(float halfExtentX, float halfExtentY, float halfExtentZ) = 0;
    virtual std::unique_ptr<IColliderShape> CreateSphereShape(float radius) = 0;
    virtual std::unique_ptr<IPhysicsMaterial> CreatePhysicsMaterial(float friction, float restitution) = 0;
    // ... other factory methods for shapes, joints, etc.
};
```
**Note:** Factory methods return `std::unique_ptr` for safe ownership and lifecycle management, similar to RAL. The `NativePhysicsOptions` struct would be defined in the Piece.Core's `NativeExports.h` and marshaled from C# for configuration.

### 4.2. `IPhysicsBody` (Physics Body)

*   **Responsibilities:** Represents a simulated object (e.g., rigid body). Manages its physical properties (mass, velocity, damping) and its current transform (position, rotation).

**Interface (Conceptual):**
```cpp
// Forward declarations for other PAL interfaces
class IColliderShape;
class IPhysicsMaterial;

class IPhysicsBody {
public:
    virtual ~IPhysicsBody() = default;

    virtual void SetShape(IColliderShape* shape) = 0;
    virtual void SetMaterial(IPhysicsMaterial* material) = 0;
    virtual void SetMass(float mass) = 0;
    virtual void SetPosition(float x, float y, float z) = 0;
    virtual void SetRotation(float x, float y, float z, float w) = 0; // Quaternion
    virtual void SetLinearVelocity(float x, float y, float z) = 0;
    virtual void SetAngularVelocity(float x, float y, float z) = 0;
    virtual void ApplyForce(float x, float y, float z) = 0;

    virtual void GetPosition(float& x, float& y, float& z) const = 0;
    virtual void GetRotation(float& x, float& y, float& z, float& w) const = 0;
};
```

### 4.3. `IColliderShape` (Collider Shape)

*   **Responsibilities:** Defines the geometry used for collision detection.

**Interface (Conceptual):**
```cpp
class IColliderShape {
public:
    virtual ~IColliderShape() = default;
    // Specific shape properties might be accessed via downcasting or additional methods
};
```

### 4.4. `IPhysicsMaterial` (Physics Material)

*   **Responsibilities:** Defines surface properties like friction and restitution.

**Interface (Conceptual):**
```cpp
class IPhysicsMaterial {
public:
    virtual ~IPhysicsMaterial() = default;
    virtual float GetFriction() const = 0;
    virtual float GetRestitution() const = 0;
};
```

### 4.5. `IJoint` (Joint/Constraint)

*   **Responsibilities:** Defines a connection or constraint between two physics bodies.

**Interface (Conceptual):**
```cpp
// Forward declaration
class IPhysicsBody;

class IJoint {
public:
    virtual ~IJoint() = default;
    virtual void SetBodies(IPhysicsBody* body1, IPhysicsBody* body2) = 0;
    // ... specific joint parameters like limits, springs, etc.
};
```

### 4.6. Common PAL Types and Enums (in `pal_types.h`)

*   **`RigidBodyCreationInfo`:** Struct containing information needed to create a rigid body (initial position, rotation, mass, etc.).
*   **`PhysicsQuality`:** Enum for simulation quality settings (e.g., low, medium, high).
*   **`CollisionFlags`:** Bitmask for collision filtering.

## 5. Low-Level Implementation Directory Structure (inside `src/`)

```
src/
├── wal/                        # Window Abstraction Layer (Interfaces)
│   └── iwindow.h               # IWindow Interface (Window Abstraction)
├── ral/                        # Render Abstraction Layer (Interfaces)
│   ├── ral_types.h             # Common Enums and Structs for RAL
│   ├── iacceleration_structure.h # IAccelerationStructure Interface
│   ├── icompute_buffer.h       # IComputeBuffer Interface
│   ├── iframebuffer.h          # IFrameBuffer Interface
│   ├── iindirect_draw_buffer.h # IIndirectDrawBuffer Interface
│   ├── iindex_buffer.h         # IIndexBuffer Interface
│   ├── irender_context.h       # IRenderContext Interface
│   ├── isampler.h              # ISampler Interface
│   ├── ishader.h               # IShader Interface (Shader Module)
│   ├── ishader_program.h       # IShaderProgram Interface (Linked Shader Program)
│   ├── itexture.h              # ITexture Interface (Texture Data)
│   ├── iuniform_buffer.h       # IUniformBuffer Interface
│   └── ivertex_buffer.h        # IVertexBuffer Interface
├── pal/                        # Physics Abstraction Layer (Interfaces)
│   ├── pal_types.h             # Common Enums and Structs for PAL
│   ├── icollider_shape.h       # IColliderShape Interface
│   ├── ijoint.h                # IJoint Interface
│   ├── iphysics_body.h         # IPhysicsBody Interface
│   ├── iphysics_material.h     # IPhysicsMaterial Interface
│   └── iphysics_world.h        # IPhysicsWorld Interface
├── gfx_glfw/                   # WAL Implementation with GLFW
├── gfx_opengl/                 # RAL Implementation with OpenGL
├── gfx_vulkan/                 # RAL Implementation with Vulkan (future)
├── gfx_directx/                # RAL Implementation with DirectX (future)
├── physics_jolt/               # PAL Implementation with Jolt Physics (example)
├── physics_physx/              # PAL Implementation with NVIDIA PhysX (future)
└── app_example/                # Example application using the backend
```

## 6. Usage Example (Removed from Core Low-Level Implementation Design)

The C++ low-level implementation usage example has been moved to the Piece.Core design document, where the interaction between the Piece.Core and the low-level implementation will be detailed.

## 7. Observations and Conclusions (Analysis of Modern Graphics Engines)

This section summarizes insights obtained from the analysis of modern graphics engines (Unreal Engine, Unity, Godot, Stride, etc.) and how they informed our architecture's design. The robustness of our abstraction architecture (WAL/RAL/PAL) is reinforced, with the separation between `IGraphicsDevice` (device and resource management), `IPhysicsWorld` (physics world management), and `IRenderContext` (command recording) being an essential pattern for modern APIs like Vulkan, DirectX 12, and advanced physics engines.

### 7.1 Resource Lifecycle Management

*   **Note:** The factory methods of `IGraphicsDevice` and `IPhysicsWorld` return `std::unique_ptr` to ensure the ownership and lifecycle of C++ resources in a safe and idiomatic way.

### 7.2 Shader Compilation and Management

*   **Note:** The explicit separation between `IShader` (single-stage shader module) and `IShaderProgram` (linked program that combines modules and defines uniforms) aligns with modern engines that manage shaders modularly. Runtime compilation from strings is suitable for development; for production, the ideal is the use of pre-compiled and offline optimized binaries. `IUniformBuffer` is the abstraction for uniform blocks, allowing efficient data passing to shaders.

### 7.3 Advanced Drawing Calls (Instancing, Indirect, Mesh Shaders, Ray Tracing)

*   **Note:** The `IRenderContext` has been extended with methods such as `DrawIndexedInstanced`, `DrawArraysInstanced`, `DrawIndirect`, `DrawIndexedIndirect`, `DispatchCompute`, `DispatchMesh`, and `TraceRays` to support modern GPU-based rendering techniques. New interfaces like `IComputeBuffer`, `IIndirectDrawBuffer`, and `IAccelerationStructure` provide the necessary resources for these operations.

### 7.4 Granularity of Render State and PSOs (Pipeline State Objects)

*   **Note:** The `IRenderContext` maintains individual state methods (e.g., `SetDepthTest`), with the expectation that implementations for modern APIs (Vulkan/DX12) will translate these calls into internal PSO management and swapping. This approach simplifies abstraction without imposing PSO complexity at the interface level.

### 7.5 Texture Formats and Sampling (Samplers)

*   **Note:** The introduction of `ISampler` as an interface separate from `ITexture` (which is now purely a container for pixel data) reflects a modern GPU practice, offering greater flexibility and optimization. `TextureFormat`, `TextureFilter`, and `TextureWrap` have been centralized in `ral_types.h`.

### 7.6 Input System (Events vs. Polling)

*   **Note:** The `IWindow` provides a polling model for input, suitable for the low-level layer. A more robust and high-level event system is expected and will be built in the upper layer.

### 7.7 Integration with Math Library

*   **Note:** The use of a mature and optimized math library (e.g., GLM for C++) is suggested as a fundamental dependency for the engine's C++ implementations, in order to simplify mathematical operations and ensure performance.

### 7.8 Variable Rate Shading (VRS)

*   **Note:** To support modern performance optimization techniques, the `IRenderContext` now includes a `SetVariableRateShading` method. This allows the engine to vary the shading rate for different regions of the screen, focusing GPU power on areas of high detail while saving resources in less critical areas (e.g., periphery or motion-blurred regions). This feature is essential for achieving high framerates at high resolutions.

## 8. Granular Extension Points

The WAL/RAL/PAL architecture is designed for extreme modularity, enabling the complete replacement or granular customization of its components. This fully adheres to the Piece Engine's philosophy of **Modular Component Architecture**, with the configuration orchestrated by the high-level C# application via .NET DI.

### 8.1 Complete Low-Level Implementation Replacement (C# DI-driven)

*   **Mechanism:** Users can provide their own `gfx_vulkan.dll`, `gfx_directx.dll`, `physics_jolt.dll`, or `physics_custom.dll` libraries. These native DLLs **must** implement the core WAL/RAL/PAL interfaces and, crucially, **export C-style factory creation functions**.
*   **Factory Exports:** Each low-level implementation DLL needs to export a function that creates and returns a raw pointer to an instance of its concrete C++ factory implementation (e.g., `VulkanGraphicsDeviceFactory*` implementing `IGraphicsDeviceFactory`). These factory functions can accept C-compatible configuration structs as parameters, marshaled from the C# application.
    **Conceptual C++ Low-Level Implementation Export Example:**
    ```cpp
    // In gfx_vulkan.dll or similar native low-level implementation project
    #include "../../Piece.Intermediate/interfaces/igraphics_device_factory.h"
    #include "vulkan_graphics_device_factory.h" // Concrete implementation
    #include "../../Piece.Intermediate/NativeExports.h" // For NativeVulkanOptions

    extern "C" {
        __declspec(dllexport) Piece::Intermediate::IGraphicsDeviceFactory* CreateVulkanGraphicsDeviceFactory(
            const NativeVulkanOptions* options) {
            // Options are passed from C# DI configuration
            return new VulkanGraphicsDeviceFactory(options);
        }

        __declspec(dllexport) void DestroyVulkanGraphicsDeviceFactory(
            Piece::Intermediate::IGraphicsDeviceFactory* factory) {
            delete factory;
        }
    }
    ```
*   **C# Orchestration:** The Piece.Framework (C#) application, via its .NET DI configuration, resolves a C# wrapper for this factory. This C# wrapper then loads the native DLL, calls the exported factory function (passing marshaled options), and eventually pushes the resulting C++ factory pointer to the Piece.Core's `ServiceLocator`.

### 8.2 Partial Replacement of RAL/WAL/PAL Implementations (C# DI-driven Abstract Factories)

*   **Mechanism:** To allow the user to replace only a specific type of RAL/WAL/PAL resource (e.g., a custom `IVertexBuffer` implementation, or an `IPhysicsMaterial` behavior) within an existing low-level implementation, the respective C++ factory interfaces (e.g., `IGraphicsDeviceFactory`, `IPhysicsWorldFactory`) can be designed to accept *internal factory interfaces* for these sub-components.
*   **Example:** An `IGraphicsDeviceFactory` might accept an `IVertexBufferFactory` during its construction. This `IVertexBufferFactory` itself could be another C++ object, managed and provided by the C# DI system.
    ```cpp
    // Vertex Buffer Factory Interface (defined in Piece.Core or common abstractions)
    class IVertexBufferFactory {
    public:
        virtual ~IVertexBufferFactory() = default;
        virtual std::unique_ptr<IVertexBuffer> create(const void* data, uint32_t size, const VertexLayout& layout) = 0;
    };
    // IGraphicsDeviceFactory might accept this in its constructor
    class VulkanGraphicsDeviceFactory : public IGraphicsDeviceFactory {
        public:
        VulkanGraphicsDeviceFactory(const NativeVulkanOptions* options, std::unique_ptr<IVertexBufferFactory> vbFactory) {
            // ... store vbFactory and use it to create vertex buffers ...
        }
        // ...
    };
    ```
*   **Benefit:** This provides granular control, where a user can provide their own `MyOptimizedVulkanVertexBufferFactory` (as a C++ DLL exporting its factory) and have it injected into the existing `VulkanGraphicsDeviceFactory` via C# DI.

### 8.3 Dynamic Extension of Functionalities (Plugins)

*   **Potential:** Low-level implementations can be designed to dynamically load DLLs that provide specific implementations for certain functionalities (e.g., a plugin for a custom broad-phase collision detection algorithm or a specific ray tracing technique). These plugins can register themselves with internal mechanisms or, again, be managed via the C# DI system providing factories for these specific functionalities to the C++ side.

These granular extension points transform the low-level implementation into a true set of **interchangeable building blocks**, where even the fundamental pieces can be customized with precision through a consistent C# DI-driven configuration.

---

**Note:** This document describes only the low-level C++ implementations (WAL/RAL/PAL). For an overview of the engine's multi-layered architecture and the guiding philosophy of **Modular Component Architecture**, please refer to the [Piece Engine General Design Document](../OVERVIEW.md). The Piece.Core (C++) and the Piece.Framework (C#) are detailed in their respective design documents.