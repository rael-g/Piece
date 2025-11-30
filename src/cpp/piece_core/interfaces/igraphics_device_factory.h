/**
 * @file igraphics_device_factory.h
 * @brief Defines the IGraphicsDeviceFactory interface for creating graphics device instances.
 */
#ifndef PIECE_CORE_GRAPHICS_DEVICE_FACTORY_H_
#define PIECE_CORE_GRAPHICS_DEVICE_FACTORY_H_

#include <piece_core/native_exports.h>
#include <ral/igraphics_device.h>
#include <wal/iwindow.h>

#include <memory>

namespace Piece
{
namespace Core
{

/**
 * @brief Abstract factory interface for creating IGraphicsDevice instances.
 *
 * @details This interface defines the contract for creating concrete graphics device implementations.
 * It is part of the Dependency Injection system where a specific implementation
 * (e.g., VulkanGraphicsDeviceFactory) is provided by the C# host and passed
 * to the Piece.Core.
 */
class IGraphicsDeviceFactory
{
  public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~IGraphicsDeviceFactory() = default;

    /**
     * @brief Creates a new IGraphicsDevice instance.
     *
     * @details This method is responsible for instantiating a concrete graphics device.
     * The provided window is used to establish a rendering context, and options
     * allow for specific configuration (e.g., enabling validation layers for Vulkan).
     *
     * @param window A pointer to the window interface that the graphics device will use.
     * @param options Configuration options for the graphics device.
     * @return A unique_ptr to the newly created IGraphicsDevice instance.
     */
    virtual std::unique_ptr<RAL::IGraphicsDevice> CreateGraphicsDevice(WAL::IWindow *window,
                                                                       const NativeVulkanOptions *options) = 0;
};

} // namespace Core
} // namespace Piece

#endif // PIECE_CORE_GRAPHICS_DEVICE_FACTORY_H_
