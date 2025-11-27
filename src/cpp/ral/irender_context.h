#ifndef PIECE_RAL_IRENDER_CONTEXT_H_
#define PIECE_RAL_IRENDER_CONTEXT_H_

#include <glm/vec4.hpp>

namespace Piece
{
namespace RAL
{

class IRenderContext
{
  public:
    virtual ~IRenderContext() = default;

    virtual void Clear(glm::vec4 color) = 0;
    // virtual void DrawIndexed(uint32_t indexCount, uint32_t startIndexLocation,
    // uint32_t baseVertexLocation) = 0; virtual void SetViewport(float x, float
    // y, float width, float height) = 0; virtual void SetScissorRect(float x,
    // float y, float width, float height) = 0;
};

} // namespace RAL
} // namespace Piece

#endif // PIECE_RAL_IRENDER_CONTEXT_H_
