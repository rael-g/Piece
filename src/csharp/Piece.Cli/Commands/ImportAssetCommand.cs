using System;
using System.CommandLine;
using System.IO;
using System.Threading.Tasks;

using Microsoft.Extensions.Logging;

using Piece.ProjectManagement;

namespace Piece.Cli.Commands;

public class ImportAssetCommand : Command
{
    public ImportAssetCommand(IProjectManager projectManager, IProjectAssetService projectAssetService, ILogger<ImportAssetCommand> logger)
        : base("import", "Imports an asset into a Piece Engine project.")
    {
        var sourceFileArgument = new Argument<FileInfo>(
            "sourceFile",
            "The path to the asset file to import.")
        {
            Arity = ArgumentArity.ExactlyOne
        };
        AddArgument(sourceFileArgument);

        var assetTypeOption = new Option<string>(
            new[] { "--type", "-t" },
            () => "Generic",
            "The type of the asset (e.g., Texture, Model, Material).");
        AddOption(assetTypeOption);

        var projectOption = new Option<FileInfo?>(
            new[] { "--project", "-p" },
            "The path to the project file (e.g., .pieceproject).")
        {
            IsRequired = true
        };
        AddOption(projectOption);

        this.SetHandler(async (sourceFile, assetType, project) =>
        {
            if (!sourceFile.Exists)
            {
                logger.LogError("Source asset file not found at '{SourceFilePath}'.", sourceFile.FullName);
                return; // Original code returned 1
            }
            if (project == null || !project.Exists)
            {
                logger.LogError("Project file not found at '{ProjectPath}'.", project?.FullName ?? "null");
                return; // Original code returned 1
            }

            logger.LogInformation("Attempting to import asset '{SourceFileName}' (Type: '{AssetType}') into project '{ProjectPath}'.", sourceFile.Name, assetType, project.FullName);

            try
            {
                string projectRootPath = project.DirectoryName
                                         ?? throw new InvalidOperationException($"Could not determine project root from {project.FullName}");

                var pieceProject = await projectManager.LoadProject(Path.Combine(projectRootPath, PieceProject.FILENAME));

                if (pieceProject == null)
                {
                    logger.LogError("Failed to load PieceProject definition from '{ProjectPath}'.", project.FullName);
                    return; // Original code returned 1
                }

                bool success = await projectAssetService.ImportAsset(pieceProject, sourceFile.FullName, assetType);

                if (success)
                {
                    logger.LogInformation("Asset '{SourceFileName}' imported successfully.", sourceFile.Name);
                    return; // Original code returned 0
                }
                else
                {
                    logger.LogError("Failed to import asset '{SourceFileName}'.", sourceFile.Name);
                    return; // Original code returned 1
                }
            }
            catch (Exception ex)
            {
                logger.LogError(ex, "An error occurred while importing asset '{SourceFileName}': {ErrorMessage}", sourceFile.Name, ex.Message);
                return; // Original code returned 1
            }
        },
        sourceFileArgument, assetTypeOption, projectOption);
    }
}