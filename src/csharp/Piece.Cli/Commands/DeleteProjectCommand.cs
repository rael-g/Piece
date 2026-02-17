using System;
using System.CommandLine;
using System.IO;
using System.Threading.Tasks;

using Microsoft.Extensions.Logging;

using Piece.ProjectManagement;

namespace Piece.Cli.Commands;

public class DeleteProjectCommand : Command
{
    public DeleteProjectCommand(IProjectManager projectManager, ILogger<DeleteProjectCommand> logger)
        : base("delete", "Deletes a Piece Engine project.")
    {
        var projectPathArgument = new Argument<DirectoryInfo>(
            "projectPath",
            "The path to the project directory to delete.")
        {
            Arity = ArgumentArity.ExactlyOne
        };
        AddArgument(projectPathArgument);

        this.SetHandler(async (projectPath) =>
        {
            if (!projectPath.Exists)
            {
                logger.LogError("Project directory not found at '{ProjectPath}'.", projectPath.FullName);
                return; // Original code returned 1
            }

            logger.LogInformation("Attempting to delete project at '{ProjectPath}'.", projectPath.FullName);

            try
            {
                bool success = await projectManager.DeleteProject(projectPath.FullName);

                if (success)
                {
                    logger.LogInformation("Project directory '{ProjectPath}' deleted successfully.", projectPath.FullName);
                    return; // Original code returned 0
                }
                else
                {
                    logger.LogError("Failed to delete project directory '{ProjectPath}'.", projectPath.FullName);
                    return; // Original code returned 1
                }
            }
            catch (Exception ex)
            {
                logger.LogError(ex, "An error occurred while deleting project directory '{ProjectPath}': {ErrorMessage}", projectPath.FullName, ex.Message);
                return; // Original code returned 1
            }
        },
        projectPathArgument);
    }
}