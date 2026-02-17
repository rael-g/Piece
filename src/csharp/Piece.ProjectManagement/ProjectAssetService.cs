using System;
using System.Collections.Generic;
using System.IO;
using System.Linq; // For Path.GetFileName
using System.Threading.Tasks;

using Microsoft.Extensions.Logging; // Added for logging

using Piece.Framework.Abstractions; // Added for IAssetManager

namespace Piece.ProjectManagement;

public class ProjectAssetService : IProjectAssetService
{
    private readonly IAssetManager _frameworkAssetManager;
    private readonly ILogger<ProjectAssetService> _logger;

    public ProjectAssetService(IAssetManager frameworkAssetManager, ILogger<ProjectAssetService> logger)
    {
        _frameworkAssetManager = frameworkAssetManager;
        _logger = logger;
    }

    public async Task<bool> ImportAsset(PieceProject project, string sourceFilePath, string assetType)
    {
        if (project == null) throw new ArgumentNullException(nameof(project));
        if (string.IsNullOrWhiteSpace(sourceFilePath))
        {
            _logger.LogError("Source file path cannot be empty for asset import.");
            throw new ArgumentException("Source file path cannot be empty.", nameof(sourceFilePath));
        }
        if (!File.Exists(sourceFilePath))
        {
            _logger.LogError("Source asset file not found at '{SourceFilePath}'.", sourceFilePath);
            throw new FileNotFoundException($"Source asset file not found at '{sourceFilePath}'.");
        }
        if (string.IsNullOrWhiteSpace(assetType))
        {
            _logger.LogError("Asset type cannot be empty for asset import.");
            throw new ArgumentException("Asset type cannot be empty.", nameof(assetType));
        }

        var assetDirectory = Path.Combine(project.Path, "Assets"); // Default asset folder
        Directory.CreateDirectory(assetDirectory); // Ensure asset directory exists

        var fileName = Path.GetFileName(sourceFilePath);
        var destinationPath = Path.Combine(assetDirectory, fileName);

        try
        {
            File.Copy(sourceFilePath, destinationPath, true); // Overwrite if exists
            // Here, we would also typically register the asset with _frameworkAssetManager
            // await _frameworkAssetManager.RegisterAsset(destinationPath, assetType); // Example call
            _logger.LogInformation("Asset '{FileName}' (Type: {AssetType}) imported successfully to '{DestinationPath}'.", fileName, assetType, destinationPath);
            return true;
        }
        catch (Exception ex)
        {
            _logger.LogError(ex, "Error importing asset '{FileName}': {ErrorMessage}", fileName, ex.Message);
            return false;
        }
    }

    public async Task<IEnumerable<string>> ListAssets(PieceProject project)
    {
        if (project == null) throw new ArgumentNullException(nameof(project));

        var assetDirectory = Path.Combine(project.Path, "Assets");
        if (!Directory.Exists(assetDirectory))
        {
            _logger.LogInformation("Asset directory '{AssetDirectory}' does not exist for project '{ProjectName}'. Returning empty list.", assetDirectory, project.Name);
            return Enumerable.Empty<string>();
        }

        try
        {
            // return await _frameworkAssetManager.ListAvailableAssets(assetDirectory); // Example call
            var assets = await Task.Run(() => Directory.GetFiles(assetDirectory).Select(Path.GetFileName).ToList());
            _logger.LogInformation("Listed {AssetCount} assets in '{AssetDirectory}'.", assets.Count, assetDirectory);
            return assets!; // Fix CS8619
        }
        catch (Exception ex)
        {
            _logger.LogError(ex, "Error listing assets in '{AssetDirectory}': {ErrorMessage}", assetDirectory, ex.Message);
            return Enumerable.Empty<string>();
        }
    }

    public async Task<bool> DeleteAsset(PieceProject project, string assetPath)
    {
        if (project == null) throw new ArgumentNullException(nameof(project));
        if (string.IsNullOrWhiteSpace(assetPath))
        {
            _logger.LogError("Asset path cannot be empty for asset deletion.");
            throw new ArgumentException("Asset path cannot be empty.", nameof(assetPath));
        }

        var fullAssetPath = Path.Combine(project.Path, "Assets", Path.GetFileName(assetPath));
        if (!File.Exists(fullAssetPath))
        {
            _logger.LogWarning("Asset file not found at '{FullAssetPath}', cannot delete.", fullAssetPath);
            return false;
        }

        try
        {
            File.Delete(fullAssetPath);
            // Here, we would also typically unregister the asset with _frameworkAssetManager
            // await _frameworkAssetManager.UnregisterAsset(fullAssetPath); // Example call
            _logger.LogInformation("Asset '{FileName}' deleted successfully from '{FullAssetPath}'.", Path.GetFileName(assetPath), fullAssetPath);
            return true;
        }
        catch (Exception ex)
        {
            _logger.LogError(ex, "Error deleting asset '{FileName}': {ErrorMessage}", Path.GetFileName(assetPath), ex.Message);
            return false;
        }
    }
}