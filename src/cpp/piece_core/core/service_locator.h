/**
 * @file service_locator.h
 * @brief Defines the ServiceLocator class, a singleton for managing global service factories.
 */
#ifndef PIECE_CORE_SERVICE_LOCATOR_H_
#define PIECE_CORE_SERVICE_LOCATOR_H_

#include <ral/igraphics_device_factory.h>
#include <pal/iphysics_world_factory.h>
#include <wal/iwindow_factory.h>

#include <memory>

namespace Piece
{
namespace Core
{

/**
 * @brief A singleton class that provides global access to service factories.
 * @details This class follows the Service Locator design pattern to decouple the rest of the
 *          application from concrete factory implementations.
 */
class ServiceLocator
{
  public:
    /**
     * @brief Gets the single instance of the ServiceLocator.
     * @return A reference to the ServiceLocator instance.
     */
    static ServiceLocator &Get()
    {
        static ServiceLocator instance;
        return instance;
    }

    ServiceLocator(const ServiceLocator &) = delete;
    ServiceLocator &operator=(const ServiceLocator &) = delete;

    /**
     * @brief Sets the graphics device factory.
     * @param factory A unique pointer to the graphics device factory.
     */
    void SetGraphicsDeviceFactory(std::unique_ptr<RAL::IGraphicsDeviceFactory> factory)
    {
        graphics_device_factory_ = std::move(factory);
    }
    /**
     * @brief Sets the window factory.
     * @param factory A unique pointer to the window factory.
     */
    void SetWindowFactory(std::unique_ptr<WAL::IWindowFactory> factory)
    {
        window_factory_ = std::move(factory);
    }
    /**
     * @brief Sets the physics world factory.
     * @param factory A unique pointer to the physics world factory.
     */
    void SetPhysicsWorldFactory(std::unique_ptr<PAL::IPhysicsWorldFactory> factory)
    {
        physics_world_factory_ = std::move(factory);
    }

    /**
     * @brief Gets the graphics device factory.
     * @return A pointer to the graphics device factory.
     */
    RAL::IGraphicsDeviceFactory *GetGraphicsDeviceFactory() const
    {
        return graphics_device_factory_.get();
    }
    /**
     * @brief Gets the window factory.
     * @return A pointer to the window factory.
     */
    WAL::IWindowFactory *GetWindowFactory() const
    {
        return window_factory_.get();
    }
    /**
     * @brief Gets the physics world factory.
     * @return A pointer to the physics world factory.
     */
    PAL::IPhysicsWorldFactory *GetPhysicsWorldFactory() const
    {
        return physics_world_factory_.get();
    }

  private:
    /**
     * @brief Private constructor to enforce singleton pattern.
     */
    ServiceLocator() = default;

    /** @brief The graphics device factory instance. */
    std::unique_ptr<RAL::IGraphicsDeviceFactory> graphics_device_factory_;
    /** @brief The window factory instance. */
    std::unique_ptr<WAL::IWindowFactory> window_factory_;
    /** @brief The physics world factory instance. */
    std::unique_ptr<PAL::IPhysicsWorldFactory> physics_world_factory_;
};

} // namespace Core
} // namespace Piece

#endif // PIECE_CORE_SERVICE_LOCATOR_H_
