# Project Structure and Build System Design

This document describes the architectural decisions regarding the project structure, build system, and integration strategy for the Piece Engine. The design is centered around three main requirements:
1.  **Hybrid Language Support:** Integrate C++ for the core engine and C# for high-level logic and tools.
2.  **IDE & Platform Agnosticism:** Allow any developer to contribute using their preferred editor (VSCode, Visual Studio, etc.) and operating system (Windows, Linux).
3.  **Modular Component Architecture:** The build system must support the engine's core philosophy of modularity, where components (especially rendering backends) are plugins selected at runtime.

---

## 1. Directory Structure

A monorepo structure is used to keep all related code and scripts in a single location. The management of external C++ libraries is handled by `vcpkg`, making a dedicated `external/` directory unnecessary.

```
/Piece/
├── .gitignore
├── CMakeLists.txt            # ⬅️ Primary C++ build orchestrator with CMake.
├── CMakePresets.json         # ⬅️ Standardized build configurations.
├── vcpkg.json                # ⬅️ C++ dependency manifest for vcpkg.
│
├── docs/
│   └── Development/
│       └── BuildSystem.md    # This file.
│
├── src/                      # ⬅️ All engine source code.
│   │
│   ├── cpp/                  # C++ source code.
│   │   ├── pal/                # Physics Abstraction Layer.
│   │   ├── ral/                # Rendering Abstraction Layer.
│   │   ├── wal/                # Windowing Abstraction Layer.
│   │   └── piece_intermediate/ # C++ intermediate orchestration layer.
│   │
│   └── csharp/               # C# source code.
│       ├── Piece.Core.Interop/ # Interoperability project, P/Invoke for the C++ layer.
│       ├── Piece.Engine/       # High-level C# framework (public API).
│       └── Piece.Editor/       # The Visual Editor application.
│
└── build/                    # ⬅️ Build output directory (ignored by Git).
```

---

## 2. Tools and Build Philosophy

### Primary Tool: CMake
**CMake** is the primary orchestrator for the C++ build. It manages complex C++ projects, is cross-platform, and provides the scripting capabilities necessary for our hybrid architecture.

### C++ Dependency Management: vcpkg
All external C++ libraries (e.g., GLFW, GLM) are managed via **vcpkg** in manifest mode. A `vcpkg.json` file in the project root declares all dependencies.

### IDE and Platform Agnosticism
CMake, along with `CMakePresets.json`, ensures that the project is not tied to any specific IDE, allowing developers to use Visual Studio, VSCode, or other editors on Windows and Linux.

---

## 3. Build Architecture: From Source to Final Artifacts

The build architecture is designed to support both local development and the delivery of robust packages for end-users and developers.

### 3.1. Build Phases

The compilation process is divided into two main phases:

1.  **Native Compilation (C++):** Using CMake, all core engine components and backends (e.g., `wal/glfw_backend`) are compiled into native libraries (`.dll` on Windows, `.so` on Linux). This process is self-contained and produces the low-level binaries.
2.  **Managed Compilation (C#):** Using the .NET SDK, C# projects are compiled. The `Piece.Core.Interop` layer defines P/Invoke signatures to load and interact with the C++ libraries compiled in the previous phase.

In the CI environment, these phases are executed sequentially to ensure all tests pass before proceeding to packaging.

### 3.2. Production Artifacts

When a release is created, the Continuous Deployment (CD) pipeline is responsible for generating two distinct types of artifacts:

**1. The Standalone Editor**

This is the "download, unzip, and run" package.
*   **Process:** The pipeline uses the `dotnet publish` command on the `Piece.Editor` project.
*   **Content:** This command creates a self-contained folder that includes:
    *   The `Piece.Editor` executable.
    *   All managed C# libraries (.DLLs) it depends on.
    *   **The necessary native C++ binaries** for the target platform (e.g., `win-x64`), which are copied into the publish folder.
*   **Final Result:** The entire publish folder is compressed into a single `.zip` file (e.g., `PieceEngine-Editor-win-x64.zip`), ready for distribution.

**2. NuGet Packages (For Developers)**

These are for developers who wish to use the `PieceEngine` as a framework in their own .NET games or applications.
*   **Process:** The pipeline uses the `dotnet pack` command on the C# library projects (e.g., `Piece.Engine`, `Piece.Vulkan`).
*   **Content:** Each `.nupkg` package contains:
    *   The C# library DLLs.
    *   A configuration that instructs NuGet to include the correct native C++ binaries (`.dll`/`.so`) when packing a consuming project.
*   **Final Result:** A set of versioned `.nupkg` packages, ready to be published to a registry like `NuGet.org`.

**3. Vcpkg Packages (For C++ Developers)**

In addition to serving the C# ecosystem, the CD pipeline will also generate packages for C++ developers using `vcpkg`.
*   **Process:** The pipeline will create and publish a `vcpkg` port for the `PieceEngine`. This involves generating the necessary `portfile.cmake` and manifest files that instruct `vcpkg` on how to build and install the engine.
*   **Content:** The port enables developers to easily consume the C++ core libraries of the Piece Engine. By adding `pieceengine` to their `vcpkg.json` manifest and running `vcpkg install`, they gain access to the engine's headers and linkable libraries.
*   **Final Result:** A versioned `vcpkg` port published to a registry, allowing any C++ developer to integrate Piece Engine into their project with a single command.

### 3.3. Runtime Orchestration (C# DI-driven)

As planned, the selection and configuration of C++ backends are controlled by the C# host through .NET Dependency Injection (DI).

1.  **Service Registration:** The C# application (whether the Editor or a game) registers the desired backends by calling extension methods on the `IServiceCollection` (e.g., `services.AddPieceVulkan()`, `services.AddPieceGlfw()`).
2.  **C# to C++ Bridge:** During initialization, the C# interoperability layer loads the native C++ libraries and passes configuration to the C++ intermediate layer.
3.  **C++ Resolution:** The C++ intermediate layer then uses the received configurations to create and provide the correct implementations of interfaces (e.g., `IGraphicsDevice`, `IWindow`).

This approach offers a flexible and type-safe way to configure the engine, aligning perfectly with modern .NET development practices and the philosophy of modular component architecture for the Piece Engine.

---

## Related Documentation

*   [Versioning Strategy (VersioningStrategy.md)](../Development/VersioningStrategy.md)
*   [CI/CD Strategy (CiCdStrategy.md)](../Development/CiCdStrategy.md)
