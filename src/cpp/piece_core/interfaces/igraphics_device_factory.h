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

class IGraphicsDeviceFactory
{
  public:
    virtual ~IGraphicsDeviceFactory() = default;

    virtual std::unique_ptr<RAL::IGraphicsDevice> CreateGraphicsDevice(WAL::IWindow *window,
                                                                       const NativeVulkanOptions *options) = 0;
};

} // namespace Core
} // namespace Piece

#endif // PIECE_CORE_GRAPHICS_DEVICE_FACTORY_H_
