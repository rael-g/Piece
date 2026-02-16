using Xunit;
using Moq;
using System.IO;
using System.Threading.Tasks;
using System;
using System.Collections.Generic;
using System.Linq;
using Piece.Framework.Abstractions;

namespace Piece.ProjectManagement.Tests;

public class ProjectManagerTests : IDisposable
{
    private readonly string _testWorkspacePath;
    private readonly Mock<IProjectAssetService> _mockAssetService;
    private readonly Mock<IProjectSceneService> _mockSceneService;
    private readonly Mock<IProjectBuildService> _mockBuildService;
    private readonly ProjectManager _projectManager;

    public ProjectManagerTests()
    {
        _testWorkspacePath = Path.Combine(Path.GetTempPath(), "ProjectManagerTests", Guid.NewGuid().ToString());
        Directory.CreateDirectory(_testWorkspacePath);

        _mockAssetService = new Mock<IProjectAssetService>();
        _mockSceneService = new Mock<IProjectSceneService>();
        _mockBuildService = new Mock<IProjectBuildService>();
        
        // Mock default behavior if not explicitly set in a test
        _mockAssetService.Setup(s => s.ListAssets(It.IsAny<PieceProject>())).ReturnsAsync(Enumerable.Empty<string>());
        _mockSceneService.Setup(s => s.OpenScene(It.IsAny<PieceProject>(), It.IsAny<string>())).ReturnsAsync(true);
        _mockBuildService.Setup(s => s.BuildProject(It.IsAny<PieceProject>(), It.IsAny<string>())).ReturnsAsync(true);

        _projectManager = new ProjectManager(
            _mockAssetService.Object,
            _mockSceneService.Object,
            _mockBuildService.Object
        );
    }

    public void Dispose()
    {
        if (Directory.Exists(_testWorkspacePath))
        {
            Directory.Delete(_testWorkspacePath, true);
        }
    }

    [Fact]
    public void Constructor_InitializesServices()
    {
        Assert.NotNull(_projectManager);
        // Additional checks could involve verifying that the injected services are indeed used.
    }

    [Fact]
    public void ListTemplates_ReturnsExpectedTemplates()
    {
        var templates = _projectManager.ListTemplates().ToList();
        Assert.NotNull(templates);
        Assert.NotEmpty(templates);
        Assert.Contains(templates, t => t.ShortName == "piece-empty");
        Assert.Contains(templates, t => t.ShortName == "console");
    }

    [Fact]
    public async Task CreateProject_CreatesDirectoryAndTomlFile()
    {
        var projectName = "MyNewGame";
        var projectPath = _testWorkspacePath;
        var templateName = "piece-empty";

        var project = await _projectManager.CreateProject(projectName, projectPath, templateName);

        Assert.NotNull(project);
        Assert.Equal(projectName, project.Name);
        Assert.Equal(Path.Combine(projectPath, projectName), project.Path);
        Assert.True(Directory.Exists(project.Path));
        Assert.True(File.Exists(Path.Combine(project.Path, "piece_project.toml")));
    }

    [Fact]
    public async Task CreateProject_UsesDotnetNewForStandardTemplates()
    {
        var projectName = "MyConsoleApp";
        var projectPath = _testWorkspacePath;
        var templateName = "console";

        // To test dotnet new, we cannot truly mock Process.Start directly.
        // This test will try to create a real dotnet new project.
        // We'll verify directory and project file existence.
        
        var project = await _projectManager.CreateProject(projectName, projectPath, templateName);

        Assert.NotNull(project);
        Assert.Equal(projectName, project.Name);
        Assert.Equal(Path.Combine(projectPath, projectName), project.Path);
        Assert.True(Directory.Exists(project.Path));
        Assert.True(File.Exists(Path.Combine(project.Path, projectName + ".csproj"))); // Dotnet new creates .csproj
        Assert.True(File.Exists(Path.Combine(project.Path, "piece_project.toml"))); // Our config file
    }

    [Fact]
    public async Task CreateProject_ThrowsOnExistingDirectory()
    {
        var projectName = "ExistingProject";
        var projectPath = Path.Combine(_testWorkspacePath, projectName);
        Directory.CreateDirectory(projectPath); // Create it beforehand

        await Assert.ThrowsAsync<InvalidOperationException>(() =>
            _projectManager.CreateProject(projectName, _testWorkspacePath, "piece-empty"));
    }

    [Fact]
    public async Task CreateProject_ThrowsOnInvalidTemplate()
    {
        await Assert.ThrowsAsync<ArgumentException>(() =>
            _projectManager.CreateProject("InvalidTemplateProject", _testWorkspacePath, "non-existent-template"));
    }

    [Fact]
    public async Task LoadProject_LoadsExistingTomlFile()
    {
        var projectName = "LoadableProject";
        var projectRoot = Path.Combine(_testWorkspacePath, projectName);
        Directory.CreateDirectory(projectRoot);
        var projectFilePath = Path.Combine(projectRoot, "piece_project.toml");

        var content = @"
Name = ""LoadableProject""
Path = ""/should/be/overwritten""
DefaultScene = ""TestScene.toml""
";
        await File.WriteAllTextAsync(projectFilePath, content);

        var loadedProject = await _projectManager.LoadProject(projectFilePath);

        Assert.NotNull(loadedProject);
        Assert.Equal(projectName, loadedProject!.Name);
        Assert.Equal(projectRoot, loadedProject.Path); // Path should be updated to actual root
        Assert.Equal("TestScene.toml", loadedProject.DefaultScene);
    }

    [Fact]
    public async Task LoadProject_ReturnsNullIfFileDoesNotExist()
    {
        var projectFilePath = Path.Combine(_testWorkspacePath, "nonexistent.toml");
        var loadedProject = await _projectManager.LoadProject(projectFilePath);
        Assert.Null(loadedProject);
    }

    [Fact]
    public async Task SaveProject_SavesModifiedProject()
    {
        var projectName = "SavableProject";
        var projectRoot = Path.Combine(_testWorkspacePath, projectName);
        Directory.CreateDirectory(projectRoot);

        var project = new PieceProject
        {
            Name = projectName,
            Path = projectRoot,
            DefaultScene = "OriginalScene.toml"
        };
        // Initial save to create the file
        project.Save();

        // Modify and save again
        project.DefaultScene = "NewScene.toml";
        await _projectManager.SaveProject(project);

        // Load to verify changes
        var loadedProject = PieceProject.Load(projectRoot);
        Assert.NotNull(loadedProject);
        Assert.Equal("NewScene.toml", loadedProject!.DefaultScene);
    }

    [Fact]
    public void GetAssetService_ReturnsInjectedService()
    {
        var project = new PieceProject { Name = "Test", Path = _testWorkspacePath };
        var service = _projectManager.GetAssetService(project);
        Assert.Equal(_mockAssetService.Object, service);
    }

    [Fact]
    public void GetSceneService_ReturnsInjectedService()
    {
        var project = new PieceProject { Name = "Test", Path = _testWorkspacePath };
        var service = _projectManager.GetSceneService(project);
        Assert.Equal(_mockSceneService.Object, service);
    }
}
