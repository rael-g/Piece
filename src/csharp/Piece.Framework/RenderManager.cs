using System;
using Microsoft.Extensions.Logging;
using System.Collections.Generic;

namespace Piece.Framework;

/// <summary>
/// Manages the collection and submission of rendering commands to the C++ RenderSystem.
/// </summary>
public class RenderManager
{
    private readonly ILogger<RenderManager> _logger;
    private readonly List<RenderCommand> _renderCommands = new List<RenderCommand>();

    public RenderManager(ILogger<RenderManager> logger)
    {
        _logger = logger;
        _logger.LogInformation("RenderManager created.");
    }

    /// <summary>
    /// Submits a mesh for rendering in the current frame.
    /// </summary>
    /// <param name="mesh">The mesh to render.</param>
    /// <param name="material">The material to use for rendering the mesh.</param>
    /// <param name="transform">The world transform of the mesh.</param>
    public void SubmitMeshForRendering(Mesh mesh, Material material, TransformComponent transform)
    {
        if (mesh == null || material == null || transform == null)
        {
            _logger.LogWarning("Attempted to submit null mesh, material, or transform for rendering.");
            return;
        }

        var command = new RenderCommand
        {
            Mesh = mesh,
            Material = material,
            WorldPosition = transform.Position,
            WorldRotation = transform.Rotation,
            WorldScale = transform.Scale
        };
        _renderCommands.Add(command);
        _logger.LogTrace("Submitted mesh '{MeshName}' with material '{MaterialName}' at position {Position} for rendering.",
            mesh.Name, material.Name, transform.Position);
    }

    /// <summary>
    /// Executes the rendering commands. This would typically be called by the GameEngine's Render loop.
    /// For now, it just logs the collected commands.
    /// </summary>
    public void ExecuteRenderCommands()
    {
        _logger.LogDebug("Executing {Count} render commands for the current frame.", _renderCommands.Count);
        foreach (var command in _renderCommands)
        {
            _logger.LogDebug("Render Command: Mesh='{Mesh}', Material='{Material}', Position='{Position}'.",
                command.Mesh.Name, command.Material.Name, command.WorldPosition);
        }
    }

    /// <summary>
    /// Clears all submitted rendering commands for the next frame.
    /// </summary>
    public void ClearRenderCommands()
    {
        _renderCommands.Clear();
        _logger.LogTrace("Cleared all render commands.");
    }

    /// <summary>
    /// Internal struct to hold rendering command data.
    /// </summary>
    private struct RenderCommand
    {
        public Mesh Mesh;
        public Material Material;
        public Vector3 WorldPosition;
        public Quaternion WorldRotation;
        public Vector3 WorldScale;
    }
}
