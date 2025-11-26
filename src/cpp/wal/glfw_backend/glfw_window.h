#ifndef PIECE_WAL_GLFW_BACKEND_GLFW_WINDOW_H_
#define PIECE_WAL_GLFW_BACKEND_GLFW_WINDOW_H_

#include <wal/iwindow.h> // Base IWindow interface
#include <GLFW/glfw3.h>      // GLFW header

namespace Piece { namespace WAL {

#include "wal_glfw_backend_exports.h"

class WAL_GLFW_BACKEND_API GlfwWindow : public IWindow {
public:
    GlfwWindow();
    virtual ~GlfwWindow();

    virtual bool Init(int width, int height, const std::string& title) override;
    virtual void PollEvents() override;
    virtual void SwapBuffers() override;
    virtual bool ShouldClose() const override;
    virtual void* GetNativeWindow() const override;

    // Input Methods
    virtual bool IsKeyPressed(KeyCode keycode) const override;
    virtual bool IsMouseButtonPressed(KeyCode button) const override;
    virtual std::pair<float, float> GetMousePosition() const override;
    virtual float GetMouseX() const override;
    virtual float GetMouseY() const override;

private:
    GLFWwindow* window_;
};

}} // namespace Piece::WAL

#endif  // PIECE_WAL_GLFW_BACKEND_GLFW_WINDOW_H_
