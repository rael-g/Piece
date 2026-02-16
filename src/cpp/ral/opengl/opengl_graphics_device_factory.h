#ifndef RAL_OPENGL_OPENGL_GRAPHICS_DEVICE_FACTORY_H_
#define RAL_OPENGL_OPENGL_GRAPHICS_DEVICE_FACTORY_H_

#include <ral/igraphics_device_factory.h>
#include <ral/native_graphics_options.h>
#include <wal/iwindow.h>

namespace Piece::RAL
{
class OpenGLGraphicsDeviceFactory : public IGraphicsDeviceFactory
{
  public:
    explicit OpenGLGraphicsDeviceFactory(const NativeGraphicsOptions *options);
    ~OpenGLGraphicsDeviceFactory() override = default;

    std::unique_ptr<IGraphicsDevice> CreateGraphicsDevice(WAL::IWindow *window,
                                                          const NativeGraphicsOptions *options) override;

  private:
    NativeGraphicsOptions options_;
};
} // namespace Piece::RAL

#endif // RAL_OPENGL_OPENGL_GRAPHICS_DEVICE_FACTORY_H_
