using System;

using Microsoft.Extensions.DependencyInjection;

using Piece.Core.Abstractions;

namespace Piece.OpenGL;

public static class OpenGLServiceCollectionExtensions
{
    public static IServiceCollection AddOpenGLRenderer(this IServiceCollection services, Action<OpenGLOptions>? configureOptions = null)
    {
        services.AddSingleton<IGraphicsDeviceFactory, OpenGLGraphicsDeviceFactory>();

        // Configure options for OpenGL low-level implementation
        if (configureOptions != null)
        {
            services.Configure(configureOptions);
        }
        else
        {
            // Register default options if no configuration is provided
            services.Configure<OpenGLOptions>(options => { /* Apply default OpenGL options */ });
        }
        return services;
    }
}