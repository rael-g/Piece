using Microsoft.Extensions.Logging;
using System;

namespace Piece.Framework;

public abstract class Component
{
    public Node Owner { get; internal set; }
    internal ILogger Logger { get; set; } // Injected by Node for its components
    internal RenderManager RenderManager { get; set; } // Injected by Node for its components

    public virtual void Start() { }
    public virtual void Update(float deltaTime) { }
    public virtual void Render() { }
    public virtual void Dispose() { } // For resources cleanup
}
