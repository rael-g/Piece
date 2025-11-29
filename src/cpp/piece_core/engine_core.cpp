#include "engine_core.h"

#include <pal/iphysics_world.h>
#include <ral/igraphics_device.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <wal/iwindow.h>

#include "core/service_locator.h"
#include "logging_api.h"
#include "native_exports.h"
#include "spdlog_interop_sink.h"

namespace Piece
{
namespace Core
{
static std::shared_ptr<spdlog::logger> g_logger;

void InitializeLogger()
{
    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    sinks.push_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>("PieceEngine.log", 1024 * 1024 * 5, 3));
    sinks.push_back(std::make_shared<Piece::Core::InteropSink_mt>());

    g_logger = std::make_shared<spdlog::logger>("PieceEngine", begin(sinks), end(sinks));
    spdlog::set_default_logger(g_logger);
    spdlog::set_level(spdlog::level::trace);
    spdlog::flush_on(spdlog::level::info);
    g_logger->info("spdlog initialized.");
}

EngineCore::EngineCore()
{
    IWindowFactory *windowFactory = ServiceLocator::Get().GetWindowFactory();
    IGraphicsDeviceFactory *graphicsFactory = ServiceLocator::Get().GetGraphicsDeviceFactory();
    IPhysicsWorldFactory *physicsFactory = ServiceLocator::Get().GetPhysicsWorldFactory();

    if (!windowFactory)
    {
        spdlog::error("IWindowFactory not set in ServiceLocator. Engine cannot initialize.");
        return;
    }
    if (!graphicsFactory)
    {
        spdlog::error("IGraphicsDeviceFactory not set in ServiceLocator. Engine cannot "
                      "initialize.");
        return;
    }
    if (!physicsFactory)
    {
        spdlog::error("IPhysicsWorldFactory not set in ServiceLocator. Engine cannot "
                      "initialize.");
        return;
    }

    Piece::Core::NativeWindowOptions defaultWindowOptions = {800, 600, 0, "Piece Engine Window"};
    window_ = windowFactory->CreateWindow(&defaultWindowOptions);
    if (!window_)
    {
        spdlog::error("Failed to create IWindow instance.");
        return;
    }
    spdlog::info("IWindow created.");

    Piece::Core::NativeVulkanOptions defaultVulkanOptions = {0, 2};
    graphics_device_ = graphicsFactory->CreateGraphicsDevice(window_.get(), &defaultVulkanOptions);
    if (!graphics_device_)
    {
        spdlog::error("Failed to create IGraphicsDevice instance.");
        return;
    }
    spdlog::info("IGraphicsDevice created.");

    Piece::Core::NativePhysicsOptions defaultPhysicsOptions = {1.0f / 60.0f, 4};
    physics_world_ = physicsFactory->CreatePhysicsWorld(&defaultPhysicsOptions);
    if (!physics_world_)
    {
        spdlog::error("Failed to create IPhysicsWorld instance.");
        return;
    }
    spdlog::info("IPhysicsWorld created.");
    spdlog::info("EngineCore: Initialized successfully.");
}

EngineCore::~EngineCore()
{
    spdlog::info("EngineCore: Destroyed.");
}

void EngineCore::Update(float deltaTime)
{
    if (physics_world_)
    {
        physics_world_->Step(deltaTime);
    }
}

void EngineCore::Render()
{
    if (window_ && graphics_device_)
    {
    }
}

} // namespace Core
} // namespace Piece

template <typename T> bool is_valid_factory_ptr(T *ptr)
{
    return ptr != nullptr;
}

extern "C"
{

    void PieceCore_SetGraphicsDeviceFactory(Piece::Core::IGraphicsDeviceFactory *factoryPtr,
                                            const Piece::Core::NativeVulkanOptions *options)
    {
        if (!is_valid_factory_ptr(factoryPtr))
        {
            spdlog::error("Invalid IGraphicsDeviceFactory pointer received.");
            return;
        }
        Piece::Core::ServiceLocator::Get().SetGraphicsDeviceFactory(
            std::unique_ptr<Piece::Core::IGraphicsDeviceFactory>(factoryPtr));
        spdlog::info("PieceCore_SetGraphicsDeviceFactory called.");
    }

    void PieceCore_SetWindowFactory(Piece::Core::IWindowFactory *factoryPtr, const Piece::Core::NativeWindowOptions *options)
    {
        if (!is_valid_factory_ptr(factoryPtr))
        {
            spdlog::error("Invalid IWindowFactory pointer received.");
            return;
        }
        Piece::Core::ServiceLocator::Get().SetWindowFactory(std::unique_ptr<Piece::Core::IWindowFactory>(factoryPtr));
        spdlog::info("PieceCore_SetWindowFactory called.");
    }

    void PieceCore_SetPhysicsWorldFactory(Piece::Core::IPhysicsWorldFactory *factoryPtr,
                                          const Piece::Core::NativePhysicsOptions *options)
    {
        if (!is_valid_factory_ptr(factoryPtr))
        {
            spdlog::error("Invalid IPhysicsWorldFactory pointer received.");
            return;
        }
        Piece::Core::ServiceLocator::Get().SetPhysicsWorldFactory(
            std::unique_ptr<Piece::Core::IPhysicsWorldFactory>(factoryPtr));
        spdlog::info("PieceCore_SetPhysicsWorldFactory called.");
    }

    Piece::Core::EngineCore *Engine_Initialize()
    {
        static bool loggerInitialized = false;
        if (!loggerInitialized)
        {
            Piece::Core::InitializeLogger();
            loggerInitialized = true;
        }
        spdlog::info("Engine_Initialize called. Attempting to create EngineCore...");
        Piece::Core::EngineCore *core = new Piece::Core::EngineCore();
        if (!core)
        {
            spdlog::error("Failed to allocate EngineCore.");
            return nullptr;
        }
        return reinterpret_cast<Piece::Core::EngineCore *>(core);
    }

    void Engine_Destroy(Piece::Core::EngineCore *corePtr)
    {
        spdlog::info("Engine_Destroy called.");
        if (corePtr)
        {
            delete reinterpret_cast<Piece::Core::EngineCore *>(corePtr);
        }
        else
        {
            spdlog::warn("Engine_Destroy called with null corePtr.");
        }
    }

    void Engine_Update(Piece::Core::EngineCore *corePtr, float deltaTime)
    {
        if (corePtr)
        {
            reinterpret_cast<Piece::Core::EngineCore *>(corePtr)->Update(deltaTime);
        }
    }

    void Engine_Render(Piece::Core::EngineCore *corePtr)
    {
        if (corePtr)
        {
            reinterpret_cast<Piece::Core::EngineCore *>(corePtr)->Render();
        }
    }

    static LogCallback s_log_callback = nullptr;

    PIECE_CORE_API void PieceCore_RegisterLogCallback(LogCallback callback)
    {
        s_log_callback = callback;
        if (s_log_callback)
        {
            spdlog::info("C# LogCallback registered.");
        }
        else
        {
            spdlog::warn("C# LogCallback unregistered (null callback).");
        }
    }

    PIECE_CORE_API void PieceCore_Log(int level, const char *message)
    {
        if (s_log_callback)
        {
            s_log_callback(level, message);
        }
    }

} // extern "C"
