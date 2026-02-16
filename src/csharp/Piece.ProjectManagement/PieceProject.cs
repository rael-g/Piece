using System.Collections.Generic;
using System.IO;
using Tomlyn;
using Tomlyn.Model;
using Tomlyn.Parsing; // For TomlModelOptions

namespace Piece.ProjectManagement;

public class PieceProject
{
    private const string FILENAME = "piece_project.toml";

    public string Name { get; set; } = string.Empty;
    public string Path { get; set; } = string.Empty; // Path to the project root directory
    public string DefaultScene { get; set; } = string.Empty;

    // Use a TomlTable for flexible engine configurations
    public TomlTable EngineConfig { get; set; } = new TomlTable();

    // Custom TomlModelOptions to preserve PascalCase
    private static readonly TomlModelOptions TomlOptions = new TomlModelOptions
    {
        ConvertPropertyName = name => name // Keep property names as-is (PascalCase)
    };

    /// <summary>
    /// Loads a PieceProject from a specified project path.
    /// </summary>
    /// <param name="projectPath">The root directory of the project.</param>
    /// <returns>A PieceProject instance if found and parsed successfully, null otherwise.</returns>
    public static PieceProject? Load(string projectPath)
    {
        var filePath = System.IO.Path.Combine(projectPath, FILENAME);
        if (!File.Exists(filePath))
        {
            // Log warning: File not found
            return null;
        }

        try
        {
            var tomlString = File.ReadAllText(filePath);
            var model = Toml.ToModel<PieceProject>(tomlString, options: TomlOptions); // Use custom options
            model.Path = projectPath; // Set the project path
            return model;
        }
        catch (Exception ex)
        {
            // Log error: Failed to parse TOML
            Console.WriteLine($"Error loading project from {filePath}: {ex.Message}"); // Temporary logging
            return null;
        }
    }

    /// <summary>
    /// Saves the current PieceProject instance to its defined path.
    /// </summary>
    public void Save()
    {
        if (string.IsNullOrEmpty(Path))
        {
            // Log error: Project path not set
            return;
        }

        var filePath = System.IO.Path.Combine(Path, FILENAME);
        try
        {
            var tomlString = Toml.FromModel(this, options: TomlOptions); // Use custom options
            File.WriteAllText(filePath, tomlString);
        }
        catch (Exception ex)
        {
            // Log error: Failed to save TOML
            Console.WriteLine($"Error saving project to {filePath}: {ex.Message}"); // Temporary logging
            return;
        }
    }
}
