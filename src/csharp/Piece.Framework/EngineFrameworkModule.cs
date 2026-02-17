using System;
using System.Runtime.InteropServices;

using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Logging;
using Microsoft.Extensions.Options;

using Piece.Core.Abstractions;
using Piece.Core.Abstractions.NativeCalls;

using Serilog;
using Serilog.Events;
using Serilog.Extensions.Logging;

namespace Piece.Framework;

public class EngineFrameworkModule : IEngineModule
{
    // Holds a reference to the C# delegate to prevent it from being garbage collected
    // while the native code still holds a pointer to it.
    private NativeCalls.LogCallback _nativeLogCallbackDelegate;

    public void ConfigureServices(IServiceCollection services, IConfiguration configuration)
    {
        // Configure EngineOptions
        services.AddOptions<EngineOptions>().Bind(configuration.GetSection(nameof(EngineOptions)));

        // Get configuration values directly
        var defaultLogLevel = configuration.GetValue<NativeCalls.LogLevel>("EngineOptions:DefaultLogLevel", NativeCalls.LogLevel.kInfo);
        var logFilePath = configuration.GetValue<string>("EngineOptions:LogFilePath", "PieceEngine.log");
        var logFileSizeLimitBytes = configuration.GetValue<long>("EngineOptions:LogFileSizeLimitBytes", 1024 * 1024 * 5);
        var retainedFileCountLimit = configuration.GetValue<int>("EngineOptions:RetainedFileCountLimit", 3);

        // Configure Serilog
        Log.Logger = new LoggerConfiguration()
            .MinimumLevel.Is(ConvertLogLevel(defaultLogLevel))
            .MinimumLevel.Override("Microsoft", LogEventLevel.Warning)
            .Enrich.FromLogContext()
            .WriteTo.Console(outputTemplate: "[{Timestamp:HH:mm:ss} {Level:u3}] {Message:lj}{NewLine}{Exception}")
            .WriteTo.File(
                logFilePath,
                fileSizeLimitBytes: logFileSizeLimitBytes,
                rollingInterval: RollingInterval.Day,
                rollOnFileSizeLimit: true,
                retainedFileCountLimit: retainedFileCountLimit,
                outputTemplate: "[{Timestamp:HH:mm:ss} {Level:u3}] {Message:lj}{NewLine}{Exception}"
            )
            .CreateLogger();

        // Register Serilog logger as the ILoggerFactory
        services.AddSingleton<ILoggerFactory>(new SerilogLoggerFactory());
        services.AddLogging(loggingBuilder => loggingBuilder.AddSerilog(dispose: true));

        // Register the GameEngine
        services.AddSingleton<GameEngine>();

        // Register the native log callback
        _nativeLogCallbackDelegate = new NativeCalls.LogCallback(NativeLogCallback);
        NativeCalls.PieceCoreRegisterLogCallback(_nativeLogCallbackDelegate);
    }

    private LogEventLevel ConvertLogLevel(NativeCalls.LogLevel level)
    {
        return level switch
        {
            NativeCalls.LogLevel.kTrace => LogEventLevel.Verbose, // Serilog Verbose = Trace
            NativeCalls.LogLevel.kDebug => LogEventLevel.Debug,
            NativeCalls.LogLevel.kInfo => LogEventLevel.Information,
            NativeCalls.LogLevel.kWarning => LogEventLevel.Warning,
            NativeCalls.LogLevel.kError => LogEventLevel.Error,
            NativeCalls.LogLevel.kFatal => LogEventLevel.Fatal,
            _ => LogEventLevel.Information,
        };
    }

    // This method is called from native C++ code via P/Invoke
    private static void NativeLogCallback(int level, string message)
    {
        // Convert the native log level to Serilog's LogEventLevel
        LogEventLevel serilogLevel = ((NativeCalls.LogLevel)level) switch
        {
            NativeCalls.LogLevel.kTrace => LogEventLevel.Verbose,
            NativeCalls.LogLevel.kDebug => LogEventLevel.Debug,
            NativeCalls.LogLevel.kInfo => LogEventLevel.Information,
            NativeCalls.LogLevel.kWarning => LogEventLevel.Warning,
            NativeCalls.LogLevel.kError => LogEventLevel.Error,
            NativeCalls.LogLevel.kFatal => LogEventLevel.Fatal,
            _ => LogEventLevel.Information,
        };

        // Write the log message using Serilog
        Log.Write(serilogLevel, "[Native] {Message}", message);
    }
}