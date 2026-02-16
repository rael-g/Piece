using Microsoft.Extensions.Logging;
using System.Numerics;

namespace Piece.Framework;

/// <summary>
/// A component that renders a mesh using a specified material.
/// </summary>
public class MeshRendererComponent : Component
{
    /// <summary>
    /// The mesh to be rendered.
    /// </summary>
    public Mesh Mesh { get; set; }

    /// <summary>
    /// The material to use for rendering the mesh.
    /// </summary>
    public Material Material { get; set; }

    public override void Render()
    {
        if (Mesh == null || Material == null)
        {
            Logger?.LogWarning("MeshRendererComponent on Node '{NodeName}' cannot render: Mesh or Material is not assigned.", Owner?.Name ?? "Unknown");
            return;
        }

        // Submit rendering command to the RenderManager
        var transform = Owner?.GetComponent<TransformComponent>();
        if (transform != null)
        {
            RenderManager?.SubmitMeshForRendering(Mesh, Material, transform);
        }
        else
        {
            Logger?.LogWarning("MeshRendererComponent on Node '{NodeName}' cannot submit for rendering: No TransformComponent found.", Owner?.Name ?? "Unknown");
        }
    }
}
