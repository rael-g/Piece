using System;
using System.IO;
using System.Diagnostics;
using System.Linq; // For Any()
using System.Runtime.InteropServices;
using System.Threading.Tasks; // For Task.Run
using Xunit; // For Xunit.Assert and Xunit.Theory

namespace Piece.Tests;

public class BuildSystemIntegrationTests
{
    private static string GetSolutionDirectory()
    {
        // Navigate up until we find the .git directory or .sln file
        string currentDirectory = AppContext.BaseDirectory;
        while (currentDirectory != null && !Directory.EnumerateFileSystemEntries(currentDirectory, "*.sln").Any() && !Directory.Exists(Path.Combine(currentDirectory, ".git")))
        {
            currentDirectory = Directory.GetParent(currentDirectory)?.FullName;
        }
        if (currentDirectory == null)
        {
            throw new InvalidOperationException("Could not find solution root directory.");
        }
        return currentDirectory;
    }

    private static string GetNativeLibraryFileName(string baseName, string configuration)
    {
        string prefix = "";
        string suffix = "";

        if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
        {
            suffix = ".dll";
        }
        else if (RuntimeInformation.IsOSPlatform(OSPlatform.Linux))
        {
            prefix = "lib";
            suffix = ".so";
        }
        else if (RuntimeInformation.IsOSPlatform(OSPlatform.OSX))
        {
            prefix = "lib";
            suffix = ".dylib";
        }
        else
        {
            throw new PlatformNotSupportedException("Unsupported operating system for native library detection.");
        }

        return $"{prefix}{baseName}{suffix}";
    }

    // Improved RunDotNetCommand with timeout, asynchronous output reading, and more robust killing
    private static void RunDotNetCommand(string workingDirectory, string command, string args, int timeoutSeconds = 600) // 10 minutes default for builds
    {
        var processStartInfo = new ProcessStartInfo("dotnet", $"{command} {args}")
        {
            WorkingDirectory = workingDirectory,
            RedirectStandardOutput = true,
            RedirectStandardError = true,
            UseShellExecute = false,
            CreateNoWindow = true,
        };

        using (var process = Process.Start(processStartInfo))
        {
            if (process == null)
            {
                throw new InvalidOperationException($"Failed to start dotnet {command} {args}.");
            }

            // Start reading the output and error streams asynchronously to prevent deadlocks
            Task<string> outputReader = Task.Run(() => process.StandardOutput.ReadToEnd());
            Task<string> errorReader = Task.Run(() => process.StandardError.ReadToEnd());

            bool exited = process.WaitForExit(timeoutSeconds * 1000); // WaitForExit takes milliseconds

            // Wait for output readers to complete or timeout
            Task.WaitAll(new Task[] { outputReader, errorReader }, 5000); // Give them a few seconds to finish reading after process exit

            string output = outputReader.IsCompleted ? outputReader.Result : "[Output stream not fully read due to timeout]";
            string error = errorReader.IsCompleted ? errorReader.Result : "[Error stream not fully read due to timeout]";

            if (!exited)
            {
                // Attempt to kill the process and its children
                try
                {
                    // Kill process and its descendants (available in .NET 6+)
                    // Note: process.Kill() without 'true' parameter sometimes only kills the parent process.
                    // For dotnet, this is typically enough, but a more robust solution might use platform-specific tools like taskkill.
                    // For simplicity and cross-platform compatibility, we rely on process.Kill() which often kills process trees.
                    process.Kill(true); 
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"Warning: Could not kill hanging process {process.Id} and its descendants: {ex.Message}");
                }
                throw new TimeoutException($"dotnet {command} {args} timed out after {timeoutSeconds} seconds.\nOutput: {output}\nError: {error}");
            }

            if (process.ExitCode != 0)
            {
                throw new InvalidOperationException($"dotnet {command} {args} failed with exit code {process.ExitCode}.\nOutput: {output}\nError: {error}");
            }
        }
    }

    [Theory]
    [InlineData("Debug")]
    [InlineData("Release")]
    public void NativeLibrariesAreCopiedToOutputAndNativeBuildTargetRuns(string configuration) // Reverted name
    {
        string solutionDir = GetSolutionDirectory();
        string pieceGlfwProjectPath = Path.Combine(solutionDir, "src", "csharp", "Piece.Glfw");
        string pieceCoreProjectPath = Path.Combine(solutionDir, "src", "csharp", "Piece.Core"); // Need to build Piece.Core as it's a dependency
        string nativeBuildTargetProjectPath = Path.Combine(solutionDir, "tests", "csharp", "TestTargets", "NativeBuildTarget");
        string targetFramework = "net9.0"; // Consistent for all projects

        // Determine the expected output directory for NativeBuildTarget
        string outputDir = Path.Combine(nativeBuildTargetProjectPath, "bin", configuration, targetFramework);

        // Define expected native libraries (only for Piece.Glfw)
        string[] expectedNativeLibrariesBaseNames = new[]
        {
            "wal_glfw", // From Piece.Glfw
            "glfw3"     // External dependency of Piece.Glfw
        };

        // 1. Clean all relevant projects explicitly for both configurations
        RunDotNetCommand(pieceCoreProjectPath, "clean", $"-c {configuration}");
        RunDotNetCommand(pieceGlfwProjectPath, "clean", $"-c {configuration}");
        RunDotNetCommand(nativeBuildTargetProjectPath, "clean", $"-c {configuration}");


        // 2. Build Piece.Core first (as it's a dependency for Glfw)
        RunDotNetCommand(pieceCoreProjectPath, "build", $"-c {configuration}");

        // 3. Build Piece.Glfw (which relies on Piece.Core). This triggers the native build for wal_glfw.
        RunDotNetCommand(pieceGlfwProjectPath, "build", $"-c {configuration}");

        // 4. Build NativeBuildTarget. This will then copy from Piece.Glfw's output (due to standard propagation).
        RunDotNetCommand(nativeBuildTargetProjectPath, "build", $"-c {configuration}");

        // 5. Verify native libraries
        foreach (string baseName in expectedNativeLibrariesBaseNames)
        {
            string nativeFileName = GetNativeLibraryFileName(baseName, configuration);
            string nativeFilePath = Path.Combine(outputDir, nativeFileName);
            Assert.True(File.Exists(nativeFilePath), $"Expected native library '{nativeFileName}' not found in '{outputDir}' for {configuration} build.");
        }
        
        // 6. Run the NativeBuildTarget executable to ensure it loads and exits cleanly, implying no 'DLL not found' errors
        string executableName = Path.Combine(outputDir, "NativeBuildTarget" + (RuntimeInformation.IsOSPlatform(OSPlatform.Windows) ? ".exe" : ""));
        Assert.True(File.Exists(executableName), $"Executable '{executableName}' not found for {configuration} build.");

        var runProcessStartInfo = new ProcessStartInfo(executableName)
        {
            WorkingDirectory = outputDir, // Important for loading local DLLs
            RedirectStandardOutput = true,
            RedirectStandardError = true,
            UseShellExecute = false,
            CreateNoWindow = true,
        };

        using (var process = Process.Start(runProcessStartInfo))
        {
            bool exited = process?.WaitForExit(60000) ?? false; // Wait for max 60 seconds
            string processOutput = process?.StandardOutput.ReadToEnd();
            string processError = process?.StandardError.ReadToEnd();

            Assert.True(exited, $"NativeBuildTarget process did not exit within 60 seconds for {configuration} build. Output: {processOutput}. Error: {processError}");
            Assert.True(process?.ExitCode == 0, $"NativeBuildTarget exited with non-zero code {process?.ExitCode} for {configuration} build. Output: {processOutput}. Error: {processError}");
        }
    }
}