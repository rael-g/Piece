using System.CommandLine;
using System.CommandLine.Invocation;
using Microsoft.Extensions.Logging;
using Piece.ProjectManagement;
using System.Threading.Tasks;
using System;
using System.IO;

namespace Piece.Cli.Commands;

public class DeleteAssetCommand : Command
{
    public DeleteAssetCommand() : base("delete", "Deletes an asset from a Piece Engine project.")
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
    }

    public class Handler : ICommandHandler
    {
        private readonly IProjectManager _projectManager;
        private readonly IProjectAssetService _projectAssetService;
        private readonly ILogger<DeleteAssetCommand> _logger;

        public string AssetPath { get; set; } = string.Empty;
        public FileInfo? Project { get; set; }

        public Handler(IProjectManager projectManager, IProjectAssetService projectAssetService, ILogger<DeleteAssetCommand> logger)
        {
            _projectManager = projectManager;
            _projectAssetService = projectAssetService;
            _logger = logger;
        }

        public async Task<int> InvokeAsync(InvocationContext context)
        {
            if (Project == null || !Project.Exists)
            {
                _logger.LogError("Project file not found at '{ProjectPath}'.", Project?.FullName ?? "null");
                return 1;
            }

            _logger.LogInformation("Attempting to delete asset '{AssetPath}' from project '{ProjectPath}'.", AssetPath, Project.FullName);

            try
            {
                string projectRootPath = Project.DirectoryName 
                                         ?? throw new InvalidOperationException($"Could not determine project root from {Project.FullName}");

                var pieceProject = await _projectManager.LoadProject(Path.Combine(projectRootPath, PieceProject.FILENAME));
                
                if (pieceProject == null)
                {
                    _logger.LogError("Failed to load PieceProject definition from '{ProjectPath}'.", Project.FullName);
                    return 1;
                }

                bool success = await _projectAssetService.DeleteAsset(pieceProject, AssetPath);

                if (success)
                {
                    _logger.LogInformation("Asset '{AssetPath}' deleted successfully from project '{ProjectName}'.", AssetPath, pieceProject.Name);
                    return 0; // Success
                }
                else
                {
                    _logger.LogError("Failed to delete asset '{AssetPath}' from project '{ProjectName}'.", AssetPath, pieceProject.Name);
                    return 1; // Failure
                }
            }
            catch (Exception ex)
            {
                _logger.LogError(ex, "An error occurred while deleting asset '{AssetPath}' from project '{ProjectPath}': {ErrorMessage}", AssetPath, Project.FullName, ex.Message);
                return 1; // Failure
            }
        }
    }
}
