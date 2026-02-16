#ifndef RAL_OPENGL_OPENGL_INDIRECT_DRAW_BUFFER_H_
#define RAL_OPENGL_OPENGL_INDIRECT_DRAW_BUFFER_H_

#include <cstdint>
#include <ral/iindirect_draw_buffer.h>
#include <ral/ral_types.h>

namespace Piece::RAL
{
class OpenGLIndirectDrawBuffer : public IIndirectDrawBuffer
{
  public:
    OpenGLIndirectDrawBuffer(uint32_t size, const void *data);
    ~OpenGLIndirectDrawBuffer() override;

    void SetData(const void *data, uint32_t size) override;
    [[nodiscard]] uint32_t GetSize() const override;

  private:
    uint32_t renderer_id_ = 0; // Placeholder for OpenGL-specific ID (e.g., Command Buffer)
    uint32_t size_ = 0;
};
} // namespace Piece::RAL

#endif // RAL_OPENGL_OPENGL_INDIRECT_DRAW_BUFFER_H_
