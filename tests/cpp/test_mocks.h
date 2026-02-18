#pragma once

#include <gmock/gmock.h>
#include <wal/iwindow.h>
#include <wal/iwindow_factory.h>
#include <ral/igraphics_device.h>
#include <ral/igraphics_device_factory.h>
#include <ral/irender_context.h> // Corrected include
#include <ral/native_graphics_options.h>
#include <pal/iphysics_world.h>
#include <pal/iphysics_world_factory.h>
#include <pal/native_physics_options.h>
#include <pal/icollider_shape.h>
#include <pal/iphysics_body.h>
#include <pal/iphysics_material.h>
#include <piece_core/ir_system.h>
#include <piece_core/core/irender_system_factory.h>
#include <piece_core/engine_core.h> // For MockEngineCore
#include <piece_core/core/iengine_core_factory.h> // For MockEngineCoreFactory

// For GraphicsDevice and RenderContext mock methods
#include <ral/interfaces/iindex_buffer.h>
#include <ral/interfaces/ishader.h>
#include <ral/interfaces/ishader_program.h>
#include <ral/interfaces/ivertex_buffer.h>
#include <ral/itexture.h>
#include <ral/isampler.h>
#include <ral/icompute_buffer.h>
#include <ral/iindirect_draw_buffer.h>
#include <ral/iacceleration_structure.h>
#include <ral/iframebuffer.h>
#include <ral/iuniform_buffer.h>

// Mocks for EngineCore dependencies
#include <piece_core/core/iresource_manager.h>         // Include IResourceManager
#include <piece_core/core/iresource_manager_factory.h> // Include the new factory interface
#include <piece_core/core/mesh.h>                      // Include for Mesh
#include <piece_core/core/resource_manager.h>          // Include concrete ResourceManager for Mocking its constructor

// Mocks for low-level interfaces
class MockWindow : public Piece::WAL::IWindow
{
  public:
    MOCK_METHOD(void, Init, (const Piece::WAL::NativeWindowOptions &options), (override));
    MOCK_METHOD(void, PollEvents, (), (override));
    MOCK_METHOD(void, SwapBuffers, (), (override));
    MOCK_METHOD(bool, ShouldClose, (), (const, override));
    MOCK_METHOD(void *, GetNativeWindow, (), (const, override));
    MOCK_METHOD(bool, IsKeyPressed, (Piece::WAL::KeyCode keycode), (const, override));
    MOCK_METHOD(bool, IsMouseButtonPressed, (Piece::WAL::KeyCode button), (const, override));
    MOCK_METHOD((std::pair<float, float>), GetMousePosition, (), (const, override));
    MOCK_METHOD(float, GetMouseX, (), (const, override));
    MOCK_METHOD(float, GetMouseY, (), (const, override));
};

class MockGraphicsDevice : public Piece::RAL::IGraphicsDevice
{
  public:
    MOCK_METHOD(bool, Init, (Piece::WAL::IWindow * window, const Piece::RAL::NativeGraphicsOptions &options),
               (override));
    MOCK_METHOD(void, BeginFrame, (), (override));
    MOCK_METHOD(void, EndFrame, (), (override));
    MOCK_METHOD(Piece::RAL::IRenderContext *, GetImmediateContext, (), (override));
    MOCK_METHOD(std::unique_ptr<Piece::RAL::IVertexBuffer>, CreateVertexBuffer,
                (const void *data, uint32_t size, const Piece::RAL::VertexLayout &layout), (override));
    MOCK_METHOD(std::unique_ptr<Piece::RAL::IIndexBuffer>, CreateIndexBuffer, (const uint32_t *data, uint32_t count),
                (override));
    MOCK_METHOD(std::unique_ptr<Piece::RAL::IShader>, CreateShader,
                (Piece::RAL::ShaderStage stage, const std::string &source), (override));
    MOCK_METHOD(std::unique_ptr<Piece::RAL::IShaderProgram>, CreateShaderProgram,
                (const std::vector<Piece::RAL::IShader *> &shaderModules), (override));
    MOCK_METHOD(std::unique_ptr<Piece::RAL::ITexture>, CreateTexture,
                (Piece::RAL::TextureType type, uint32_t width, uint32_t height, uint32_t depth,
                 Piece::RAL::TextureFormat format, const void *data),
                (override));
    MOCK_METHOD(std::unique_ptr<Piece::RAL::ISampler>, CreateSampler,
                (Piece::RAL::TextureFilter minFilter, Piece::RAL::TextureFilter magFilter,
                 Piece::RAL::TextureWrap sWrap, Piece::RAL::TextureWrap tWrap),
                (override));
    MOCK_METHOD(std::unique_ptr<Piece::RAL::IComputeBuffer>, CreateComputeBuffer,
                (uint32_t size, Piece::RAL::BufferUsage usage, const void *data), (override));
    MOCK_METHOD(std::unique_ptr<Piece::RAL::IIndirectDrawBuffer>, CreateIndirectDrawBuffer,
                (uint32_t size, const void *data), (override));
    MOCK_METHOD(std::unique_ptr<Piece::RAL::IAccelerationStructure>, CreateAccelerationStructure,
                (const Piece::RAL::AccelerationStructureBuildInfo &info), (override));
    MOCK_METHOD(std::unique_ptr<Piece::RAL::IFrameBuffer>, CreateFrameBuffer, (uint32_t width, uint32_t height),
                (override));
    MOCK_METHOD(std::unique_ptr<Piece::RAL::IUniformBuffer>, CreateUniformBuffer, (uint32_t size, const void *data),
                (override));
};

class MockRenderContext : public Piece::RAL::IRenderContext
{
  public:
    MOCK_METHOD(void, Clear, (float r, float g, float b, float a), (override));
    MOCK_METHOD(void, SetViewport, (int x, int y, int width, int height), (override));
    MOCK_METHOD(void, BindFrameBuffer, (Piece::RAL::IFrameBuffer * framebuffer), (override));
    MOCK_METHOD(void, SetVertexBuffer, (Piece::RAL::IVertexBuffer * vertexBuffer), (override));
    MOCK_METHOD(void, SetIndexBuffer, (Piece::RAL::IIndexBuffer * indexBuffer), (override));
    MOCK_METHOD(void, SetShaderProgram, (Piece::RAL::IShaderProgram * shaderProgram), (override));
    MOCK_METHOD(void, SetTexture, (Piece::RAL::ITexture * texture, uint32_t slot), (override));
    MOCK_METHOD(void, SetSampler, (Piece::RAL::ISampler * sampler, uint32_t slot), (override));
    MOCK_METHOD(void, SetUniformBuffer, (Piece::RAL::IUniformBuffer * uniformBuffer, uint32_t bindingPoint), (override));
    MOCK_METHOD(void, SetStorageBuffer, (Piece::RAL::IComputeBuffer * computeBuffer, uint32_t bindingPoint), (override));
    MOCK_METHOD(void, SetAccelerationStructure, (Piece::RAL::IAccelerationStructure * as), (override));
    MOCK_METHOD(void, SetDepthTest, (bool enable), (override));
    MOCK_METHOD(void, SetDepthFunc, (Piece::RAL::DepthFunc func), (override));
    MOCK_METHOD(void, SetDepthMask, (bool enable), (override));
    MOCK_METHOD(void, SetBlendMode, (Piece::RAL::BlendMode mode), (override));
    MOCK_METHOD(void, SetCullMode, (Piece::RAL::CullMode mode), (override));
    MOCK_METHOD(void, DrawIndexed, (uint32_t indexCount, uint32_t startIndex, int32_t baseVertex), (override));
    MOCK_METHOD(void, DrawArrays, (uint32_t vertexCount, uint32_t startVertex), (override));
    MOCK_METHOD(void, DrawIndexedInstanced, (uint32_t indexCount, uint32_t instanceCount, uint32_t startIndex, int32_t baseVertex, uint32_t baseInstance), (override));
    MOCK_METHOD(void, DrawArraysInstanced, (uint32_t vertexCount, uint32_t instanceCount, uint32_t startVertex, uint32_t baseInstance), (override));
    MOCK_METHOD(void, DispatchCompute, (uint32_t groupX, uint32_t groupY, uint32_t groupZ), (override));
    MOCK_METHOD(void, DispatchMesh, (uint32_t groupX, uint32_t groupY, uint32_t groupZ), (override));
    MOCK_METHOD(void, TraceRays, (uint32_t width, uint32_t height, uint32_t depth), (override));
    MOCK_METHOD(void, SetVariableRateShading, (const Piece::RAL::VRSSettings &settings), (override));
};

class MockPhysicsWorld : public Piece::PAL::IPhysicsWorld
{
  public:
    MOCK_METHOD(void, Init, (const Piece::PAL::NativePhysicsOptions &options), (override));
    MOCK_METHOD(void, Step, (float delta_time), (override));
    MOCK_METHOD(std::unique_ptr<Piece::PAL::IPhysicsBody>, CreateRigidBody,
                (const Piece::PAL::RigidBodyCreationInfo &info), (override));
    MOCK_METHOD(void, SetGravity, (float x, float y, float z), (override));
    MOCK_METHOD(std::unique_ptr<Piece::PAL::IColliderShape>, CreateBoxShape,
                (float halfExtentX, float halfExtentY, float halfExtentZ), (override));
    MOCK_METHOD(std::unique_ptr<Piece::PAL::IColliderShape>, CreateSphereShape, (float radius), (override));
    MOCK_METHOD(std::unique_ptr<Piece::PAL::IPhysicsMaterial>, CreatePhysicsMaterial,
                (float friction, float restitution), (override));
};

// Mock for RenderSystem
class MockRenderSystem : public Piece::Core::IRenderSystem
{
  public:
    MOCK_METHOD(void, RenderFrame, (const Piece::Core::Camera &camera, const Piece::Core::Light &light,
                                    const std::vector<std::shared_ptr<Piece::Core::Model>> &models),
                (override));
};

// Mock for EngineCore
class MockEngineCore : public Piece::Core::EngineCore
{
public:
    MOCK_METHOD(void, Update, (float deltaTime), (override));
    MOCK_METHOD(void, Render, (), (override));
    // Note: Do not mock constructor/destructor unless absolutely necessary and carefully
    // Virtual methods must be public.
    MOCK_METHOD(Piece::Core::IResourceManager*, GetResourceManager, (), (const, override)); // Added override
    MOCK_METHOD(Piece::WAL::IWindow*, GetWindow, (), (const, override)); // Added override
};


// Mocks for factories
class MockWindowFactory : public Piece::WAL::IWindowFactory
{
  public:
    MOCK_METHOD(std::unique_ptr<Piece::WAL::IWindow>, CreateGlfwWindow,
                (const Piece::WAL::NativeWindowOptions *options), (override));
};

class MockGraphicsDeviceFactory : public Piece::RAL::IGraphicsDeviceFactory
{
  public:
    MOCK_METHOD(std::unique_ptr<Piece::RAL::IGraphicsDevice>, CreateGraphicsDevice,
                (Piece::WAL::IWindow * window, const Piece::RAL::NativeGraphicsOptions *options), (override));
};

class MockPhysicsWorldFactory : public Piece::PAL::IPhysicsWorldFactory
{
  public:
    MOCK_METHOD(std::unique_ptr<Piece::PAL::IPhysicsWorld>, CreatePhysicsWorld,
                (const Piece::PAL::NativePhysicsOptions *options), (override));
};

// Mock for IRenderSystemFactory
class MockRenderSystemFactory : public Piece::Core::IRenderSystemFactory
{
  public:
    MOCK_METHOD(std::unique_ptr<Piece::Core::IRenderSystem>, CreateRenderSystem,
                (Piece::RAL::IGraphicsDevice * graphicsDevice), (override));
};

// Mock for IResourceManagerFactory
class MockResourceManagerFactory : public Piece::Core::IResourceManagerFactory
{
  public:
    MOCK_METHOD(std::unique_ptr<Piece::Core::IResourceManager>, CreateResourceManager,
                (Piece::RAL::IGraphicsDevice * graphics_device), (override));
};

// Mock for IEngineCoreFactory
class MockEngineCoreFactory : public Piece::Core::IEngineCoreFactory
{
  public:
    MOCK_METHOD(std::unique_ptr<Piece::Core::EngineCore>, CreateEngineCore, (), (override));
};

// Mock for Mesh (base class methods are not virtual, so no 'override' keyword)
class MockMesh : public Piece::Core::Mesh
{
public:
    // Mesh constructor now explicitly takes IGraphicsDevice*
    explicit MockMesh(Piece::RAL::IGraphicsDevice* graphicsDevice)
        : Piece::Core::Mesh(graphicsDevice) {}

    // Methods are mocked, but without 'override' as they are not virtual in the base class.
    MOCK_METHOD(void, SetVertexBuffer, (std::unique_ptr<Piece::RAL::IVertexBuffer> vertex_buffer));
    MOCK_METHOD(void, SetIndexBuffer, (std::unique_ptr<Piece::RAL::IIndexBuffer> index_buffer));
    MOCK_METHOD(Piece::RAL::IVertexBuffer*, GetVertexBuffer, (), (const));
    MOCK_METHOD(Piece::RAL::IIndexBuffer*, GetIndexBuffer, (), (const));
};

// Mock for ResourceManager (inherits from IResourceManager)
class MockResourceManager : public Piece::Core::IResourceManager // Removed constructor
{
public:
    MOCK_METHOD(std::shared_ptr<Piece::Core::Mesh>, LoadMesh, (const std::string& path), (override));
    MOCK_METHOD(std::shared_ptr<Piece::Core::Material>, LoadMaterial, (const std::string& path), (override));
    MOCK_METHOD(std::shared_ptr<Piece::RAL::ITexture>, LoadTexture, (const std::string& path), (override));
    MOCK_METHOD(std::shared_ptr<Piece::RAL::IShaderProgram>, LoadShaderProgram, (const std::string& path), (override));
};

// Mock for ITexture
class MockTexture : public Piece::RAL::ITexture
{
public:
    MOCK_METHOD(void, Bind, (uint32_t slot), (const, override));
    MOCK_METHOD(void, Unbind, (), (const, override));
    MOCK_METHOD(uint32_t, GetWidth, (), (const, override));
    MOCK_METHOD(uint32_t, GetHeight, (), (const, override));
    MOCK_METHOD(void*, GetNativeID, (), (const, override));
};