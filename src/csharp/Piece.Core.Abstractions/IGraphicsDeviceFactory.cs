using System;

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
    IntPtr GetNativeFactoryPtr();
}