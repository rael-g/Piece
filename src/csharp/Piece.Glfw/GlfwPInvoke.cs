using System;
using System.Runtime.InteropServices;

namespace Piece.Glfw;

internal static partial class GlfwPInvoke
{
    [LibraryImport("wal_glfw", EntryPoint = "CreateGlfwWindowFactory")]
    [UnmanagedCallConv(CallConvs = new[] { typeof(System.Runtime.CompilerServices.CallConvCdecl) })]
    public static partial IntPtr CreateFactory();
}
