using System.Threading.Tasks;
using System.IO;
using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.Linq; // Added for LINQ operations
using Tomlyn; // Added for Tomlyn.TomlTable
using Microsoft.Extensions.Logging; // Added for logging

namespace Piece.ProjectManagement;

public class ProjectManager : IProjectManager
{
    private readonly IProjectAssetService _assetService;
    private readonly IProjectSceneService _sceneService;
    private readonly IProjectBuildService _buildService;
    private readonly ILogger<ProjectManager> _logger;

    public ProjectManager(
        IProjectAssetService assetService,
        IProjectSceneService sceneService,
        IProjectBuildService buildService,
        ILogger<ProjectManager> logger)
    {
        _assetService = assetService;
        _sceneService = sceneService;
        _buildService = buildService;
        _logger = logger;
    }

    /// <summary>
    /// Lists available project templates.
    /// </summary>
    /// <returns>A collection of ProjectTemplate objects.</returns>
    public IEnumerable<ProjectTemplate> ListTemplates()
    {
        _logger.LogInformation("Listing available project templates.");
        // For now, return a hardcoded list. In a real scenario, this would involve
        // parsing 'dotnet new --list' output or discovering custom templates.
        return new List<ProjectTemplate>
        {
            new ProjectTemplate { Name = "Piece Engine Empty Project", ShortName = "piece-empty", Description = "An empty Piece Engine project.", IsPieceEngineTemplate = true },
            new ProjectTemplate { Name = "Piece Engine 2D Game", ShortName = "piece-2d-game", Description = "A basic 2D game project for Piece Engine.", IsPieceEngineTemplate = true },
            new ProjectTemplate { Name = "Piece Engine 3D Game", ShortName = "piece-3d-game", Description = "A basic 3D game project for Piece Engine.", IsPieceEngineTemplate = true },
            new ProjectTemplate { Name = "Console Application", ShortName = "console", Description = "A project for creating a command-line application." },
            new ProjectTemplate { Name = "Class Library", ShortName = "classlib", Description = "A project for creating a class library." }
        };
    }

    public async Task<PieceProject> CreateProject(string name, string path, string templateName)
    {
        // 1. Validate inputs
        if (string.IsNullOrWhiteSpace(name))
            throw new ArgumentException("Project name cannot be empty.", nameof(name));
        if (string.IsNullOrWhiteSpace(path))
            throw new ArgumentException("Project path cannot be empty.", nameof(path));
        
        // Ensure path is absolute and create directory if it doesn't exist
        var projectRootPath = System.IO.Path.Combine(path, name);
        if (Directory.Exists(projectRootPath))
        {
            _logger.LogError("Project directory already exists at {ProjectRootPath}.", projectRootPath);
            throw new InvalidOperationException($"Project directory already exists at {projectRootPath}.");
        }
        
        // Find the selected template
        var template = ListTemplates().FirstOrDefault(t => t.ShortName.Equals(templateName, StringComparison.OrdinalIgnoreCase));
        if (template == null)
        {
            _logger.LogError("Template '{TemplateName}' not found.", templateName);
            throw new ArgumentException($"Template '{templateName}' not found.", nameof(templateName));
        }

        _logger.LogInformation("Scaffolding project '{ProjectName}' at '{ProjectRootPath}' using template '{TemplateName}'...", name, projectRootPath, templateName);
        
        // 2. Leverage dotnet CLI for scaffolding
        if (template.IsPieceEngineTemplate)
        {
            _logger.LogInformation("Applying custom Piece Engine template for '{TemplateName}'.", templateName);
            Directory.CreateDirectory(projectRootPath);
            Directory.CreateDirectory(System.IO.Path.Combine(projectRootPath, "Assets"));
            Directory.CreateDirectory(System.IO.Path.Combine(projectRootPath, "Scripts"));
            // ... more specific Piece Engine structure like default scenes, config files etc.
            // This part can be expanded to copy files from an embedded resource template.
        }
        else
        {
            _logger.LogInformation("Using dotnet new for standard template '{TemplateName}'.", templateName);
            var startInfo = new ProcessStartInfo("dotnet", $"new {template.ShortName} -n {name} -o \"{projectRootPath}\"")
            {
                RedirectStandardOutput = true,
                RedirectStandardError = true,
                UseShellExecute = false,
                CreateNoWindow = true,
            };

            using (var process = Process.Start(startInfo))
            {
                if (process == null)
                {
                    _logger.LogError("Failed to start dotnet process for template '{TemplateName}'.", templateName);
                    throw new InvalidOperationException("Failed to start dotnet process.");
                }
                
                string output = await process.StandardOutput.ReadToEndAsync();
                string error = await process.StandardError.ReadToEndAsync();
                await process.WaitForExitAsync();

                if (process.ExitCode != 0)
                {
                    _logger.LogError("dotnet new failed with exit code {ExitCode}. Output: {Output}, Error: {Error}", process.ExitCode, output, error);
                    throw new InvalidOperationException($"dotnet new failed with exit code {process.ExitCode}.");
                }
                _logger.LogInformation("dotnet new output: {Output}", output);
            }
        }
        
        // 3. Create PieceProject instance and save its configuration
        var project = new PieceProject
        {
            Name = name,
            Path = projectRootPath,
            DefaultScene = "MainScene.toml", // Default scene name
            // Initialize new Rendering and Window config properties
            Rendering = new PieceProject.RenderingConfig { VSync = true, Fullscreen = false },
            Window = new PieceProject.WindowConfig { Width = 1280, Height = 720, Title = name }
        };
        project.Save(); // Save piece_project.toml

        _logger.LogInformation("Project '{ProjectName}' created successfully at {ProjectRootPath}.", name, projectRootPath);
        return project;
    }

    public async Task<PieceProject?> LoadProject(string projectFilePath)
    {
        if (string.IsNullOrWhiteSpace(projectFilePath))
            throw new ArgumentException("Project file path cannot be empty.", nameof(projectFilePath));

        // Assuming projectFilePath is the path to the piece_project.toml itself
        var projectRootPath = System.IO.Path.GetDirectoryName(projectFilePath);
        if (projectRootPath == null || !Directory.Exists(projectRootPath))
        {
            _logger.LogError("Invalid project path or directory not found: {ProjectRootPath}.", projectRootPath);
            return null;
        }

        var project = PieceProject.Load(projectRootPath);
        if (project == null)
        {
            _logger.LogError("Failed to load project configuration from {ProjectRootPath}.", projectRootPath);
            return null;
        }
        _logger.LogInformation("Project '{ProjectName}' loaded successfully from {ProjectRootPath}.", project.Name, project.Path);
        return project;
    }

    public async Task SaveProject(PieceProject project)
    {
        if (project == null)
            throw new ArgumentNullException(nameof(project));
        if (string.IsNullOrWhiteSpace(project.Path))
        {
            _logger.LogError("Project path is not set for project '{ProjectName}', cannot save.", project.Name);
            throw new InvalidOperationException("Project path is not set, cannot save.");
        }

        project.Save();
        _logger.LogInformation("Project '{ProjectName}' saved successfully to {ProjectRootPath}.", project.Name, project.Path);
    }

    public IProjectAssetService GetAssetService(PieceProject project)
    {
        return _assetService; // Will be properly injected later
    }

    public IProjectSceneService GetSceneService(PieceProject project)
    {
        return _sceneService; // Will be properly injected later
    }

    public async Task<bool> AddModule(PieceProject project, string moduleName)
    {
        if (project == null) throw new ArgumentNullException(nameof(project));
        if (string.IsNullOrWhiteSpace(moduleName))
            throw new ArgumentException("Module name (NuGet package ID) cannot be empty.", nameof(moduleName));
        if (string.IsNullOrWhiteSpace(project.Path))
        {
            _logger.LogError("Project path is not set for project '{ProjectName}', cannot add module.", project.Name);
            throw new InvalidOperationException("Project path is not set, cannot add module.");
        }

        _logger.LogInformation("Attempting to add module '{ModuleName}' to project '{ProjectName}'.", moduleName, project.Name);

        // Find the main .csproj file for the project
        var projectCsprojFile = Directory.GetFiles(project.Path, "*.csproj").FirstOrDefault();
        if (projectCsprojFile == null)
        {
            _logger.LogError("No .csproj file found in project path '{ProjectPath}'. Cannot add module.", project.Path);
            return false;
        }

        var startInfo = new ProcessStartInfo("dotnet", $"add \"{projectCsprojFile}\" package {moduleName}")
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
                _logger.LogError("Failed to start dotnet process to add module '{ModuleName}'.", moduleName);
                return false;
            }

            string output = await process.StandardOutput.ReadToEndAsync();
            string error = await process.StandardError.ReadToEndAsync();
            await process.WaitForExitAsync();

            _logger.LogInformation("dotnet add package output: {Output}", output);
            if (!string.IsNullOrWhiteSpace(error))
            {
                _logger.LogError("dotnet add package error: {Error}", error);
            }

            if (process.ExitCode != 0)
            {
                _logger.LogError("dotnet add package failed with exit code {ExitCode} for module '{ModuleName}'.", process.ExitCode, moduleName);
                return false;
            }
            
            _logger.LogInformation("Module '{ModuleName}' added successfully to project '{ProjectName}'.", moduleName, project.Name);
            return true;
        }
    }

    public async Task<bool> RemoveModule(PieceProject project, string moduleName)
    {
        if (project == null) throw new ArgumentNullException(nameof(project));
        if (string.IsNullOrWhiteSpace(moduleName))
            throw new ArgumentException("Module name (NuGet package ID) cannot be empty.", nameof(moduleName));
        if (string.IsNullOrWhiteSpace(project.Path))
        {
            _logger.LogError("Project path is not set for project '{ProjectName}', cannot remove module.", project.Name);
            throw new InvalidOperationException("Project path is not set, cannot remove module.");
        }

        _logger.LogInformation("Attempting to remove module '{ModuleName}' from project '{ProjectName}'.", moduleName, project.Name);

        // Find the main .csproj file for the project
        var projectCsprojFile = Directory.GetFiles(project.Path, "*.csproj").FirstOrDefault();
        if (projectCsprojFile == null)
        {
            _logger.LogError("No .csproj file found in project path '{ProjectPath}'. Cannot remove module.", project.Path);
            return false;
        }

        var startInfo = new ProcessStartInfo("dotnet", $"remove \"{projectCsprojFile}\" package {moduleName}")
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
                _logger.LogError("Failed to start dotnet process to remove module '{ModuleName}'.", moduleName);
                return false;
            }

            string output = await process.StandardOutput.ReadToEndAsync();
            string error = await process.StandardError.ReadToEndAsync();
            await process.WaitForExitAsync();

            _logger.LogInformation("dotnet remove package output: {Output}", output);
            if (!string.IsNullOrWhiteSpace(error))
            {
                _logger.LogError("dotnet remove package error: {Error}", error);
            }

            if (process.ExitCode != 0)
            {
                _logger.LogError("dotnet remove package failed with exit code {ExitCode} for module '{ModuleName}'.", process.ExitCode, moduleName);
                return false;
            }
            
            _logger.LogInformation("Module '{ModuleName}' removed successfully from project '{ProjectName}'.", moduleName, project.Name);
            return true;
        }
    }
}


