using System;
using System.Runtime.InteropServices;
using Microsoft.Extensions.Options;
using Piece.Core.Abstractions;
using Piece.Core.Abstractions.NativeCalls; // For NativeWindowOptions

namespace Piece.Glfw;

public class GlfwWindowFactory : IWindowFactory
{
    private IntPtr _nativeFactoryPtr;
    private IntPtr _optionsPtr = IntPtr.Zero;
    private IntPtr _windowTitlePtr = IntPtr.Zero;

    public GlfwWindowFactory(IOptions<GlfwOptions> glfwOptions)
    {
        var options = glfwOptions.Value;

        // Marshal C# options to C++ NativeWindowOptions struct
        _optionsPtr = Marshal.AllocHGlobal(Marshal.SizeOf<NativeWindowOptions>());
        var nativeOptions = new NativeWindowOptions
        {
            initial_window_width = options.InitialWindowWidth,
            initial_window_height = options.InitialWindowHeight,
            window_flags = options.WindowResizable ? 1u : 0u, // Using 1 for resizable, 0 for not
        };

        // Marshal WindowTitle string
        _windowTitlePtr = Marshal.StringToHGlobalAnsi(options.WindowTitle);
        nativeOptions.window_title = _windowTitlePtr;

        Marshal.StructureToPtr(nativeOptions, _optionsPtr, false);

        _nativeFactoryPtr = GlfwPInvoke.CreateFactory(_optionsPtr);

        if (_nativeFactoryPtr == IntPtr.Zero)
        {
            throw new InvalidOperationException("Failed to create native GLFW window factory.");
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
            GlfwPInvoke.DestroyFactory(_nativeFactoryPtr);
            _nativeFactoryPtr = IntPtr.Zero;
        }

        // Free allocated unmanaged memory
        if (_optionsPtr != IntPtr.Zero)
        {
            Marshal.FreeHGlobal(_optionsPtr);
            _optionsPtr = IntPtr.Zero;
        }
        if (_windowTitlePtr != IntPtr.Zero)
        {
            Marshal.FreeHGlobal(_windowTitlePtr);
            _windowTitlePtr = IntPtr.Zero;
        }

        GC.SuppressFinalize(this);
    }

    ~GlfwWindowFactory()
    {
        Dispose();
    }
}
