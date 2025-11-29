#ifndef NATIVE_EXPORTS_H
#define NATIVE_EXPORTS_H

#include <piece_core/native_interop_types.h>
#include <piece_core/piece_core_exports.h>

namespace Piece
{
namespace Core
{
struct EngineCore;
class IGraphicsDeviceFactory;
class IWindowFactory;
class IPhysicsWorldFactory;
} // namespace Core
} // namespace Piece

extern "C"
{
    PIECE_CORE_API void PieceCore_SetGraphicsDeviceFactory(Piece::Core::IGraphicsDeviceFactory *factory_ptr,
                                                           const NativeVulkanOptions *options);

    PIECE_CORE_API void PieceCore_SetWindowFactory(Piece::Core::IWindowFactory *factory_ptr,
                                                   const NativeWindowOptions *options);
    PIECE_CORE_API void PieceCore_SetPhysicsWorldFactory(Piece::Core::IPhysicsWorldFactory *factory_ptr,
                                                         const NativePhysicsOptions *options);

    PIECE_CORE_API Piece::Core::EngineCore *Engine_Initialize();

    PIECE_CORE_API void Engine_Destroy(Piece::Core::EngineCore *core_ptr);

    PIECE_CORE_API void Engine_Update(Piece::Core::EngineCore *core_ptr, float delta_time);

    PIECE_CORE_API void Engine_Render(Piece::Core::EngineCore *core_ptr);

    typedef void (*LogCallback)(int level, const char *message);
    PIECE_CORE_API void PieceCore_RegisterLogCallback(LogCallback callback);
    PIECE_CORE_API void PieceCore_Log(int level, const char *message);
}

#endif // NATIVE_EXPORTS_H