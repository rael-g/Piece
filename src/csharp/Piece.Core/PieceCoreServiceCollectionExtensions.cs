using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;
using Microsoft.Extensions.Logging;

namespace Piece.Core;

public static class PieceCoreServiceCollectionExtensions
{
    public static IServiceCollection AddPieceCore(this IServiceCollection services)
    {
        services.AddSingleton<Engine>();
        services.AddHostedService<EngineHostedService>();
        return services;
    }

    private class EngineHostedService : IHostedService, IDisposable
    {
        private readonly Engine _engine;
        private readonly ILogger<EngineHostedService> _logger; 

        public EngineHostedService(Engine engine, ILogger<EngineHostedService> logger)
        {
            _engine = engine;
            _logger = logger;
        }

        public Task StartAsync(CancellationToken cancellationToken)
        {
            _logger.LogInformation("Starting Piece Engine initialization...");
            _engine.Initialize();
            _logger.LogInformation("Piece Engine initialized.");
            return Task.CompletedTask;
        }

        public Task StopAsync(CancellationToken cancellationToken)
        {
            _logger.LogInformation("Stopping Piece Engine...");
            _engine.Dispose();
            _logger.LogInformation("Piece Engine stopped.");
            return Task.CompletedTask;
        }

        public void Dispose()
        {
            _engine.Dispose();
        }
    }
}