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
        Assert.NotNull(project.EngineConfig);
        Assert.Empty(project.Name);
        Assert.Empty(project.Path);
        Assert.Empty(project.DefaultScene);
        Assert.Empty(project.EngineConfig.Keys); // TomlTable starts empty
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
        project.EngineConfig.Add("rendering", new Tomlyn.Model.TomlTable { { "vsync", true } });

        project.Save();

        var filePath = Path.Combine(_testProjectPath, "piece_project.toml");
        Assert.True(File.Exists(filePath));

        var content = await File.ReadAllTextAsync(filePath);
        Assert.Contains("Name = \"TestProject\"", content);
        Assert.Contains("Path = \"", content); // Path will be system-dependent temp path
        Assert.Contains("DefaultScene = \"Scene01.toml\"", content);
        Assert.Contains("[rendering]", content);
        Assert.Contains("vsync = true", content);
    }

    [Fact]
    public async Task Load_ReadsTomlFileCorrectly()
    {
        var filePath = Path.Combine(_testProjectPath, "piece_project.toml");
        var tomlContent = "Name = \"LoadedProject\"\n" +
                          "Path = \"/fake/path\"\n" +
                          "DefaultScene = \"StartScene.toml\"\n" +
                          "\n" +
                          "[rendering]\n" +
                          "fullscreen = true\n";
        await File.WriteAllTextAsync(filePath, tomlContent);

        var loadedProject = PieceProject.Load(_testProjectPath);

        Assert.NotNull(loadedProject);
        Assert.Equal("LoadedProject", loadedProject!.Name);
        Assert.Equal(_testProjectPath, loadedProject.Path); // Path should be overwritten by Load method
        Assert.Equal("StartScene.toml", loadedProject.DefaultScene);
        Assert.True(loadedProject.EngineConfig.ContainsKey("rendering"));
        Assert.True(loadedProject.EngineConfig["rendering"] is Tomlyn.Model.TomlTable renderingTable);
        Assert.Equal(true, renderingTable["fullscreen"]);
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
