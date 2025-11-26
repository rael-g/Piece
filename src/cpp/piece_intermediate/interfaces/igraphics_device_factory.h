#ifndef PIECE_INTERMEDIATE_GRAPHICS_DEVICE_FACTORY_H_
#define PIECE_INTERMEDIATE_GRAPHICS_DEVICE_FACTORY_H_

#include <memory>
#include <ral/igraphics_device.h>
#include <wal/iwindow.h>

#include <piece_intermediate/native_exports.h>

namespace Piece { namespace Intermediate {

class IGraphicsDeviceFactory {
public:
    virtual ~IGraphicsDeviceFactory() = default;

    virtual std::unique_ptr<RAL::IGraphicsDevice> CreateGraphicsDevice(WAL::IWindow* window, const NativeVulkanOptions* options) = 0;
};

}} // namespace Piece::Intermediate

#endif  // PIECE_INTERMEDIATE_GRAPHICS_DEVICE_FACTORY_H_
