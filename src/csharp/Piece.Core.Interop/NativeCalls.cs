using System.Runtime.InteropServices;

using Serilog;
using Serilog.Events;

namespace Piece.Core.Interop;

/// <summary>
/// Contains the P/Invoke definitions for calling the native C++ core library.
/// </summary>
public static partial class NativeCalls
{
    /// <summary>
    /// Delegate for the callback function that processes log messages from C++.
    /// </summary>
    /// <param name="level">The log level.</param>
    /// <param name="messagePtr">A pointer to the log message string.</param>
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void CppLogCallback(int level, IntPtr messagePtr);

    /// <summary>
    /// Registers a log callback function with the native C++ library.
    /// </summary>
    /// <param name="callback">The callback function to register.</param>
    [LibraryImport("piece_intermediate.dll", EntryPoint = "PieceIntermediate_RegisterLogCallback")]
    [UnmanagedCallConv(CallConvs = [typeof(System.Runtime.CompilerServices.CallConvCdecl)])]
    public static partial void RegisterLogCallback(CppLogCallback callback);

    /// <summary>
    /// Logs a message from the native side.
    /// </summary>
    /// <param name="level">The log level.</param>
    /// <param name="messagePtr">A pointer to the log message string.</param>
    [LibraryImport("piece_intermediate.dll", EntryPoint = "PieceIntermediate_Log")]
    [UnmanagedCallConv(CallConvs = [typeof(System.Runtime.CompilerServices.CallConvCdecl)])]
    public static partial void PieceIntermediateLog(int level, IntPtr messagePtr);

    /// <summary>
    /// Processes a log message received from the C++ core.
    /// </summary>
    /// <param name="level">The integer log level from C++.</param>
    /// <param name="messagePtr">A pointer to the UTF-8 encoded log message.</param>
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

    /// <summary>
    /// Maps a C++ integer log level to a Serilog LogEventLevel.
    /// </summary>
    /// <param name="cppLevel">The C++ integer log level.</param>
    /// <returns>The corresponding Serilog LogEventLevel.</returns>
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

    /// <summary>
    /// Sets the graphics device factory in the native service locator.
    /// </summary>
    /// <param name="factoryPtr">A pointer to the graphics device factory.</param>
    /// <param name="optionsPtr">A pointer to the graphics options.</param>
    [LibraryImport("piece_intermediate.dll", EntryPoint = "PieceIntermediate_SetGraphicsDeviceFactory")]
    [UnmanagedCallConv(CallConvs = [typeof(System.Runtime.CompilerServices.CallConvCdecl)])]
    public static partial void SetGraphicsDeviceFactory(IntPtr factoryPtr, IntPtr optionsPtr);

    /// <summary>
    /// Sets the window factory in the native service locator.
    /// </summary>
    /// <param name="factoryPtr">A pointer to the window factory.</param>
    /// <param name="optionsPtr">A pointer to the window options.</param>
    [LibraryImport("piece_intermediate.dll", EntryPoint = "PieceIntermediate_SetWindowFactory")]
    [UnmanagedCallConv(CallConvs = [typeof(System.Runtime.CompilerServices.CallConvCdecl)])]
    public static partial void SetWindowFactory(IntPtr factoryPtr, IntPtr optionsPtr);

    /// <summary>
    /// Initializes the native engine core.
    /// </summary>
    /// <returns>A pointer to the created EngineCore instance.</returns>
    [LibraryImport("piece_intermediate.dll", EntryPoint = "Engine_Initialize")]
    [UnmanagedCallConv(CallConvs = [typeof(System.Runtime.CompilerServices.CallConvCdecl)])]
    public static partial IntPtr Engine_Initialize();

    /// <summary>
    /// Destroys the native engine core.
    /// </summary>
    /// <param name="engineCorePtr">A pointer to the EngineCore instance to destroy.</param>
    [LibraryImport("piece_intermediate.dll", EntryPoint = "Engine_Destroy")]
    [UnmanagedCallConv(CallConvs = [typeof(System.Runtime.CompilerServices.CallConvCdecl)])]
    public static partial void Engine_Destroy(IntPtr engineCorePtr);
}