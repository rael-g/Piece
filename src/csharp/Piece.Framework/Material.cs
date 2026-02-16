using System;

namespace Piece.Framework;

/// <summary>
/// Represents a material resource in the C# framework.
/// This is a placeholder for a future C# wrapper around a C++ Material object.
/// </summary>
public class Material : IDisposable
{
    public string Name { get; private set; }
    public IntPtr NativePtr { get; private set; } // Pointer to the native C++ Material object
    // TODO: Add properties for shader, textures, and uniform parameters

    public Material(string name, IntPtr nativePtr)
    {
        Name = name;
        NativePtr = nativePtr;
    }

    // Default constructor for placeholder usage
    public Material() : this("Default Material", IntPtr.Zero) { }

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

            // TODO: Release unmanaged resources (native C++ Material object)
            // For now, just log the release. Actual release will happen via P/Invoke.
            if (NativePtr != IntPtr.Zero)
            {
                // Example: NativeCalls.EngineReleaseMaterial(NativePtr);
                // Console.WriteLine($"[Material] Disposing native Material: {Name} ({NativePtr})");
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

    ~Material()
    {
        Dispose(false);
    }
}
