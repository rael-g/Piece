using System.Threading.Tasks;

namespace Piece.ProjectManagement;

public interface IProjectBuildService
{
    Task<bool> BuildProject(PieceProject project, string configuration);
    Task<bool> CleanProject(PieceProject project, string configuration);
}
