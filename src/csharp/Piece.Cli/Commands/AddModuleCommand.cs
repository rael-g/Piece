using System.CommandLine;
using System.CommandLine.Invocation;
using Microsoft.Extensions.Logging;
using Piece.ProjectManagement;
using System.Threading.Tasks;
using System;
using System.IO;

namespace Piece.Cli.Commands;

public class AddModuleCommand : Command
{
    public AddModuleCommand() : base("add", "Adds a module (NuGet package) to a Piece Engine project.")
    {
        var moduleNameArgument = new Argument<string>(
            "moduleName",
            "The name of the module (NuGet package ID) to add.");
        AddArgument(moduleNameArgument);

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
        private readonly ILogger<AddModuleCommand> _logger;

        public string ModuleName { get; set; } = string.Empty;
        public FileInfo? Project { get; set; }

        public Handler(IProjectManager projectManager, ILogger<AddModuleCommand> logger)
        {
            _projectManager = projectManager;
            _logger = logger;
        }

        public async Task<int> InvokeAsync(InvocationContext context)
        {
            if (Project == null || !Project.Exists)
            {
                _logger.LogError("Project file not found at '{ProjectPath}'.", Project?.FullName ?? "null");
                return 1;
            }

            _logger.LogInformation("Attempting to add module '{ModuleName}' to project '{ProjectPath}'.", ModuleName, Project.FullName);

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

                bool success = await _projectManager.AddModule(pieceProject, ModuleName);

                if (success)
                {
                    _logger.LogInformation("Module '{ModuleName}' added successfully to project '{ProjectName}'.", ModuleName, pieceProject.Name);
                    return 0; // Success
                }
                else
                {
                    _logger.LogError("Failed to add module '{ModuleName}' to project '{ProjectName}'.", ModuleName, pieceProject.Name);
                    return 1; // Failure
                }
            }
            catch (Exception ex)
            {
                _logger.LogError(ex, "An error occurred while adding module '{ModuleName}' to project '{ProjectPath}': {ErrorMessage}", ModuleName, Project.FullName, ex.Message);
                return 1; // Failure
            }
        }
    }
}
