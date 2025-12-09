using System;
using Microsoft.Extensions.DependencyInjection;
using Piece.Core.Interop;

namespace Piece.Glfw;

public static class GlfwServiceCollectionExtensions
{
    public static IServiceCollection AddGlfwWindow(this IServiceCollection services)
    {
        IntPtr factoryPtr = GlfwPInvoke.CreateFactory();
        NativeCalls.SetWindowFactory(factoryPtr);
        return services;
    }
}
