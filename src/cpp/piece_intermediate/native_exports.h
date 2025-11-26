#ifndef NATIVE_EXPORTS_H
#define NATIVE_EXPORTS_H

#include <piece_intermediate/native_interop_types.h>
#include <piece_intermediate/piece_intermediate_exports.h>

namespace Piece { namespace Intermediate {
    class IGraphicsDeviceFactory;
    class IWindowFactory;
    class IPhysicsWorldFactory;
}} // namespace Piece::Intermediate

extern "C" {
    PIECE_INTERMEDIATE_API void PieceIntermediate_SetGraphicsDeviceFactory(
        Piece::Intermediate::IGraphicsDeviceFactory* factory_ptr,
        const NativeVulkanOptions* options
    );

    PIECE_INTERMEDIATE_API void PieceIntermediate_SetWindowFactory(
        Piece::Intermediate::IWindowFactory* factory_ptr,
        const NativeWindowOptions* options
    );

    PIECE_INTERMEDIATE_API void PieceIntermediate_SetPhysicsWorldFactory(
        Piece::Intermediate::IPhysicsWorldFactory* factory_ptr,
        const NativePhysicsOptions* options
    );

    PIECE_INTERMEDIATE_API EngineCore* Engine_Initialize();

    PIECE_INTERMEDIATE_API void Engine_Destroy(EngineCore* core_ptr);

    PIECE_INTERMEDIATE_API void Engine_Update(EngineCore* core_ptr, float delta_time);

    PIECE_INTERMEDIATE_API void Engine_Render(EngineCore* core_ptr);

    typedef void (*LogCallback)(int level, const char* message);
    PIECE_INTERMEDIATE_API void PieceIntermediate_RegisterLogCallback(LogCallback callback);
    PIECE_INTERMEDIATE_API void PieceIntermediate_Log(int level, const char* message);
}

#endif // NATIVE_EXPORTS_H