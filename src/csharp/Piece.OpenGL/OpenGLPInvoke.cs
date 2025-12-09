using System;
using System.Runtime.InteropServices;

namespace Piece.OpenGL;

internal static partial class OpenGLPInvoke
{
    [LibraryImport("ral_opengl_backend", EntryPoint = "CreateOpenGLGraphicsDeviceFactory")]
    [UnmanagedCallConv(CallConvs = new[] { typeof(System.Runtime.CompilerServices.CallConvCdecl) })]
    public static partial IntPtr CreateFactory();
}
