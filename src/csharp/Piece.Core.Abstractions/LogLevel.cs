namespace Piece.Core;

/// <summary>
/// Defines the severity levels for log messages, mirroring the native C++ LogLevel enum.
/// </summary>
public enum LogLevel : int
{
    /// <summary>
    /// Detailed trace information, typically for debugging.
    /// </summary>
    Trace = 0,
    /// <summary>
    /// Information useful for debugging purposes.
    /// </summary>
    Debug,
    /// <summary>
    /// General information about engine operation.
    /// </summary>
    Info,
    /// <summary>
    /// Indicates a potential issue that does not prevent the engine from running.
    /// </summary>
    Warning,
    /// <summary>
    /// Indicates a significant issue that may affect engine functionality.
    /// </summary>
    Error,
    /// <summary>
    /// Indicates a critical error that will likely lead to engine termination.
    /// </summary>
    Fatal
}