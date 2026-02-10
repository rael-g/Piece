using System.Diagnostics;
using System.Runtime.InteropServices;

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
    public void NativeLibrariesAreCopiedToOutputAndNativeBuildTargetRuns(string configuration)
    {
        string solutionDir = GetSolutionDirectory();
        string pieceGlfwProjectPath = Path.Combine(solutionDir, "src", "csharp", "Piece.Glfw");
        string pieceCoreProjectPath = Path.Combine(solutionDir, "src", "csharp", "Piece.Core");
        string nativeBuildTargetProjectPath = Path.Combine(solutionDir, "tests", "csharp", "TestTargets", "NativeBuildTarget");
        string targetFramework = "net9.0";

        string outputDir = Path.Combine(nativeBuildTargetProjectPath, "bin", configuration, targetFramework);

        string[] expectedNativeLibrariesBaseNames = new[]
        {
            "wal_glfw"
        };

        RunDotNetCommand(pieceCoreProjectPath, "clean", $"-c {configuration}");
        RunDotNetCommand(pieceGlfwProjectPath, "clean", $"-c {configuration}");
        RunDotNetCommand(nativeBuildTargetProjectPath, "clean", $"-c {configuration}");

        RunDotNetCommand(pieceCoreProjectPath, "build", $"-c {configuration}");
        RunDotNetCommand(pieceGlfwProjectPath, "build", $"-c {configuration}");
        RunDotNetCommand(nativeBuildTargetProjectPath, "build", $"-c {configuration}");
        
        // Determine RID
        string runtimeIdentifier = "";
        if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
        {
            runtimeIdentifier = "win-x64";
        }
        else if (RuntimeInformation.IsOSPlatform(OSPlatform.Linux))
        {
            runtimeIdentifier = "linux-x64";
        }
        else if (RuntimeInformation.IsOSPlatform(OSPlatform.OSX))
        {
            runtimeIdentifier = "osx-x64";
        }
        else
        {
            throw new PlatformNotSupportedException("Unsupported operating system for native library detection in test.");
        }

        // Construct the expected path within the runtimes folder
        string runtimesNativeDir = Path.Combine(outputDir, "runtimes", runtimeIdentifier, "native");

        // 5. Verify native libraries
        foreach (string baseName in expectedNativeLibrariesBaseNames)
        {
            string nativeFileName = GetNativeLibraryFileName(baseName, configuration);
            string nativeFilePath = Path.Combine(runtimesNativeDir, nativeFileName);
            Assert.True(File.Exists(nativeFilePath), $"Expected native library '{nativeFileName}' not found in '{runtimesNativeDir}' for {configuration} build.");
        }
        
        // 6. Run the NativeBuildTarget executable to ensure it loads and exits cleanly, implying no 'DLL not found' errors
        string executableName = Path.Combine(outputDir, "NativeBuildTarget" + (RuntimeInformation.IsOSPlatform(OSPlatform.Windows) ? ".exe" : ""));
        Assert.True(File.Exists(executableName), $"Executable '{executableName}' not found for {configuration} build.");

        var runProcessStartInfo = new ProcessStartInfo(executableName)
        {
            WorkingDirectory = outputDir,
            RedirectStandardOutput = true,
            RedirectStandardError = true,
            UseShellExecute = false,
            CreateNoWindow = true,
        };

        using (var process = Process.Start(runProcessStartInfo))
        {
            bool exited = process?.WaitForExit(60000) ?? false;
            string processOutput = process?.StandardOutput.ReadToEnd();
            string processError = process?.StandardError.ReadToEnd();

            Assert.True(exited, $"NativeBuildTarget process did not exit within 60 seconds for {configuration} build. Output: {processOutput}. Error: {processError}");
            Assert.True(process?.ExitCode == 0, $"NativeBuildTarget exited with non-zero code {process?.ExitCode} for {configuration} build. Output: {processOutput}. Error: {processError}");
        }
    }

    [Theory]
    [InlineData("Debug")]
    [InlineData("Release")]
    public void MissingNativeLibraryTargetFailsAtRuntime(string configuration)
    {
        string solutionDir = GetSolutionDirectory();
        string pieceGlfwProjectPath = Path.Combine(solutionDir, "src", "csharp", "Piece.Glfw");
        string missingNativeLibTargetProjectPath = Path.Combine(solutionDir, "tests", "csharp", "TestTargets", "MissingNativeLibTarget");
        string targetFramework = "net9.0"; // Consistent for all projects

        // Determine the expected output directory for MissingNativeLibTarget
        string outputDir = Path.Combine(missingNativeLibTargetProjectPath, "bin", configuration, targetFramework);

        // Determine RID
        string runtimeIdentifier = "";
        if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
        {
            runtimeIdentifier = "win-x64";
        }
        else if (RuntimeInformation.IsOSPlatform(OSPlatform.Linux))
        {
            runtimeIdentifier = "linux-x64";
        }
        else if (RuntimeInformation.IsOSPlatform(OSPlatform.OSX))
        {
            runtimeIdentifier = "osx-x64";
        }
        else
        {
            throw new PlatformNotSupportedException("Unsupported operating system for native library detection in test.");
        }

        // Construct the expected path within the runtimes folder where the native DLL should be
        string runtimesNativeDir = Path.Combine(outputDir, "runtimes", runtimeIdentifier, "native");
        string nativeFileName = GetNativeLibraryFileName("wal_glfw", configuration);
        string nativeFilePathInRuntimes = Path.Combine(runtimesNativeDir, nativeFileName);
        string nativeFilePathInRoot = Path.Combine(outputDir, nativeFileName); // Also check root, as NuGet may flatten

        // 1. Clean and Build MissingNativeLibTarget to ensure it generates all output, including the native DLL
        RunDotNetCommand(missingNativeLibTargetProjectPath, "clean", $"-c {configuration}");
        RunDotNetCommand(pieceGlfwProjectPath, "build", $"-c {configuration}"); // Build Piece.Glfw first as it's a dependency
        RunDotNetCommand(missingNativeLibTargetProjectPath, "build", $"-c {configuration}");

        // Ensure native library is present after build (sanity check)
        Assert.True(File.Exists(nativeFilePathInRuntimes) || File.Exists(nativeFilePathInRoot), $"Pre-test sanity check failed: Native library '{nativeFileName}' not found after building MissingNativeLibTarget.");


        // 2. DELIBERATELY DELETE the native library to simulate a missing dependency
        if (File.Exists(nativeFilePathInRuntimes))
        {
            File.Delete(nativeFilePathInRuntimes);
        }
        // If NuGet flattened, it might also be in the root, delete it there too
        if (File.Exists(nativeFilePathInRoot))
        {
            File.Delete(nativeFilePathInRoot);
        }
        
        Assert.False(File.Exists(nativeFilePathInRuntimes) || File.Exists(nativeFilePathInRoot), $"Pre-test sanity check failed: Native library '{nativeFileName}' was not deleted successfully.");


        // 3. Run MissingNativeLibTarget executable and assert it FAILS to load the DLL
        string executableName = Path.Combine(outputDir, "MissingNativeLibTarget" + (RuntimeInformation.IsOSPlatform(OSPlatform.Windows) ? ".exe" : ""));
        Assert.True(File.Exists(executableName), $"Executable '{executableName}' not found for {configuration} build.");

        var runProcessStartInfo = new ProcessStartInfo(executableName)
        {
            WorkingDirectory = outputDir, 
            RedirectStandardOutput = true,
            RedirectStandardError = true,
            UseShellExecute = false,
            CreateNoWindow = true,
        };

        using (var process = Process.Start(runProcessStartInfo))
        {
            bool exited = process?.WaitForExit(60000) ?? false;
            string processOutput = process?.StandardOutput.ReadToEnd();
            string processError = process?.StandardError.ReadToEnd();

            Assert.True(exited, $"MissingNativeLibTarget process did not exit within 60 seconds for {configuration} build. Output: {processOutput}. Error: {processError}");
            // Assert that the process exited with a non-zero code (indicating failure to load DLL)
            Assert.True(process?.ExitCode != 0, $"MissingNativeLibTarget exited with code {process?.ExitCode} (expected non-zero) for {configuration} build. Output: {processOutput}. Error: {processError}");
            // Optionally, check for specific error messages related to DLL loading failure
            Assert.Contains("wal_glfw", processOutput + processError, StringComparison.OrdinalIgnoreCase);
        }
    }
}