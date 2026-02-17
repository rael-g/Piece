using System;
using System.Diagnostics;
using System.IO;
using System.Linq; // For FirstOrDefault
using System.Threading.Tasks;

using Microsoft.Extensions.Logging; // Added for logging

namespace Piece.ProjectManagement;

public class ProjectBuildService : IProjectBuildService
{
    private readonly ILogger<ProjectBuildService> _logger;

    public ProjectBuildService(ILogger<ProjectBuildService> logger)
    {
        _logger = logger;
    }

    public async Task<bool> BuildProject(PieceProject project, string configuration)
    {
        if (project == null) throw new ArgumentNullException(nameof(project));
        if (string.IsNullOrWhiteSpace(configuration))
            throw new ArgumentException("Configuration cannot be empty.", nameof(configuration));

        _logger.LogInformation("Building project '{ProjectName}' ({Configuration})...", project.Name, configuration);

        var projectFile = Directory.GetFiles(project.Path, "*.csproj").FirstOrDefault();
        if (projectFile == null)
        {
            projectFile = Directory.GetFiles(project.Path, "*.sln").FirstOrDefault();
        }

        if (projectFile == null)
        {
            _logger.LogError("No .csproj or .sln file found in project path '{ProjectPath}'.", project.Path);
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
                _logger.LogError("Failed to start dotnet build process for project '{ProjectName}'.", project.Name);
                return false;
            }

            string output = await process.StandardOutput.ReadToEndAsync();
            string error = await process.StandardError.ReadToEndAsync();
            await process.WaitForExitAsync();

            _logger.LogInformation("dotnet build output: {Output}", output);
            if (!string.IsNullOrWhiteSpace(error))
            {
                _logger.LogError("dotnet build error: {Error}", error);
            }

            if (process.ExitCode != 0)
            {
                _logger.LogError("dotnet build failed with exit code {ExitCode} for project '{ProjectName}'.", process.ExitCode, project.Name);
                return false;
            }

            _logger.LogInformation("Project '{ProjectName}' built successfully ({Configuration}).", project.Name, configuration);
            return true;
        }
    }

    public async Task<bool> CleanProject(PieceProject project, string configuration)
    {
        if (project == null) throw new ArgumentNullException(nameof(project));
        if (string.IsNullOrWhiteSpace(configuration))
            throw new ArgumentException("Configuration cannot be empty.", nameof(configuration));

        _logger.LogInformation("Cleaning project '{ProjectName}' ({Configuration})...", project.Name, configuration);

        var projectFile = Directory.GetFiles(project.Path, "*.csproj").FirstOrDefault();
        if (projectFile == null)
        {
            projectFile = Directory.GetFiles(project.Path, "*.sln").FirstOrDefault();
        }

        if (projectFile == null)
        {
            _logger.LogError("No .csproj or .sln file found in project path '{ProjectPath}'.", project.Path);
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
                _logger.LogError("Failed to start dotnet clean process for project '{ProjectName}'.", project.Name);
                return false;
            }

            string output = await process.StandardOutput.ReadToEndAsync();
            string error = await process.StandardError.ReadToEndAsync();
            await process.WaitForExitAsync();

            _logger.LogInformation("dotnet clean output: {Output}", output);
            if (!string.IsNullOrWhiteSpace(error))
            {
                _logger.LogError("dotnet clean error: {Error}", error);
            }

            if (process.ExitCode != 0)
            {
                _logger.LogError("dotnet clean failed with exit code {ExitCode} for project '{ProjectName}'.", process.ExitCode, project.Name);
                return false;
            }

            _logger.LogInformation("Project '{ProjectName}' cleaned successfully ({Configuration}).", project.Name, configuration);
            return true;
        }
    }
}