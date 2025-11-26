#include "engine_core.h"
#include "core/service_locator.h"
#include "native_exports.h"


#include <wal/iwindow.h>
#include <ral/igraphics_device.h>
#include <pal/iphysics_world.h>

#include <iostream>

namespace Piece { namespace Intermediate {

EngineCore::EngineCore() {

    IWindowFactory* windowFactory = ServiceLocator::Get().GetWindowFactory();
    IGraphicsDeviceFactory* graphicsFactory = ServiceLocator::Get().GetGraphicsDeviceFactory();
    IPhysicsWorldFactory* physicsFactory = ServiceLocator::Get().GetPhysicsWorldFactory();


    if (!windowFactory) {
        std::cerr << "Error: IWindowFactory not set in ServiceLocator. Engine cannot initialize." << std::endl;
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






    NativeWindowOptions defaultWindowOptions = { 800, 600, 0, "Piece Engine Window" }; // Example defaults
    window_ = windowFactory->CreateWindow(&defaultWindowOptions);
    if (!window_) {
        std::cerr << "Error: Failed to create IWindow instance." << std::endl;
        return;
    }
    std::cout << "EngineCore: IWindow created." << std::endl;



    NativeVulkanOptions defaultVulkanOptions = { 0, 2 }; // Example defaults
    graphics_device_ = graphicsFactory->CreateGraphicsDevice(window_.get(), &defaultVulkanOptions);
    if (!graphics_device_) {
        std::cerr << "Error: Failed to create IGraphicsDevice instance." << std::endl;
        return;
    }
    std::cout << "EngineCore: IGraphicsDevice created." << std::endl;



    NativePhysicsOptions defaultPhysicsOptions = { 1.0f / 60.0f, 4 }; // Example defaults
    physics_world_ = physicsFactory->CreatePhysicsWorld(&defaultPhysicsOptions);
    if (!physics_world_) {
        std::cerr << "Error: Failed to create IPhysicsWorld instance." << std::endl;
        return;
    }
    std::cout << "EngineCore: IPhysicsWorld created." << std::endl;

    std::cout << "EngineCore: Initialized successfully." << std::endl;
}

EngineCore::~EngineCore() {
    std::cout << "EngineCore: Destroyed." << std::endl;
}

void EngineCore::Update(float deltaTime) {

    if (physics_world_) {
        physics_world_->Step(deltaTime);
    }

}

void EngineCore::Render() {

    if (window_ && graphics_device_) {

    }
}

}} // namespace Piece::Intermediate



template<typename T>
bool is_valid_factory_ptr(T* ptr) {

    return ptr != nullptr;
}


extern "C" {

void PieceIntermediate_SetGraphicsDeviceFactory(
    Piece::Intermediate::IGraphicsDeviceFactory* factoryPtr,
    const NativeVulkanOptions* options) {
    if (!is_valid_factory_ptr(factoryPtr)) {
        std::cerr << "Error: Invalid IGraphicsDeviceFactory pointer received." << std::endl;
        return;
    }



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
    Piece::Intermediate::EngineCore* core = new Piece::Intermediate::EngineCore();
    if (!core) {
        std::cerr << "Error: Failed to allocate EngineCore." << std::endl;
        return nullptr;
    }
    return reinterpret_cast<EngineCore*>(core);
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
