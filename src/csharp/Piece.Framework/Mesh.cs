using System;

namespace Piece.Framework;

/// <summary>
/// Represents a mesh resource in the C# framework.
/// This is a placeholder for a future C# wrapper around a C++ Mesh object.
/// </summary>
public class Mesh : IDisposable
{
    public string Name { get; private set; }
    public IntPtr NativePtr { get; private set; } // Pointer to the native C++ Mesh object

    public Mesh(string name, IntPtr nativePtr)
    {
        Name = name;
        NativePtr = nativePtr;
    }

    // Default constructor for placeholder usage
    public Mesh() : this("Default Mesh", IntPtr.Zero) { }

    // IDisposable implementation
    private bool _disposed = false;

    protected virtual void Dispose(bool disposing)
    {
        if (!_disposed)
        {
            if (disposing)
            {
                // Dispose managed state (managed objects)
            }

            // TODO: Release unmanaged resources (native C++ Mesh object)
            // For now, just log the release. Actual release will happen via P/Invoke.
            if (NativePtr != IntPtr.Zero)
            {
                // Example: NativeCalls.EngineReleaseMesh(NativePtr);
                // Console.WriteLine($"[Mesh] Disposing native Mesh: {Name} ({NativePtr})");
                // NativePtr = IntPtr.Zero; // Prevent double disposal
            }
            _disposed = true;
        }
    }

    public void Dispose()
    {
        Dispose(true);
        GC.SuppressFinalize(this);
    }

    ~Mesh()
    {
        Dispose(false);
    }
}