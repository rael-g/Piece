using System.CommandLine;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;
using Piece.ProjectManagement;
using Serilog; // For Serilog configuration
using Piece.Cli; // Added for CliServiceCollectionExtensions
using Piece.Cli.Commands; // Added for all command classes

namespace Piece.Cli;

public class Program
{
    public static async Task<int> Main(string[] args)
    {
        var rootCommand = new RootCommand("Piece Engine CLI");

        // Define main commands
        var projectCommand = new Command("project", "Commands for managing Piece Engine projects.");
        rootCommand.Add(projectCommand);

        var newProjectCommand = new NewProjectCommand();
        projectCommand.Add(newProjectCommand);

        var buildCommand = new BuildProjectCommand();
        rootCommand.Add(buildCommand);

        var assetCommand = new Command("asset", "Commands for managing project assets.");
        rootCommand.Add(assetCommand);

        var importAssetCommand = new ImportAssetCommand();
        assetCommand.Add(importAssetCommand);

        var listAssetsCommand = new ListAssetsCommand();
        assetCommand.Add(listAssetsCommand);

        var deleteAssetCommand = new DeleteAssetCommand();
        assetCommand.Add(deleteAssetCommand);

        var moduleCommand = new Command("module", "Commands for managing project modules (NuGet packages).");
        rootCommand.Add(moduleCommand);

        var addModuleCommand = new AddModuleCommand();
        moduleCommand.Add(addModuleCommand);

        var removeModuleCommand = new RemoveModuleCommand();
        moduleCommand.Add(removeModuleCommand);

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
                services.AddTransient<ListAssetsCommand.Handler>();
                services.AddTransient<AddModuleCommand.Handler>();
                services.AddTransient<RemoveModuleCommand.Handler>();
                services.AddTransient<DeleteAssetCommand.Handler>();
            });
}
