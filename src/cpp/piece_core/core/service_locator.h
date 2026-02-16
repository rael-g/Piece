/**
 * @file service_locator.h
 * @brief Defines the ServiceLocator class, a singleton for managing global service factories.
 */
#ifndef PIECE_CORE_SERVICE_LOCATOR_H_
#define PIECE_CORE_SERVICE_LOCATOR_H_

#include <pal/iphysics_world_factory.h>
#include <ral/igraphics_device_factory.h>
#include <wal/iwindow_factory.h>

#include <memory>
#include <piece_core/logging_api.h>
#include <piece_core/piece_core_exports.h> // For PIECE_CORE_API

namespace Piece::Core
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
    static PIECE_CORE_API ServiceLocator &Get();

    ServiceLocator(const ServiceLocator &) = delete;
    ServiceLocator &operator=(const ServiceLocator &) = delete;

    /**
     * @brief Sets the graphics device factory.
     * @param factory A unique pointer to the graphics device factory.
     */
    void SetGraphicsDeviceFactory(std::unique_ptr<Piece::RAL::IGraphicsDeviceFactory> factory)
    {
        PIECE_INFO("ServiceLocator::SetGraphicsDeviceFactory(factory: {0})", fmt::ptr(factory.get()));
        graphics_device_factory_ = std::move(factory);
    }
    /**
     * @brief Sets the window factory.
     * @param factory A unique pointer to the window factory.
     */
    void SetWindowFactory(std::unique_ptr<Piece::WAL::IWindowFactory> factory)
    {
        if (factory)
        {
            PIECE_INFO("ServiceLocator::SetWindowFactory - Setting factory: {0}", fmt::ptr(factory.get()));
        }
        else
        {
            PIECE_INFO("ServiceLocator::SetWindowFactory - Setting factory to nullptr.");
        }
        window_factory_ = std::move(factory);
    }
    /**
     * @brief Sets the physics world factory.
     * @param factory A unique pointer to the physics world factory.
     */
    void SetPhysicsWorldFactory(std::unique_ptr<Piece::PAL::IPhysicsWorldFactory> factory)
    {
        PIECE_INFO("ServiceLocator::SetPhysicsWorldFactory(factory: {0})", fmt::ptr(factory.get()));
        physics_world_factory_ = std::move(factory);
    }

    /**
     * @brief Gets the graphics device factory.
     * @return A pointer to the graphics device factory.
     */
    [[nodiscard]] Piece::RAL::IGraphicsDeviceFactory *GetGraphicsDeviceFactory() const
    {
        PIECE_TRACE("ServiceLocator::GetGraphicsDeviceFactory() -> {0}", fmt::ptr(graphics_device_factory_.get()));
        return graphics_device_factory_.get();
    }
    /**
     * @brief Gets the window factory.
     * @return A pointer to the window factory.
     */
    [[nodiscard]] Piece::WAL::IWindowFactory *GetWindowFactory() const
    {
        PIECE_INFO("ServiceLocator::GetWindowFactory() -> {0}", fmt::ptr(window_factory_.get()));
        return window_factory_.get();
    }
    /**
     * @brief Gets the physics world factory.
     * @return A pointer to the physics world factory.
     */
    [[nodiscard]] Piece::PAL::IPhysicsWorldFactory *GetPhysicsWorldFactory() const
    {
        PIECE_TRACE("ServiceLocator::GetPhysicsWorldFactory() -> {0}", fmt::ptr(physics_world_factory_.get()));
        return physics_world_factory_.get();
    }

  private:
    /**
     * @brief Private constructor to enforce singleton pattern.
     */
    ServiceLocator();

    /** @brief The graphics device factory instance. */
    std::unique_ptr<Piece::RAL::IGraphicsDeviceFactory> graphics_device_factory_;
    /** @brief The window factory instance. */
    std::unique_ptr<Piece::WAL::IWindowFactory> window_factory_;
    /** @brief The physics world factory instance. */
    std::unique_ptr<Piece::PAL::IPhysicsWorldFactory> physics_world_factory_;
};

} // namespace Piece::Core

#endif // PIECE_CORE_SERVICE_LOCATOR_H_
