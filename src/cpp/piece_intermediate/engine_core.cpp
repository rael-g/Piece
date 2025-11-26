#include "engine_core.h"
#include "core/service_locator.h"
#include "native_exports.h" // For C-compatible structs and API

// Include necessary low-level interfaces for creation
#include <wal/iwindow.h> // Placeholder for actual WAL interface
#include <ral/igraphics_device.h> // Placeholder for actual RAL interface
#include <pal/iphysics_world.h> // Placeholder for actual PAL interface

#include <iostream> // For basic logging in placeholders

namespace Piece { namespace Intermediate {

EngineCore::EngineCore() {
    // Retrieve factories from the Service Locator
    IWindowFactory* windowFactory = ServiceLocator::Get().GetWindowFactory();
    IGraphicsDeviceFactory* graphicsFactory = ServiceLocator::Get().GetGraphicsDeviceFactory();
    IPhysicsWorldFactory* physicsFactory = ServiceLocator::Get().GetPhysicsWorldFactory();

    // Basic validation: factories must be set by the C# application
    if (!windowFactory) {
        std::cerr << "Error: IWindowFactory not set in ServiceLocator. Engine cannot initialize." << std::endl;
        // In a real engine, this would likely throw an exception or result in a critical error state.
        return;
    }
    if (!graphicsFactory) {
        std::cerr << "Error: IGraphicsDeviceFactory not set in ServiceLocator. Engine cannot initialize." << std::endl;
        return;
    }
    if (!physicsFactory) {
        std::cerr << "Error: IPhysicsWorldFactory not set in ServiceLocator. Engine cannot initialize." << std::endl;
        return;
    }

    // Use factories to create concrete backend instances
    // Note: Placeholder options (nullptr) are used here. Real options would come from C# config.

    // 1. Create Window (assuming default options for now)
    // Need to define NativeWindowOptions somewhere or pass default
    NativeWindowOptions defaultWindowOptions = { 800, 600, 0, "Piece Engine Window" }; // Example defaults
    window_ = windowFactory->CreateWindow(&defaultWindowOptions);
    if (!window_) {
        std::cerr << "Error: Failed to create IWindow instance." << std::endl;
        return;
    }
    std::cout << "EngineCore: IWindow created." << std::endl;
    // window_->Init() or other setup would typically happen here or inside CreateWindow

    // 2. Create Graphics Device (assuming default Vulkan options for now)
    // Need to define NativeVulkanOptions somewhere or pass default
    NativeVulkanOptions defaultVulkanOptions = { 0, 2 }; // Example defaults
    graphics_device_ = graphicsFactory->CreateGraphicsDevice(window_.get(), &defaultVulkanOptions);
    if (!graphics_device_) {
        std::cerr << "Error: Failed to create IGraphicsDevice instance." << std::endl;
        return;
    }
    std::cout << "EngineCore: IGraphicsDevice created." << std::endl;
    // graphics_device_->Init() or other setup would typically happen here

    // 3. Create Physics World (assuming default options for now)
    // Need to define NativePhysicsOptions somewhere or pass default
    NativePhysicsOptions defaultPhysicsOptions = { 1.0f / 60.0f, 4 }; // Example defaults
    physics_world_ = physicsFactory->CreatePhysicsWorld(&defaultPhysicsOptions);
    if (!physics_world_) {
        std::cerr << "Error: Failed to create IPhysicsWorld instance." << std::endl;
        return;
    }
    std::cout << "EngineCore: IPhysicsWorld created." << std::endl;
    // physics_world_->Init() or other setup would typically happen here

    std::cout << "EngineCore: Initialized successfully." << std::endl;
}

EngineCore::~EngineCore() {
    // std::unique_ptr automatically handles destruction of owned objects.
    std::cout << "EngineCore: Destroyed." << std::endl;
}

void EngineCore::Update(float deltaTime) {
    // Placeholder for update logic
    // std::cout << "EngineCore: Updating (dt=" << deltaTime << ")" << std::endl;
    if (physics_world_) {
        physics_world_->Step(deltaTime);
    }
    // ... other system updates
}

void EngineCore::Render() {
    // Placeholder for rendering logic
    // std::cout << "EngineCore: Rendering" << std::endl;
    if (window_ && graphics_device_) {
        // Example: Graphics device usage
        // graphics_device_->BeginFrame();
        // graphics_device_->GetImmediateContext()->Clear(0.1f, 0.2f, 0.3f, 1.0f);
        // graphics_device_->EndFrame();
    }
}

}} // namespace Piece::Intermediate


// Helper to check if a factory pointer is valid
template<typename T>
bool is_valid_factory_ptr(T* ptr) {
    // In a real scenario, more robust validation might be needed (e.g., magic numbers, vtable checks)
    return ptr != nullptr;
}

// --- C-compatible API implementations from NativeExports.h ---
extern "C" {

void PieceIntermediate_SetGraphicsDeviceFactory(
    Piece::Intermediate::IGraphicsDeviceFactory* factoryPtr,
    const NativeVulkanOptions* options) {
    if (!is_valid_factory_ptr(factoryPtr)) {
        std::cerr << "Error: Invalid IGraphicsDeviceFactory pointer received." << std::endl;
        return;
    }
    // The options parameter is ignored here as the factory is passed as a pointer
    // and options are usually handled during the factory's own creation, not when setting it in the ServiceLocator.
    // If options were to configure the *ServiceLocator's* behavior, they'd be used here.
    Piece::Intermediate::ServiceLocator::Get().SetGraphicsDeviceFactory(
        std::unique_ptr<Piece::Intermediate::IGraphicsDeviceFactory>(factoryPtr)
    );
    std::cout << "PieceIntermediate_SetGraphicsDeviceFactory called." << std::endl;
}

void PieceIntermediate_SetWindowFactory(
    Piece::Intermediate::IWindowFactory* factoryPtr,
    const NativeWindowOptions* options) {
    if (!is_valid_factory_ptr(factoryPtr)) {
        std::cerr << "Error: Invalid IWindowFactory pointer received." << std::endl;
        return;
    }
    Piece::Intermediate::ServiceLocator::Get().SetWindowFactory(
        std::unique_ptr<Piece::Intermediate::IWindowFactory>(factoryPtr)
    );
    std::cout << "PieceIntermediate_SetWindowFactory called." << std::endl;
}

void PieceIntermediate_SetPhysicsWorldFactory(
    Piece::Intermediate::IPhysicsWorldFactory* factoryPtr,
    const NativePhysicsOptions* options) {
    if (!is_valid_factory_ptr(factoryPtr)) {
        std::cerr << "Error: Invalid IPhysicsWorldFactory pointer received." << std::endl;
        return;
    }
    Piece::Intermediate::ServiceLocator::Get().SetPhysicsWorldFactory(
        std::unique_ptr<Piece::Intermediate::IPhysicsWorldFactory>(factoryPtr)
    );
    std::cout << "PieceIntermediate_SetPhysicsWorldFactory called." << std::endl;
}

EngineCore* Engine_Initialize() {
    std::cout << "Engine_Initialize called. Attempting to create EngineCore..." << std::endl;
    // The EngineCore constructor relies on factories being set in ServiceLocator
    Piece::Intermediate::EngineCore* core = new Piece::Intermediate::EngineCore();
    if (!core) {
        std::cerr << "Error: Failed to allocate EngineCore." << std::endl;
        return nullptr;
    }
    // Perform any post-construction checks or initialization here if needed
    return reinterpret_cast<EngineCore*>(core); // Cast to opaque pointer
}

void Engine_Destroy(EngineCore* corePtr) {
    std::cout << "Engine_Destroy called." << std::endl;
    if (corePtr) {
        delete reinterpret_cast<Piece::Intermediate::EngineCore*>(corePtr);
    } else {
        std::cerr << "Warning: Engine_Destroy called with null corePtr." << std::endl;
    }
}

void Engine_Update(EngineCore* corePtr, float deltaTime) {
    if (corePtr) {
        reinterpret_cast<Piece::Intermediate::EngineCore*>(corePtr)->Update(deltaTime);
    }
}

void Engine_Render(EngineCore* corePtr) {
    if (corePtr) {
        reinterpret_cast<Piece::Intermediate::EngineCore*>(corePtr)->Render();
    }
}

} // extern "C"
