#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <piece_core/core/resource_manager.h>
#include <ral/igraphics_device.h> // For mocking IGraphicsDevice
#include <ral/interfaces/ivertex_buffer.h>
#include <ral/interfaces/iindex_buffer.h>
#include <ral/interfaces/ishader.h>
#include <ral/interfaces/ishader_program.h>
#include <ral/interfaces/itexture.h>
#include <ral/interfaces/isampler.h>
#include <ral/interfaces/icompute_buffer.h>
#include <ral/interfaces/iindirect_draw_buffer.h>
#include <ral/interfaces/iacceleration_structure.h>
#include <ral/interfaces/iframebuffer.h>
#include <ral/interfaces/iuniform_buffer.h>
#include <ral/irender_context.h> // Also needed for GetImmediateContext
#include <wal/iwindow.h> // For Init method of MockGraphicsDevice
#include <ral/native_graphics_options.h> // For Init method of MockGraphicsDevice


// Mock IGraphicsDevice for ResourceManager
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

// Test fixture for ResourceManager
class ResourceManagerTest : public ::testing::Test
{
  protected:
    MockGraphicsDevice mock_graphics_device;
};

TEST_F(ResourceManagerTest, ResourceManager_Constructor_InitializesCorrectly)
{
    // Ensure the constructor doesn't throw and the object is created
    ASSERT_NO_THROW(Piece::Core::ResourceManager resource_manager(&mock_graphics_device));
}
