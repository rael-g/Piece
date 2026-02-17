using System;

namespace Piece.Core.Abstractions;

/// <summary>
/// Defines a contract for factories that create and manage native C++ IPhysicsWorld instances.
/// This interface is central to the Dependency Injection system, allowing the C# host
/// to provide specific physics backend implementations (e.g., Box2D, Jolt Physics) to the Piece.Core.
/// </summary>
public interface IPhysicsWorldFactory : IDisposable
{
    /// <summary>
    /// Gets the raw native C++ factory pointer.
    /// This pointer is used by the Piece.Core to create actual C++ physics world implementations.
    /// </summary>
    IntPtr GetNativeFactoryPtr();
}