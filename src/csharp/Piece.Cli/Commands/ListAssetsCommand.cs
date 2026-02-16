using System.CommandLine;
using System.CommandLine.Invocation;
using Microsoft.Extensions.Logging;
using Piece.ProjectManagement;
using System.Threading.Tasks;
using System;
using System.IO;
using System.Linq;

namespace Piece.Cli.Commands;

public class ListAssetsCommand : Command
{
    public ListAssetsCommand() : base("list", "Lists assets in a Piece Engine project.")
    {
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
        private readonly ILogger<ListAssetsCommand> _logger;

        public FileInfo? Project { get; set; }

        public Handler(IProjectManager projectManager, IProjectAssetService projectAssetService, ILogger<ListAssetsCommand> logger)
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

            _logger.LogInformation("Attempting to list assets for project '{ProjectPath}'.", Project.FullName);

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

                var assets = await _projectAssetService.ListAssets(pieceProject);

                if (assets.Any())
                {
                    _logger.LogInformation("Assets in project '{ProjectName}':", pieceProject.Name);
                    foreach (var asset in assets)
                    {
                        _logger.LogInformation("- {Asset}", asset);
                    }
                }
                else
                {
                    _logger.LogInformation("No assets found in project '{ProjectName}'.", pieceProject.Name);
                }
                return 0; // Success
            }
            catch (Exception ex)
            {
                _logger.LogError(ex, "An error occurred while listing assets for project '{ProjectPath}': {ErrorMessage}", Project.FullName, ex.Message);
                return 1; // Failure
            }
        }
    }
}
