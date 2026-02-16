using Microsoft.Extensions.Logging;
using System.Numerics;

namespace Piece.Framework;

/// <summary>
/// Defines different types of lights.
/// </summary>
public enum LightType
{
    Directional,
    Point,
    Spot
}

/// <summary>
/// A component that defines a light source's properties.
/// </summary>
public class LightComponent : Component
{
    /// <summary>
    /// The type of light (Directional, Point, Spot).
    /// </summary>
    public LightType Type { get; set; } = LightType.Point;

    /// <summary>
    /// The color of the light.
    /// </summary>
    public Vector3 Color { get; set; } = Vector3.One; // White light

    /// <summary>
    /// The intensity of the light.
    /// </summary>
    public float Intensity { get; set; } = 1.0f;

    /// <summary>
    /// For Point and Spot lights, the range of effect.
    /// </summary>
    public float Range { get; set; } = 10.0f;

    // For Spot lights
    public float InnerConeAngle { get; set; } = 30.0f;
    public float OuterConeAngle { get; set; } = 45.0f;

    public override void Start()
    {
        Logger?.LogInformation("LightComponent '{NodeName}' initialized as {LightType} light with Color: {Color}, Intensity: {Intensity}, Range: {Range}.",
            Owner?.Name ?? "Unknown Light", Type, Color, Intensity, Range);
    }

    public override void Update(float deltaTime)
    {
        // For now, just log its presence. Actual light logic will involve
        // passing light properties to the rendering system.
        var transform = Owner?.GetComponent<TransformComponent>();
        if (transform != null)
        {
            Logger?.LogTrace("LightComponent '{NodeName}' ({LightType}) at Position: {Position}.",
                Owner?.Name ?? "Unknown Light", Type, transform.Position);
        }
        else
        {
            Logger?.LogTrace("LightComponent '{NodeName}' ({LightType}) updated (no TransformComponent).", Owner?.Name ?? "Unknown Light", Type);
        }
    }
}
