using Piece.Core;

using System.Runtime.InteropServices; // For Marshal.StringToHGlobalAnsi

namespace Piece.Tests;

public class LogInteropTests
{
    // Helper to call NativeCalls.PieceCoreLog with string conversion
    private void CallPieceCoreLog(LogLevel level, string message)
    {
        IntPtr messagePtr = IntPtr.Zero;
        try
        {
            messagePtr = Marshal.StringToHGlobalAnsi(message);
            NativeCalls.PieceCoreLog((int)level, messagePtr);
        }
        finally
        {
            if (messagePtr != IntPtr.Zero)
            {
                Marshal.FreeHGlobal(messagePtr);
            }
        }
    }

    [Fact]
    public void PieceCoreLog_CallsWithoutCrashing_TraceLevel()
    {
        // Arrange
        var level = LogLevel.Trace;
        var message = "Test trace message from C# interop.";

        // Act & Assert
        try
        {
            CallPieceCoreLog(level, message);
        }
        catch (Exception ex)
        {
            Assert.Fail($"PieceCoreLog call crashed with exception at {level} level: {ex.Message}");
        }
    }

    [Fact]
    public void PieceCoreLog_CallsWithoutCrashing_DebugLevel()
    {
        // Arrange
        var level = LogLevel.Debug;
        var message = "Test debug message from C# interop.";

        // Act & Assert
        try
        {
            CallPieceCoreLog(level, message);
        }
        catch (Exception ex)
        {
            Assert.Fail($"PieceCoreLog call crashed with exception at {level} level: {ex.Message}");
        }
    }

    [Fact]
    public void PieceCoreLog_CallsWithoutCrashing_InfoLevel()
    {
        // Arrange
        var level = LogLevel.Info;
        var message = "Test info message from C# interop.";

        // Act & Assert
        try
        {
            CallPieceCoreLog(level, message);
        }
        catch (Exception ex)
        {
            Assert.Fail($"PieceCoreLog call crashed with exception at {level} level: {ex.Message}");
        }
    }

    [Fact]
    public void PieceCoreLog_CallsWithoutCrashing_WarningLevel()
    {
        // Arrange
        var level = LogLevel.Warning;
        var message = "Test warning message from C# interop.";

        // Act & Assert
        try
        {
            CallPieceCoreLog(level, message);
        }
        catch (Exception ex)
        {
            Assert.Fail($"PieceCoreLog call crashed with exception at {level} level: {ex.Message}");
        }
    }

    [Fact]
    public void PieceCoreLog_CallsWithoutCrashing_ErrorLevel()
    {
        // Arrange
        var level = LogLevel.Error;
        var message = "Test error message from C# interop.";

        // Act & Assert
        try
        {
            CallPieceCoreLog(level, message);
        }
        catch (Exception ex)
        {
            Assert.Fail($"PieceCoreLog call crashed with exception at {level} level: {ex.Message}");
        }
    }

    [Fact]
    public void PieceCoreLog_CallsWithoutCrashing_FatalLevel()
    {
        // Arrange
        var level = LogLevel.Fatal;
        var message = "Test fatal message from C# interop.";

        // Act & Assert
        try
        {
            CallPieceCoreLog(level, message);
        }
        catch (Exception ex)
        {
            Assert.Fail($"PieceCoreLog call crashed with exception at {level} level: {ex.Message}");
        }
    }
}