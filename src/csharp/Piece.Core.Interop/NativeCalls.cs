using System.Runtime.InteropServices;

using Serilog;
using Serilog.Events;

namespace Piece.Core.Interop;

public static partial class NativeCalls
{
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void CppLogCallback(int level, IntPtr messagePtr);

    [LibraryImport("piece_intermediate.dll", EntryPoint = "PieceIntermediate_RegisterLogCallback")]
    [UnmanagedCallConv(CallConvs = [typeof(System.Runtime.CompilerServices.CallConvCdecl)])]
    public static partial void RegisterLogCallback(CppLogCallback callback);

    [LibraryImport("piece_intermediate.dll", EntryPoint = "PieceIntermediate_Log")]
    [UnmanagedCallConv(CallConvs = [typeof(System.Runtime.CompilerServices.CallConvCdecl)])]
    public static partial void PieceIntermediateLog(int level, IntPtr messagePtr);

    public static void ProcessCppLog(int level, IntPtr messagePtr)
    {
        try
        {
            var message = Marshal.PtrToStringAnsi(messagePtr);
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
        return (LogLevel)cppLevel switch
        {
            LogLevel.Trace => LogEventLevel.Verbose,
            LogLevel.Debug => LogEventLevel.Debug,
            LogLevel.Info => LogEventLevel.Information,
            LogLevel.Warning => LogEventLevel.Warning,
            LogLevel.Error => LogEventLevel.Error,
            LogLevel.Fatal => LogEventLevel.Fatal,
            _ => LogEventLevel.Information,
        };
    }

    [LibraryImport("piece_intermediate.dll", EntryPoint = "PieceIntermediate_SetGraphicsDeviceFactory")]
    [UnmanagedCallConv(CallConvs = [typeof(System.Runtime.CompilerServices.CallConvCdecl)])]
    public static partial void SetGraphicsDeviceFactory(IntPtr factoryPtr, IntPtr optionsPtr);

    [LibraryImport("piece_intermediate.dll", EntryPoint = "PieceIntermediate_SetWindowFactory")]
    [UnmanagedCallConv(CallConvs = [typeof(System.Runtime.CompilerServices.CallConvCdecl)])]
    public static partial void SetWindowFactory(IntPtr factoryPtr, IntPtr optionsPtr);

    [LibraryImport("piece_intermediate.dll", EntryPoint = "Engine_Initialize")]
    [UnmanagedCallConv(CallConvs = [typeof(System.Runtime.CompilerServices.CallConvCdecl)])]
    public static partial IntPtr Engine_Initialize();

    [LibraryImport("piece_intermediate.dll", EntryPoint = "Engine_Destroy")]
    [UnmanagedCallConv(CallConvs = [typeof(System.Runtime.CompilerServices.CallConvCdecl)])]
    public static partial void Engine_Destroy(IntPtr engineCorePtr);
}