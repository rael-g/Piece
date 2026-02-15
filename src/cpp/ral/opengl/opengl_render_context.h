#ifndef RAL_OPENGL_OPENGL_RENDER_CONTEXT_H_
#define RAL_OPENGL_OPENGL_RENDER_CONTEXT_H_

#include <ral/irender_context.h>

namespace Piece::RAL
{
class OpenGLRenderContext : public IRenderContext
{
  public:
    OpenGLRenderContext();
    ~OpenGLRenderContext() override;

    // IRenderContext interface
    void Clear(glm::vec4 color) override;
    void DrawIndexed(uint32_t indexCount, uint32_t startIndexLocation, uint32_t baseVertexLocation) override;
    void SetViewport(float x, float y, float width, float height) override;
    void SetScissorRect(float x, float y, float width, float height) override;

    // Custom OpenGL-specific methods
    void SwapBuffers(); // Note: This is not an override from IRenderContext, keep as a custom method
};
} // namespace Piece::RAL

#endif // RAL_OPENGL_OPENGL_RENDER_CONTEXT_H_
