namespace Piece.ProjectManagement;

public class PieceProject
{
    public string Name { get; set; } = string.Empty;
    public string Path { get; set; } = string.Empty;
    public string DefaultScene { get; set; } = string.Empty;
    // Potentially a dictionary or custom class for engine configurations
    public Dictionary<string, string> EngineConfig { get; set; } = new Dictionary<string, string>();
}
