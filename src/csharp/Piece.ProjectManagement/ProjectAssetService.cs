using System.Collections.Generic;
using System.IO;
using System.Threading.Tasks;
using System;
using System.Linq; // For Path.GetFileName

namespace Piece.ProjectManagement;

public class ProjectAssetService : IProjectAssetService
{
    // Placeholder for Piece.Framework.AssetManager
    // This would ideally be an interface (e.g., IAssetManager) from Piece.Framework.Abstractions
    // For now, we simulate asset management logic.
    // private readonly Piece.Framework.IAssetManager _frameworkAssetManager; 

    // public ProjectAssetService(Piece.Framework.IAssetManager frameworkAssetManager)
    // {
    //     _frameworkAssetManager = frameworkAssetManager;
    // }

    public async Task<bool> ImportAsset(PieceProject project, string sourceFilePath, string assetType)
    {
        if (project == null) throw new ArgumentNullException(nameof(project));
        if (string.IsNullOrWhiteSpace(sourceFilePath))
            throw new ArgumentException("Source file path cannot be empty.", nameof(sourceFilePath));
        if (!File.Exists(sourceFilePath))
            throw new FileNotFoundException($"Source asset file not found at '{sourceFilePath}'.");
        if (string.IsNullOrWhiteSpace(assetType))
            throw new ArgumentException("Asset type cannot be empty.", nameof(assetType));

        var assetDirectory = Path.Combine(project.Path, "Assets"); // Default asset folder
        Directory.CreateDirectory(assetDirectory); // Ensure asset directory exists

        var fileName = Path.GetFileName(sourceFilePath);
        var destinationPath = Path.Combine(assetDirectory, fileName);

        try
        {
            File.Copy(sourceFilePath, destinationPath, true); // Overwrite if exists
            // Here, we would also typically register the asset with _frameworkAssetManager
            // For now, simple file copy suffices.
            Console.WriteLine($"Asset '{fileName}' (Type: {assetType}) imported successfully to '{destinationPath}'.");
            return true;
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error importing asset '{fileName}': {ex.Message}"); // Temporary logging
            return false;
        }
    }

    public async Task<IEnumerable<string>> ListAssets(PieceProject project)
    {
        if (project == null) throw new ArgumentNullException(nameof(project));

        var assetDirectory = Path.Combine(project.Path, "Assets");
        if (!Directory.Exists(assetDirectory))
        {
            return Enumerable.Empty<string>();
        }

        try
        {
            return await Task.Run(() => Directory.GetFiles(assetDirectory).Select(Path.GetFileName).ToList());
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error listing assets in '{assetDirectory}': {ex.Message}"); // Temporary logging
            return Enumerable.Empty<string>();
        }
    }

    public async Task<bool> DeleteAsset(PieceProject project, string assetPath)
    {
        if (project == null) throw new ArgumentNullException(nameof(project));
        if (string.IsNullOrWhiteSpace(assetPath))
            throw new ArgumentException("Asset path cannot be empty.", nameof(assetPath));

        var fullAssetPath = Path.Combine(project.Path, "Assets", Path.GetFileName(assetPath));
        if (!File.Exists(fullAssetPath))
        {
            Console.WriteLine($"Warning: Asset file not found at '{fullAssetPath}', cannot delete.");
            return false;
        }

        try
        {
            File.Delete(fullAssetPath);
            Console.WriteLine($"Asset '{Path.GetFileName(assetPath)}' deleted successfully from '{fullAssetPath}'.");
            return true;
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error deleting asset '{Path.GetFileName(assetPath)}': {ex.Message}"); // Temporary logging
            return false;
        }
    }
}
