using System;
using System.Runtime.InteropServices;
using Piece.Core.Interop;
using Serilog;

namespace Piece.Core;

public class Engine : IDisposable
{
    private IntPtr _nativeEngineCorePtr;
    private bool _disposed;
    private NativeCalls.CppLogCallback _logCallbackDelegate = null!; // Keep delegate alive

    public Engine()
    {
        _nativeEngineCorePtr = IntPtr.Zero;
        _disposed = false;
    }

    public void Initialize()
    {
        if (_nativeEngineCorePtr != IntPtr.Zero)
        {
            Log.Warning("Engine already initialized.");
            return;
        }

        // Register the C++ log callback
        _logCallbackDelegate = (level, messagePtr) =>
        {
            NativeCalls.ProcessCppLog(level, messagePtr);
        };
        // Ensure the delegate is not garbage collected
        GCHandle.Alloc(_logCallbackDelegate); 
        NativeCalls.RegisterLogCallback(_logCallbackDelegate);

        _nativeEngineCorePtr = NativeCalls.Engine_Initialize();

        if (_nativeEngineCorePtr == IntPtr.Zero)
        {
            throw new InvalidOperationException("Failed to initialize native EngineCore.");
        }

        Log.Information("Native EngineCore initialized successfully.");
    }

    public void Update(float deltaTime)
    {
        ThrowIfDisposed();
        if (_nativeEngineCorePtr != IntPtr.Zero)
        {
            NativeCalls.Engine_Update(_nativeEngineCorePtr, deltaTime);
        }
    }

    public void Render()
    {
        ThrowIfDisposed();
        if (_nativeEngineCorePtr != IntPtr.Zero)
        {
            NativeCalls.Engine_Render(_nativeEngineCorePtr);
        }
    }

    protected virtual void Dispose(bool disposing)
    {
        if (!_disposed)
        {
            if (_nativeEngineCorePtr != IntPtr.Zero)
            {
                NativeCalls.Engine_Destroy(_nativeEngineCorePtr);
                _nativeEngineCorePtr = IntPtr.Zero;
                Log.Information("Native EngineCore destroyed.");
            }
            _disposed = true;
        }
    }

    public void Dispose()
    {
        Dispose(true);
        GC.SuppressFinalize(this);
    }

    private void ThrowIfDisposed()
    {
        if (_disposed)
        {
            throw new ObjectDisposedException(nameof(Engine));
        }
    }
}

