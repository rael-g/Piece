using System;

using Microsoft.Extensions.DependencyInjection;

using Piece.Glfw;
using Piece.MinimalPal;
using Piece.OpenGL;

namespace Piece.Framework;

public static class EngineServiceCollectionExtensions
{
    public static IServiceCollection AddPieceCore(this IServiceCollection services, Action<EngineOptions>? configureOptions = null)
    {
        // Add the EngineFrameworkModule directly for now.
        // In the future, this would be auto-discovered by a source generator.
        new EngineFrameworkModule().ConfigureServices(services, null); // Pass null for configuration for now

        // Configure EngineOptions
        if (configureOptions != null)
        {
            services.Configure(configureOptions);
        }
        else
        {
            services.Configure<EngineOptions>(options => { /* Apply default Engine options */ });
        }

        // Add C++ backend factory wrappers to the DI container
        services.AddGlfwWindow();
        services.AddOpenGLRenderer();
        services.AddMinimalPalPhysics();

        services.AddSingleton<InputManager>(); // Register InputManager
        services.AddSingleton<AssetManager>(); // Register AssetManager
        services.AddSingleton<RenderManager>(); // Register RenderManager

        return services;
    }
}