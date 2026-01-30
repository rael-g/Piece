# Piece Engine Architecture: Project Management Layer Design

## 1. Purpose and Responsibilities

The `Piece.ProjectManagement` layer will provide a high-level abstraction for the concept of a "Piece Project" and its associated lifecycle. It will serve as the central point for managing project-related operations, decoupling the specific editor implementations (visual and CLI) from the underlying details of project structure, asset handling, and engine configuration.

**Key Responsibilities:**
*   **Project Definition:** Define the structure and metadata of a `.pieceproject` file (e.g., project name, version, root directory, default scene, engine configuration).
*   **Project Creation:** Provide functionality to create new Piece projects with a predefined structure (e.g., project file, default folders like `Assets`, `Scenes`, `Scripts`).
*   **Project Loading/Unloading:** Handle the parsing and deserialization of `.pieceproject` files to load project state into memory. Manage the lifecycle of resources associated with an open project.
*   **Project Saving:** Serialize and write the current project state back to the `.pieceproject` file.
*   **Asset Management Integration:** Provide an interface for interacting with the `AssetManager` (from `Piece.Framework`) for project-specific asset operations (e.g., import, delete, list assets within the project context).
*   **Scene Management Integration:** Provide an interface for interacting with the `SceneManager` (from `Piece.Framework`) for project-specific scene operations (e.g., open scene, save scene, add/remove entities).
*   **Configuration Management:** Manage project-specific configurations that might influence the `Piece.Framework` or `Piece.Core`.
*   **Error Handling and Validation:** Ensure project integrity and provide robust error reporting for project-related operations.

## 2. Project Structure (C#)

The `Piece.ProjectManagement` project will contain both interfaces and their concrete implementations.

*   `PieceProject.cs`: Data model for the `.pieceproject` file, including properties like `Name`, `RootPath`, `DefaultScenePath`, `EngineConfiguration`, etc.
*   `ProjectOptions.cs`: Data structure for project creation/loading options (e.g., `TemplateType`, `ProjectName`, `OutputPath`).
*   `IProjectManager.cs`: Interface defining core project operations.
    ```csharp
    public interface IProjectManager
    {
        PieceProject CreateProject(ProjectOptions options);
        PieceProject LoadProject(string projectFilePath);
        void SaveProject(PieceProject project);
        // Potentially methods for asset/scene operations, or separate services for them.
        IProjectAssetService GetAssetService(PieceProject project);
        IProjectSceneService GetSceneService(PieceProject project);
    }
    ```
*   `ProjectManager.cs`: Concrete implementation of `IProjectManager`. This class will handle file I/O for `.pieceproject` files, interact with `Piece.Framework` components, and manage default project structures.
*   `IProjectAssetService.cs`: Interface for project-specific asset operations.
    ```csharp
    public interface IProjectAssetService
    {
        void ImportAsset(PieceProject project, string sourceFilePath, string assetType);
        // ... other asset operations like ListAssets, DeleteAsset
    }
    ```
*   `ProjectAssetService.cs`: Implementation of `IProjectAssetService`, wrapping `Piece.Framework.AssetManager`.
*   `IProjectSceneService.cs`: Interface for project-specific scene operations.
    ```csharp
    public interface IProjectSceneService
    {
        void OpenScene(PieceProject project, string scenePath);
        void SaveScene(PieceProject project, string scenePath);
        // ... other scene operations like AddEntity, RemoveEntity
    }
    ```
*   `ProjectSceneService.cs`: Implementation of `IProjectSceneService`, wrapping `Piece.Framework.SceneManager`.
*   `ProjectServiceCollectionExtensions.cs`: Extension methods for configuring `Piece.ProjectManagement` services in a .NET `IServiceCollection` (for dependency injection).

## 3. Interactions with Lower Layers (`Piece.Framework`)

`Piece.ProjectManagement` will depend on and consume `Piece.Framework`. For example:
*   When a project is loaded, `ProjectManager` might configure `Piece.Framework`'s `AssetManager` with project-specific asset roots.
*   Operations like "import asset" initiated by `IProjectAssetService` will internally call the `AssetManager` from `Piece.Framework` to perform the actual asset processing.
*   Opening/saving a scene will involve invoking `SceneManager` from `Piece.Framework`.

## 4. Interactions with Upper Layers (`Piece.Editor`, `Piece.Cli`)

Both `Piece.Editor` and `Piece.Cli` will depend directly on the `Piece.ProjectManagement` project. They will use dependency injection to obtain instances of `IProjectManager`, `IProjectAssetService`, and `IProjectSceneService` to perform all project-related tasks.

## 5. Design Considerations

*   **Stateless CLI Support:** `IProjectManager` methods for loading and saving will operate on explicit `PieceProject` objects or file paths passed as arguments. Each CLI command will load the project, perform its operation, and then potentially save it, making each command invocation self-contained.
*   **Serialization Format:** JSON is recommended for `.pieceproject` files due to its human-readability, ease of parsing in C#, and extensibility.
*   **Default Project Structure:** A standard directory layout will be enforced for new projects (e.g., `Assets/`, `Scenes/`, `Scripts/`, `Config/`, `Build/`).
*   **Error Reporting:** Clear, specific error messages and custom exceptions for project-related failures.
