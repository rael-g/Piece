# Piece Engine Architecture: Project Management Layer Design

## 1. Purpose and Responsibilities

The `Piece.ProjectManagement` layer will provide a high-level abstraction for the concept of a "Piece Project" and its associated lifecycle. It will serve as the central point for managing project-related operations, decoupling the specific editor implementations (visual and CLI) from the underlying details of project structure, asset handling, and engine configuration.

**Key Responsibilities:**
*   **Project Definition:** Define the structure and metadata of a `.pieceproject` file (e.g., project name, version, root directory, default scene, engine configuration).
*   **Project Creation:** Provide functionality to create new Piece projects. Instead of creating files manually, the project manager will leverage the `dotnet` CLI. This includes creating a new C# project from a template (potentially a custom Piece Engine template), adding required NuGet package dependencies, and generating the standard folder structure (`Assets`, `Scenes`, etc.).
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
*   **Project Scaffolding via `dotnet` CLI:** To ensure projects are robust, maintainable, and integrate well with the .NET ecosystem, the project creation process will be built on top of the `dotnet` CLI. Instead of creating `.csproj` and `Program.cs` files manually, the `ProjectManager` will orchestrate commands like:
    *   `dotnet new <template>`: To scaffold a new project. This allows for using standard templates (like `console`) or creating a custom Piece Engine project template in the future for a richer out-of-the-box experience.
    *   `dotnet add package <package-name>`: To add engine dependencies (e.g., `Piece.Framework`, `Piece.Core`) as NuGet packages. This simplifies dependency management and updates.
    *   `dotnet restore`: To ensure all dependencies are correctly downloaded and the project is buildable from the start.
    This approach makes the project manager a facilitator that uses standard .NET tooling, rather than a system that reinvents project management.
*   **Serialization Format:** JSON is recommended for `.pieceproject` files due to its human-readability, ease of parsing in C#, and extensibility.
*   **Default Project Structure:** A standard directory layout will be enforced for new projects (e.g., `Assets/`, `Scenes/`, `Scripts/`, `Config/`, `Build/`).
*   **Error Reporting:** Clear, specific error messages and custom exceptions for project-related failures.

## 6. Automatic Dependency Registration

To simplify the process of adding new engine features and avoid manual editing of the project's `Program.cs` file, an automatic dependency registration mechanism will be implemented using **Source Generators**.

### 6.1. The `IEngineModule` Interface

A new interface, `IEngineModule`, will be defined in a core abstractions package (e.g., `Piece.Core.Abstractions`). This interface will serve as a contract for all modules that need to register services with the application's dependency injection container.

```csharp
public interface IEngineModule
{
    void ConfigureServices(IServiceCollection services, IConfiguration configuration);
}
```

Each NuGet package that provides engine functionality (e.g., `Piece.Box2d`, `Piece.OpenGL`) will include a public, parameterless class that implements this interface. For the source generator to discover these modules, it is recommended to either:
1.  Apply a specific attribute (e.g., `[EngineModuleAttribute]`) to the implementing class.
2.  Have the module class reside in a predefined namespace or follow a naming convention.

### 6.2. Source Generator for Module Discovery and Service Registration

A dedicated **Source Generator NuGet package** (e.g., `Piece.EngineModuleGenerator`) will be created. This source generator will be referenced by the game project and will perform the following actions at **compile time**:

1.  **Scan Compilation:** The source generator will analyze the project's compilation for types that implement `IEngineModule` (and optionally have a specific marker attribute).
2.  **Generate Registration Code:** For each discovered `IEngineModule` implementation, the source generator will generate a static extension method. This generated method will explicitly instantiate each module and call its `ConfigureServices` method.

**Example of Generated Code (by `Piece.EngineModuleGenerator`):**

```csharp
// <auto-generated/>
// This file is generated by Piece.EngineModuleGenerator. Do not edit.

using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using Piece.Core.Abstractions; // Assuming IEngineModule is here

namespace PieceEngine.Generated
{
    public static class GeneratedEngineModules
    {
        public static IServiceCollection AddPieceEngineModules(this IServiceCollection services, IConfiguration configuration)
        {
            // Example: Discovered Piece.Box2d.Box2dEngineModule
            new Piece.Box2d.Box2dEngineModule().ConfigureServices(services, configuration);

            // Example: Discovered Piece.OpenGL.OpenGLEngineModule
            // new Piece.OpenGL.OpenGLEngineModule().ConfigureServices(services, configuration);

            // ... other discovered engine modules

            return services;
        }
    }
}
```

This generated code is then included in the project's compilation, meaning there is no runtime reflection overhead and the solution is fully compatible with AOT compilation.

### 6.3. Project Template and `Program.cs`

The default project template's `Program.cs` will then be concise and AOT-friendly:

```csharp
using Microsoft.Extensions.Hosting;
using PieceEngine.Generated; // Namespace for generated code

public static class Program
{
    public static void Main(string[] args)
    {
        var builder = Host.CreateApplicationBuilder(args);

        // Call the source-generated extension method to register all engine modules
        builder.Services.AddPieceEngineModules(builder.Configuration);

        var host = builder.Build();
        host.Run();
    }
}
```

### 6.4. Impact on Project Management

*   **Project Creation:** The `ProjectManager`'s responsibility for creating new projects remains the same. It will still use `dotnet new` and `dotnet add package`. Additionally, the project template will automatically include a reference to the `Piece.EngineModuleGenerator` NuGet package.
*   **Adding Dependencies:** When the user wants to add a new feature, they simply use the `ProjectManager` (or `dotnet add package`) to add the corresponding NuGet package (e.g., `Piece.Box2d`). Upon the next build, the `Piece.EngineModuleGenerator` will detect the new module and update the generated service registration code. No manual code changes are needed.
*   **Decoupling and AOT Compatibility:** This approach fully decouples the project from specific engine features, eliminates runtime reflection, and ensures compatibility with AOT compilation, aligning with modern .NET best practices.
