# Piece Engine Architecture: CLI Editor Design

## 1. Purpose and Responsibilities

The `Piece.Cli` editor will provide a command-line interface for interacting with the Piece Engine for automation, scripting, and development workflows. It will expose a set of stateless commands that operate on specific projects, mimicking the behavior of tools like `dotnet`.

**Key Responsibilities:**
*   **Command Parsing:** Parse command-line arguments and map them to specific engine operations.
*   **Command Execution:** Invoke the appropriate services from the `Piece.ProjectManagement` layer to perform requested operations.
*   **Output Management:** Present clear and concise feedback to the user on the success or failure of operations.
*   **Error Handling:** Catch and report errors gracefully.

## 2. Project Structure (C#)

*   `Piece.Cli` (New C# Console Application Project):
    *   **`Program.cs`:**
        *   Main entry point of the CLI application.
        *   Configures a .NET `HostBuilder`.
        *   Registers services from `Piece.ProjectManagement` via its `ProjectServiceCollectionExtensions`.
        *   Sets up `System.CommandLine` with the defined commands.
        *   Executes the command-line parser.
    *   **`Commands/`:** Directory to hold definitions for individual commands. Each command will typically:
        *   Derive from a base `Command` class provided by `System.CommandLine`.
        *   Define its name, description, arguments, and options.
        *   Implement a `SetHandler` or `InvokeAsync` method that:
            *   Resolves necessary services (e.g., `IProjectManager`, `IProjectAssetService`) via dependency injection.
            *   Loads the target `PieceProject` if a `--project` option is provided.
            *   Performs the core logic by calling methods on the resolved services.
            *   Provides console output based on the operation's outcome.
        *   `NewProjectCommand.cs`: Implements the `piece project new` command.
        *   `BuildProjectCommand.cs`: Implements the `piece build` command.
        *   `ImportAssetCommand.cs`: Implements the `piece asset import` command.
        *   `ListAssetsCommand.cs`: Implements the `piece asset list` command.
        *   ... (other commands as needed)
    *   **`CliServiceCollectionExtensions.cs`:** Extension methods for registering CLI-specific services (if any) or grouping command registrations.

## 3. Interactions with Lower Layers (`Piece.ProjectManagement`)

`Piece.Cli` will directly depend on the `Piece.ProjectManagement` project. All project-related commands will internally utilize instances of `IProjectManager`, `IProjectAssetService`, and `IProjectSceneService` (obtained via dependency injection) to perform their tasks.

## 4. Command-Line Syntax (Example):

The command structure will use subcommands for better organization and clarity.

```bash
piece <command> [<args>]
piece <command> <subcommand> [<args>]
```

**Examples:**

*   **Create a new project:**
    ```bash
    piece project new MyGame --path ./Projects/MyGame --template Basic
    ```
    (This command would resolve `IProjectManager` and call `projectManager.CreateProject(options)`.)

*   **Build a specific project:**
    ```bash
    piece build --project ./Projects/MyGame/MyGame.pieceproject --configuration Release
    ```
    (This command would resolve `IProjectManager`, load the project, and then call an internal build method that potentially uses `Piece.Framework` to trigger the build process.)

*   **Import an asset into a project:**
    ```bash
    piece asset import ./Textures/player.png --type Texture --project ./Projects/MyGame/MyGame.pieceproject
    ```
    (This command would resolve `IProjectManager` and `IProjectAssetService`, load the project, and then call `assetService.ImportAsset(project, sourceFilePath, assetType)`.)

*   **List assets in a project:**
    ```bash
    piece asset list --project ./Projects/MyGame/MyGame.pieceproject
    ```
    (This command would resolve `IProjectManager` and `IProjectAssetService`, load the project, and then call `assetService.ListAssets(project)` to retrieve and display a list of assets within the project.)

## 5. Design Considerations

*   **`System.CommandLine`:** This library will be central to handling command-line parsing, argument validation, and help text generation. Its integration with .NET DI makes it ideal for injecting the `Piece.ProjectManagement` services.
*   **Statelessness:** Each command execution is designed to be self-contained and perform a single, finite task. The `--project` option will be crucial for targeting specific projects. The `IProjectManager` will handle loading and saving the project state for the duration of a single command's execution, ensuring no persistent state is maintained between separate command invocations.
*   **User Feedback:** Commands should provide clear, actionable success/failure messages and utilize appropriate console output formatting (e.g., colors for warnings/errors).
*   **Extensibility:** New commands can be easily added by creating new classes in the `Commands/` directory and registering them in `Program.cs`.
*   **Configuration:** The CLI might utilize `appsettings.json` for its own configurations (e.g., default log levels, paths to engine binaries).
