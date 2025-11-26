using System;
using System.Runtime.InteropServices;

namespace Piece.Core.Interop
{
    /// <summary>
    /// Placeholder for native C++ function calls (P/Invoke).
    /// All unsafe code and direct P/Invoke declarations will reside here
    /// and expose a safe API to higher-level C# frameworks.
    /// </summary>
    public static class NativeCalls
    {
        // Example: Placeholder for setting the GraphicsDeviceFactory in the C++ Service Locator
        // The actual DLL name and function signature will depend on the C++ NativeExports.h
        // and the specific backend being integrated.
        [DllImport("PieceIntermediate.dll", EntryPoint = "PieceIntermediate_SetGraphicsDeviceFactory", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetGraphicsDeviceFactory(IntPtr factoryPtr, IntPtr optionsPtr);

        // Example: Placeholder for setting the WindowFactory
        [DllImport("PieceIntermediate.dll", EntryPoint = "PieceIntermediate_SetWindowFactory", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetWindowFactory(IntPtr factoryPtr, IntPtr optionsPtr);

        // Placeholder for core engine initialization
        [DllImport("PieceIntermediate.dll", EntryPoint = "Engine_Initialize", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr Engine_Initialize();

        // Placeholder for core engine destruction
        [DllImport("PieceIntermediate.dll", EntryPoint = "Engine_Destroy", CallingConvention = CallingConvention.Cdecl)]
        public static extern void Engine_Destroy(IntPtr engineCorePtr);
    }
}
