using System;

using Microsoft.Extensions.DependencyInjection;

using Piece.Core.Abstractions;

namespace Piece.MinimalPal;

public static class MinimalPalServiceCollectionExtensions
{
    public static IServiceCollection AddMinimalPalPhysics(this IServiceCollection services, Action<MinimalPalOptions>? configureOptions = null)
    {
        services.AddSingleton<IPhysicsWorldFactory, MinimalPalPhysicsWorldFactory>();

        // Configure options for Minimal PAL low-level implementation
        if (configureOptions != null)
        {
            services.Configure(configureOptions);
        }
        else
        {
            // Register default options if no configuration is provided
            services.Configure<MinimalPalOptions>(options => { /* Apply default Minimal PAL options */ });
        }
        return services;
    }
}