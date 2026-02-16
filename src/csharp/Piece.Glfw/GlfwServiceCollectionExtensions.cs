using Microsoft.Extensions.DependencyInjection;
using Piece.Core.Abstractions;
using System;

namespace Piece.Glfw;

public static class GlfwServiceCollectionExtensions
{
    public static IServiceCollection AddGlfwWindow(this IServiceCollection services, Action<GlfwOptions>? configureOptions = null)
    {
        services.AddSingleton<IWindowFactory, GlfwWindowFactory>();

        // Configure options for Glfw low-level implementation
        if (configureOptions != null)
        {
            services.Configure(configureOptions);
        }
        else
        {
            // Register default options if no configuration is provided
            services.Configure<GlfwOptions>(options => { /* Apply default Glfw options */ });
        }
        return services;
    }
}
