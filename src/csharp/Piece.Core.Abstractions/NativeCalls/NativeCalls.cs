using System.Runtime.InteropServices;
using System;

namespace Piece.Core.Abstractions.NativeCalls;

public static class NativeCalls
{
    // C# mirror of C++ LogLevel enum
    public enum LogLevel : int
    {
        kTrace = 0,
        kDebug,
        kInfo,
        kWarning,
        kError,
        kFatal
    }

    // Function pointer type for log callbacks
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void LogCallback(int level, string message);

    // DllImport for core engine functions
    [DllImport("PieceCore.dll")]
    public static extern IntPtr Engine_Initialize();

    [DllImport("PieceCore.dll")]
    public static extern void Engine_Destroy(IntPtr corePtr);

    [DllImport("PieceCore.dll")]
    public static extern void Engine_Update(IntPtr corePtr, float deltaTime);

    [DllImport("PieceCore.dll")]
    public static extern void Engine_Render(IntPtr corePtr);

    // Factory setting functions
    [DllImport("PieceCore.dll")]
    public static extern void PieceCore_SetGraphicsDeviceFactory(IntPtr factoryPtr);

    [DllImport("PieceCore.dll")]
    public static extern void PieceCore_SetWindowFactory(IntPtr factoryPtr);

    [DllImport("PieceCore.dll")]
    public static extern void PieceCore_SetPhysicsWorldFactory(IntPtr factoryPtr);

    // Logging functions
    [DllImport("PieceCore.dll")]
    public static extern void PieceCoreRegisterLogCallback(LogCallback callback);

    [DllImport("PieceCore.dll")]
    public static extern void PieceCoreLog(int level, string message); // This one might not be directly called from C#, but is for completeness
}

// C# mirror of C++ NativeGraphicsOptions struct
[StructLayout(LayoutKind.Sequential)]
public struct NativeGraphicsOptions
{
    public uint enable_debug_layers; // Matching C++ uint32_t
    public int max_frames_in_flight;
}

// C# mirror of C++ NativeWindowOptions struct
[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
public struct NativeWindowOptions
{
    public int initial_window_width;
    public int initial_window_height;
    public uint window_flags;
    public IntPtr window_title; // Marshaled as const char*
}

// C# mirror of C++ NativePhysicsOptions struct
[StructLayout(LayoutKind.Sequential)]
public struct NativePhysicsOptions
{
    public float fixed_delta_time;
    public uint max_physics_steps; // Matching C++ uint32_t
}
