using System;
using System.Runtime.InteropServices;

namespace Piece.MinimalPal;

internal static partial class MinimalPalPInvoke
{
    private const string NativeLib = "pal_minimal"; // Name of the native minimal PAL DLL

    [LibraryImport(NativeLib, EntryPoint = "CreateMinimalPhysicsWorldFactory")]
    [UnmanagedCallConv(CallConvs = new[] { typeof(System.Runtime.CompilerServices.CallConvCdecl) })]
    public static partial IntPtr CreateFactory(IntPtr optionsPtr);

    [LibraryImport(NativeLib, EntryPoint = "DestroyMinimalPhysicsWorldFactory")]
    [UnmanagedCallConv(CallConvs = new[] { typeof(System.Runtime.CompilerServices.CallConvCdecl) })]
    public static partial void DestroyFactory(IntPtr factoryPtr);
}
