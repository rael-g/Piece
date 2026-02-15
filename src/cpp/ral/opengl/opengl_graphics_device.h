#ifndef RAL_OPENGL_OPENGL_GRAPHICS_DEVICE_H_
#define RAL_OPENGL_OPENGL_GRAPHICS_DEVICE_H_

#include <ral/igraphics_device.h>

namespace Piece::RAL
{
class OpenGLGraphicsDevice : public IGraphicsDevice
{
  public:
    OpenGLGraphicsDevice();
    ~OpenGLGraphicsDevice() override;

    // IGraphicsDevice interface
    void Init() override;
    void BeginFrame() override;
    void EndFrame() override;
    IRenderContext *GetImmediateContext() override;
    std::unique_ptr<IVertexBuffer> CreateVertexBuffer() override;
    std::unique_ptr<IIndexBuffer> CreateIndexBuffer() override;
    std::unique_ptr<IShader> CreateShader() override;
    std::unique_ptr<IShaderProgram> CreateShaderProgram() override;
};
} // namespace Piece::RAL

#endif // RAL_OPENGL_OPENGL_GRAPHICS_DEVICE_H_
