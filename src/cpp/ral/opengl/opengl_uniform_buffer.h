#ifndef RAL_OPENGL_OPENGL_UNIFORM_BUFFER_H_
#define RAL_OPENGL_OPENGL_UNIFORM_BUFFER_H_

#include <cstdint>
#include <ral/iuniform_buffer.h>

namespace Piece::RAL
{
class OpenGLUniformBuffer : public IUniformBuffer
{
  public:
    OpenGLUniformBuffer(uint32_t size, const void *data);
    ~OpenGLUniformBuffer() override;

    void SetData(const void *data, uint32_t size) override;
    [[nodiscard]] uint32_t GetSize() const override;

  private:
    uint32_t renderer_id_ = 0; // Placeholder for OpenGL Uniform Buffer Object ID
    uint32_t size_ = 0;
};
} // namespace Piece::RAL

#endif // RAL_OPENGL_OPENGL_UNIFORM_BUFFER_H_
