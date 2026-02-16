using Piece.Core.Abstractions.NativeCalls;

namespace Piece.Framework;

public class EngineOptions
{
    public NativeCalls.LogLevel DefaultLogLevel { get; set; } = NativeCalls.LogLevel.kInfo;
    public string LogFilePath { get; set; } = "PieceEngine.log";
    public long LogFileSizeLimitBytes { get; set; } = 1024 * 1024 * 5; // 5 MB
    public int RetainedFileCountLimit { get; set; } = 3;
    public string AssetRootPath { get; set; } = "Assets/";
    // Add other engine-wide options here
}
