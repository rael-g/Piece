using System.Runtime.InteropServices;

namespace Piece.Box2d;

internal static class Box2dPInvoke
{
    private const string NativeLib = "pal_box2d_backend";

    [DllImport(NativeLib, EntryPoint = "CreateBox2DPhysicsWorldFactory", CallingConvention = CallingConvention.Cdecl)]
    internal static extern IntPtr CreateFactory();
}
