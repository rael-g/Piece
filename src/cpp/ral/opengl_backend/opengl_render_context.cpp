#include "opengl_render_context.h"

// Para o futuro: incluir cabeçalhos OpenGL (GLEW/GLAD) e GLFW

namespace Piece {
    namespace RAL {
        OpenGLRenderContext::OpenGLRenderContext() {}
        OpenGLRenderContext::~OpenGLRenderContext() {}

        void OpenGLRenderContext::Clear(glm::vec4 color) {
            // Futuramente: Chamar glClear com a cor
        }

        void OpenGLRenderContext::DrawIndexed(uint32_t indexCount, uint32_t startIndexLocation, uint32_t baseVertexLocation) {
            // Stub
        }

        void OpenGLRenderContext::SetViewport(float x, float y, float width, float height) {
            // Stub
        }

        void OpenGLRenderContext::SetScissorRect(float x, float y, float width, float height) {
            // Stub
        }

        void OpenGLRenderContext::SwapBuffers() {
            // Futuramente: Chamar glfwSwapBuffers
        }
    }
}
