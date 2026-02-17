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

### Refactor C++ Error Handling to New Policy

  - tags: [v1.0.0, c++, error-handling, refactor]
  - priority: high
  - steps:
      - [ ] Implement `Result<T>` or equivalent type for recoverable errors.
      - [ ] Replace exceptions for recoverable failures with `Result<T>`.
      - [ ] Ensure exceptions do not cross module or ABI boundaries.
      - [ ] Replace programming error checks with assertions.
      - [ ] Identify and implement `fail` for fatal errors.
      - [ ] Update function signatures to reflect `Result<T>` return types where applicable.
      - [ ] Review and update existing C++ codebase (PAL, RAL, WAL, Piece.Core) to conform to the new policy.
      - [ ] Verify that all error types (recoverable, programming, fatal) are handled according to the guidelines.
    ```md
    Review and refactor the entire C++ codebase to strictly adhere to the newly defined error handling strategy. This includes implementing Result-based error handling for recoverable errors, using assertions for programming errors, and employing a 'fail' mechanism for fatal errors, ensuring deterministic and zero-overhead error management.
    ```

### Implement RAL OpenGL Backend (Real)

  - tags: [v1.0.0, c++, ral, opengl]
  - priority: high
  - steps:
      - [ ] Implement `OpenGLGraphicsDevice` and `OpenGLRenderContext` methods with actual OpenGL API calls.
      - [ ] Implement all `OpenGLXxx` resource classes (`OpenGLVertexBuffer`, `OpenGLTexture`, etc.) with actual OpenGL API calls.
    ```md
    Replace the stubbed OpenGL backend implementations with functional, OpenGL-API-driven code for rendering.
    ```

### Implement PAL Minimal Backend (Real)

  - tags: [v1.0.0, c++, pal, minimal]
  - priority: high
  - steps:
      - [ ] Implement `MinimalPhysicsWorld` methods with basic simulation logic (e.g., AABB collision, simple integration).
      - [ ] Implement `MinimalPhysicsBody` methods with actual state changes and interactions.
      - [ ] Implement `MinimalColliderShape` and `MinimalPhysicsMaterial` with basic properties.
    ```md
    Replace the stubbed Minimal PAL backend implementations with basic but functional physics simulation logic.
    ```

### Implement full ResourceManager for loading assets

  - tags: [v1.0.0, c++, resources]
  - priority: high
  - steps:
      - [ ] Implement `ResourceManager::LoadMaterial` to parse material files and create RAL materials.
      - [ ] Implement `ResourceManager::LoadMesh` to load 3D models (e.g., OBJ, glTF) and create RAL vertex/index buffers.
      - [ ] Implement `ResourceManager::LoadTexture` to load image files (e.g., PNG, JPG) and create RAL textures.
      - [ ] Implement `ResourceManager::LoadShaderProgram` to load shader source from files and create RAL shader programs.
      - [ ] Add caching mechanisms to avoid redundant loading.
    ```md
    Develop a robust ResourceManager that can load various asset types from files, convert them into RAL resources, and manage their lifecycle efficiently.
    ```

### Automatic Dependency Registration (Source Generator)

  - tags: [v1.0.0, infrastructure, c#, source-generator]
  - priority: high
  - steps:
      - [ ] Define `IEngineModule` interface (in `Piece.Core.Abstractions`).
      - [ ] Create `Piece.EngineModuleGenerator` Source Generator to scan for `IEngineModule` implementations.
      - [ ] Implement source generation logic to create extension method for `IServiceCollection` to register discovered modules.
      - [ ] Update project templates to include reference to `Piece.EngineModuleGenerator`.
      - [ ] Implement Tests for Source Generator.
    ```md
    Implement an automatic dependency registration mechanism using C# Source Generators. This will scan for `IEngineModule` implementations across various engine NuGet packages and automatically generate DI registration code, removing the need for manual setup and ensuring AOT compatibility.
    ```

## In Progress

### Unit Test Coverage - src/cpp

  - tags: [v1.0.0, c++, testing, unit-tests, coverage]
  - priority: high
  - defaultExpanded: true
  - steps:
      - [x] EngineCore_Constructor_InitializesSuccessfully
      - [x] EngineCore_Initialize_FailsIfWindowFactoryMissing
      - [x] EngineCore_Initialize_FailsIfGraphicsDeviceFactoryMissing
      - [x] EngineCore_Initialize_FailsIfPhysicsWorldFactoryMissing
      - [x] EngineCore_Constructor_FailsIfWindowCreationFails
      - [x] EngineCore_Constructor_FailsIfGraphicsDeviceCreationFails
      - [x] EngineCore_Constructor_FailsIfPhysicsWorldCreationFails
      - [x] EngineCore_Destructor_CleansUpResources
      - [x] EngineCore_Update_CallsPhysicsSystemStep
      - [ ] EngineCore_Update_HandlesNullPhysicsSystem
      - [x] EngineCore_Render_CallsGraphicsDeviceBeginEndFrame
      - [x] EngineCore_Render_CallsRenderContextClear
      - [ ] EngineCore_Render_CallsRenderSystemRenderFrame
      - [x] EngineCore_Render_HandlesNullDependencies
      - [ ] EngineCore_Render_SkipsIncompleteModels
      - [ ] NativeExports_SetGraphicsDeviceFactory_SetsFactoryCorrectly
      - [ ] NativeExports_SetGraphicsDeviceFactory_HandlesNullPtr
      - [ ] NativeExports_SetWindowFactory_SetsFactoryCorrectly
      - [ ] NativeExports_SetWindowFactory_HandlesNullPtr
      - [ ] NativeExports_SetPhysicsWorldFactory_SetsFactoryCorrectly
      - [ ] NativeExports_SetPhysicsWorldFactory_HandlesNullPtr
      - [ ] NativeExports_EngineInitialize_InitializesLoggerOnce
      - [x] NativeExports_EngineInitialize_CreatesEngineCore
      - [ ] NativeExports_EngineInitialize_ReturnsNullOnCoreAllocationFailure
      - [ ] NativeExports_EngineInitialize_LogsInitializationInfo
      - [ ] NativeExports_EngineDestroy_DeletesEngineCore
      - [ ] NativeExports_EngineDestroy_HandlesNullCorePtr
      - [ ] NativeExports_EngineUpdate_CallsCoreUpdate
      - [ ] NativeExports_EngineUpdate_HandlesNullCorePtr
      - [ ] NativeExports_EngineRender_CallsCoreRender
      - [ ] NativeExports_EngineRender_HandlesNullCorePtr
      - [ ] NativeExports_EngineIsKeyPressed_CallsWindowIsKeyPressed
      - [ ] NativeExports_EngineIsKeyPressed_HandlesNullCoreOrWindow
      - [ ] NativeExports_EngineIsMouseButtonPressed_CallsWindowIsMouseButtonPressed
      - [ ] NativeExports_EngineIsMouseButtonPressed_HandlesNullCoreOrWindow
      - [ ] NativeExports_EngineGetMouseX_CallsWindowGetMouseX
      - [ ] NativeExports_EngineGetMouseX_HandlesNullCoreOrWindow
      - [ ] NativeExports_EngineGetMouseY_CallsWindowGetMouseY
      - [ ] NativeExports_EngineGetMouseY_HandlesNullCoreOrWindow
      - [ ] NativeExports_EngineLoadMesh_CallsResourceManagerLoadMesh
      - [ ] NativeExports_EngineLoadMesh_ReturnsMeshOnSuccess
      - [ ] NativeExports_EngineLoadMesh_ReturnsNullOnFailure
      - [ ] NativeExports_EngineLoadMesh_HandlesNullCoreOrResourceManager
      - [ ] NativeExports_EngineLoadMaterial_CallsResourceManagerLoadMaterial
      - [ ] NativeExports_EngineLoadMaterial_ReturnsMaterialOnSuccess
      - [ ] NativeExports_EngineLoadMaterial_ReturnsNullOnFailure
      - [ ] NativeExports_EngineLoadMaterial_HandlesNullCoreOrResourceManager
      - [ ] NativeExports_EngineLoadTexture_CallsResourceManagerLoadTexture
      - [ ] NativeExports_EngineLoadTexture_ReturnsTextureOnSuccess
      - [ ] NativeExports_EngineLoadTexture_ReturnsNullOnFailure
      - [ ] NativeExports_EngineLoadTexture_HandlesNullCoreOrResourceManager
      - [ ] NativeExports_RegisterLogCallback_SetsCallback
      - [ ] NativeExports_RegisterLogCallback_HandlesNullCallback
      - [ ] NativeExports_RegisterLogCallback_LogsRegistrationStatus
      - [ ] NativeExports_PieceCoreLog_InvokesCallbackWhenRegistered
      - [ ] NativeExports_PieceCoreLog_FallsBackToStdOutWhenNoCallback
      - [ ] NativeExports_PieceCoreLog_HandlesNullMessage
      - [ ] ResourceManager_Constructor_InitializesCorrectly
      - [ ] ResourceManager_Destructor_CleansUpResources
      - [ ] ResourceManager_LoadMaterial_ReturnsValidMaterial
      - [ ] ResourceManager_LoadMaterial_ReturnsDifferentMaterialForDifferentPaths
      - [ ] ResourceManager_LoadMaterial_ReturnsSameMaterialForSamePath
      - [ ] ResourceManager_LoadMaterial_HandlesInvalidPath
      - [ ] ResourceManager_LoadMesh_ReturnsValidMesh
      - [ ] ResourceManager_LoadMesh_ReturnsDifferentMeshForDifferentPaths
      - [ ] ResourceManager_LoadMesh_HandlesInvalidPath
      - [ ] ResourceManager_LoadTexture_ReturnsValidTexture
      - [ ] ResourceManager_LoadTexture_HandlesInvalidPath
      - [ ] ResourceManager_LoadShaderProgram_ReturnsValidShaderProgram
      - [ ] ResourceManager_LoadShaderProgram_HandlesInvalidPaths
      - [ ] ResourceManager_LoadFunctions_LogAppropriateMessages
      - [ ] RenderSystem_Constructor_InitializesCorrectly
      - [ ] RenderSystem_Constructor_HandlesNullGraphicsDevice
      - [ ] RenderSystem_Destructor_CompletesWithoutErrors
      - [ ] RenderSystem_RenderFrame_CallsGraphicsDeviceBeginEndFrame
      - [ ] RenderSystem_RenderFrame_CallsRenderContextClear
      - [ ] RenderSystem_RenderFrame_BindsShaderPrograms
      - [ ] RenderSystem_RenderFrame_SetsShaderUniforms
      - [ ] RenderSystem_RenderFrame_BindsVertexIndexBuffers
      - [ ] RenderSystem_RenderFrame_CallsRenderContextDrawIndexed
      - [ ] RenderSystem_RenderFrame_SkipsIncompleteModels
      - [ ] RenderSystem_RenderFrame_HandlesEmptyModelList
      - [ ] RenderSystem_RenderFrame_HandlesNullGraphicsDeviceGracefully
      - [ ] RenderSystem_Logging_GeneratesAppropriateMessages
      - [ ] PhysicsSystemCpp_Constructor_InitializesCorrectly
      - [ ] PhysicsSystemCpp_Destructor_CompletesWithoutErrors
      - [ ] PhysicsSystemCpp_Step_CallsPhysicsWorldStep
      - [ ] PhysicsSystemCpp_Step_HandlesNullPhysicsWorld
      - [ ] PhysicsSystemCpp_CreateBody_CallsPhysicsWorldCreateRigidBody
      - [ ] PhysicsSystemCpp_CreateBody_ReturnsValidBodyOnSuccess
      - [ ] PhysicsSystemCpp_CreateBody_StoresCreatedBody
      - [ ] PhysicsSystemCpp_CreateBody_ReturnsNullOnPhysicsWorldFailure
      - [ ] PhysicsSystemCpp_CreateBody_ReturnsNullOnNullPhysicsWorld
      - [ ] PhysicsSystemCpp_Logging_GeneratesAppropriateMessages
      - [ ] JobSystem_Constructor_CreatesCorrectNumberOfThreads
      - [ ] JobSystem_Destructor_JoinsAllThreads
      - [ ] JobSystem_EnqueueJob_AddsJobToQueue
      - [ ] JobSystem_EnqueueJob_ExecutesSingleJob
      - [ ] JobSystem_EnqueueJob_ExecutesMultipleJobs
      - [ ] JobSystem_EnqueueJob_HandlesJobsFromMultipleThreads
      - [ ] JobSystem_WaitUntilAllDone_BlocksUntilAllJobsComplete
      - [ ] JobSystem_WaitUntilAllDone_DoesNotBlockIfNoJobs
      - [ ] JobSystem_Concurrency_NoRaceConditionsOnSharedState
      - [ ] JobSystem_Logging_GeneratesAppropriateMessages
      - [x] ServiceLocator_Get_ReturnsSingletonInstance
      - [ ] ServiceLocator_SetGraphicsDeviceFactory_SetsFactory
      - [ ] ServiceLocator_GetGraphicsDeviceFactory_ReturnsSetFactory
      - [ ] ServiceLocator_SetWindowFactory_SetsFactory
      - [ ] ServiceLocator_GetWindowFactory_ReturnsSetFactory
      - [ ] ServiceLocator_SetPhysicsWorldFactory_SetsFactory
      - [ ] ServiceLocator_GetPhysicsWorldFactory_ReturnsSetFactory
      - [ ] ServiceLocator_GetFactory_ReturnsNullWhenNotSet
      - [ ] ServiceLocator_Logging_GeneratesAppropriateMessages
      - [ ] MinimalPhysicsWorldFactory_Constructor_StoresOptions
      - [ ] MinimalPhysicsWorldFactory_CreatePhysicsWorld_UsesFactoryOptionsWhenNoneProvided
      - [ ] MinimalPhysicsWorldFactory_CreatePhysicsWorld_UsesProvidedOptions
      - [ ] MinimalPhysicsWorldFactory_CreatePhysicsWorld_ReturnsValidWorld
      - [ ] MinimalPhysicsWorldFactory_CreatePhysicsWorld_ReturnsNullOnWorldInitFailure
      - [ ] MinimalPhysicsWorldFactory_Logging_GeneratesAppropriateMessages
      - [ ] MinimalPhysicsWorld_Constructor_InitializesCorrectly
      - [ ] MinimalPhysicsWorld_Destructor_CompletesWithoutErrors
      - [ ] MinimalPhysicsWorld_Init_SetsOptions
      - [ ] MinimalPhysicsWorld_Step_NoOpButDoesNotCrash
      - [ ] MinimalPhysicsWorld_SetGravity_SetsInternalState
      - [ ] MinimalPhysicsWorld_CreateRigidBody_ReturnsValidBody
      - [ ] MinimalPhysicsWorld_CreateBoxShape_ReturnsValidShape
      - [ ] MinimalPhysicsWorld_CreateSphereShape_ReturnsValidShape
      - [ ] MinimalPhysicsWorld_CreatePhysicsMaterial_ReturnsValidMaterial
      - [ ] MinimalPhysicsWorld_Logging_GeneratesAppropriateMessages
      - [ ] MinimalPhysicsBody_Constructor_InitializesCorrectly
      - [ ] MinimalPhysicsBody_Destructor_CompletesWithoutErrors
      - [ ] MinimalPhysicsBody_SetShape_StoresShape
      - [ ] MinimalPhysicsBody_SetMaterial_StoresMaterial
      - [ ] MinimalPhysicsBody_SetMass_UpdatesMass
      - [ ] MinimalPhysicsBody_SetPosition_UpdatesPosition
      - [ ] MinimalPhysicsBody_GetPosition_ReturnsCurrentPosition
      - [ ] MinimalPhysicsBody_SetRotation_UpdatesRotation
      - [ ] MinimalPhysicsBody_GetRotation_ReturnsCurrentRotation
      - [ ] MinimalPhysicsBody_SetLinearVelocity_UpdatesVelocity
      - [ ] MinimalPhysicsBody_SetAngularVelocity_UpdatesVelocity
      - [ ] MinimalPhysicsBody_ApplyForce_NoOpButDoesNotCrash
      - [ ] MinimalPhysicsBody_Logging_GeneratesAppropriateMessages
      - [ ] MinimalColliderShape_Constructor_InitializesCorrectly
      - [ ] MinimalColliderShape_Destructor_CompletesWithoutErrors
      - [ ] MinimalColliderShape_Logging_GeneratesAppropriateMessages
      - [ ] MinimalJoint_Constructor_InitializesCorrectly
      - [ ] MinimalJoint_Destructor_CompletesWithoutErrors
      - [ ] MinimalJoint_SetBodies_StoresBodies
      - [ ] MinimalJoint_Logging_GeneratesAppropriateMessages
      - [ ] MinimalPhysicsMaterial_Constructor_InitializesCorrectly
      - [ ] MinimalPhysicsMaterial_Destructor_CompletesWithoutErrors
      - [ ] MinimalPhysicsMaterial_GetFriction_ReturnsCorrectValue
      - [ ] MinimalPhysicsMaterial_GetRestitution_ReturnsCorrectValue
      - [ ] MinimalPhysicsMaterial_Logging_GeneratesAppropriateMessages
      - [ ] CreateBox2DPhysicsWorldFactory_ReturnsValidFactory
      - [ ] CreateBox2DPhysicsWorldFactory_Logging_GeneratesAppropriateMessages
      - [ ] DestroyBox2DPhysicsWorldFactory_DeletesFactory
      - [ ] DestroyBox2DPhysicsWorldFactory_Logging_GeneratesAppropriateMessages
      - [ ] Box2DPhysicsWorldFactory_Constructor_InitializesCorrectly
      - [ ] Box2DPhysicsWorldFactory_Destructor_CompletesWithoutErrors
      - [ ] Box2DPhysicsWorldFactory_CreatePhysicsWorld_ReturnsValidWorld
      - [ ] Box2DPhysicsWorldFactory_CreatePhysicsWorld_PassesOptionsToWorldInit
      - [ ] Box2DPhysicsWorldFactory_Logging_GeneratesAppropriateMessages
      - [ ] Box2DWorld_Constructor_InitializesCorrectly
      - [ ] Box2DWorld_Destructor_CompletesWithoutErrors
      - [ ] Box2DWorld_Init_StoresOptionsAndLogs
      - [ ] Box2DWorld_Step_NoOpButDoesNotCrash
      - [ ] Box2DWorld_CreateRigidBody_ReturnsValidBody
      - [ ] Box2DWorld_SetGravity_SetsInternalStateAndLogs
      - [ ] Box2DWorld_CreateBoxShape_ReturnsNullAndLogsWarning
      - [ ] Box2DWorld_CreateSphereShape_ReturnsNullAndLogsWarning
      - [ ] Box2DWorld_CreatePhysicsMaterial_ReturnsNullAndLogsWarning
      - [ ] Box2DWorld_Logging_GeneratesAppropriateMessages
      - [ ] Box2DBody_Constructor_InitializesCorrectly
      - [ ] Box2DBody_Destructor_CompletesWithoutErrors
      - [ ] Box2DBody_SetShape_NoOpAndLogs
      - [ ] Box2DBody_SetMaterial_NoOpAndLogs
      - [ ] Box2DBody_SetMass_NoOpAndLogs
      - [ ] Box2DBody_SetPosition_NoOpAndLogs
      - [ ] Box2DBody_GetPosition_ReturnsZeroVectorAndLogs
      - [ ] Box2DBody_SetRotation_NoOpAndLogs
      - [ ] Box2DBody_GetRotation_ReturnsIdentityQuaternionAndLogs
      - [ ] Box2DBody_SetLinearVelocity_NoOpAndLogs
      - [ ] Box2DBody_SetAngularVelocity_NoOpAndLogs
      - [ ] Box2DBody_ApplyForce_NoOpAndLogs
      - [ ] Box2DBody_ApplyImpulse_NoOpAndLogs
      - [ ] Box2DBody_Logging_GeneratesAppropriateMessages
      - [ ] OpenGLGraphicsDeviceFactory_Constructor_StoresOptions
      - [ ] OpenGLGraphicsDeviceFactory_CreateGraphicsDevice_UsesFactoryOptionsWhenNoneProvided
      - [ ] OpenGLGraphicsDeviceFactory_CreateGraphicsDevice_UsesProvidedOptions
      - [ ] OpenGLGraphicsDeviceFactory_CreateGraphicsDevice_ReturnsValidDevice
      - [ ] OpenGLGraphicsDeviceFactory_CreateGraphicsDevice_ReturnsNullOnDeviceInitFailure
      - [ ] OpenGLGraphicsDeviceFactory_Logging_GeneratesAppropriateMessages
      - [ ] OpenGLGraphicsDevice_Constructor_InitializesCorrectly
      - [ ] OpenGLGraphicsDevice_Destructor_CompletesWithoutErrors
      - [ ] OpenGLGraphicsDevice_Init_ReturnsTrueForStub
      - [ ] OpenGLGraphicsDevice_BeginFrame_NoOpDoesNotCrash
      - [ ] OpenGLGraphicsDevice_EndFrame_NoOpDoesNotCrash
      - [ ] OpenGLGraphicsDevice_GetImmediateContext_ReturnsNull
      - [ ] OpenGLGraphicsDevice_CreateVertexBuffer_ReturnsNull
      - [ ] OpenGLGraphicsDevice_CreateIndexBuffer_ReturnsNull
      - [ ] OpenGLGraphicsDevice_CreateShader_ReturnsNull
      - [ ] OpenGLGraphicsDevice_CreateShaderProgram_ReturnsNull
      - [ ] OpenGLGraphicsDevice_CreateTexture_ReturnsNull
      - [ ] OpenGLGraphicsDevice_CreateSampler_ReturnsNull
      - [ ] OpenGLGraphicsDevice_CreateComputeBuffer_ReturnsNull
      - [ ] OpenGLGraphicsDevice_CreateIndirectDrawBuffer_ReturnsNull
      - [ ] OpenGLGraphicsDevice_CreateAccelerationStructure_ReturnsNull
      - [ ] OpenGLGraphicsDevice_CreateFrameBuffer_ReturnsNull
      - [ ] OpenGLGraphicsDevice_CreateUniformBuffer_ReturnsNull
      - [ ] OpenGLGraphicsDevice_Logging_GeneratesAppropriateMessages
      - [ ] OpenGLRenderContext_Constructor_InitializesCorrectly
      - [ ] OpenGLRenderContext_Destructor_CompletesWithoutErrors
      - [ ] OpenGLRenderContext_Clear_NoOpDoesNotCrash
      - [ ] OpenGLRenderContext_SetViewport_NoOpDoesNotCrash
      - [ ] OpenGLRenderContext_BindFrameBuffer_NoOpDoesNotCrash
      - [ ] OpenGLRenderContext_SetVertexBuffer_NoOpDoesNotCrash
      - [ ] OpenGLRenderContext_SetIndexBuffer_NoOpDoesNotCrash
      - [ ] OpenGLRenderContext_SetShaderProgram_NoOpDoesNotCrash
      - [ ] OpenGLRenderContext_SetTexture_NoOpDoesNotCrash
      - [ ] OpenGLRenderContext_SetSampler_NoOpDoesNotCrash
      - [ ] OpenGLRenderContext_SetUniformBuffer_NoOpDoesNotCrash
      - [ ] OpenGLRenderContext_SetStorageBuffer_NoOpDoesNotCrash
      - [ ] OpenGLRenderContext_SetAccelerationStructure_NoOpDoesNotCrash
      - [ ] OpenGLRenderContext_SetDepthTest_NoOpDoesNotCrash
      - [ ] OpenGLRenderContext_SetDepthFunc_NoOpDoesNotCrash
      - [ ] OpenGLRenderContext_SetDepthMask_NoOpDoesNotCrash
      - [ ] OpenGLRenderContext_SetBlendMode_NoOpDoesNotCrash
      - [ ] OpenGLRenderContext_SetCullMode_NoOpDoesNotCrash
      - [ ] OpenGLRenderContext_DrawIndexed_NoOpDoesNotCrash
      - [ ] OpenGLRenderContext_DrawArrays_NoOpDoesNotCrash
      - [ ] OpenGLRenderContext_DrawIndexedInstanced_NoOpDoesNotCrash
      - [ ] OpenGLRenderContext_DrawArraysInstanced_NoOpDoesNotCrash
      - [ ] OpenGLRenderContext_DispatchCompute_NoOpDoesNotCrash
      - [ ] OpenGLRenderContext_DispatchMesh_NoOpDoesNotCrash
      - [ ] OpenGLRenderContext_TraceRays_NoOpDoesNotCrash
      - [ ] OpenGLRenderContext_SetVariableRateShading_NoOpDoesNotCrash
      - [ ] OpenGLRenderContext_SwapBuffers_NoOpDoesNotCrash
      - [ ] OpenGLRenderContext_Logging_GeneratesAppropriateMessages
      - [ ] OpenGLShaderProgram_Constructor_InitializesCorrectly
      - [ ] OpenGLShaderProgram_Destructor_CompletesWithoutErrors
      - [ ] OpenGLShaderProgram_Bind_NoOpDoesNotCrash
      - [ ] OpenGLShaderProgram_Unbind_NoOpDoesNotCrash
      - [ ] OpenGLShaderProgram_SetUniformInt_NoOpDoesNotCrash
      - [ ] OpenGLShaderProgram_SetUniformFloat_NoOpDoesNotCrash
      - [ ] OpenGLShaderProgram_SetUniformFloat2_NoOpDoesNotCrash
      - [ ] OpenGLShaderProgram_SetUniformFloat3_NoOpDoesNotCrash
      - [ ] OpenGLShaderProgram_SetUniformFloat4_NoOpDoesNotCrash
      - [ ] OpenGLShaderProgram_SetUniformMat4_NoOpDoesNotCrash
      - [ ] OpenGLShaderProgram_Logging_GeneratesAppropriateMessages
      - [ ] OpenGLTexture_Constructor_InitializesCorrectly
      - [ ] OpenGLTexture_Destructor_CompletesWithoutErrors
      - [ ] OpenGLTexture_Bind_NoOpDoesNotCrash
      - [ ] OpenGLTexture_Unbind_NoOpDoesNotCrash
      - [ ] OpenGLTexture_GetWidth_ReturnsCorrectWidth
      - [ ] OpenGLTexture_GetHeight_ReturnsCorrectHeight
      - [ ] OpenGLTexture_GetNativeID_ReturnsZero
      - [ ] OpenGLTexture_Logging_GeneratesAppropriateMessages
      - [ ] OpenGLVertexBuffer_Constructor_InitializesCorrectly
      - [ ] OpenGLVertexBuffer_Destructor_CompletesWithoutErrors
      - [ ] OpenGLVertexBuffer_Bind_NoOpDoesNotCrash
      - [ ] OpenGLVertexBuffer_Unbind_NoOpDoesNotCrash
      - [ ] OpenGLVertexBuffer_SetData_UpdatesSizeAndNoCrash
      - [ ] OpenGLVertexBuffer_GetSize_ReturnsCorrectSize
      - [ ] OpenGLVertexBuffer_GetLayout_ReturnsCorrectLayout
      - [ ] OpenGLVertexBuffer_Logging_GeneratesAppropriateMessages
      - [ ] OpenGLIndexBuffer_Constructor_InitializesCorrectly
      - [ ] OpenGLIndexBuffer_Destructor_CompletesWithoutErrors
      - [ ] OpenGLIndexBuffer_Bind_NoOpDoesNotCrash
      - [ ] OpenGLIndexBuffer_Unbind_NoOpDoesNotCrash
      - [ ] OpenGLIndexBuffer_SetData_UpdatesCountAndNoCrash
      - [ ] OpenGLIndexBuffer_GetCount_ReturnsCorrectCount
      - [ ] OpenGLIndexBuffer_Logging_GeneratesAppropriateMessages
      - [ ] OpenGLFrameBuffer_Constructor_InitializesCorrectly
      - [ ] OpenGLFrameBuffer_Destructor_CompletesWithoutErrors
      - [ ] OpenGLFrameBuffer_Bind_NoOpDoesNotCrash
      - [ ] OpenGLFrameBuffer_Unbind_NoOpDoesNotCrash
      - [ ] OpenGLFrameBuffer_GetColorAttachment_ReturnsNull
      - [ ] OpenGLFrameBuffer_GetDepthAttachment_ReturnsNull
      - [ ] OpenGLFrameBuffer_Resize_UpdatesDimensionsAndNoCrash
      - [ ] OpenGLFrameBuffer_Logging_GeneratesAppropriateMessages
      - [ ] OpenGLShader_Constructor_InitializesCorrectly
      - [ ] OpenGLShader_Destructor_CompletesWithoutErrors
      - [ ] OpenGLShader_Logging_GeneratesAppropriateMessages
      - [ ] OpenGLUniformBuffer_Constructor_InitializesCorrectly
      - [ ] OpenGLUniformBuffer_Destructor_CompletesWithoutErrors
      - [ ] OpenGLUniformBuffer_SetData_NoOpDoesNotCrash
      - [ ] OpenGLUniformBuffer_GetSize_ReturnsCorrectSize
      - [ ] OpenGLUniformBuffer_Logging_GeneratesAppropriateMessages
      - [ ] OpenGLSampler_Constructor_InitializesCorrectly
      - [ ] OpenGLSampler_Destructor_CompletesWithoutErrors
      - [ ] OpenGLSampler_Bind_NoOpDoesNotCrash
      - [ ] OpenGLSampler_Unbind_NoOpDoesNotCrash
      - [ ] OpenGLSampler_Logging_GeneratesAppropriateMessages
      - [ ] OpenGLIndirectDrawBuffer_Constructor_InitializesCorrectly
      - [ ] OpenGLIndirectDrawBuffer_Destructor_CompletesWithoutErrors
      - [ ] OpenGLIndirectDrawBuffer_SetData_NoOpDoesNotCrash
      - [ ] OpenGLIndirectDrawBuffer_GetSize_ReturnsCorrectSize
      - [ ] OpenGLIndirectDrawBuffer_Logging_GeneratesAppropriateMessages
      - [ ] OpenGLComputeBuffer_Constructor_InitializesCorrectly
      - [ ] OpenGLComputeBuffer_Destructor_CompletesWithoutErrors
      - [ ] OpenGLComputeBuffer_SetData_NoOpDoesNotCrash
      - [ ] OpenGLComputeBuffer_GetSize_ReturnsCorrectSize
      - [ ] OpenGLComputeBuffer_Logging_GeneratesAppropriateMessages
      - [ ] OpenGLAccelerationStructure_Constructor_LogsWarning
      - [ ] OpenGLAccelerationStructure_Destructor_CompletesWithoutErrors
      - [ ] OpenGLAccelerationStructure_Build_LogsWarning
      - [ ] OpenGLAccelerationStructure_Update_LogsWarning
      - [ ] OpenGLAccelerationStructure_Logging_GeneratesAppropriateMessages
      - [ ] CreateOpenGLGraphicsDeviceFactory_ReturnsValidFactory
      - [ ] CreateOpenGLGraphicsDeviceFactory_Logging_GeneratesAppropriateMessages
      - [ ] DestroyOpenGLGraphicsDeviceFactory_DeletesFactory
      - [ ] DestroyOpenGLGraphicsDeviceFactory_Logging_GeneratesAppropriateMessages
      - [ ] GlfwWindowFactory_Constructor_StoresOptions
      - [ ] GlfwWindowFactory_CreateGlfwWindow_UsesFactoryOptionsWhenNoneProvided
      - [ ] GlfwWindowFactory_CreateGlfwWindow_UsesProvidedOptions
      - [ ] GlfwWindowFactory_CreateGlfwWindow_ReturnsValidWindow
      - [ ] GlfwWindowFactory_CreateGlfwWindow_ReturnsNullOnWindowInitFailure
      - [ ] GlfwWindowFactory_Logging_GeneratesAppropriateMessages
      - [ ] GlfwWindow_Constructor_InitializesGLFWAndLogs
      - [ ] GlfwWindow_Constructor_HandlesGLFWInitFailure
      - [ ] GlfwWindow_Destructor_TerminatesGLFWAndDestroysWindow
      - [ ] GlfwWindow_Init_CreatesGLFWWindowSuccessfully
      - [ ] GlfwWindow_Init_HandlesGLFWWindowCreationFailure
      - [ ] GlfwWindow_Init_SetsWindowHintsCorrectly
      - [ ] GlfwWindow_Init_MakesContextCurrentAndEnablesVSync
      - [ ] GlfwWindow_Init_WarnsIfAlreadyInitialized
      - [ ] GlfwWindow_PollEvents_CallsGlfwPollEvents
      - [ ] GlfwWindow_SwapBuffers_CallsGlfwSwapBuffers
      - [ ] GlfwWindow_ShouldClose_ReturnsCorrectState
      - [ ] GlfwWindow_GetNativeWindow_ReturnsCorrectHandle
      - [ ] GlfwWindow_IsKeyPressed_CallsGlfwGetKey
      - [ ] GlfwWindow_IsMouseButtonPressed_CallsGlfwGetMouseButton
      - [ ] GlfwWindow_GetMousePosition_CallsGlfwGetCursorPos
      - [ ] GlfwWindow_GetMouseX_ReturnsCorrectCoordinate
      - [ ] GlfwWindow_GetMouseY_ReturnsCorrectCoordinate
      - [ ] GlfwWindow_InputMethods_HandleNullWindowGracefully
      - [ ] GlfwWindow_Logging_GeneratesAppropriateMessages
      - [ ] CreateGlfwWindowFactory_ReturnsValidFactory
      - [ ] CreateGlfwWindowFactory_Logging_GeneratesAppropriateMessages
      - [ ] DestroyGlfwWindowFactory_DeletesFactory
      - [ ] DestroyGlfwWindowFactory_Logging_GeneratesAppropriateMessages
    ```md
    Develop the core C++ engine components, including minimal but functional implementations of the Window, Render, and Physics Abstraction Layers (WAL/RAL/PAL) with at least one backend each. Establish the Intermediate C++ layer with its Service Locator, P/Invoke interface, and managers for resources, rendering, and physics.
    ```

## Done

### Phase 3.5: Piece.ProjectManagement Layer

  - tags: [v1.0.0, c#, project-management, core]
  - priority: high
  - steps:
      - [x] Define `PieceProject` data model (name, path, default scene, engine config).
      - [x] Implement `IProjectManager` interface (Create, Load, Save project, GetAssetService, GetSceneService).
      - [x] Define `IProjectAssetService` (ImportAsset, ListAssets, DeleteAsset).
      - [x] Define `IProjectSceneService` (OpenScene, SaveScene, AddEntity, RemoveEntity).
      - [x] Define 'piece_project.toml' file format and implement serialization/deserialization.
      - [x] Implement `ProjectManager` (handles `.pieceproject` file I/O, leverages `dotnet` CLI for scaffolding).
      - [x] Implement Project Template Management (discovery, application of templates).
      - [x] Implement `dotnet CLI` integration for project scaffolding and template application.
      - [x] Implement `IProjectBuildService` (or integrate build logic into `IProjectManager`).
      - [x] Implement `ProjectAssetService` (wraps `Piece.Framework.AssetManager`, handles file system operations and metadata).
      - [x] Implement `ProjectSceneService` (wraps `Piece.Framework.SceneManager`, handles scene file I/O).c
      - [x] Establish project references to `Piece.Framework.Abstractions` for AssetManager and SceneManager integration.
      - [x] Implement `ProjectServiceCollectionExtensions` for DI setup.
      - [x] Implement Tests for Piece.ProjectManagement Layer.
      - [x] Ensure robust logging and error handling across all implementations.
    ```md
    Develop the Piece.ProjectManagement layer to provide a high-level, editor-agnostic abstraction for managing engine projects. This layer will handle project creation, loading, saving, and provide services for asset and scene management, consuming the Piece.Framework APIs. It will leverage the 'dotnet' CLI for project scaffolding.
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
      - [x] Implement robust cross-language logging setup for C++ (`spdlog`) and C# (`Serilog`).

### Phase 3: High-Level C# Framework

  - tags: [v1.0.0, c#, high-level]
  - priority: high
  - steps:
      - [x] Implement `GameEngine` lifecycle (Initialization, Update, Draw loops).
      - [x] Implement `Scene`, `Node`, `Component` base classes for the scene graph.
      - [x] Implement `TransformComponent` (position, rotation, scale).
      - [x] Implement `MeshRendererComponent` for rendering.
      - [x] Implement `CameraComponent` and `LightComponent`.
      - [x] Implement `InputManager` using C# WAL wrapper.
      - [x] Implement `AssetManager` (C# wrapper for C++ `ResourceManager`).
      - [x] Implement `RenderManager` (C# wrapper for C++ `RenderSystem`).
      - [x] Implement C# wrappers (`IntPtr` + `IDisposable`) for all core C++ classes (Camera, Light, Material, Mesh, Model, etc.).
      - [x] Develop C# wrapper NuGet packages for core C++ backends (GLFW, OpenGL, Minimal PAL), integrating with .NET DI.
      - [x] Configure `Piece.Engine` to use .NET DI to resolve and configure these C# factory wrappers, populating the C++ `ServiceLocator`.
      - [x] Implement Logging for High-Level C# Framework.
      - [x] Implement Tests for High-Level C# Framework.
    ```md
    Develop the High-Level C# Framework, providing a user-friendly API for game developers. This includes the core game loop, scene graph components, input and asset management, rendering orchestration, and robust C# wrappers for all underlying C++ engine functionalities, orchestrated via .NET Dependency Injection.
    ```

### Phase 4.5: Minimal CLI Editor

  - tags: [v1.0.0, cli, c#, tooling]
  - priority: high
  - steps:
      - [x] Create `Piece.Cli` console application project.
      - [x] Implement `Program.cs` to configure HostBuilder, register services, and setup `System.CommandLine`.
      - [x] Create `CliServiceCollectionExtensions.cs` for CLI-specific service registrations.
      - [x] Define base command structure in `Commands/` directory.
      - [x] Implement `piece project new` command (`NewProjectCommand.cs`) using `IProjectManager`.
      - [x] Implement `piece build` command (`BuildProjectCommand.cs`) using `IProjectManager` and `IProjectBuildService`.
      - [x] Implement `piece asset import` command (`ImportAssetCommand.cs`) using `IProjectAssetService`.
      - [x] Implement `piece asset list` command (`ListAssetsCommand.cs`) using `IProjectAssetService`.
      - [x] Implement `piece asset delete` command (`DeleteAssetCommand.cs`) using `IProjectAssetService`.
      - [x] Implement `piece module add` command (`AddModuleCommand.cs`) using `IProjectManager`.
      - [x] Implement `piece module remove` command (`RemoveModuleCommand.cs`) using `IProjectManager`.
      - [x] Implement general Logging for Minimal CLI Editor (using `ILogger`).
      - [x] Implement Tests for Minimal CLI Editor.
    ```md
    Develop a minimal but functional CLI editor for the Piece Engine, providing stateless commands for project creation, building, and basic asset management. It will use `System.CommandLine` and interact with the `Piece.ProjectManagement` layer.
    ```

