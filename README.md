# Piece Engine

**A highly modular, cross-platform, and extensible game engine built on a Modular Component Architecture philosophy.**

---

## 🚀 Overview

The Piece Engine is a next-generation game development platform designed for flexibility, performance, and extensibility. Inspired by the Modular Component Architecture philosophy, every component of the engine, from rendering implementations to Piece.Framework (C#) gameplay systems, is designed to be a swappable and extendable unit. This allows developers to fine-tune the engine to their specific needs, integrating preferred technologies and custom solutions with ease.

Built with a hybrid C++ and C# architecture, the Piece Engine leverages the performance of native code for its core systems, which are orchestrated and configured by a powerful and approachable C# framework. Crucially, C++ low-level implementations (like rendering or windowing) are compiled on-demand from source via `MSBuild.targets` within C# NuGet packages, ensuring platform-native compatibility and seamless integration through .NET Dependency Injection.

## ✨ Key Features

*   **Hybrid Language Architecture:** Combines performance-critical C++ (for low-level and Piece.Core layers) with a rich C# framework (for Piece.Framework (C#) game logic and editor tooling).
*   **Multi-Layered Design:** A clear separation of concerns across Low-Level (C++), Piece.Core (C++), Piece.Framework (C#), and Editor (C#) layers.
*   **Ultimate Modularity & Extensibility:**
    *   **Source-Compiled C++ Low-Level Implementations:** C++ low-level implementations (rendering, windowing, physics) are provided as source code within C# NuGet packages. They are compiled on-demand via MSBuild.targets for the target platform, ensuring native performance and maximum compatibility.
    *   **.NET Dependency Injection:** Extensible C# framework for custom components and services, orchestrating the integration of C++ low-level implementations.
    *   **Multi-language Scripting:** Support for various scripting languages via "Bridge Components."
*   **IDE & Platform Agnostic:** Develop on Windows, Linux, or macOS using your preferred IDE (Visual Studio, VSCode, Vim, etc.).
*   **Example-Driven Development (EDD):** A guided development workflow that ensures functionality is tied to concrete, executable examples.

## 🏛️ Architecture

The engine is structured in a multi-layered approach to maximize performance, maintainability, and extensibility:

1.  **Low-Level Layer (C++ Implementations):**
    *   Standalone C++ dynamic libraries (e.g., `wal_glfw.dll`, `ral_opengl.dll`) that strictly implement abstract interfaces (WAL, RAL, PAL). They are completely agnostic to `piece_core` C++.
    *   Export a C-style `CreateFactory()` function to create an instance of their factory implementation.

2.  **Piece.Core (C++):**
    *   Acts as a high-performance, passive service container and orchestrator. It receives already-created C++ factory instances (via P/Invoke from C#) and makes them available through its `ServiceLocator`.
    *   Manages GPU resources, rendering pipeline, and includes a `JobSystem` for multithreading.

3.  **Piece.Framework (C#):**
    *   Acts as the intelligent orchestrator, using .NET Dependency Injection to manage and integrate C++ low-level implementations into the `piece_core`.
    *   Offers a user-friendly API for game developers, abstracting C++ complexities.
    *   Features a Node/Component system, core managers (`GameEngine`, `Scene`, `InputManager`, `AssetManager`, `RenderManager`), and advanced AI systems.
    *   Primary interface for game logic development and backend configuration.

4.  **Visual Editor (C#):**
    *   A C# application built on the Piece.Framework, providing an intuitive graphical interface for scene manipulation, asset management, and content authoring.
    *   Also designed for extensibility, allowing for custom editor tools and workflows.

## 🛠️ Build System

The Piece Engine employs a robust build system designed for hybrid language support, platform agnosticism, and modularity:

*   **Monorepo Structure:** All related code and scripts are kept in a single repository.
*   **Core Tooling:**
    *   **CMake:** Used by C# NuGet packages' `MSBuild.targets` to configure and build C++ low-level projects.
    *   **vcpkg:** Manages all external C++ dependencies (e.g., GLFW, OpenGL) for native implementations. It's invoked by `MSBuild.targets` during the C++ compilation step.
    *   **.NET SDK:** For building and managing C# projects and orchestrating the native C++ builds via NuGet packages.
*   **Hybrid Build Process:**
    *   **C++ Low-Level Implementations:** C++ low-level implementations (WAL, RAL, PAL implementations) are distributed as source code within their respective C# NuGet wrapper packages. When a C# project referencing these NuGet packages is built, the NuGet package's `MSBuild.targets` file automatically invokes CMake and vcpkg to compile the native C++ code into dynamic libraries (`.dll`, `.so`) tailored for the target platform. These compiled native libraries are then placed in the C# project's output directory.
    *   **C# Projects:** Standard .NET SDK build process.

## 🚀 Development Workflow

The development process is guided by several key methodologies and practices:

*   **Example-Driven Development (EDD):**
    *   Progressively builds engine features starting from low-level functionalities upwards.
    *   Each development cycle begins with defining a simple, executable example (e.g., "Open a window," "Draw a red triangle").
    *   Focuses on implementing the minimum necessary software for the example and developing tests concurrently.
*   **GitHub Flow:**
    *   `main` branch represents the latest stable, production-ready version.
    *   Feature branches are created from `main` for all new features, bug fixes, or examples.
    *   All changes are merged via Pull Requests (PRs) after code review and passing CI checks.
*   **Conventional Commits:** All commit messages adhere to the Conventional Commits specification, enabling automated changelog generation and semantic versioning.

## 🧪 Testing Strategy

A comprehensive testing strategy ensures the reliability and correctness of the Piece Engine:

*   **Unit Tests:** Verify the correctness of individual components, functions, or classes across all layers (Low-Level C++, Piece.Core (C++), Piece.Framework (C#)), often developed concurrently with features via EDD.
*   **Integration Tests:** Validate the correct interaction between multiple components or layers (e.g., C++ `ResourceManager` with RAL, C# `GameEngine` lifecycle).
*   **End-to-End Tests:** Cover full system functionality, including game loop execution, specific rendering scenarios, platform compatibility, and performance metrics.
*   **Testing Tools:** CTest for C++ tests, `dotnet test` for C# tests.
*   **Test Documentation:** Details on the comprehensive testing strategy are integrated within the Example-Driven Development documentation.

## ⚙️ CI/CD & Versioning

The Piece Engine utilizes an automated Continuous Integration/Continuous Delivery (CI/CD) pipeline with a robust versioning strategy:

*   **GitHub Actions:** The primary tool for orchestrating automated builds, tests, and releases.
*   **Continuous Integration (CI):** Pull Request pipelines ensure every code change is automatically built and tested before merging into `main`.
*   **Continuous Delivery (CD):** The `main` branch pipeline automates the release process, including:
    *   **Semantic Versioning (SemVer):** Version numbers (`MAJOR.MINOR.PATCH`) are automatically determined based on Conventional Commits.
    *   **`semantic-release`:** Automates version bumping, `CHANGELOG.md` generation, Git tagging, and GitHub Release creation.
    *   **Production Packaging:** Automated creation of "lean packages" (optimized for distribution) and artifact publishing to GitHub Releases.

## 🚀 Getting Started

To get started with the Piece Engine, follow these high-level steps:

1.  **Clone the Repository:**
    ```bash
    git clone https://github.com/rael-g/Piece.git
    cd Piece
    ```
2.  **Requirements** (For Windows, Linux, macOS)
    1.  [vcpkg](https://learn.microsoft.com/vcpkg/get_started/get-started) - Ensure `VCPKG_ROOT` environment variable is set.
    2.  [CMake](https://github.com/Kitware/CMake)
    3.  [.NET SDK 8.0+](https://dotnet.microsoft.com/download)

3.  **Build C# Projects (which includes C++ low-level implementations):**
    *   Navigate to the solution directory (or your specific C# project).
    *   Run `dotnet build` from your terminal. This will automatically trigger the CMake/vcpkg build process for C++ low-level implementations via NuGet's `MSBuild.targets`.
    *   Example: `dotnet build tests/csharp/Piece.ExampleGame/Piece.ExampleGame.csproj`

4.  **Run Your Game/Example:**
    *   Executables for your C# project (and the compiled native DLLs) will be in the project's output directory (e.g., `tests/csharp/Piece.ExampleGame/bin/Debug/net8.0/`).
    *   Example: `dotnet run --project tests/csharp/Piece.ExampleGame/Piece.ExampleGame.csproj`

*Refer to [BuildSystem](docs/Development/BuildSystem.md) for detailed build information.*

## 👋 Contributing

We welcome contributions to the Piece Engine! Please adhere to the following guidelines:

*   **Example-Driven Development:** Start by defining clear examples for new features or bug fixes.
*   **Conventional Commits:** Ensure all your commit messages follow the Conventional Commits specification.
*   **GitHub Flow:** Create feature branches for your work and submit Pull Requests to the `main` branch.
*   **Code Style:**
    *   **General:** Please ensure all code is formatted correctly *before* committing. This helps maintain consistency and readability across the codebase.
    *   **C++:** Adheres to the Microsoft C++ Style Guide. Formatting is enforced using `clang-format`. To format C++ code:
        ```bash
        cmake --build build --preset <preset> --target format_cpp
        ```
        Ensure `clang-format` is installed and accessible in your system's PATH.
    *   **C#:** Follows standard .NET coding conventions. To format C# code:
        ```bash
        dotnet format
        ```
    *   **Comments:** Follows the "Clean Code" philosophy. Comments should explain *why* something is done, not *what* is being done. Avoid obvious or redundant comments; the code should be self-documenting.
*   **Testing:** Write comprehensive unit and integration tests for your changes.

*Refer to [ExampleDrivenDevelopment](docs/Development/ExampleDrivenDevelopment.md) and [VersioningStrategy](docs/Development/VersioningStrategy.md) for more details on development practices.*

## 📄 License

This project is licensed under the [MIT License](LICENSE).

---