#include "mesh.h"

namespace Piece::Core
{

Mesh::Mesh(Piece::RAL::IGraphicsDevice *graphics_device) : graphics_device_(graphics_device)
{
    // Implementation for Mesh constructor
}

Mesh::~Mesh()
{
    // Implementation for Mesh destructor
}

void Mesh::SetVertexBuffer(std::unique_ptr<Piece::RAL::IVertexBuffer> vertex_buffer)
{
    vertex_buffer_ = std::move(vertex_buffer);
}

void Mesh::SetIndexBuffer(std::unique_ptr<Piece::RAL::IIndexBuffer> index_buffer)
{
    index_buffer_ = std::move(index_buffer);
}

} // namespace Piece::Core