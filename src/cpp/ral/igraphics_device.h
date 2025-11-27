#ifndef PIECE_RAL_IGRAPHICS_DEVICE_H_
#define PIECE_RAL_IGRAPHICS_DEVICE_H_

#include <memory>

#include "irender_context.h"

namespace Piece
{
namespace RAL
{

class IVertexBuffer;
class IIndexBuffer;
class IShader;
class IShaderProgram;

class IGraphicsDevice
{
  public:
    virtual ~IGraphicsDevice() = default;

    virtual void Init() = 0;
    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;

    virtual IRenderContext *GetImmediateContext() = 0;

    virtual std::unique_ptr<IVertexBuffer> CreateVertexBuffer() = 0;
    virtual std::unique_ptr<IIndexBuffer> CreateIndexBuffer() = 0;
    virtual std::unique_ptr<IShader> CreateShader() = 0;
    virtual std::unique_ptr<IShaderProgram> CreateShaderProgram() = 0;
};

} // namespace RAL
} // namespace Piece

#endif // PIECE_RAL_IGRAPHICS_DEVICE_H_
