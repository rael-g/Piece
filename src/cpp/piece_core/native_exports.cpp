#include "native_exports.h"
#include "core/service_locator.h"
#include "engine_core.h"
#include "logging_api.h"
#include "spdlog_interop_sink.h"
#include <pal/iphysics_world_factory.h>
#include <ral/igraphics_device_factory.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <wal/iwindow.h> // Explicitly include IWindow
#include <wal/iwindow_factory.h>

#include <iostream>

namespace Piece::Core
{
// Global logger callback for the interop sink
static LogCallback s_log_callback = nullptr;

/**
 * @brief Global spdlog logger instance.
 */
static std::shared_ptr<spdlog::logger> g_logger;

/**
 * @brief Initializes the spdlog logger with multiple sinks.
 */
static void InitializeLogger()
{
    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    sinks.push_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>("PieceEngine.log", 1024 * 1024 * 5, 3));
    sinks.push_back(std::make_shared<Piece::Core::InteropSink_mt>());

    g_logger = std::make_shared<spdlog::logger>("PieceEngine", begin(sinks), end(sinks));
    spdlog::set_default_logger(g_logger);
    spdlog::set_level(spdlog::level::trace);
    spdlog::flush_on(spdlog::level::info);
    PIECE_INFO("spdlog initialized.");
}

} // namespace Piece::Core

extern "C"
{
    PIECE_CORE_API void SetGraphicsDeviceFactory(Piece::RAL::IGraphicsDeviceFactory *factory_ptr)
    {
        PIECE_TRACE("SetGraphicsDeviceFactory(factory_ptr: {0})", fmt::ptr(factory_ptr));
        if (!factory_ptr)
        {
            PIECE_WARN("Received null IGraphicsDeviceFactory pointer. Clearing existing factory.");
            Piece::Core::ServiceLocator::Get().SetGraphicsDeviceFactory(nullptr); // Explicitly clear if null
            return;
        }
        Piece::Core::ServiceLocator::Get().SetGraphicsDeviceFactory(
            std::unique_ptr<Piece::RAL::IGraphicsDeviceFactory>(factory_ptr));
        PIECE_INFO("GraphicsDeviceFactory set successfully.");
    }

    PIECE_CORE_API void SetWindowFactory(Piece::WAL::IWindowFactory *factory_ptr)
    {
        PIECE_TRACE("SetWindowFactory(factory_ptr: {0})", fmt::ptr(factory_ptr));
        if (!factory_ptr)
        {
            PIECE_WARN("Received null IWindowFactory pointer. Clearing existing factory.");
            Piece::Core::ServiceLocator::Get().SetWindowFactory(nullptr); // Explicitly clear if null
            return;
        }
        Piece::Core::ServiceLocator::Get().SetWindowFactory(std::unique_ptr<Piece::WAL::IWindowFactory>(factory_ptr));
        PIECE_INFO("WindowFactory set successfully.");
    }

    PIECE_CORE_API void SetPhysicsWorldFactory(Piece::PAL::IPhysicsWorldFactory *factory_ptr)
    {
        PIECE_TRACE("SetPhysicsWorldFactory(factory_ptr: {0})", fmt::ptr(factory_ptr));
        if (!factory_ptr)
        {
            PIECE_WARN("Received null IPhysicsWorldFactory pointer. Clearing existing factory.");
            Piece::Core::ServiceLocator::Get().SetPhysicsWorldFactory(nullptr); // Explicitly clear if null
            return;
        }
        Piece::Core::ServiceLocator::Get().SetPhysicsWorldFactory(
            std::unique_ptr<Piece::PAL::IPhysicsWorldFactory>(factory_ptr));
        PIECE_INFO("PhysicsWorldFactory set successfully.");
    }

    PIECE_CORE_API Piece::Core::EngineCore *EngineInitialize()
    {
        PIECE_TRACE("EngineInitialize()");
        static bool logger_initialized = false;
        if (!logger_initialized)
        {
            Piece::Core::InitializeLogger();
            logger_initialized = true;
        }
        PIECE_INFO("EngineInitialize called. Attempting to create EngineCore...");
        auto *core = new Piece::Core::EngineCore();
        if (!core)
        {
            PIECE_FATAL("Failed to allocate EngineCore.");
            return nullptr;
        }
        PIECE_INFO("EngineCore created successfully at {0}.", fmt::ptr(core));

        // Perform full initialization
        if (!core->Initialize())
        {
            PIECE_ERROR("EngineCore full initialization failed. Destroying partially created core.");
            delete core;
            return nullptr;
        }
        PIECE_INFO("EngineCore fully initialized.");
        return core;
    }

    PIECE_CORE_API void EngineDestroy(Piece::Core::EngineCore *core_ptr)
    {
        PIECE_TRACE("EngineDestroy(core_ptr: {0})", fmt::ptr(core_ptr));
        if (core_ptr)
        {
            delete core_ptr;
            PIECE_INFO("EngineCore at {0} destroyed.", fmt::ptr(core_ptr));
        }
        else
        {
            PIECE_WARN("EngineDestroy called with null core_ptr.");
        }
    }

    PIECE_CORE_API void EngineUpdate(Piece::Core::EngineCore *core_ptr, float delta_time)
    {
        PIECE_TRACE("EngineUpdate(core_ptr: {0}, delta_time: {1})", fmt::ptr(core_ptr), delta_time);
        if (core_ptr)
        {
            core_ptr->Update(delta_time);
        }
    }

    PIECE_CORE_API void EngineRender(Piece::Core::EngineCore *core_ptr)
    {
        PIECE_TRACE("EngineRender(core_ptr: {0})", fmt::ptr(core_ptr));
        if (core_ptr)
        {
            core_ptr->Render();
        }
    }

    PIECE_CORE_API bool EngineIsKeyPressed(Piece::Core::EngineCore *core_ptr, Piece::WAL::KeyCode keycode)
    {
        PIECE_TRACE("EngineIsKeyPressed(core_ptr: {0}, keycode: {1})", fmt::ptr(core_ptr), static_cast<int>(keycode));
        if (!core_ptr || !core_ptr->GetWindow())
        {
            PIECE_WARN("EngineIsKeyPressed called with null core_ptr or window_.");
            return false;
        }
        return core_ptr->GetWindow()->IsKeyPressed(keycode);
    }

    PIECE_CORE_API bool EngineIsMouseButtonPressed(Piece::Core::EngineCore *core_ptr, Piece::WAL::KeyCode button)
    {
        PIECE_TRACE("EngineIsMouseButtonPressed(core_ptr: {0}, button: {1})", fmt::ptr(core_ptr),
                    static_cast<int>(button));
        if (!core_ptr || !core_ptr->GetWindow())
        {
            PIECE_WARN("EngineIsMouseButtonPressed called with null core_ptr or window_.");
            return false;
        }
        return core_ptr->GetWindow()->IsMouseButtonPressed(button);
    }

    PIECE_CORE_API float EngineGetMouseX(Piece::Core::EngineCore *core_ptr)
    {
        PIECE_TRACE("EngineGetMouseX(core_ptr: {0})", fmt::ptr(core_ptr));
        if (!core_ptr || !core_ptr->GetWindow())
        {
            PIECE_WARN("EngineGetMouseX called with null core_ptr or window_.");
            return 0.0f;
        }
        return core_ptr->GetWindow()->GetMouseX();
    }

    PIECE_CORE_API float EngineGetMouseY(Piece::Core::EngineCore *core_ptr)
    {
        PIECE_TRACE("EngineGetMouseY(core_ptr: {0})", fmt::ptr(core_ptr));
        if (!core_ptr || !core_ptr->GetWindow())
        {
            PIECE_WARN("EngineGetMouseY called with null core_ptr or window_.");
            return 0.0f;
        }
        return core_ptr->GetWindow()->GetMouseY();
    }

    PIECE_CORE_API void *EngineLoadMesh(Piece::Core::EngineCore *core_ptr, const char *path)
    {
        PIECE_TRACE("EngineLoadMesh(core_ptr: {0}, path: \"{1}\")", fmt::ptr(core_ptr), path);
        if (!core_ptr || !core_ptr->GetResourceManager())
        {
            PIECE_WARN("EngineLoadMesh called with null core_ptr or resource_manager_.");
            return nullptr;
        }
        std::string s_path(path);
        auto mesh_ptr = core_ptr->GetResourceManager()->LoadMesh(s_path);
        if (!mesh_ptr)
        {
            PIECE_ERROR("Failed to load mesh: {}.", s_path);
            return nullptr;
        }
        PIECE_INFO("Loaded mesh: {}. Returning raw pointer {}.", s_path, static_cast<void *>(mesh_ptr.get()));
        return mesh_ptr.get(); // Return raw pointer, ResourceManager owns shared_ptr
    }

    PIECE_CORE_API void *EngineLoadMaterial(Piece::Core::EngineCore *core_ptr, const char *path)
    {
        PIECE_TRACE("EngineLoadMaterial(core_ptr: {0}, path: \"{1}\")", fmt::ptr(core_ptr), path);
        if (!core_ptr || !core_ptr->GetResourceManager())
        {
            PIECE_WARN("EngineLoadMaterial called with null core_ptr or resource_manager_.");
            return nullptr;
        }
        std::string s_path(path);
        auto material_ptr = core_ptr->GetResourceManager()->LoadMaterial(s_path);
        if (!material_ptr)
        {
            PIECE_ERROR("Failed to load material: {}.", s_path);
            return nullptr;
        }
        PIECE_INFO("Loaded material: {}. Returning raw pointer {}.", s_path, static_cast<void *>(material_ptr.get()));
        return material_ptr.get(); // Return raw pointer, ResourceManager owns shared_ptr
    }

    PIECE_CORE_API void *EngineLoadTexture(Piece::Core::EngineCore *core_ptr, const char *path)
    {
        PIECE_TRACE("EngineLoadTexture(core_ptr: {0}, path: \"{1}\")", fmt::ptr(core_ptr), path);
        if (!core_ptr || !core_ptr->GetResourceManager())
        {
            PIECE_WARN("EngineLoadTexture called with null core_ptr or resource_manager_.");
            return nullptr;
        }
        std::string s_path(path);
        auto texture_ptr = core_ptr->GetResourceManager()->LoadTexture(s_path);
        if (!texture_ptr)
        {
            PIECE_ERROR("Failed to load texture: {}.", s_path);
            return nullptr;
        }
        PIECE_INFO("Loaded texture: {}. Returning raw pointer {}.", s_path, static_cast<void *>(texture_ptr.get()));
        return texture_ptr.get(); // Return raw pointer, ResourceManager owns shared_ptr
    }

    PIECE_CORE_API void PieceCoreRegisterLogCallback(LogCallback callback)
    {
        PIECE_TRACE("PieceCoreRegisterLogCallback(callback: {0})", fmt::ptr(callback));
        Piece::Core::s_log_callback = callback;
        if (Piece::Core::s_log_callback)
        {
            PIECE_INFO("C# LogCallback registered.");
        }
        else
        {
            PIECE_WARN("C# LogCallback unregistered (null callback).");
        }
    }

    PIECE_CORE_API void PieceCoreLog(int level, const char *message)
    {
        // No PIECE_TRACE here to avoid infinite recursion.
        if (Piece::Core::s_log_callback)
        {
            Piece::Core::s_log_callback(level, message);
        }
        else
        {
            // Fallback to std::cout if no callback is registered. This is a potential issue.
            // A PIECE_WARN would be ideal, but could lead to recursion if the interop sink is the only one.
            std::cout << "[Native Log Fallback] " << message << std::endl;
        }
    }
}
