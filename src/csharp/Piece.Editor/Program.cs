using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Hosting;

using Piece.Core;

using Serilog;
using Serilog.Events;

namespace Piece.Editor;

/// <summary>
/// The main entry point for the Piece Editor application.
/// </summary>
public class Program
{
    private static NativeCalls.CppLogCallback? _cppLogCallbackDelegate;

    /// <summary>
    /// The main entry point of the application.
    /// </summary>
    /// <param name="args">The command-line arguments.</param>
    public static void Main(string[] args)
    {
        var configuration = new ConfigurationBuilder()
            .SetBasePath(AppContext.BaseDirectory)
            .AddJsonFile("appsettings.json", optional: false, reloadOnChange: true)
            .AddEnvironmentVariables()
            .Build();

        Log.Logger = new LoggerConfiguration()
            .ReadFrom.Configuration(configuration)
            .Enrich.FromLogContext()
            .MinimumLevel.Override("Microsoft", LogEventLevel.Warning)
            .CreateLogger();

        try
        {
            Log.Information("Starting Piece Editor host.");

            _cppLogCallbackDelegate = new NativeCalls.CppLogCallback(NativeCalls.ProcessCppLog);
            NativeCalls.RegisterLogCallback(_cppLogCallbackDelegate);

            CreateHostBuilder(args).Build().Run();
        }
        catch (Exception ex)
        {
            Log.Fatal(ex, "Piece Editor terminated unexpectedly.");
        }
        finally
        {
            Log.CloseAndFlush();
        }
    }

    /// <summary>
    /// Creates and configures the application's host builder.
    /// </summary>
    /// <param name="args">The command-line arguments.</param>
    /// <returns>The configured <see cref="IHostBuilder"/>.</returns>
    public static IHostBuilder CreateHostBuilder(string[] args) =>
        Host.CreateDefaultBuilder(args)
            .UseSerilog();
}