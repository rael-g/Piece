using System;
using System.Runtime.InteropServices;

namespace Piece.OpenGL;

[StructLayout(LayoutKind.Sequential)]
public struct NativeGraphicsOptions
{
    public uint enable_debug_layers;
    public int max_frames_in_flight;
}

internal static partial class OpenGLPInvoke
{
    private const string NativeLib = "ral_opengl";

    [LibraryImport(NativeLib, EntryPoint = "CreateOpenGLGraphicsDeviceFactory")]
    [UnmanagedCallConv(CallConvs = new[] { typeof(System.Runtime.CompilerServices.CallConvCdecl) })]
    public static partial IntPtr CreateFactory(IntPtr windowPtr, in NativeGraphicsOptions options);

    [LibraryImport(NativeLib, EntryPoint = "DestroyOpenGLGraphicsDeviceFactory")]
    [UnmanagedCallConv(CallConvs = new[] { typeof(System.Runtime.CompilerServices.CallConvCdecl) })]
    public static partial void DestroyFactory(IntPtr factoryPtr);
}
