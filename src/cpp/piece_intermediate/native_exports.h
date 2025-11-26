#ifndef NATIVE_EXPORTS_H
#define NATIVE_EXPORTS_H

#include <piece_intermediate/native_interop_types.h>

namespace Piece { namespace Intermediate {
    class IGraphicsDeviceFactory;
    class IWindowFactory;
    class IPhysicsWorldFactory;
}} // namespace Piece::Intermediate

extern "C" {
    // Functions for the C# layer to set C++ factory instances in the Service Locator.
    // The factoryPtr is the raw C++ factory pointer (IntPtr from C#).
    // The options parameter points to a marshaled C-compatible struct from C#.
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

    // Main engine initialization function.
    // Returns an opaque pointer to the EngineCore instance (IntPtr in C#).
    __declspec(dllexport) EngineCore* Engine_Initialize();

    // Main engine destruction function.
    // Takes the opaque pointer to the EngineCore instance.
    __declspec(dllexport) void Engine_Destroy(EngineCore* core_ptr);

    // Placeholder for Engine Update function
    __declspec(dllexport) void Engine_Update(EngineCore* core_ptr, float delta_time);

    // Placeholder for Engine Render function
    __declspec(dllexport) void Engine_Render(EngineCore* core_ptr);
}

#endif // NATIVE_EXPORTS_H