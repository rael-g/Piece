#ifndef RAL_OPENGL_OPENGL_INDEX_BUFFER_H_
#define RAL_OPENGL_OPENGL_INDEX_BUFFER_H_

#include <cstdint>
#include <ral/interfaces/iindex_buffer.h>

namespace Piece::RAL
{
class OpenGLIndexBuffer : public IIndexBuffer
{
  public:
    OpenGLIndexBuffer(const uint32_t *data, uint32_t count);
    ~OpenGLIndexBuffer() override;

    void Bind() const override;
    void Unbind() const override;
    void SetData(const uint32_t *data, uint32_t count) override;
    [[nodiscard]] uint32_t GetCount() const override;

  private:
    uint32_t renderer_id_ = 0;
    uint32_t count_ = 0;
};
} // namespace Piece::RAL

#endif // RAL_OPENGL_OPENGL_INDEX_BUFFER_H_
