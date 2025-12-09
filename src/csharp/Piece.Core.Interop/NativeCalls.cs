using System.Runtime.InteropServices;

using Serilog;
using Serilog.Events;

namespace Piece.Core.Interop;

/// <summary>
/// Contains the P/Invoke definitions for calling the native C++ core library.
/// </summary>
public static partial class NativeCalls
{
    // C++ Log Callback
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void CppLogCallback(int level, IntPtr messagePtr);

    [LibraryImport("piece_core.dll", EntryPoint = "PieceCore_RegisterLogCallback")] // Changed to piece_core.dll
    [UnmanagedCallConv(CallConvs = [typeof(System.Runtime.CompilerServices.CallConvCdecl)])]
    public static partial void RegisterLogCallback(CppLogCallback callback);

    [LibraryImport("piece_core.dll", EntryPoint = "PieceCore_Log")] // Changed to piece_core.dll
    [UnmanagedCallConv(CallConvs = [typeof(System.Runtime.CompilerServices.CallConvCdecl)])]
    public static partial void PieceCoreLog(int level, IntPtr messagePtr); // Changed name to avoid conflict and align with PieceCore_

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

    // New API for setting factories
    [LibraryImport("piece_core.dll", EntryPoint = "SetGraphicsDeviceFactory")] // Changed to piece_core.dll, new entry point
    [UnmanagedCallConv(CallConvs = [typeof(System.Runtime.CompilerServices.CallConvCdecl)])]
    public static partial void SetGraphicsDeviceFactory(IntPtr factoryPtr);

    [LibraryImport("piece_core.dll", EntryPoint = "SetWindowFactory")] // Changed to piece_core.dll, new entry point
    [UnmanagedCallConv(CallConvs = [typeof(System.Runtime.CompilerServices.CallConvCdecl)])]
    public static partial void SetWindowFactory(IntPtr factoryPtr);

    [LibraryImport("piece_core.dll", EntryPoint = "SetPhysicsWorldFactory")] // Changed to piece_core.dll, new entry point
    [UnmanagedCallConv(CallConvs = [typeof(System.Runtime.CompilerServices.CallConvCdecl)])]
    public static partial void SetPhysicsWorldFactory(IntPtr factoryPtr);

    // Engine Lifecycle
    [LibraryImport("piece_core.dll", EntryPoint = "Engine_Initialize")] // Changed to piece_core.dll
    [UnmanagedCallConv(CallConvs = [typeof(System.Runtime.CompilerServices.CallConvCdecl)])]
    public static partial IntPtr Engine_Initialize();

    [LibraryImport("piece_core.dll", EntryPoint = "Engine_Destroy")] // Changed to piece_core.dll
    [UnmanagedCallConv(CallConvs = [typeof(System.Runtime.CompilerServices.CallConvCdecl)])]
    public static partial void Engine_Destroy(IntPtr engineCorePtr);

    [LibraryImport("piece_core.dll", EntryPoint = "Engine_Update")] // Added
    [UnmanagedCallConv(CallConvs = [typeof(System.Runtime.CompilerServices.CallConvCdecl)])]
    public static partial void Engine_Update(IntPtr engineCorePtr, float deltaTime); // Added

    [LibraryImport("piece_core.dll", EntryPoint = "Engine_Render")] // Added
    [UnmanagedCallConv(CallConvs = [typeof(System.Runtime.CompilerServices.CallConvCdecl)])]
    public static partial void Engine_Render(IntPtr engineCorePtr); // Added
}
