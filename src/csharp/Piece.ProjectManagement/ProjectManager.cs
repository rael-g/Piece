using System.Threading.Tasks;
using System.IO;
using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.Linq; // Added for LINQ operations

namespace Piece.ProjectManagement;

public class ProjectManager : IProjectManager
{
    // Placeholder for actual asset and scene services
    private readonly IProjectAssetService _assetService; // To be properly injected
    private readonly IProjectSceneService _sceneService; // To be properly injected
    private readonly IProjectBuildService _buildService; // To be properly injected

    public ProjectManager(IProjectAssetService assetService, IProjectSceneService sceneService, IProjectBuildService buildService)
    {
        _assetService = assetService;
        _sceneService = sceneService;
        _buildService = buildService;
    }

    /// <summary>
    /// Lists available project templates.
    /// </summary>
    /// <returns>A collection of ProjectTemplate objects.</returns>
    public IEnumerable<ProjectTemplate> ListTemplates()
    {
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
            throw new InvalidOperationException($"Project directory already exists at {projectRootPath}.");
        
        // Find the selected template
        var template = ListTemplates().FirstOrDefault(t => t.ShortName.Equals(templateName, StringComparison.OrdinalIgnoreCase));
        if (template == null)
        {
            throw new ArgumentException($"Template '{templateName}' not found.", nameof(templateName));
        }

        Console.WriteLine($"Scaffolding project '{name}' at '{projectRootPath}' using template '{templateName}'...");
        
        // 2. Leverage dotnet CLI for scaffolding
        if (template.IsPieceEngineTemplate)
        {
            // For custom Piece Engine templates, we create the root directory
            // and add specific Piece Engine project structure later if needed.
            Directory.CreateDirectory(projectRootPath);
            // Example: Create default Assets and Scripts directories
            Directory.CreateDirectory(System.IO.Path.Combine(projectRootPath, "Assets"));
            Directory.CreateDirectory(System.IO.Path.Combine(projectRootPath, "Scripts"));
            // ... more specific Piece Engine structure like default scenes, config files etc.
            // This part can be expanded to copy files from an embedded resource template.
        }
        else
        {
            // Use dotnet new for standard templates
            var startInfo = new ProcessStartInfo("dotnet", $"new {template.ShortName} -n {name} -o \"{projectRootPath}\"")
            {
                RedirectStandardOutput = true,
                RedirectStandardError = true,
                UseShellExecute = false,
                CreateNoWindow = true,
            };

            using (var process = Process.Start(startInfo))
            {
                if (process == null) throw new InvalidOperationException("Failed to start dotnet process.");
                
                string output = await process.StandardOutput.ReadToEndAsync();
                string error = await process.StandardError.ReadToEndAsync();
                await process.WaitForExitAsync();

                if (process.ExitCode != 0)
                {
                    Console.WriteLine($"dotnet new output: {output}");
                    Console.WriteLine($"dotnet new error: {error}");
                    throw new InvalidOperationException($"dotnet new failed with exit code {process.ExitCode}.");
                }
                Console.WriteLine(output);
            }
        }
        
        // 3. Create PieceProject instance and save its configuration
        var project = new PieceProject
        {
            Name = name,
            Path = projectRootPath,
            DefaultScene = "MainScene.toml", // Default scene name
            EngineConfig = new Toml.TomlTable // Initialize with an empty TomlTable
            {
                { "window", new Toml.TomlTable { { "width", 1280 }, { "height", 720 } } },
                { "rendering", new Toml.TomlTable { { "vsync", true } } }
            }
        };
        project.Save(); // Save piece_project.toml

        Console.WriteLine($"Project '{name}' created successfully at {projectRootPath}.");
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
            // Log error: Invalid project path
            return null;
        }

        var project = PieceProject.Load(projectRootPath);
        if (project == null)
        {
            // Log error: Failed to load project configuration
            return null;
        }
        Console.WriteLine($"Project '{project.Name}' loaded successfully from {project.Path}.");
        return project;
    }

    public async Task SaveProject(PieceProject project)
    {
        if (project == null)
            throw new ArgumentNullException(nameof(project));
        if (string.IsNullOrWhiteSpace(project.Path))
            throw new InvalidOperationException("Project path is not set, cannot save.");

        project.Save();
        Console.WriteLine($"Project '{project.Name}' saved successfully to {project.Path}.");
    }

    public IProjectAssetService GetAssetService(PieceProject project)
    {
        return _assetService; // Will be properly injected later
    }

    public IProjectSceneService GetSceneService(PieceProject project)
    {
        return _sceneService; // Will be properly injected later
    }
}
