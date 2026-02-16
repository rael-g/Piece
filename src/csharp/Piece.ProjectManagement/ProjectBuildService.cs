using System.Diagnostics;
using System.IO;
using System.Threading.Tasks;
using System;

namespace Piece.ProjectManagement;

public class ProjectBuildService : IProjectBuildService
{
    public async Task<bool> BuildProject(PieceProject project, string configuration)
    {
        if (project == null) throw new ArgumentNullException(nameof(project));
        if (string.IsNullOrWhiteSpace(configuration))
            throw new ArgumentException("Configuration cannot be empty.", nameof(configuration));

        Console.WriteLine($"Building project '{project.Name}' ({configuration})...");

        // Assuming the main project file is directly in the project root for now,
        // or we need a way to discover it (e.g., .sln or .csproj file).
        // For simplicity, let's assume project.Path is the directory containing the .csproj or .sln
        var projectFile = Directory.GetFiles(project.Path, "*.csproj").FirstOrDefault();
        if (projectFile == null)
        {
            projectFile = Directory.GetFiles(project.Path, "*.sln").FirstOrDefault();
        }

        if (projectFile == null)
        {
            Console.WriteLine($"Error: No .csproj or .sln file found in project path '{project.Path}'.");
            return false;
        }

        var startInfo = new ProcessStartInfo("dotnet", $"build \"{projectFile}\" -c {configuration}")
        {
            RedirectStandardOutput = true,
            RedirectStandardError = true,
            UseShellExecute = false,
            CreateNoWindow = true,
            WorkingDirectory = project.Path // Ensure dotnet command runs in the project's root
        };

        using (var process = Process.Start(startInfo))
        {
            if (process == null)
            {
                Console.WriteLine("Error: Failed to start dotnet build process.");
                return false;
            }

            string output = await process.StandardOutput.ReadToEndAsync();
            string error = await process.StandardError.ReadToEndAsync();
            await process.WaitForExitAsync();

            Console.WriteLine(output);
            if (!string.IsNullOrWhiteSpace(error))
            {
                Console.WriteLine($"dotnet build error: {error}");
            }

            if (process.ExitCode != 0)
            {
                Console.WriteLine($"dotnet build failed with exit code {process.ExitCode}.");
                return false;
            }
            
            Console.WriteLine($"Project '{project.Name}' built successfully ({configuration}).");
            return true;
        }
    }

    public async Task<bool> CleanProject(PieceProject project, string configuration)
    {
        if (project == null) throw new ArgumentNullException(nameof(project));
        if (string.IsNullOrWhiteSpace(configuration))
            throw new ArgumentException("Configuration cannot be empty.", nameof(configuration));

        Console.WriteLine($"Cleaning project '{project.Name}' ({configuration})...");

        var projectFile = Directory.GetFiles(project.Path, "*.csproj").FirstOrDefault();
        if (projectFile == null)
        {
            projectFile = Directory.GetFiles(project.Path, "*.sln").FirstOrDefault();
        }

        if (projectFile == null)
        {
            Console.WriteLine($"Error: No .csproj or .sln file found in project path '{project.Path}'.");
            return false;
        }

        var startInfo = new ProcessStartInfo("dotnet", $"clean \"{projectFile}\" -c {configuration}")
        {
            RedirectStandardOutput = true,
            RedirectStandardError = true,
            UseShellExecute = false,
            CreateNoWindow = true,
            WorkingDirectory = project.Path
        };

        using (var process = Process.Start(startInfo))
        {
            if (process == null)
            {
                Console.WriteLine("Error: Failed to start dotnet clean process.");
                return false;
            }

            string output = await process.StandardOutput.ReadToEndAsync();
            string error = await process.StandardError.ReadToEndAsync();
            await process.WaitForExitAsync();

            Console.WriteLine(output);
            if (!string.IsNullOrWhiteSpace(error))
            {
                Console.WriteLine($"dotnet clean error: {error}");
            }

            if (process.ExitCode != 0)
            {
                Console.WriteLine($"dotnet clean failed with exit code {process.ExitCode}.");
                return false;
            }
            
            Console.WriteLine($"Project '{project.Name}' cleaned successfully ({configuration}).");
            return true;
        }
    }
}
