using System;
using Microsoft.Extensions.Logging;
using Piece.Core; // For NativeCalls and KeyCode
using System.Numerics; // For Vector2

namespace Piece.Framework;

/// <summary>
/// Manages all input from the windowing system.
/// </summary>
public class InputManager
{
    private readonly ILogger<InputManager> _logger;
    private IntPtr _nativeEngineCorePtr;

    public InputManager(ILogger<InputManager> logger)
    {
        _logger = logger;
    }

    /// <summary>
    /// Initializes the InputManager with the native engine core pointer.
    /// This should be called by the GameEngine after successful native engine initialization.
    /// </summary>
    /// <param name="nativeEngineCorePtr">The raw pointer to the native C++ EngineCore instance.</param>
    public void Initialize(IntPtr nativeEngineCorePtr)
    {
        _nativeEngineCorePtr = nativeEngineCorePtr;
        _logger.LogInformation("InputManager initialized with native engine core pointer: {Pointer}", _nativeEngineCorePtr);
    }

    /// <summary>
    /// Checks if a specific keyboard key is currently pressed.
    /// </summary>
    /// <param name="keyCode">The key to check.</param>
    /// <returns>True if the key is pressed, false otherwise.</returns>
    public bool IsKeyPressed(KeyCode keyCode)
    {
        if (_nativeEngineCorePtr == IntPtr.Zero)
        {
            _logger.LogWarning("IsKeyPressed called before InputManager is initialized.");
            return false;
        }
        bool pressed = NativeCalls.EngineIsKeyPressed(_nativeEngineCorePtr, keyCode);
        _logger.LogTrace("Key {KeyCode} is pressed: {IsPressed}", keyCode, pressed);
        return pressed;
    }

    /// <summary>
    /// Checks if a specific mouse button is currently pressed.
    /// </summary>
    /// <param name="button">The mouse button to check.</param>
    /// <returns>True if the button is pressed, false otherwise.</returns>
    public bool IsMouseButtonPressed(KeyCode button)
    {
        if (_nativeEngineCorePtr == IntPtr.Zero)
        {
            _logger.LogWarning("IsMouseButtonPressed called before InputManager is initialized.");
            return false;
        }
        bool pressed = NativeCalls.EngineIsMouseButtonPressed(_nativeEngineCorePtr, button);
        _logger.LogTrace("Mouse Button {MouseButton} is pressed: {IsPressed}", button, pressed);
        return pressed;
    }

    /// <summary>
    /// Gets the current mouse cursor position relative to the window.
    /// </summary>
    /// <returns>A Vector2 representing the X and Y coordinates of the mouse.</returns>
    public Vector2 GetMousePosition()
    {
        if (_nativeEngineCorePtr == IntPtr.Zero)
        {
            _logger.LogWarning("GetMousePosition called before InputManager is initialized.");
            return Vector2.Zero;
        }
        float x = NativeCalls.EngineGetMouseX(_nativeEngineCorePtr);
        float y = NativeCalls.EngineGetMouseY(_nativeEngineCorePtr);
        Vector2 position = new Vector2(x, y);
        _logger.LogTrace("Mouse Position: {Position}", position);
        return position;
    }

    /// <summary>
    /// Gets the current mouse cursor X coordinate.
    /// </summary>
    /// <returns>The X coordinate of the mouse.</returns>
    public float GetMouseX()
    {
        if (_nativeEngineCorePtr == IntPtr.Zero)
        {
            _logger.LogWarning("GetMouseX called before InputManager is initialized.");
            return 0.0f;
        }
        float x = NativeCalls.EngineGetMouseX(_nativeEngineCorePtr);
        _logger.LogTrace("Mouse X: {X}", x);
        return x;
    }

    /// <summary>
    /// Gets the current mouse cursor Y coordinate.
    /// </summary>
    /// <returns>The Y coordinate of the mouse.</returns>
    public float GetMouseY()
    {
        if (_nativeEngineCorePtr == IntPtr.Zero)
        {
            _logger.LogWarning("GetMouseY called before InputManager is initialized.");
            return 0.0f;
        }
        float y = NativeCalls.EngineGetMouseY(_nativeEngineCorePtr);
        _logger.LogTrace("Mouse Y: {Y}", y);
        return y;
    }
}
