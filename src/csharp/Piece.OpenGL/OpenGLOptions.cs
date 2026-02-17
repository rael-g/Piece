namespace Piece.OpenGL;

public class OpenGLOptions
{
    public bool EnableValidationLayers { get; set; } = true;
    public int MaxFramesInFlight { get; set; } = 2;
    // Add other OpenGL specific options here if needed
}