#ifndef NATIVE_EXPORTS_H
#define NATIVE_EXPORTS_H

#include <pal/iphysics_world_factory.h>
#include <piece_core/piece_core_exports.h>
#include <ral/igraphics_device_factory.h>
#include <wal/iwindow_factory.h>

namespace Piece::Core
{
struct EngineCore; // Opaque struct moved here
} // namespace Piece::Core

extern "C"
{
    PIECE_CORE_API void SetGraphicsDeviceFactory(Piece::RAL::IGraphicsDeviceFactory *factory_ptr);
    PIECE_CORE_API void SetWindowFactory(Piece::WAL::IWindowFactory *factory_ptr);
    PIECE_CORE_API void SetPhysicsWorldFactory(Piece::PAL::IPhysicsWorldFactory *factory_ptr);

    /**
     * @brief Initializes the engine.
     * @return A pointer to the created EngineCore instance.
     */
    PIECE_CORE_API Piece::Core::EngineCore *EngineInitialize();

    /**
     * @brief Destroys the engine.
     * @param core_ptr A pointer to the EngineCore instance to destroy.
     */
    PIECE_CORE_API void EngineDestroy(Piece::Core::EngineCore *core_ptr);

    /**
     * @brief Updates the engine state.
     * @param core_ptr A pointer to the EngineCore instance.
     * @param delta_time The time elapsed since the last frame.
     */
    PIECE_CORE_API void EngineUpdate(Piece::Core::EngineCore *core_ptr, float delta_time);

    /**
     * @brief Renders a frame.
     * @param core_ptr A pointer to the EngineCore instance.
     */
    PIECE_CORE_API void EngineRender(Piece::Core::EngineCore *core_ptr);

    /**
     * @brief Function pointer type for log callbacks.
     * @param level The log level.
     * @param message The log message.
     */
    using LogCallback = void (*)(int level, const char *message);
    /**
     * @brief Registers a log callback function.
     * @param callback The callback function to register.
     */
    PIECE_CORE_API void PieceCoreRegisterLogCallback(LogCallback callback);
    /**
     * @brief Logs a message from the native side.
     * @param level The log level.
     * @param message The log message.
     */
    PIECE_CORE_API void PieceCoreLog(int level, const char *message);
}

#endif // NATIVE_EXPORTS_H
