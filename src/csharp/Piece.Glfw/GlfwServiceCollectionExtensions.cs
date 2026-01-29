using Microsoft.Extensions.DependencyInjection;
using Piece.Core;

namespace Piece.Glfw;

public static class GlfwServiceCollectionExtensions
{
    public static IServiceCollection AddGlfwWindow(this IServiceCollection services)
    {
        IntPtr factoryPtr = GlfwPInvoke.CreateFactory(default);
        NativeCalls.SetWindowFactory(factoryPtr);
        return services;
    }
}
