#ifndef RAL_OPENGL_OPENGL_COMPUTE_BUFFER_H_
#define RAL_OPENGL_OPENGL_COMPUTE_BUFFER_H_

#include <cstdint>
#include <ral/icompute_buffer.h>
#include <ral/ral_types.h>

namespace Piece::RAL
{
class OpenGLComputeBuffer : public IComputeBuffer
{
  public:
    OpenGLComputeBuffer(uint32_t size, BufferUsage usage, const void *data);
    ~OpenGLComputeBuffer() override;

    void SetData(const void *data, uint32_t size) override;
    [[nodiscard]] uint32_t GetSize() const override;

  private:
    uint32_t renderer_id_ = 0; // Placeholder for OpenGL-specific ID (e.g., Shader Storage Buffer Object)
    uint32_t size_ = 0;
};
} // namespace Piece::RAL

#endif // RAL_OPENGL_OPENGL_COMPUTE_BUFFER_H_
