using System.CommandLine;
using System.CommandLine.Invocation;
using Microsoft.Extensions.Logging;
using Piece.ProjectManagement;
using System.Threading.Tasks;
using System;
using System.IO;

namespace Piece.Cli.Commands;

public class DeleteProjectCommand : Command
{
    public DeleteProjectCommand() : base("delete", "Deletes a Piece Engine project.")
    {
        var projectPathArgument = new Argument<DirectoryInfo>(
            "projectPath",
            "The path to the project directory to delete.")
        {
            Arity = ArgumentArity.ExactlyOne
        };
        AddArgument(projectPathArgument);
    }

    public class Handler : ICommandHandler
    {
        private readonly IProjectManager _projectManager;
        private readonly ILogger<DeleteProjectCommand> _logger;

        public DirectoryInfo ProjectPath { get; set; } = default!;

        public Handler(IProjectManager projectManager, ILogger<DeleteProjectCommand> logger)
        {
            _projectManager = projectManager;
            _logger = logger;
        }

        public async Task<int> InvokeAsync(InvocationContext context)
        {
            if (!ProjectPath.Exists)
            {
                _logger.LogError("Project directory not found at '{ProjectPath}'.", ProjectPath.FullName);
                return 1;
            }

            _logger.LogInformation("Attempting to delete project at '{ProjectPath}'.", ProjectPath.FullName);

            try
            {
                bool success = await _projectManager.DeleteProject(ProjectPath.FullName);

                if (success)
                {
                    _logger.LogInformation("Project directory '{ProjectPath}' deleted successfully.", ProjectPath.FullName);
                    return 0; // Success
                }
                else
                {
                    _logger.LogError("Failed to delete project directory '{ProjectPath}'.", ProjectPath.FullName);
                    return 1; // Failure
                }
            }
            catch (Exception ex)
            {
                _logger.LogError(ex, "An error occurred while deleting project directory '{ProjectPath}': {ErrorMessage}", ProjectPath.FullName, ex.Message);
                return 1; // Failure
            }
        }
    }
}
