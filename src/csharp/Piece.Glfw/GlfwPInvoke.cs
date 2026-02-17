using System;
using System.Runtime.InteropServices;

namespace Piece.Glfw;

internal static partial class GlfwPInvoke
{
    private const string NativeLib = "wal_glfw";

    [LibraryImport(NativeLib, EntryPoint = "CreateGlfwWindowFactory")]
    [UnmanagedCallConv(CallConvs = new[] { typeof(System.Runtime.CompilerServices.CallConvCdecl) })]
    public static partial IntPtr CreateFactory(IntPtr optionsPtr);

    [LibraryImport(NativeLib, EntryPoint = "DestroyGlfwWindowFactory")]
    [UnmanagedCallConv(CallConvs = new[] { typeof(System.Runtime.CompilerServices.CallConvCdecl) })]
    public static partial void DestroyFactory(IntPtr factoryPtr);
}