using System.CommandLine;
using System.CommandLine.Builder; // Re-added

using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;

using Piece.Cli; // Added for CliServiceCollectionExtensions
using Piece.Cli.Commands; // Added for all command classes
using Piece.ProjectManagement;

using Serilog; // For Serilog configuration

namespace Piece.Cli;

public class Program
{
    public static async Task<int> Main(string[] args)
    {
        // Configure and build the host
        var host = Host.CreateDefaultBuilder(args)
            .UseSerilog((hostContext, services, loggerConfiguration) =>
            {
                loggerConfiguration.ReadFrom.Configuration(hostContext.Configuration)
                             .ReadFrom.Services(services)
                             .Enrich.FromLogContext()
                             .WriteTo.Console(); // Configure Serilog to write to console
            })
            .ConfigureServices((hostContext, services) =>
            {
                // Register ProjectManagement services
                services.AddProjectManagement();

                // Register CLI-specific services
                services.AddCliServices();

                // Add all command definitions to the DI container
                // These commands will now directly contain their handlers
                services.AddTransient<NewProjectCommand>();
                services.AddTransient<DeleteProjectCommand>();
                services.AddTransient<BuildProjectCommand>();
                services.AddTransient<ImportAssetCommand>();
                services.AddTransient<ListAssetsCommand>();
                services.AddTransient<AddModuleCommand>();
                services.AddTransient<RemoveModuleCommand>();
                services.AddTransient<DeleteAssetCommand>();
            })
            .Build();

        // Create the root command and add subcommands
        var rootCommand = new RootCommand("Piece Engine CLI");

        // Resolve commands from the host's service provider
        // These are the top-level commands like 'piece build'
        rootCommand.Add(host.Services.GetRequiredService<BuildProjectCommand>());

        // Add "project" subcommand container
        var projectCommand = new Command("project", "Commands for managing Piece Engine projects.");
        projectCommand.Add(host.Services.GetRequiredService<NewProjectCommand>());
        projectCommand.Add(host.Services.GetRequiredService<DeleteProjectCommand>());
        rootCommand.Add(projectCommand);

        // Add "asset" subcommand container
        var assetCommand = new Command("asset", "Commands for managing project assets.");
        assetCommand.Add(host.Services.GetRequiredService<ImportAssetCommand>());
        assetCommand.Add(host.Services.GetRequiredService<ListAssetsCommand>());
        assetCommand.Add(host.Services.GetRequiredService<DeleteAssetCommand>());
        rootCommand.Add(assetCommand);

        // Add "module" subcommand container
        var moduleCommand = new Command("module", "Commands for managing project modules (NuGet packages).");
        moduleCommand.Add(host.Services.GetRequiredService<AddModuleCommand>());
        moduleCommand.Add(host.Services.GetRequiredService<RemoveModuleCommand>());
        rootCommand.Add(moduleCommand);


        // Invoke the root command
        return await rootCommand.InvokeAsync(args);
    }
}