using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;

namespace Piece.Core.Abstractions;

/// <summary>
/// Interface for engine modules that need to configure services with the Dependency Injection container.
/// </summary>
public interface IEngineModule
{
    /// <summary>
    /// Configures services for the module.
    /// </summary>
    /// <param name="services">The IServiceCollection to add services to.</param>
    /// <param name="configuration">The IConfiguration of the application.</param>
    void ConfigureServices(IServiceCollection services, IConfiguration configuration);
}