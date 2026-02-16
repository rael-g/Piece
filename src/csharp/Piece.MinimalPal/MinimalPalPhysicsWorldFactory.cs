using System;
using System.Runtime.InteropServices;
using Microsoft.Extensions.Options;
using Piece.Core.Abstractions;
using Piece.Core.Abstractions.NativeCalls; // For NativePhysicsOptions

namespace Piece.MinimalPal;

public class MinimalPalPhysicsWorldFactory : IPhysicsWorldFactory
{
    private IntPtr _nativeFactoryPtr;
    private IntPtr _optionsPtr = IntPtr.Zero;

    public MinimalPalPhysicsWorldFactory(IOptions<MinimalPalOptions> minimalPalOptions)
    {
        var options = minimalPalOptions.Value;

        // Marshal C# options to C++ NativePhysicsOptions struct
        _optionsPtr = Marshal.AllocHGlobal(Marshal.SizeOf<NativePhysicsOptions>());
        var nativeOptions = new NativePhysicsOptions
        {
            fixed_delta_time = options.FixedDeltaTime,
            max_physics_steps = options.MaxPhysicsSteps
        };

        Marshal.StructureToPtr(nativeOptions, _optionsPtr, false);

        _nativeFactoryPtr = MinimalPalPInvoke.CreateFactory(_optionsPtr);

        if (_nativeFactoryPtr == IntPtr.Zero)
        {
            throw new InvalidOperationException("Failed to create native Minimal PAL physics world factory.");
        }
    }

    public IntPtr GetNativeFactoryPtr()
    {
        return _nativeFactoryPtr;
    }

    public void Dispose()
    {
        if (_nativeFactoryPtr != IntPtr.Zero)
        {
            MinimalPalPInvoke.DestroyFactory(_nativeFactoryPtr);
            _nativeFactoryPtr = IntPtr.Zero;
        }

        // Free allocated unmanaged memory
        if (_optionsPtr != IntPtr.Zero)
        {
            Marshal.FreeHGlobal(_optionsPtr);
            _optionsPtr = IntPtr.Zero;
        }

        GC.SuppressFinalize(this);
    }

    ~MinimalPalPhysicsWorldFactory()
    {
        Dispose();
    }
}
