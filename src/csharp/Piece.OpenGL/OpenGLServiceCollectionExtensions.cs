using System;
using Microsoft.Extensions.DependencyInjection;
using Piece.Core;

namespace Piece.OpenGL;

public static class OpenGLServiceCollectionExtensions
{
    public static IServiceCollection AddOpenGLRenderer(this IServiceCollection services)
    {
        IntPtr factoryPtr = OpenGLPInvoke.CreateFactory();
        NativeCalls.SetGraphicsDeviceFactory(factoryPtr);
        return services;
    }
}
