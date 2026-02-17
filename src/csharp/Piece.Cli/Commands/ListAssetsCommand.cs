using System;
using System.CommandLine;
using System.IO;
using System.Linq;
using System.Threading.Tasks;

using Microsoft.Extensions.Logging;

using Piece.ProjectManagement;

namespace Piece.Cli.Commands;

public class ListAssetsCommand : Command
{
    public ListAssetsCommand(IProjectManager projectManager, IProjectAssetService projectAssetService, ILogger<ListAssetsCommand> logger)
        : base("list", "Lists assets in a Piece Engine project.")
    {
        var projectOption = new Option<FileInfo?>(
            new[] { "--project", "-p" },
            "The path to the project file (e.g., .pieceproject).")
        {
            IsRequired = true
        };
        AddOption(projectOption);

        this.SetHandler(async (project) =>
        {
            if (project == null || !project.Exists)
            {
                logger.LogError("Project file not found at '{ProjectPath}'.", project?.FullName ?? "null");
                return; // Original code returned 1
            }

            logger.LogInformation("Attempting to list assets for project '{ProjectPath}'.", project.FullName);

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

                var assets = await projectAssetService.ListAssets(pieceProject);

                if (assets.Any())
                {
                    logger.LogInformation("Assets in project '{ProjectName}':", pieceProject.Name);
                    foreach (var asset in assets)
                    {
                        logger.LogInformation("- {Asset}", asset);
                    }
                }
                else
                {
                    logger.LogInformation("No assets found in project '{ProjectName}'.", pieceProject.Name);
                }
                return; // Original code returned 0
            }
            catch (Exception ex)
            {
                logger.LogError(ex, "An error occurred while listing assets for project '{ProjectPath}': {ErrorMessage}", project.FullName, ex.Message);
                return; // Original code returned 1
            }
        },
        projectOption);
    }
}