using System;
using System.Runtime.InteropServices;

namespace Piece.Glfw;

[StructLayout(LayoutKind.Sequential)]
public struct NativeWindowOptions
{
    public int initial_window_width;
    public int initial_window_height;
    public uint window_flags;
    public IntPtr window_title;}

internal static partial class GlfwPInvoke
{
    private const string NativeLib = "wal_glfw";

    [LibraryImport(NativeLib, EntryPoint = "CreateGlfwWindowFactory")]
    [UnmanagedCallConv(CallConvs = new[] { typeof(System.Runtime.CompilerServices.CallConvCdecl) })]
    public static partial IntPtr CreateFactory(NativeWindowOptions options);

    [LibraryImport(NativeLib, EntryPoint = "DestroyGlfwWindowFactory")]
    [UnmanagedCallConv(CallConvs = new[] { typeof(System.Runtime.CompilerServices.CallConvCdecl) })]
    public static partial void DestroyFactory(IntPtr factoryPtr);
}
