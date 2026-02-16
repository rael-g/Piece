using System.Threading.Tasks;
using System.IO;
using System;
using System.Diagnostics;
using System.Collections.Generic;

namespace Piece.ProjectManagement;

public class ProjectManager : IProjectManager
{
    // Placeholder for actual asset and scene services
    private readonly IProjectAssetService _assetService = new ProjectAssetService(); // To be properly injected
    private readonly IProjectSceneService _sceneService = new ProjectSceneService(); // To be properly injected

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
        
        Directory.CreateDirectory(projectRootPath);

        // 2. Leverage dotnet CLI for scaffolding (simplified for now)
        // This would ideally involve discovering and applying specific project templates
        // For now, it's a basic directory structure setup.
        Console.WriteLine($"Scaffolding project '{name}' at '{projectRootPath}' using template '{templateName}'...");
        // Example: Create a 'Content' directory
        Directory.CreateDirectory(System.IO.Path.Combine(projectRootPath, "Content"));
        // Example: Create a 'Scripts' directory
        Directory.CreateDirectory(System.IO.Path.Combine(projectRootPath, "Scripts"));

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
