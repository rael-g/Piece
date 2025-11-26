#ifndef PIECE_RAL_IGRAPHICS_DEVICE_H_
#define PIECE_RAL_IGRAPHICS_DEVICE_H_

#include <memory> // For std::unique_ptr
#include "irender_context.h" // Include IRenderContext

namespace Piece { namespace RAL {

// Forward declarations for resource interfaces
class IVertexBuffer;
class IIndexBuffer;
class IShader;
class IShaderProgram;

// Placeholder for IGraphicsDevice interface
class IGraphicsDevice {
public:
    virtual ~IGraphicsDevice() = default;

    virtual void Init() = 0; // Placeholder init method
    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;

    virtual IRenderContext* GetImmediateContext() = 0;

    // Placeholder for resource creation methods
    virtual std::unique_ptr<IVertexBuffer> CreateVertexBuffer() = 0;
    virtual std::unique_ptr<IIndexBuffer> CreateIndexBuffer() = 0;
    virtual std::unique_ptr<IShader> CreateShader() = 0;
    virtual std::unique_ptr<IShaderProgram> CreateShaderProgram() = 0;
};

}} // namespace Piece::RAL

#endif  // PIECE_RAL_IGRAPHICS_DEVICE_H_
