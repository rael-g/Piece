using System;
using System.Runtime.InteropServices;
using Microsoft.Extensions.Options;
using Piece.Core.Abstractions;
using Piece.Core.Abstractions.NativeCalls; // For NativeGraphicsOptions

namespace Piece.OpenGL;

public class OpenGLGraphicsDeviceFactory : IGraphicsDeviceFactory
{
    private IntPtr _nativeFactoryPtr;
    private IntPtr _optionsPtr = IntPtr.Zero;

    public OpenGLGraphicsDeviceFactory(IOptions<OpenGLOptions> openGLOptions)
    {
        var options = openGLOptions.Value;

        // Marshal C# options to C++ NativeGraphicsOptions struct
        _optionsPtr = Marshal.AllocHGlobal(Marshal.SizeOf<NativeGraphicsOptions>());
        var nativeOptions = new NativeGraphicsOptions
        {
            enable_debug_layers = options.EnableValidationLayers ? 1u : 0u, // Convert bool to uint
            max_frames_in_flight = options.MaxFramesInFlight
        };

        Marshal.StructureToPtr(nativeOptions, _optionsPtr, false);

        // For simplicity, passing a null IWindow* as the first parameter for now.
        // In the future, the C# wrapper for IWindow will be passed here.
        _nativeFactoryPtr = OpenGLPInvoke.CreateFactory(IntPtr.Zero, _optionsPtr);

        if (_nativeFactoryPtr == IntPtr.Zero)
        {
            throw new InvalidOperationException("Failed to create native OpenGL graphics device factory.");
        }
    }

    public IntPtr GetNativeFactoryPtr()
    {
        return _nativeFactoryPtr;
    }

    public void Dispose()
    {
        if (_nativeFactoryPtr != IntPtr.Zero)
        {
            OpenGLPInvoke.DestroyFactory(_nativeFactoryPtr);
            _nativeFactoryPtr = IntPtr.Zero;
        }

        // Free allocated unmanaged memory
        if (_optionsPtr != IntPtr.Zero)
        {
            Marshal.FreeHGlobal(_optionsPtr);
            _optionsPtr = IntPtr.Zero;
        }

        GC.SuppressFinalize(this);
    }

    ~OpenGLGraphicsDeviceFactory()
    {
        Dispose();
    }
}
