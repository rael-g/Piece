#include "mesh.h"
#include <iostream>

namespace Piece::Core
{
using namespace Piece::RAL;

Mesh::Mesh(IGraphicsDevice *graphics_device) : graphics_device_(graphics_device)
{
    std::cout << "Mesh created." << std::endl;
}

Mesh::~Mesh()
{
    std::cout << "Mesh destroyed." << std::endl;
}

void Mesh::SetVertexBuffer(std::unique_ptr<RAL::IVertexBuffer> vertex_buffer)
{
    vertex_buffer_ = std::move(vertex_buffer);
}

void Mesh::SetIndexBuffer(std::unique_ptr<RAL::IIndexBuffer> index_buffer)
{
    index_buffer_ = std::move(index_buffer);
}

} // namespace Piece::Core
