#ifndef PIECE_CORE_MESH_H_
#define PIECE_CORE_MESH_H_

#include <memory>
#include <vector>

#include <ral/igraphics_device.h>
#include <ral/interfaces/iindex_buffer.h>
#include <ral/interfaces/ivertex_buffer.h>
#include "piece_core_exports.h" // Include PIECE_CORE_API definition

namespace Piece::Core
{

/**
 * @brief Represents a mesh, encapsulating vertex and index buffers.
 */
class PIECE_CORE_API Mesh
{
  public:
    explicit Mesh(Piece::RAL::IGraphicsDevice *graphics_device);
    virtual ~Mesh();

    void SetVertexBuffer(std::unique_ptr<Piece::RAL::IVertexBuffer> vertex_buffer);
    void SetIndexBuffer(std::unique_ptr<Piece::RAL::IIndexBuffer> index_buffer);

    [[nodiscard]] Piece::RAL::IVertexBuffer *GetVertexBuffer() const
    {
        return vertex_buffer_.get();
    }
    [[nodiscard]] Piece::RAL::IIndexBuffer *GetIndexBuffer() const
    {
        return index_buffer_.get();
    }

  private:
    Piece::RAL::IGraphicsDevice *graphics_device_;
    std::unique_ptr<Piece::RAL::IVertexBuffer> vertex_buffer_;
    std::unique_ptr<Piece::RAL::IIndexBuffer> index_buffer_;
};

} // namespace Piece::Core

#endif // PIECE_CORE_MESH_H_
