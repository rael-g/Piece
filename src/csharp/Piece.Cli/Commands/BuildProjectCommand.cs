using System.CommandLine;
using System.CommandLine.Invocation;
using Microsoft.Extensions.Logging;
using Piece.ProjectManagement;
using System.Threading.Tasks;
using System;
using System.IO;

namespace Piece.Cli.Commands;

public class BuildProjectCommand : Command
{
    public BuildProjectCommand() : base("build", "Builds a Piece Engine project.")
    {
        var projectOption = new Option<FileInfo?>(
            new[] { "--project", "-p" },
            "The path to the project file (e.g., .pieceproject or .csproj/.sln inside the project root).")
        {
            IsRequired = true
        };
        AddOption(projectOption);

        var configurationOption = new Option<string>(
            new[] { "--configuration", "-c" },
            () => "Debug",
            "The build configuration (e.g., Debug, Release).");
        AddOption(configurationOption);
    }

    public class Handler : ICommandHandler
    {
        private readonly IProjectManager _projectManager;
        private readonly IProjectBuildService _projectBuildService;
        private readonly ILogger<BuildProjectCommand> _logger;

        public FileInfo? Project { get; set; }
        public string Configuration { get; set; } = string.Empty;

        public Handler(IProjectManager projectManager, IProjectBuildService projectBuildService, ILogger<BuildProjectCommand> logger)
        {
            _projectManager = projectManager;
            _projectBuildService = projectBuildService;
            _logger = logger;
        }

        public async Task<int> InvokeAsync(InvocationContext context)
        {
            if (Project == null || !Project.Exists)
            {
                _logger.LogError("Project file not found at '{ProjectPath}'.", Project?.FullName ?? "null");
                return 1;
            }

            _logger.LogInformation("Attempting to build project '{ProjectPath}' with configuration '{Configuration}'.", Project.FullName, Configuration);

            try
            {
                // The Project file could be .pieceproject, or a .csproj/.sln within the project root.
                // For simplicity, ProjectManager.LoadProject expects the project root path.
                // We need to resolve the project root path from the provided Project FileInfo.
                string projectRootPath = Project.DirectoryName 
                                         ?? throw new InvalidOperationException($"Could not determine project root from {Project.FullName}");

                var pieceProject = await _projectManager.LoadProject(Path.Combine(projectRootPath, PieceProject.FILENAME));
                
                if (pieceProject == null)
                {
                    _logger.LogError("Failed to load PieceProject definition from '{ProjectPath}'.", Project.FullName);
                    return 1;
                }

                bool success = await _projectBuildService.BuildProject(pieceProject, Configuration);

                if (success)
                {
                    _logger.LogInformation("Project '{ProjectName}' built successfully.", pieceProject.Name);
                    return 0; // Success
                }
                else
                {
                    _logger.LogError("Project '{ProjectName}' failed to build.", pieceProject.Name);
                    return 1; // Failure
                }
            }
            catch (Exception ex)
            {
                _logger.LogError(ex, "An error occurred while building project '{ProjectPath}': {ErrorMessage}", Project.FullName, ex.Message);
                return 1; // Failure
            }
        }
    }
}
