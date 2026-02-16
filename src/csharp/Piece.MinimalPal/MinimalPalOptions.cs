namespace Piece.MinimalPal;

public class MinimalPalOptions
{
    public float FixedDeltaTime { get; set; } = 1.0f / 60.0f;
    public uint MaxPhysicsSteps { get; set; } = 4;
    // Add other minimal PAL specific options here if needed
}
