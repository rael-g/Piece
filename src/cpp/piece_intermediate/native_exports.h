#ifndef NATIVE_EXPORTS_H
#define NATIVE_EXPORTS_H

#include <piece_intermediate/native_interop_types.h>

namespace Piece { namespace Intermediate {
    class IGraphicsDeviceFactory;
    class IWindowFactory;
    class IPhysicsWorldFactory;
}} // namespace Piece::Intermediate

extern "C" {



    __declspec(dllexport) void PieceIntermediate_SetGraphicsDeviceFactory(
        Piece::Intermediate::IGraphicsDeviceFactory* factory_ptr,
        const NativeVulkanOptions* options
    );

    __declspec(dllexport) void PieceIntermediate_SetWindowFactory(
        Piece::Intermediate::IWindowFactory* factory_ptr,
        const NativeWindowOptions* options
    );

    __declspec(dllexport) void PieceIntermediate_SetPhysicsWorldFactory(
        Piece::Intermediate::IPhysicsWorldFactory* factory_ptr,
        const NativePhysicsOptions* options
    );



    __declspec(dllexport) EngineCore* Engine_Initialize();



    __declspec(dllexport) void Engine_Destroy(EngineCore* core_ptr);


    __declspec(dllexport) void Engine_Update(EngineCore* core_ptr, float delta_time);


    __declspec(dllexport) void Engine_Render(EngineCore* core_ptr);
}

#endif // NATIVE_EXPORTS_H