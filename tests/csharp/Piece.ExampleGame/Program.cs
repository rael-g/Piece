using Microsoft.Extensions.Hosting;
using Piece.Core;
using Piece.Glfw;
using Piece.OpenGL;
using Piece.Box2d;

using Serilog;

namespace Piece.ExampleGame;

internal class Program
{
    static async Task Main(string[] args)
    {
        // Configure Serilog (kept for direct C# logging if needed)
        Log.Logger = new LoggerConfiguration()
            .MinimumLevel.Debug()
            .WriteTo.Console()
            .CreateLogger();
        
        Log.Information("Starting Piece Engine Example Game...");

        try
        {
            var builder = Host.CreateApplicationBuilder(args);

            // Configure Piece Engine services using the new wrapper and DI extensions
            builder.Services.AddPieceCore();
            builder.Services.AddGlfwWindow();
            builder.Services.AddOpenGLRenderer();
            builder.Services.AddBox2dPhysics();
            builder.Services.AddBox2dPhysics();

            // Build and run the host
            var app = builder.Build();
            await app.RunAsync();

            Log.Information("Piece Engine Example Game finished.");
        }
        catch (Exception ex)
        {
            Log.Fatal(ex, "Application terminated unexpectedly.");
        }
        finally
        {
            Log.CloseAndFlush();
        }
    }
}
