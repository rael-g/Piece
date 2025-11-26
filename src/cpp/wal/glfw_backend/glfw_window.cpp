#include "glfw_window.h"
#include <iostream> // For error logging

namespace Piece { namespace WAL {

GlfwWindow::GlfwWindow() : window_(nullptr) {
    
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        
    }
}

GlfwWindow::~GlfwWindow() {
    if (window_) {
        glfwDestroyWindow(window_);
        window_ = nullptr;
    }
    glfwTerminate();
}

bool GlfwWindow::Init(int width, int height, const std::string& title) {
    if (window_) {
        std::cerr << "Window already initialized." << std::endl;
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); // Example for resizable

    window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window_) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window_);
    
    glfwSwapInterval(1);

    return true;
}

void GlfwWindow::PollEvents() {
    glfwPollEvents();
}

void GlfwWindow::SwapBuffers() {
    if (window_) {
        glfwSwapBuffers(window_);
    }
}

bool GlfwWindow::ShouldClose() const {
    return window_ ? glfwWindowShouldClose(window_) : true;
}

void* GlfwWindow::GetNativeWindow() const {
    return static_cast<void*>(window_);
}


bool GlfwWindow::IsKeyPressed(KeyCode keycode) const {
    if (window_) {
        return glfwGetKey(window_, static_cast<int>(keycode)) == GLFW_PRESS;
    }
    return false;
}

bool GlfwWindow::IsMouseButtonPressed(KeyCode button) const {
    if (window_) {
        return glfwGetMouseButton(window_, static_cast<int>(button)) == GLFW_PRESS;
    }
    return false;
}

std::pair<float, float> GlfwWindow::GetMousePosition() const {
    double xpos, ypos;
    if (window_) {
        glfwGetCursorPos(window_, &xpos, &ypos);
        return { static_cast<float>(xpos), static_cast<float>(ypos) };
    }
    return { 0.0f, 0.0f };
}

float GlfwWindow::GetMouseX() const {
    return GetMousePosition().first;
}

float GlfwWindow::GetMouseY() const {
    return GetMousePosition().second;
}

}} // namespace Piece::WAL
