using System.Collections.Generic;
using System.Threading.Tasks;

namespace Piece.ProjectManagement;

public interface IProjectAssetService
{
    Task<bool> ImportAsset(PieceProject project, string sourceFilePath, string assetType);
    Task<IEnumerable<string>> ListAssets(PieceProject project);
    Task<bool> DeleteAsset(PieceProject project, string assetPath);
}