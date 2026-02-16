using System.CommandLine;
using System.CommandLine.Builder;
using System.CommandLine.Hosting;
using System.CommandLine.Parsing;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;
using Microsoft.Extensions.Logging;
using Piece.ProjectManagement;
using Serilog; // For Serilog configuration

namespace Piece.Cli;

public class Program
{
    public static async Task<int> Main(string[] args)
    {
        var rootCommand = new RootCommand("Piece Engine CLI");

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

                // Register other CLI-specific services here if any
                // For now, we'll just have the basic setup
            });
}
