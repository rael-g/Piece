using System;
using System.Runtime.InteropServices;

namespace Piece.OpenGL;

internal static partial class OpenGLPInvoke
{
    private const string NativeLib = "ral_opengl";

    [LibraryImport(NativeLib, EntryPoint = "CreateOpenGLGraphicsDeviceFactory")]
    [UnmanagedCallConv(CallConvs = new[] { typeof(System.Runtime.CompilerServices.CallConvCdecl) })]
    public static partial IntPtr CreateFactory(IntPtr windowPtr, IntPtr optionsPtr);

    [LibraryImport(NativeLib, EntryPoint = "DestroyOpenGLGraphicsDeviceFactory")]
    [UnmanagedCallConv(CallConvs = new[] { typeof(System.Runtime.CompilerServices.CallConvCdecl) })]
    public static partial void DestroyFactory(IntPtr factoryPtr);
}