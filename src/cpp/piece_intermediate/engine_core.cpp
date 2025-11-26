#include "engine_core.h"
#include "core/service_locator.h"
#include "native_exports.h"
#include "logging_api.h"

#include <wal/iwindow.h>
#include <ral/igraphics_device.h>
#include <pal/iphysics_world.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include "spdlog_interop_sink.h"

namespace Piece { namespace Intermediate {

static std::shared_ptr<spdlog::logger> g_logger;

void InitializeLogger() {
    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    sinks.push_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>("PieceEngine.log", 1024 * 1024 * 5, 3));
    sinks.push_back(std::make_shared<Piece::Intermediate::InteropSink_mt>());

    g_logger = std::make_shared<spdlog::logger>("PieceEngine", begin(sinks), end(sinks));
    spdlog::set_default_logger(g_logger);
    spdlog::set_level(spdlog::level::trace);
    spdlog::flush_on(spdlog::level::info);
    g_logger->info("spdlog initialized.");
}

EngineCore::EngineCore() {
    IWindowFactory* windowFactory = ServiceLocator::Get().GetWindowFactory();
    IGraphicsDeviceFactory* graphicsFactory = ServiceLocator::Get().GetGraphicsDeviceFactory();
    IPhysicsWorldFactory* physicsFactory = ServiceLocator::Get().GetPhysicsWorldFactory();

    if (!windowFactory) {
        spdlog::error("IWindowFactory not set in ServiceLocator. Engine cannot initialize.");
        return;
    }
    if (!graphicsFactory) {
        spdlog::error("IGraphicsDeviceFactory not set in ServiceLocator. Engine cannot initialize.");
        return;
    }
    if (!physicsFactory) {
        spdlog::error("IPhysicsWorldFactory not set in ServiceLocator. Engine cannot initialize.");
        return;
    }

    NativeWindowOptions defaultWindowOptions = { 800, 600, 0, "Piece Engine Window" };
    window_ = windowFactory->CreateWindow(&defaultWindowOptions);
    if (!window_) {
        spdlog::error("Failed to create IWindow instance.");
        return;
    }
    spdlog::info("IWindow created.");

    NativeVulkanOptions defaultVulkanOptions = { 0, 2 };
    graphics_device_ = graphicsFactory->CreateGraphicsDevice(window_.get(), &defaultVulkanOptions);
    if (!graphics_device_) {
        spdlog::error("Failed to create IGraphicsDevice instance.");
        return;
    }
    spdlog::info("IGraphicsDevice created.");

    NativePhysicsOptions defaultPhysicsOptions = { 1.0f / 60.0f, 4 };
    physics_world_ = physicsFactory->CreatePhysicsWorld(&defaultPhysicsOptions);
    if (!physics_world_) {
        spdlog::error("Failed to create IPhysicsWorld instance.");
        return;
    }
    spdlog::info("IPhysicsWorld created.");
    spdlog::info("EngineCore: Initialized successfully.");
}

EngineCore::~EngineCore() {
    spdlog::info("EngineCore: Destroyed.");
}

void EngineCore::Update(float deltaTime) {

    if (physics_world_) {
        physics_world_->Step(deltaTime);
    }

}

void EngineCore::Render() {

    if (window_ && graphics_device_) {

    }
}

}} // namespace Piece::Intermediate



template<typename T>
bool is_valid_factory_ptr(T* ptr) {

    return ptr != nullptr;
}


extern "C" {

void PieceIntermediate_SetGraphicsDeviceFactory(
    Piece::Intermediate::IGraphicsDeviceFactory* factoryPtr,
    const NativeVulkanOptions* options) {
        if (!is_valid_factory_ptr(factoryPtr)) {
            spdlog::error("Invalid IGraphicsDeviceFactory pointer received.");
            return;
        }
        Piece::Intermediate::ServiceLocator::Get().SetGraphicsDeviceFactory(
            std::unique_ptr<Piece::Intermediate::IGraphicsDeviceFactory>(factoryPtr)
        );
        spdlog::info("PieceIntermediate_SetGraphicsDeviceFactory called.");
}

void PieceIntermediate_SetWindowFactory(
    Piece::Intermediate::IWindowFactory* factoryPtr,
    const NativeWindowOptions* options) {
    if (!is_valid_factory_ptr(factoryPtr)) {
        spdlog::error("Invalid IWindowFactory pointer received.");
        return;
    }
    Piece::Intermediate::ServiceLocator::Get().SetWindowFactory(
        std::unique_ptr<Piece::Intermediate::IWindowFactory>(factoryPtr)
    );
    spdlog::info("PieceIntermediate_SetWindowFactory called.");
}

void PieceIntermediate_SetPhysicsWorldFactory(
    Piece::Intermediate::IPhysicsWorldFactory* factoryPtr,
    const NativePhysicsOptions* options) {
    if (!is_valid_factory_ptr(factoryPtr)) {
        spdlog::error("Invalid IPhysicsWorldFactory pointer received.");
        return;
    }
    Piece::Intermediate::ServiceLocator::Get().SetPhysicsWorldFactory(
        std::unique_ptr<Piece::Intermediate::IPhysicsWorldFactory>(factoryPtr)
    );
    spdlog::info("PieceIntermediate_SetPhysicsWorldFactory called.");
}

EngineCore* Engine_Initialize() {
    static bool loggerInitialized = false;
    if (!loggerInitialized) {
        Piece::Intermediate::InitializeLogger();
        loggerInitialized = true;
    }
    spdlog::info("Engine_Initialize called. Attempting to create EngineCore...");
    Piece::Intermediate::EngineCore* core = new Piece::Intermediate::EngineCore();
    if (!core) {
        spdlog::error("Failed to allocate EngineCore.");
        return nullptr;
    }
    return reinterpret_cast<EngineCore*>(core);
}

void Engine_Destroy(EngineCore* corePtr) {
    spdlog::info("Engine_Destroy called.");
    if (corePtr) {
        delete reinterpret_cast<Piece::Intermediate::EngineCore*>(corePtr);
    } else {
        spdlog::warn("Engine_Destroy called with null corePtr.");
    }
}

void Engine_Update(EngineCore* corePtr, float deltaTime) {
    if (corePtr) {
        reinterpret_cast<Piece::Intermediate::EngineCore*>(corePtr)->Update(deltaTime);
    }
}

void Engine_Render(EngineCore* corePtr) {
    if (corePtr) {
        reinterpret_cast<Piece::Intermediate::EngineCore*>(corePtr)->Render();
    }
}

static LogCallback s_log_callback = nullptr;

PIECE_INTERMEDIATE_API void PieceIntermediate_RegisterLogCallback(LogCallback callback) {
        s_log_callback = callback;
        if (s_log_callback) {
            spdlog::info("C# LogCallback registered.");
        } else {
            spdlog::warn("C# LogCallback unregistered (null callback).");
        }
    }

    PIECE_INTERMEDIATE_API void PieceIntermediate_Log(int level, const char* message) {
        if (s_log_callback) {
            s_log_callback(level, message);
        }
    }

} // extern "C"
