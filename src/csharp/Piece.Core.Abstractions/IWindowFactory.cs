using System;

namespace Piece.Core.Abstractions;

/// <summary>
/// Defines a contract for factories that create and manage native C++ IWindow instances.
/// This interface is central to the Dependency Injection system, allowing the C# host
/// to provide specific windowing backend implementations (e.g., GLFW) to the Piece.Core.
/// </summary>
public interface IWindowFactory : IDisposable
{
    /// <summary>
    /// Gets the raw native C++ factory pointer.
    /// This pointer is used by the Piece.Core to create actual C++ window implementations.
    /// </summary>
    IntPtr GetNativeFactoryPtr();
}