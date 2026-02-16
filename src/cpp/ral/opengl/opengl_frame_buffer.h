#ifndef RAL_OPENGL_OPENGL_FRAME_BUFFER_H_
#define RAL_OPENGL_OPENGL_FRAME_BUFFER_H_

#include <cstdint>
#include <memory>
#include <ral/iframebuffer.h>
#include <ral/itexture.h>

namespace Piece::RAL
{
class OpenGLFrameBuffer : public IFrameBuffer
{
  public:
    OpenGLFrameBuffer(uint32_t width, uint32_t height);
    ~OpenGLFrameBuffer() override;

    void Bind() const override;
    void Unbind() const override;
    [[nodiscard]] ITexture *GetColorAttachment(uint32_t index = 0) const override;
    [[nodiscard]] ITexture *GetDepthAttachment() const override;
    void Resize(uint32_t width, uint32_t height) override;

  private:
    uint32_t renderer_id_ = 0; // Placeholder for OpenGL FBO ID
    uint32_t width_ = 0;
    uint32_t height_ = 0;
    std::unique_ptr<ITexture> color_attachment_;
    std::unique_ptr<ITexture> depth_attachment_;
};
} // namespace Piece::RAL

#endif // RAL_OPENGL_OPENGL_FRAME_BUFFER_H_
