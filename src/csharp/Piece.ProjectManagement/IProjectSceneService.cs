using System.Threading.Tasks;

namespace Piece.ProjectManagement;

public interface IProjectSceneService
{
    Task<bool> OpenScene(PieceProject project, string scenePath);
    Task<bool> SaveScene(PieceProject project, string scenePath);
    Task<bool> AddEntity(PieceProject project, string scenePath, string entityType);
    Task<bool> RemoveEntity(PieceProject project, string scenePath, string entityId);
}