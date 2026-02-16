#ifndef RAL_OPENGL_OPENGL_GRAPHICS_DEVICE_H_
#define RAL_OPENGL_OPENGL_GRAPHICS_DEVICE_H_

#include <ral/iacceleration_structure.h>
#include <ral/icompute_buffer.h>
#include <ral/iframebuffer.h>
#include <ral/igraphics_device.h>
#include <ral/iindirect_draw_buffer.h>
#include <ral/isampler.h>
#include <ral/itexture.h>
#include <ral/iuniform_buffer.h>
#include <ral/ral_types.h>
#include <string>
#include <vector>

namespace Piece::RAL
{
class OpenGLGraphicsDevice : public IGraphicsDevice
{
  public:
    OpenGLGraphicsDevice();
    ~OpenGLGraphicsDevice() override;

    // IGraphicsDevice interface
    bool Init(WAL::IWindow *window, const NativeGraphicsOptions &options) override;
    void BeginFrame() override;
    void EndFrame() override;
    IRenderContext *GetImmediateContext() override;

    std::unique_ptr<IVertexBuffer> CreateVertexBuffer(const void *data, uint32_t size,
                                                      const VertexLayout &layout) override;
    std::unique_ptr<IIndexBuffer> CreateIndexBuffer(const uint32_t *data, uint32_t count) override;
    std::unique_ptr<IShader> CreateShader(ShaderStage stage, const std::string &source) override;
    std::unique_ptr<IShaderProgram> CreateShaderProgram(const std::vector<IShader *> &shaderModules) override;
    std::unique_ptr<ITexture> CreateTexture(TextureType type, uint32_t width, uint32_t height, uint32_t depth,
                                            TextureFormat format, const void *data = nullptr) override;
    std::unique_ptr<ISampler> CreateSampler(TextureFilter minFilter, TextureFilter magFilter, TextureWrap sWrap,
                                            TextureWrap tWrap) override;
    std::unique_ptr<IComputeBuffer> CreateComputeBuffer(uint32_t size, BufferUsage usage,
                                                        const void *data = nullptr) override;
    std::unique_ptr<IIndirectDrawBuffer> CreateIndirectDrawBuffer(uint32_t size, const void *data = nullptr) override;
    std::unique_ptr<IAccelerationStructure> CreateAccelerationStructure(
        const AccelerationStructureBuildInfo &info) override;
    std::unique_ptr<IFrameBuffer> CreateFrameBuffer(uint32_t width, uint32_t height) override;
    std::unique_ptr<IUniformBuffer> CreateUniformBuffer(uint32_t size, const void *data = nullptr) override;
};
} // namespace Piece::RAL

#endif // RAL_OPENGL_OPENGL_GRAPHICS_DEVICE_H_
