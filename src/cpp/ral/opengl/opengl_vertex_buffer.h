#ifndef RAL_OPENGL_OPENGL_VERTEX_BUFFER_H_
#define RAL_OPENGL_OPENGL_VERTEX_BUFFER_H_

#include <cstdint>
#include <ral/interfaces/ivertex_buffer.h>
#include <ral/ral_types.h>

namespace Piece::RAL
{
class OpenGLVertexBuffer : public IVertexBuffer
{
  public:
    OpenGLVertexBuffer(const void *data, uint32_t size, VertexLayout layout);
    ~OpenGLVertexBuffer() override;

    void Bind() const override;
    void Unbind() const override;
    void SetData(const void *data, uint32_t size) override;
    [[nodiscard]] uint32_t GetSize() const override;
    [[nodiscard]] const VertexLayout &GetLayout() const override;

  private:
    uint32_t renderer_id_ = 0;
    uint32_t size_ = 0;
    VertexLayout layout_;
};
} // namespace Piece::RAL

#endif // RAL_OPENGL_OPENGL_VERTEX_BUFFER_H_
