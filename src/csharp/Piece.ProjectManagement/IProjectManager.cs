using System.Threading.Tasks;

namespace Piece.ProjectManagement;

public interface IProjectManager
{
    Task<PieceProject> CreateProject(string name, string path, string templateName);
    Task<PieceProject?> LoadProject(string projectFilePath);
    Task SaveProject(PieceProject project);
    IProjectAssetService GetAssetService(PieceProject project);
    IProjectSceneService GetSceneService(PieceProject project);
    Task<bool> AddModule(PieceProject project, string moduleName);
    Task<bool> RemoveModule(PieceProject project, string moduleName);
    Task<bool> DeleteProject(string projectPath);
}