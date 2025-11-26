using System;
using System.Runtime.InteropServices;

namespace Piece.Intermediate.Abstractions
{
    // This interface is a placeholder to represent the concept of a factory
    // for a native C++ IGraphicsDevice. It would typically be implemented
    // by specific backend wrappers (e.g., Piece.OpenGL, Piece.Vulkan).
    public interface IGraphicsDeviceFactory : IDisposable
    {
        // This IntPtr would represent the raw C++ factory pointer obtained via P/Invoke.
        IntPtr NativeFactoryPointer { get; }

        // Placeholder for a method that might be exposed via P/Invoke
        // to set this factory in the C++ Service Locator.
        // The actual P/Invoke methods would be in a dedicated interop class.
        void SetNativeFactoryInServiceLocator(IntPtr factoryPointer, IntPtr options);
    }
}
