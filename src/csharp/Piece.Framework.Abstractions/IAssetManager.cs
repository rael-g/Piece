using System.Collections.Generic;
using System.Threading.Tasks;

namespace Piece.Framework.Abstractions;

public interface IAssetManager
{
    Task<T?> LoadAsset<T>(string assetPath);
    Task<IEnumerable<string>> ListAvailableAssets(string assetTypeFilter = "");
    // More methods like UnloadAsset, GetAssetMetadata, etc.
}
