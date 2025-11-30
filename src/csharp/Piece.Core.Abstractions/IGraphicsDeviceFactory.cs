namespace Piece.Core.Abstractions;

/// <summary>
/// Defines a contract for factories that create and manage native C++ IGraphicsDevice instances.
/// This interface is central to the Dependency Injection system, allowing the C# host
/// to provide specific graphics backend implementations (e.g., Vulkan, DirectX) to the Piece.Core.
/// </summary>
public interface IGraphicsDeviceFactory : IDisposable
{
    /// <summary>
    /// Gets the raw native C++ factory pointer.
    /// This pointer is used by the Piece.Core to create actual C++ graphics device implementations.
    /// </summary>
    IntPtr NativeFactoryPointer { get; }

    /// <summary>
    /// Sets the native C++ factory pointer within the C++ Service Locator.
    /// This method is called via P/Invoke to inject the concrete C++ factory
    
    /// into the Piece.Core's Dependency Injection system.
    /// </summary>
    /// <param name="factoryPointer">The raw pointer to the C++ factory instance.</param>
    /// <param name="options">A pointer to C-compatible configuration options for the factory.</param>
    void SetNativeFactoryInServiceLocator(IntPtr factoryPointer, IntPtr options);
}