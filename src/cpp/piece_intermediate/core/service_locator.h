#ifndef PIECE_INTERMEDIATE_SERVICE_LOCATOR_H_
#define PIECE_INTERMEDIATE_SERVICE_LOCATOR_H_

#include <memory>
#include <piece_intermediate/interfaces/igraphics_device_factory.h>
#include <piece_intermediate/interfaces/iwindow_factory.h>
#include <piece_intermediate/interfaces/iphysics_world_factory.h>

namespace Piece { namespace Intermediate {

class ServiceLocator {
public:
    static ServiceLocator& Get() {
        static ServiceLocator instance;
        return instance;
    }

    // Deleted copy constructor and assignment operator (moved to public as per Google Style for better error messages)
    ServiceLocator(const ServiceLocator&) = delete;
    ServiceLocator& operator=(const ServiceLocator&) = delete;

    // Setters for factory interfaces
    void SetGraphicsDeviceFactory(std::unique_ptr<IGraphicsDeviceFactory> factory) {
        graphics_device_factory_ = std::move(factory);
    }
    void SetWindowFactory(std::unique_ptr<IWindowFactory> factory) {
        window_factory_ = std::move(factory);
    }
    void SetPhysicsWorldFactory(std::unique_ptr<IPhysicsWorldFactory> factory) {
        physics_world_factory_ = std::move(factory);
    }

    // Getters for factory interfaces
    IGraphicsDeviceFactory* GetGraphicsDeviceFactory() const {
        return graphics_device_factory_.get();
    }
    IWindowFactory* GetWindowFactory() const {
        return window_factory_.get();
    }
    IPhysicsWorldFactory* GetPhysicsWorldFactory() const {
        return physics_world_factory_.get();
    }

private:
    ServiceLocator() = default; // Private constructor for singleton

    std::unique_ptr<IGraphicsDeviceFactory> graphics_device_factory_;
    std::unique_ptr<IWindowFactory> window_factory_;
    std::unique_ptr<IPhysicsWorldFactory> physics_world_factory_;
};

}} // namespace Piece::Intermediate

#endif  // PIECE_INTERMEDIATE_SERVICE_LOCATOR_H_
