using System;
using System.CommandLine;
using System.Threading.Tasks;

using Microsoft.Extensions.Logging;

using Piece.ProjectManagement;

namespace Piece.Cli.Commands;

public class NewProjectCommand : Command
{
    public NewProjectCommand(IProjectManager projectManager, ILogger<NewProjectCommand> logger)
        : base("new", "Creates a new Piece Engine project.")
    {
        var nameArgument = new Argument<string>(
            "name",
            "The name of the new project.");
        AddArgument(nameArgument);

        var pathOption = new Option<string>(
            new[] { "--path", "-p" },
            () => Environment.CurrentDirectory,
            "The directory where the project will be created.");
        AddOption(pathOption);

        var templateOption = new Option<string>(
            new[] { "--template", "-t" },
            () => "piece-empty",
            "The template to use for the new project.");
        AddOption(templateOption);

        this.SetHandler(async (name, path, template) =>
        {
            logger.LogInformation("Attempting to create new project '{ProjectName}' at '{ProjectPath}' using template '{Template}'.", name, path, template);

            try
            {
                var project = await projectManager.CreateProject(name, path, template);
                logger.LogInformation("Project '{ProjectName}' created successfully.", project.Name);
                return; // Original code returned 0
            }
            catch (Exception ex)
            {
                logger.LogError(ex, "Failed to create project '{ProjectName}': {ErrorMessage}", name, ex.Message);
                return; // Original code returned 1
            }
        },
        nameArgument, pathOption, templateOption);
    }
}