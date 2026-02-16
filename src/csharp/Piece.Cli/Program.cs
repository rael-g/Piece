using System.CommandLine;
using System.CommandLine.Builder;
using System.CommandLine.Hosting;
using System.CommandLine.Parsing;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;
using Microsoft.Extensions.Logging;
using Piece.ProjectManagement;
using Serilog; // For Serilog configuration
using Piece.Cli; // Added for CliServiceCollectionExtensions
using Piece.Cli.Commands; // Added for NewProjectCommand, BuildProjectCommand, and ImportAssetCommand

namespace Piece.Cli;

public class Program
{
    public static async Task<int> Main(string[] args)
    {
        var rootCommand = new RootCommand("Piece Engine CLI");

        // Define main commands
        var projectCommand = new Command("project", "Commands for managing Piece Engine projects.");
        rootCommand.AddCommand(projectCommand);

        var newProjectCommand = new NewProjectCommand();
        projectCommand.AddCommand(newProjectCommand);

        var buildCommand = new BuildProjectCommand();
        rootCommand.AddCommand(buildCommand);

        var assetCommand = new Command("asset", "Commands for managing project assets.");
        rootCommand.AddCommand(assetCommand);

        var importAssetCommand = new ImportAssetCommand();
        assetCommand.AddCommand(importAssetCommand);

        var parser = new CommandLineBuilder(rootCommand)
            .UseHost(CreateHostBuilder)
            .UseDefaults()
            .Build();

        return await parser.InvokeAsync(args);
    }

    private static IHostBuilder CreateHostBuilder(string[] args) =>
        Host.CreateDefaultBuilder(args)
            .UseSerilog((hostContext, services, configuration) => {
                configuration.ReadFrom.Configuration(hostContext.Configuration)
                             .ReadFrom.Services(services)
                             .Enrich.FromLogContext()
                             .WriteTo.Console(); // Configure Serilog to write to console
            })
            .ConfigureServices((hostContext, services) =>
            {
                // Register ProjectManagement services
                services.AddProjectManagement(hostContext.Configuration);

                // Register CLI-specific services
                services.AddCliServices();

                // Register command handlers
                services.AddTransient<NewProjectCommand.Handler>();
                services.AddTransient<BuildProjectCommand.Handler>();
                services.AddTransient<ImportAssetCommand.Handler>();
            });
}
