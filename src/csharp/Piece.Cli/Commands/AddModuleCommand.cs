using System;
using System.CommandLine;
using System.IO;
using System.Threading.Tasks;

using Microsoft.Extensions.Logging;

using Piece.ProjectManagement;

namespace Piece.Cli.Commands;

public class AddModuleCommand : Command
{
    public AddModuleCommand(IProjectManager projectManager, ILogger<AddModuleCommand> logger)
        : base("add", "Adds a module (NuGet package) to a Piece Engine project.")
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

        this.SetHandler(async (moduleName, project) =>
        {
            if (project == null || !project.Exists)
            {
                logger.LogError("Project file not found at '{ProjectPath}'.", project?.FullName ?? "null");
                // Original code returned 1, now just log error and return void Task
                return;
            }

            logger.LogInformation("Attempting to add module '{ModuleName}' to project '{ProjectPath}'.", moduleName, project.FullName);

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

                bool success = await projectManager.AddModule(pieceProject, moduleName);

                if (success)
                {
                    logger.LogInformation("Module '{ModuleName}' added successfully to project '{ProjectName}'.", moduleName, pieceProject.Name);
                    return; // Original code returned 0
                }
                else
                {
                    logger.LogError("Failed to add module '{ModuleName}' to project '{ProjectName}'.", moduleName, pieceProject.Name);
                    return; // Original code returned 1
                }
            }
            catch (Exception ex)
            {
                logger.LogError(ex, "An error occurred while adding module '{ModuleName}' to project '{ProjectPath}': {ErrorMessage}", moduleName, project.FullName, ex.Message);
                return; // Original code returned 1
            }
        },
        moduleNameArgument, projectOption); // Parameters for SetHandler
    }
}