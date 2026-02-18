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
     * @brief Checks if a specific key is currently pressed.
     * @param core_ptr A pointer to the EngineCore instance.
     * @param keycode The key to check.
     * @return True if the key is pressed, false otherwise.
     */
    PIECE_CORE_API bool EngineIsKeyPressed(Piece::Core::EngineCore *core_ptr, Piece::WAL::KeyCode keycode);

    /**
     * @brief Checks if a specific mouse button is currently pressed.
     * @param core_ptr A pointer to the EngineCore instance.
     * @param button The mouse button to check.
     * @return True if the button is pressed, false otherwise.
     */
    PIECE_CORE_API bool EngineIsMouseButtonPressed(Piece::Core::EngineCore *core_ptr, Piece::WAL::KeyCode button);

    /**
     * @brief Gets the current x-coordinate of the mouse cursor.
     * @param core_ptr A pointer to the EngineCore instance.
     * @return The x-coordinate of the mouse.
     */
    PIECE_CORE_API float EngineGetMouseX(Piece::Core::EngineCore *core_ptr);

    /**
     * @brief Gets the current y-coordinate of the mouse cursor.
     * @param core_ptr A pointer to the EngineCore instance.
     * @return The y-coordinate of the mouse.
     */
    PIECE_CORE_API float EngineGetMouseY(Piece::Core::EngineCore *core_ptr);

    /**
     * @brief Loads a mesh asset.
     * @param core_ptr A pointer to the EngineCore instance.
     * @param path The path to the mesh asset.
     * @return A pointer to the loaded C++ Mesh object, or nullptr if loading fails.
     */
    PIECE_CORE_API void *EngineLoadMesh(Piece::Core::EngineCore *core_ptr, const char *path);

    /**
     * @brief Loads a material asset.
     * @param core_ptr A pointer to the EngineCore instance.
     * @param path The path to the material asset.
     * @return A pointer to the loaded C++ Material object, or nullptr if loading fails.
     */
    PIECE_CORE_API void *EngineLoadMaterial(Piece::Core::EngineCore *core_ptr, const char *path);

    /**
     * @brief Loads a texture asset.
     * @param core_ptr A pointer to the EngineCore instance.
     * @param path The path to the texture asset.
     * @return A pointer to the loaded C++ Texture object, or nullptr if loading fails.
     */
    PIECE_CORE_API void *EngineLoadTexture(Piece::Core::EngineCore *core_ptr, const char *path);

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

    /**
     * @brief Initializes the logger. This should be called once at the start of the application.
     */
    PIECE_CORE_API void PieceCoreInitializeLogger();
}

#endif // NATIVE_EXPORTS_H
