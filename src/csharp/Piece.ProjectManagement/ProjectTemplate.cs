namespace Piece.ProjectManagement;

public class ProjectTemplate
{
    public string Name { get; set; } = string.Empty;
    public string ShortName { get; set; } = string.Empty; // e.g., "console", "webapi"
    public string Description { get; set; } = string.Empty;
    public string Language { get; set; } = "C#"; // e.g., "C#", "F#", "VB"
    public bool IsPieceEngineTemplate { get; set; } = false; // Custom flag for engine-specific templates
}