using System;


using Microsoft.Extensions.DependencyInjection;
using Piece.Core;

namespace Piece.Box2d;

public static class Box2dServiceCollectionExtensions
{
    public static IServiceCollection AddBox2dPhysics(this IServiceCollection services)
    {
        IntPtr factoryPtr = Box2dPInvoke.CreateFactory();
        NativeCalls.SetPhysicsWorldFactory(factoryPtr);
        return services;
    }
}
