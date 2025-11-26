using Serilog;
using Serilog.Events;
using Microsoft.Extensions.Hosting;
using Microsoft.Extensions.Configuration;
using System.IO;
using System;
using Piece.Core.Interop;

namespace Piece.Editor
{
    public class Program
    {
        private static NativeCalls.CppLogCallback _cppLogCallbackDelegate;

        public static void Main(string[] args)
        {
            var configuration = new ConfigurationBuilder()
                .SetBasePath(Directory.GetCurrentDirectory())
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

        public static IHostBuilder CreateHostBuilder(string[] args) =>
            Host.CreateDefaultBuilder(args)
                .UseSerilog();
    }
}