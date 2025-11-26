using System;
using System.Runtime.InteropServices;
using Serilog;
using Serilog.Events;

namespace Piece.Core.Interop
{
    public static class NativeCalls
    {
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void CppLogCallback(int level, IntPtr messagePtr);

        [DllImport("piece_intermediate.dll", EntryPoint = "PieceIntermediate_RegisterLogCallback", CallingConvention = CallingConvention.Cdecl)]
        public static extern void RegisterLogCallback(CppLogCallback callback);

        [DllImport("piece_intermediate.dll", EntryPoint = "PieceIntermediate_Log", CallingConvention = CallingConvention.Cdecl)]
        public static extern void PieceIntermediate_Log(int level, IntPtr messagePtr);

        [AllowReversePInvokeCalls]
        public static void ProcessCppLog(int level, IntPtr messagePtr)
        {
            try
            {
                string message = Marshal.PtrToStringAnsi(messagePtr);
                LogEventLevel serilogLevel = MapLogLevel(level);
                Log.Write(serilogLevel, "[C++] {Message}", message);
            }
            catch (Exception ex)
            {
                Log.Error(ex, "Error processing C++ log message.");
            }
        }

        private static LogEventLevel MapLogLevel(int cppLevel)
        {
            return (Piece.Core.Interop.LogLevel)cppLevel switch
            {
                Piece.Core.Interop.LogLevel.Trace => LogEventLevel.Verbose,
                Piece.Core.Interop.LogLevel.Debug => LogEventLevel.Debug,
                Piece.Core.Interop.LogLevel.Info => LogEventLevel.Information,
                Piece.Core.Interop.LogLevel.Warning => LogEventLevel.Warning,
                Piece.Core.Interop.LogLevel.Error => LogEventLevel.Error,
                Piece.Core.Interop.LogLevel.Fatal => LogEventLevel.Fatal,
                _ => LogEventLevel.Information,
            };
        }

        [DllImport("piece_intermediate.dll", EntryPoint = "PieceIntermediate_SetGraphicsDeviceFactory", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetGraphicsDeviceFactory(IntPtr factoryPtr, IntPtr optionsPtr);

        [DllImport("piece_intermediate.dll", EntryPoint = "PieceIntermediate_SetWindowFactory", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetWindowFactory(IntPtr factoryPtr, IntPtr optionsPtr);

        [DllImport("piece_intermediate.dll", EntryPoint = "Engine_Initialize", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr Engine_Initialize();

        [DllImport("piece_intermediate.dll", EntryPoint = "Engine_Destroy", CallingConvention = CallingConvention.Cdecl)]
        public static extern void Engine_Destroy(IntPtr engineCorePtr);
    }
}