using System.Numerics;

namespace Piece.Framework;

public class TransformComponent : Component
{
    public Vector3 Position { get; set; } = Vector3.Zero;
    public Quaternion Rotation { get; set; } = Quaternion.Identity;
    public Vector3 Scale { get; set; } = Vector3.One;

    // TODO: Add methods for calculating local and global matrices
    // TODO: Add methods for parenting/unparenting
}
