using System;
using Microsoft.Extensions.Logging;
using Piece.Core; // For NativeCalls
using System.Collections.Generic;

namespace Piece.Framework;

/// <summary>
/// Manages loading and caching of assets.
/// </summary>
public class AssetManager
{
    private readonly ILogger<AssetManager> _logger;
    private IntPtr _nativeEngineCorePtr;

    // A simple cache for now. In a real engine, this would be more sophisticated.
    private readonly Dictionary<string, Mesh> _meshCache = new Dictionary<string, Mesh>();
    private readonly Dictionary<string, Material> _materialCache = new Dictionary<string, Material>();
    // For now, we'll use our simple Material and Mesh classes.
    // When real C++ wrappers are implemented, these would become wrapper types.

    public AssetManager(ILogger<AssetManager> logger)
    {
        _logger = logger;
    }

    /// <summary>
    /// Initializes the AssetManager with the native engine core pointer.
    /// This should be called by the GameEngine after successful native engine initialization.
    /// </summary>
    /// <param name="nativeEngineCorePtr">The raw pointer to the native C++ EngineCore instance.</param>
    public void Initialize(IntPtr nativeEngineCorePtr)
    {
        _nativeEngineCorePtr = nativeEngineCorePtr;
        _logger.LogInformation("AssetManager initialized with native engine core pointer: {Pointer}", _nativeEngineCorePtr);
    }

    /// <summary>
    /// Loads a mesh asset from the specified path.
    /// </summary>
    /// <param name="path">The path to the mesh asset.</param>
    /// <returns>The loaded Mesh object, or null if loading fails.</returns>
    public Mesh LoadMesh(string path)
    {
        if (_nativeEngineCorePtr == IntPtr.Zero)
        {
            _logger.LogWarning("LoadMesh called before AssetManager is initialized.");
            return null;
        }

        if (_meshCache.TryGetValue(path, out var cachedMesh))
        {
            _logger.LogDebug("Returning cached mesh for path: {Path}", path);
            return cachedMesh;
        }

        _logger.LogInformation("Attempting to load mesh from path: {Path}", path);
        IntPtr nativeMeshPtr = NativeCalls.EngineLoadMesh(_nativeEngineCorePtr, path);

        if (nativeMeshPtr == IntPtr.Zero)
        {
            _logger.LogError("Failed to load native mesh from path: {Path}", path);
            return null;
        }

        var mesh = new Mesh(System.IO.Path.GetFileNameWithoutExtension(path), nativeMeshPtr);
        _meshCache[path] = mesh;

        _logger.LogInformation("Successfully loaded mesh '{MeshName}' from path: {Path}. Native pointer: {NativePtr}", mesh.Name, path, nativeMeshPtr);
        return mesh;
    }

    /// <summary>
    /// Loads a material asset from the specified path.
    /// </summary>
    /// <param name="path">The path to the material asset.</param>
    /// <returns>The loaded Material object, or null if loading fails.</returns>
    public Material LoadMaterial(string path)
    {
        if (_nativeEngineCorePtr == IntPtr.Zero)
        {
            _logger.LogWarning("LoadMaterial called before AssetManager is initialized.");
            return null;
        }

        if (_materialCache.TryGetValue(path, out var cachedMaterial))
        {
            _logger.LogDebug("Returning cached material for path: {Path}", path);
            return cachedMaterial;
        }

        _logger.LogInformation("Attempting to load material from path: {Path}", path);
        IntPtr nativeMaterialPtr = NativeCalls.EngineLoadMaterial(_nativeEngineCorePtr, path);

        if (nativeMaterialPtr == IntPtr.Zero)
        {
            _logger.LogError("Failed to load native material from path: {Path}", path);
            return null;
        }

        var material = new Material(System.IO.Path.GetFileNameWithoutExtension(path), nativeMaterialPtr);
        _materialCache[path] = material;

        _logger.LogInformation("Successfully loaded material '{MaterialName}' from path: {Path}. Native pointer: {NativePtr}", material.Name, path, nativeMaterialPtr);
        return material;
    }

    // TODO: Implement LoadTexture when C# wrapper for C++ ITexture is available.
    // TODO: Implement proper resource cleanup and IDisposable pattern for C# wrappers.
}
