using Microsoft.Extensions.Logging;
using System.Collections.Generic;
using System;

namespace Piece.Framework;

public class Node
{
    private readonly ILogger<Node> _logger;
    private readonly List<Component> _components;
    private readonly List<Node> _children;
    private readonly RenderManager _renderManager; // Added for rendering management

    public string Name { get; set; } = "New Node";
    public TransformComponent Transform { get; private set; }
    public Node Parent { get; private set; }

    public Node(ILogger<Node> logger, RenderManager renderManager) // RenderManager injected
    {
        _logger = logger;
        _renderManager = renderManager; // Assigned injected RenderManager
        _components = new List<Component>();
        _children = new List<Node>();
        Transform = AddComponent<TransformComponent>(); // Every node has a transform

        _logger.LogInformation("Node '{NodeName}' created.", Name);
    }

    public T AddComponent<T>() where T : Component, new()
    {
        T component = new T { Owner = this, Logger = _logger, RenderManager = _renderManager }; // Inject logger AND RenderManager into component
        _components.Add(component);
        _logger.LogDebug("Component {ComponentName} added to Node '{NodeName}'.", typeof(T).Name, Name);
        return component;
    }

    public T GetComponent<T>() where T : Component
    {
        foreach (var component in _components)
        {
            if (component is T typedComponent)
            {
                return typedComponent;
            }
        }
        return null;
    }

    public void AddChild(Node child)
    {
        if (child.Parent != null)
        {
            child.Parent._children.Remove(child);
        }
        child.Parent = this;
        _children.Add(child);
        _logger.LogDebug("Node '{ChildName}' added as child to Node '{ParentName}'.", child.Name, Name);
    }

    public void RemoveChild(Node child)
    {
        if (_children.Remove(child))
        {
            child.Parent = null;
            _logger.LogDebug("Node '{ChildName}' removed from children of Node '{ParentName}'.", child.Name, Name);
        }
    }

    public void Update(float deltaTime)
    {
        foreach (var component in _components)
        {
            component.Update(deltaTime);
        }
        foreach (var child in _children)
        {
            child.Update(deltaTime);
        }
    }

    public void Render()
    {
        foreach (var component in _components)
        {
            component.Render();
        }
        foreach (var child in _children)
        {
            child.Render();
        }
    }
}
