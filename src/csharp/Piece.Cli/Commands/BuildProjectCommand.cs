using System;
using System.CommandLine;
using System.IO;
using System.Threading.Tasks;

using Microsoft.Extensions.Logging;

using Piece.ProjectManagement;

namespace Piece.Cli.Commands;

public class BuildProjectCommand : Command
{
    public BuildProjectCommand(IProjectManager projectManager, IProjectBuildService projectBuildService, ILogger<BuildProjectCommand> logger)
        : base("build", "Builds a Piece Engine project.")
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

        this.SetHandler(async (project, configuration) =>
        {
            if (project == null || !project.Exists)
            {
                logger.LogError("Project file not found at '{ProjectPath}'.", project?.FullName ?? "null");
                // Original code returned 1, now just log error and return void Task
                return;
            }

            logger.LogInformation("Attempting to build project '{ProjectPath}' with configuration '{Configuration}'.", project.FullName, configuration);

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

                bool success = await projectBuildService.BuildProject(pieceProject, configuration);

                if (success)
                {
                    logger.LogInformation("Project '{ProjectName}' built successfully.", pieceProject.Name);
                    return; // Original code returned 0
                }
                else
                {
                    logger.LogError("Project '{ProjectName}' failed to build.", pieceProject.Name);
                    return; // Original code returned 1
                }
            }
            catch (Exception ex)
            {
                logger.LogError(ex, "An error occurred while building project '{ProjectPath}': {ErrorMessage}", project.FullName, ex.Message);
                return; // Original code returned 1
            }
        },
        projectOption, configurationOption);
    }
}