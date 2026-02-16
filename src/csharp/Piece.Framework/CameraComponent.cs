using Microsoft.Extensions.Logging;
using System.Numerics;

namespace Piece.Framework;

/// <summary>
/// A component that defines a camera's properties and view.
/// </summary>
public class CameraComponent : Component
{
    /// <summary>
    /// The field of view in degrees.
    /// </summary>
    public float FieldOfView { get; set; } = 60.0f;

    /// <summary>
    /// The aspect ratio of the camera (width / height).
    /// </summary>
    public float AspectRatio { get; set; } = 16.0f / 9.0f;

    /// <summary>
    /// The near clipping plane distance.
    /// </summary>
    public float NearPlane { get; set; } = 0.1f;

    /// <summary>
    /// The far clipping plane distance.
    /// </summary>
    public float FarPlane { get; set; } = 100.0f;

    public override void Start()
    {
        Logger?.LogInformation("CameraComponent '{NodeName}' initialized with FOV: {FOV}, AspectRatio: {AspectRatio}, Near: {Near}, Far: {Far}.",
            Owner?.Name ?? "Unknown Camera", FieldOfView, AspectRatio, NearPlane, FarPlane);
    }

    public override void Update(float deltaTime)
    {
        // For now, just log its presence. Actual camera logic will involve
        // calculating view and projection matrices based on the owner's transform.
        var transform = Owner?.GetComponent<TransformComponent>();
        if (transform != null)
        {
            Logger?.LogTrace("CameraComponent '{NodeName}' at Position: {Position}, Rotation: {Rotation}.",
                Owner?.Name ?? "Unknown Camera", transform.Position, transform.Rotation);
        }
        else
        {
            Logger?.LogTrace("CameraComponent '{NodeName}' updated (no TransformComponent).", Owner?.Name ?? "Unknown Camera");
        }
    }
}
