namespace Piece.Glfw;

public class GlfwOptions
{
    public int InitialWindowWidth { get; set; } = 800;
    public int InitialWindowHeight { get; set; } = 600;
    public bool WindowResizable { get; set; } = true;
    public string WindowTitle { get; set; } = "Piece Engine Window";
    // Add other GLFW specific options here if needed
}
