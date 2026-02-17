using System.Collections.Generic;

using Microsoft.Extensions.Logging;

namespace Piece.Framework;

public class Scene
{
    private readonly ILogger<Scene> _logger;
    private readonly List<Node> _nodes;

    public Scene(ILogger<Scene> logger)
    {
        _logger = logger;
        _nodes = new List<Node>();
        _logger.LogInformation("Scene created.");
    }

    public void AddNode(Node node)
    {
        _nodes.Add(node);
        _logger.LogDebug("Node added to scene: {NodeName}", node.Name);
    }

    public void RemoveNode(Node node)
    {
        _nodes.Remove(node);
        _logger.LogDebug("Node removed from scene: {NodeName}", node.Name);
    }

    public void Update(float deltaTime)
    {
        foreach (var node in _nodes)
        {
            node.Update(deltaTime);
        }
    }

    // This method will eventually call into the RenderManager
    public void Render()
    {
        foreach (var node in _nodes)
        {
            node.Render();
        }
    }
}