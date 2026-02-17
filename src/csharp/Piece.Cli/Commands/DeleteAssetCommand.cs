using System;
using System.CommandLine;
using System.IO;
using System.Threading.Tasks;

using Microsoft.Extensions.Logging;

using Piece.ProjectManagement;

namespace Piece.Cli.Commands;

public class DeleteAssetCommand : Command
{
    public DeleteAssetCommand(IProjectManager projectManager, IProjectAssetService projectAssetService, ILogger<DeleteAssetCommand> logger)
        : base("delete", "Deletes an asset from a Piece Engine project.")
    {
        var assetPathArgument = new Argument<string>(
            "assetPath",
            "The relative path to the asset to delete within the project's asset folder.");
        AddArgument(assetPathArgument);

        var projectOption = new Option<FileInfo?>(
            new[] { "--project", "-p" },
            "The path to the project file (e.g., .pieceproject).")
        {
            IsRequired = true
        };
        AddOption(projectOption);

        this.SetHandler(async (assetPath, project) =>
        {
            if (project == null || !project.Exists)
            {
                logger.LogError("Project file not found at '{ProjectPath}'.", project?.FullName ?? "null");
                return; // Original code returned 1
            }

            logger.LogInformation("Attempting to delete asset '{AssetPath}' from project '{ProjectPath}'.", assetPath, project.FullName);

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

                bool success = await projectAssetService.DeleteAsset(pieceProject, assetPath);

                if (success)
                {
                    logger.LogInformation("Asset '{AssetPath}' deleted successfully from project '{ProjectName}'.", assetPath, pieceProject.Name);
                    return; // Original code returned 0
                }
                else
                {
                    logger.LogError("Failed to delete asset '{AssetPath}' from project '{ProjectName}'.", assetPath, pieceProject.Name);
                    return; // Original code returned 1
                }
            }
            catch (Exception ex)
            {
                logger.LogError(ex, "An error occurred while deleting asset '{AssetPath}' from project '{ProjectPath}': {ErrorMessage}", assetPath, project.FullName, ex.Message);
                return; // Original code returned 1
            }
        },
        assetPathArgument, projectOption);
    }
}