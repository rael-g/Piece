using Xunit;
using Moq;
using System.IO;
using System.Threading.Tasks;
using System;
using System.Collections.Generic;

namespace Piece.ProjectManagement.Tests;

public class PieceProjectTests : IDisposable
{
    private readonly string _testProjectPath;

    public PieceProjectTests()
    {
        _testProjectPath = Path.Combine(Path.GetTempPath(), Guid.NewGuid().ToString());
        Directory.CreateDirectory(_testProjectPath);
    }

    public void Dispose()
    {
        if (Directory.Exists(_testProjectPath))
        {
            Directory.Delete(_testProjectPath, true);
        }
    }

    [Fact]
    public void Constructor_InitializesPropertiesCorrectly()
    {
        var project = new PieceProject();
        Assert.NotNull(project.Name);
        Assert.NotNull(project.Path);
        Assert.NotNull(project.DefaultScene);
        Assert.Equal("MainScene.toml", project.DefaultScene); // Assert default value
        Assert.NotNull(project.Rendering);
        Assert.NotNull(project.Window);
        Assert.Empty(project.Name);
        Assert.Empty(project.Path);
    }

    [Fact]
    public void Properties_CanBeSetAndGet()
    {
        var project = new PieceProject
        {
            Name = "MyGame",
            Path = "/games/mygame",
            DefaultScene = "Level1.toml"
        };

        Assert.Equal("MyGame", project.Name);
        Assert.Equal("/games/mygame", project.Path);
        Assert.Equal("Level1.toml", project.DefaultScene);
    }

    [Fact]
    public async Task Save_CreatesTomlFileWithCorrectContent()
    {
        var project = new PieceProject
        {
            Name = "TestProject",
            Path = _testProjectPath,
            DefaultScene = "Scene01.toml"
        };
        project.Rendering.VSync = true;
        project.Window.Width = 1920;
        project.Window.Height = 1080;

        project.Save();

        var filePath = Path.Combine(_testProjectPath, "piece_project.toml");
        Assert.True(File.Exists(filePath));

        var content = await File.ReadAllTextAsync(filePath);
        Assert.Contains("Name = \"TestProject\"", content);
        Assert.Contains("Path = \"", content); // Path will be system-dependent temp path
        Assert.Contains("DefaultScene = \"Scene01.toml\"", content);
        Assert.Contains("[Rendering]", content);
        Assert.Contains("VSync = true", content);
        Assert.Contains("[Window]", content);
        Assert.Contains("Width = 1920", content);
        Assert.Contains("Height = 1080", content);
    }

    [Fact]
    public async Task Load_ReadsTomlFileCorrectly()
    {
        var filePath = Path.Combine(_testProjectPath, "piece_project.toml");
        var tomlContent = "Name = \"LoadedProject\"\n" +
                          "Path = \"/fake/path\"\n" +
                          "DefaultScene = \"StartScene.toml\"\n" +
                          "\n" +
                          "[Rendering]\n" +
                          "VSync = true\n" +
                          "\n" +
                          "[Window]\n" +
                          "Width = 800\n" +
                          "Height = 600\n";
        await File.WriteAllTextAsync(filePath, tomlContent);

        var loadedProject = PieceProject.Load(_testProjectPath);

        Assert.NotNull(loadedProject);
        Assert.Equal("LoadedProject", loadedProject!.Name);
        Assert.Equal(_testProjectPath, loadedProject.Path); // Path should be overwritten by Load method
        Assert.Equal("StartScene.toml", loadedProject.DefaultScene);
        Assert.True(loadedProject.Rendering.VSync);
        Assert.Equal(800, loadedProject.Window.Width);
        Assert.Equal(600, loadedProject.Window.Height);
    }

    [Fact]
    public void Load_ReturnsNullIfFileDoesNotExist()
    {
        var loadedProject = PieceProject.Load(Path.Combine(_testProjectPath, "nonexistent"));
        Assert.Null(loadedProject);
    }

    [Fact]
    public async Task Load_ReturnsNullIfFileIsInvalidToml()
    {
        var filePath = Path.Combine(_testProjectPath, "piece_project.toml");
        await File.WriteAllTextAsync(filePath, "This is not valid TOML [section");

        var loadedProject = PieceProject.Load(_testProjectPath);
        Assert.Null(loadedProject);
    }
}
