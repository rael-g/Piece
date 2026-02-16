using System.CommandLine;
using System.CommandLine.Invocation;
using Microsoft.Extensions.Logging;
using Piece.ProjectManagement;
using System.Threading.Tasks;

namespace Piece.Cli.Commands;

public class NewProjectCommand : Command
{
    public NewProjectCommand() : base("new", "Creates a new Piece Engine project.")
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
    }

    public class NewProjectCommandParameters
    {
        public string Name { get; set; } = string.Empty;
        public string Path { get; set; } = string.Empty;
        public string Template { get; set; } = string.Empty;
        public IProjectManager ProjectManager { get; set; } = default!;
        public ILogger<NewProjectCommand> Logger { get; set; } = default!;
    }

    public new class Handler : ICommandHandler
    {
        private readonly IProjectManager _projectManager;
        private readonly ILogger<NewProjectCommand> _logger;

        public string Name { get; set; } = string.Empty;
        public string Path { get; set; } = string.Empty;
        public string Template { get; set; } = string.Empty;

        public Handler(IProjectManager projectManager, ILogger<NewProjectCommand> logger)
        {
            _projectManager = projectManager;
            _logger = logger;
        }

        public async Task<int> InvokeAsync(InvocationContext context)
        {
            _logger.LogInformation("Attempting to create new project '{ProjectName}' at '{ProjectPath}' using template '{Template}'.", Name, Path, Template);

            try
            {
                var project = await _projectManager.CreateProject(Name, Path, Template);
                _logger.LogInformation("Project '{ProjectName}' created successfully.", project.Name);
                return 0; // Success
            }
            catch (Exception ex)
            {
                _logger.LogError(ex, "Failed to create project '{ProjectName}': {ErrorMessage}", Name, ex.Message);
                return 1; // Failure
            }
        }
    }
}
