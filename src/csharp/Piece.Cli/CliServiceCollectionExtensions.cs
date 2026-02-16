using Microsoft.Extensions.DependencyInjection;

namespace Piece.Cli;

public static class CliServiceCollectionExtensions
{
    public static IServiceCollection AddCliServices(this IServiceCollection services)
    {
        // Register CLI-specific services here
        // For example:
        // services.AddSingleton<IConsole, SystemConsole>();

        return services;
    }
}
