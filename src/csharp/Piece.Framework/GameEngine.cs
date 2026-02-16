using Microsoft.Extensions.Logging;
using Piece.Core.Abstractions;
using Piece.Core.Abstractions.NativeCalls;

namespace Piece.Framework;

public class GameEngine : IDisposable
{
    private IntPtr _nativeEnginePtr;
    private readonly ILogger<GameEngine> _logger;
    private readonly IGraphicsDeviceFactory _graphicsDeviceFactory; // Used for configuring C++
    private readonly IWindowFactory _windowFactory; // Used for configuring C++
    private readonly IPhysicsWorldFactory _physicsWorldFactory; // Used for configuring C++
    private readonly InputManager _inputManager; // Added for input management
    private readonly AssetManager _assetManager; // Added for asset management
    private readonly RenderManager _renderManager; // Added for rendering management

    public GameEngine(ILogger<GameEngine> logger, 
                      IGraphicsDeviceFactory graphicsDeviceFactory, 
                      IWindowFactory windowFactory, 
                      IPhysicsWorldFactory physicsWorldFactory,
                      InputManager inputManager, // InputManager injected
                      AssetManager assetManager, // AssetManager injected
                      RenderManager renderManager) // RenderManager injected
    {
        _logger = logger;
        _graphicsDeviceFactory = graphicsDeviceFactory;
        _windowFactory = windowFactory;
        _physicsWorldFactory = physicsWorldFactory;
        _inputManager = inputManager; // Assigned injected InputManager
        _assetManager = assetManager; // Assigned injected AssetManager
        _renderManager = renderManager; // Assigned injected RenderManager

        _logger.LogInformation("GameEngine created.");
    }

    public void Initialize()
    {
        // Set the C++ factories using the resolved C# factory wrappers
        // The C# factory wrappers handle loading native DLLs and getting C++ factory pointers
        NativeCalls.PieceCore_SetGraphicsDeviceFactory(_graphicsDeviceFactory.GetNativeFactoryPtr());
        NativeCalls.PieceCore_SetWindowFactory(_windowFactory.GetNativeFactoryPtr());
        NativeCalls.PieceCore_SetPhysicsWorldFactory(_physicsWorldFactory.GetNativeFactoryPtr());

        _nativeEnginePtr = NativeCalls.Engine_Initialize();
        if (_nativeEnginePtr == IntPtr.Zero)
        {
            _logger.LogError("Failed to initialize native EngineCore.");
            throw new InvalidOperationException("Native EngineCore initialization failed.");
        }
        _logger.LogInformation("Native EngineCore initialized.");

        // Initialize the InputManager with the native engine core pointer
        _inputManager.Initialize(_nativeEnginePtr);

        // Initialize the AssetManager with the native engine core pointer
        _assetManager.Initialize(_nativeEnginePtr);
    }

    public void Update(float deltaTime)
    {
        if (_nativeEnginePtr != IntPtr.Zero)
        {
            NativeCalls.Engine_Update(_nativeEnginePtr, deltaTime);
        }
    }

    public void Render()
    {
        if (_nativeEnginePtr != IntPtr.Zero)
        {
            // First, components submit their rendering commands to the RenderManager
            // (This step will involve traversing the scene graph and calling component Render() methods)
            // For now, we manually execute commands and clear them
            _renderManager.ExecuteRenderCommands();

            // Then, the native engine renders the frame
            NativeCalls.Engine_Render(_nativeEnginePtr);

            // Clear commands for the next frame
            _renderManager.ClearRenderCommands();
        }
    }

    public void Dispose()
    {
        if (_nativeEnginePtr != IntPtr.Zero)
        {
            NativeCalls.Engine_Destroy(_nativeEnginePtr);
            _nativeEnginePtr = IntPtr.Zero;
            _logger.LogInformation("Native EngineCore destroyed.");
        }
    }
}