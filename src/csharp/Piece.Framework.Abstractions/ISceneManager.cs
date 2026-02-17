using System.Threading.Tasks;

namespace Piece.Framework.Abstractions;

public interface ISceneManager
{
    Task<bool> LoadScene(string scenePath);
    Task<bool> SaveScene(string scenePath);
    Task<bool> AddEntity(string scenePath, string entityType);
    Task<bool> RemoveEntity(string scenePath, string entityId);
    // More methods like GetActiveScene, CreateNewScene, etc.
}