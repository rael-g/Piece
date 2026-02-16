using System.CommandLine;
using System.CommandLine.Invocation;
using Microsoft.Extensions.Logging;
using Piece.ProjectManagement;
using System.Threading.Tasks;
using System;
using System.IO;

namespace Piece.Cli.Commands;

public class ImportAssetCommand : Command
{
    public ImportAssetCommand() : base("import", "Imports an asset into a Piece Engine project.")
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
    }

    public class Handler : ICommandHandler
    {
        private readonly IProjectManager _projectManager;
        private readonly IProjectAssetService _projectAssetService;
        private readonly ILogger<ImportAssetCommand> _logger;

        public FileInfo SourceFile { get; set; } = default!;
        public string Type { get; set; } = string.Empty;
        public FileInfo? Project { get; set; }

        public Handler(IProjectManager projectManager, IProjectAssetService projectAssetService, ILogger<ImportAssetCommand> logger)
        {
            _projectManager = projectManager;
            _projectAssetService = projectAssetService;
            _logger = logger;
        }

        public async Task<int> InvokeAsync(InvocationContext context)
        {
            if (!SourceFile.Exists)
            {
                _logger.LogError("Source asset file not found at '{SourceFilePath}'.", SourceFile.FullName);
                return 1;
            }
            if (Project == null || !Project.Exists)
            {
                _logger.LogError("Project file not found at '{ProjectPath}'.", Project?.FullName ?? "null");
                return 1;
            }

            _logger.LogInformation("Attempting to import asset '{SourceFileName}' (Type: '{AssetType}') into project '{ProjectPath}'.", SourceFile.Name, Type, Project.FullName);

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

                bool success = await _projectAssetService.ImportAsset(pieceProject, SourceFile.FullName, Type);

                if (success)
                {
                    _logger.LogInformation("Asset '{SourceFileName}' imported successfully.", SourceFile.Name);
                    return 0; // Success
                }
                else
                {
                    _logger.LogError("Failed to import asset '{SourceFileName}'.", SourceFile.Name);
                    return 1; // Failure
                }
            }
            catch (Exception ex)
            {
                _logger.LogError(ex, "An error occurred while importing asset '{SourceFileName}': {ErrorMessage}", SourceFile.Name, ex.Message);
                return 1; // Failure
            }
        }
    }
}
