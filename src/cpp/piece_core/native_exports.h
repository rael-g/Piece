/**
 * @file native_exports.h
 * @brief Defines the C-style API exported by the Piece Core library.
 *        This API is used for interoperability with other languages, such as C#.
 */
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
    /**
     * @brief Sets the graphics device factory.
     * @param factory_ptr A pointer to an IGraphicsDeviceFactory implementation.
     * @param options A pointer to the native Vulkan options.
     */
    PIECE_CORE_API void PieceCore_SetGraphicsDeviceFactory(Piece::Core::IGraphicsDeviceFactory *factory_ptr,
                                                           const Piece::Core::NativeVulkanOptions *options);

    /**
     * @brief Sets the window factory.
     * @param factory_ptr A pointer to an IWindowFactory implementation.
     * @param options A pointer to the native window options.
     */
    PIECE_CORE_API void PieceCore_SetWindowFactory(Piece::Core::IWindowFactory *factory_ptr,
                                                   const Piece::Core::NativeWindowOptions *options);
    /**
     * @brief Sets the physics world factory.
     * @param factory_ptr A pointer to an IPhysicsWorldFactory implementation.
     * @param options A pointer to the native physics options.
     */
    PIECE_CORE_API void PieceCore_SetPhysicsWorldFactory(Piece::Core::IPhysicsWorldFactory *factory_ptr,
                                                         const Piece::Core::NativePhysicsOptions *options);

    /**
     * @brief Initializes the engine.
     * @return A pointer to the created EngineCore instance.
     */
    PIECE_CORE_API Piece::Core::EngineCore *Engine_Initialize();

    /**
     * @brief Destroys the engine.
     * @param core_ptr A pointer to the EngineCore instance to destroy.
     */
    PIECE_CORE_API void Engine_Destroy(Piece::Core::EngineCore *core_ptr);

    /**
     * @brief Updates the engine state.
     * @param core_ptr A pointer to the EngineCore instance.
     * @param delta_time The time elapsed since the last frame.
     */
    PIECE_CORE_API void Engine_Update(Piece::Core::EngineCore *core_ptr, float delta_time);

    /**
     * @brief Renders a frame.
     * @param core_ptr A pointer to the EngineCore instance.
     */
    PIECE_CORE_API void Engine_Render(Piece::Core::EngineCore *core_ptr);

    /**
     * @brief Function pointer type for log callbacks.
     * @param level The log level.
     * @param message The log message.
     */
    typedef void (*LogCallback)(int level, const char *message);
    /**
     * @brief Registers a log callback function.
     * @param callback The callback function to register.
     */
    PIECE_CORE_API void PieceCore_RegisterLogCallback(LogCallback callback);
    /**
     * @brief Logs a message from the native side.
     * @param level The log level.
     * @param message The log message.
     */
    PIECE_CORE_API void PieceCore_Log(int level, const char *message);
}

#endif // NATIVE_EXPORTS_H