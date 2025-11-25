# Project Structure and Build System Design

This document outlines the architectural decisions regarding the project structure, build system, and integration strategy for the Piece Engine. The design is centered around three core requirements:
1.  **Hybrid Language Support:** Seamlessly integrate C++ for the core engine and C# for high-level logic and tools.
2.  **IDE & Platform Agnosticism:** Allow any developer to contribute using their preferred editor (VSCode, Visual Studio, Vim, etc.) and operating system (Windows, Linux, macOS).
3.  **Modular Component Architecture Philosophy:** The build system must support the engine's core philosophy of true modularity, where components, especially rendering backends, are plugins selected at runtime, not compile time.

---

## 1. Directory Structure

A monorepo structure will be used to keep all related code and scripts in a single, manageable location. The management of external C++ libraries will be handled by `vcpkg`, making a dedicated `external/` directory unnecessary.

```
/Piece/
├── .gitignore
├── CMakeLists.txt            # ⬅️ Root CMake file: The main build orchestrator.
├── CMakePresets.json         # ⬅️ Standardized build configurations.
├── vcpkg.json                # ⬅️ C++ dependency manifest for vcpkg.
│
├── docs/                     # Project documentation.
│   └── Development/
│       └── BUILD_SYSTEM.md   # This file.
│
├── src/                      # ⬅️ All engine source code.
│   │
│   ├── cpp/                  # C++ source code.
│   │   ├── Piece.Core/         # Shared C++ logic, types, etc.
│   │   ├── Piece.Intermediate/ # The intermediate orchestration layer.
│   │   │
│   │   └── Backends/           # Rendering backend plugins.
│   │       ├── opengl/         # OpenGL backend implementation (compiles to gfx_opengl.dll).
│   │       └── vulkan/         # Vulkan backend implementation (compiles to gfx_vulkan.dll).
│   │
│   └── csharp/               # C# source code.
│       ├── Piece.sln           # C# solution for IDE convenience.
│       ├── Piece.Engine/       # High-level C# framework (the public API).
│       └── Piece.Editor/       # The Visual Editor application.
│
└── build/                    # ⬅️ Build output directory (untracked by Git).
```

---

## 2. Build Philosophy and Tooling

### Core Tool: CMake
**CMake** will be the primary build system orchestrator. It excels at managing complex C++ projects, is cross-platform, and provides powerful scripting capabilities necessary for our hybrid architecture.

### C++ Dependency Management: vcpkg
All external C++ libraries (e.g., GLFW, GLM, etc.) will be managed via **vcpkg** in its manifest-based mode. A `vcpkg.json` file in the project root will declare all dependencies. The CMake configuration will be integrated with the vcpkg toolchain, which will automatically download, build, and link the required libraries. This ensures a consistent and reproducible dependency environment for all developers.

### IDE & Platform Agnosticism
CMake itself ensures that the project is not tied to any single IDE. It uses **Generators** to create project files native to a specific environment. For example:
- A developer on Windows using Visual Studio can run `cmake -G "Visual Studio 17 2022" ..` to generate a `.sln` file.
- A developer on Linux or using VSCode can run `cmake -G "Ninja" ..` to generate build files for the fast Ninja build tool.
- The **VSCode CMake Tools extension** integrates seamlessly with this workflow by reading `CMakeLists.txt` and `CMakePresets.json` directly, offering a first-class development experience.

### Build Configuration: CMakePresets.json
To provide a consistent and easy-to-use build configuration experience across all platforms and IDEs, we will use a `CMakePresets.json` file at the root of the project. This file replaces custom build scripts and is the modern, standard way to define configure, build, and test options.

**Conceptual `CMakePresets.json`:**
```json
{
  "version": 3,
  "configurePresets": [
    {
      "name": "windows-debug",
      "displayName": "Windows Debug",
      "generator": "Ninja",
      "binaryDir": "${sourceDir}/build",
      "cacheVariables": { "CMAKE_BUILD_TYPE": "Debug" },
      "toolchainFile": "$env{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
    }
  ],
  "buildPresets": [ /* ... */ ],
  "installPresets": [
    {
      "name": "package-opengl-release",
      "configurePreset": "windows-release",
      "components": ["Core", "OpenGL"]
    }
  ]
}
```

---

## 3. Realizing the Modular Component Architecture Philosophy: Build, Packaging, and C# DI Orchestration

The philosophy of **Modular Component Architecture** of the Piece Engine, enabling deep modularity and extensibility, is powerfully realized through a refined build and packaging system that integrates tightly with the C# .NET Dependency Injection (DI) mechanism. This approach ensures that developers can easily "plug and play" C++ backends and other native components directly from their C# projects.

The build system's role is to produce all available native C++ DLLs. The packaging and selection of these components are then primarily managed through C# NuGet packages and the .NET DI system, empowering the C# host application to orchestrate which specific native backends are used and how they are configured.

### 3.1. Development Workflow: The "Fat Build"

The default build process for native C++ components is optimized for developers.
1.  **Build All C++ Plugins:** CMake is configured to automatically find and compile every native backend (e.g., in `src/cpp/Backends/`) into a separate DLL **unconditionally**. This ensures all interchangeable components are available.
2.  **Orchestrate Hybrid Compilation:** A single build command (`cmake --build .`) compiles all C++ targets and also invokes `dotnet build` on the C# solution.
3.  **Intermediate Output:** All compiled C++ artifacts, including **all backend DLLs**, are placed in an intermediate build directory (e.g., `build/bin/Debug`). These DLLs then serve as inputs for their respective C# NuGet packages.

This "fat build" ensures that all native backends are compiled and ready to be consumed by the various C# wrapper NuGet packages. The actual selection and deployment into a C# project's output will be handled by the NuGet system.

### 3.2. Production Workflow: The "Lean Package" via NuGet

For a final production release, a "lean package" containing only the necessary native DLLs is desired. With the C# DI-driven architecture, this is implicitly handled by the .NET packaging and dependency management system (NuGet).

1.  **NuGet Package Creation:** Each native C++ backend (e.g., `gfx_vulkan.dll`) is wrapped in its own C# NuGet package (e.g., `Piece.Vulkan`). This NuGet package's `.csproj` configuration specifies that the native DLL should be included in the package and deployed to the `runtimes/{rid}/native/` folder of the consuming project.
2.  **C# Host Application Packaging:** When a user's C# game or application builds and publishes, the .NET SDK automatically includes only the native DLLs that are direct or transitive dependencies of its NuGet references.
    *   If the C# application uses `Piece.Vulkan` and `Piece.GLFW`, only `gfx_vulkan.dll` and `gfx_glfw.dll` (along with `Piece.Intermediate.dll` and other core DLLs) will be included in the final output. Unused backend DLLs (e.g., `gfx_opengl.dll` if `Piece.OpenGL` was not referenced) will not be present.
3.  **Resulting Lean Package:** The final deployed application contains only the native interchangeable components (DLLs) that were explicitly chosen by the C# application's NuGet dependencies and DI configuration. This eliminates the need for manual `cmake --install --component` selections for the primary deployment scenario.

While `cmake --install --component` remains available for specific C++-only deployments or development-time testing of native artifacts, the primary mechanism for delivering and managing native backend dependencies within the Piece Engine ecosystem is through NuGet.

### 3.3. Runtime Selection and Configuration (C# DI-driven)

The runtime selection and configuration of C++ backends are now handled entirely by the C# host application's .NET Dependency Injection (DI) setup. The `engine.json` file is no longer used for backend selection.

1.  **Backend Registration:** The C# application explicitly registers the desired backends by calling extension methods (e.g., `services.AddPieceVulkan()`, `services.AddPieceGlfw()`) on the `IServiceCollection`. These methods typically accept configuration options.
    ```csharp
    Host.CreateDefaultBuilder(args)
        .ConfigureServices((hostContext, services) => {
            services.AddPieceVulkan(options => {
                options.EnableValidationLayers = hostContext.HostingEnvironment.IsDevelopment();
            });
            services.AddPieceGlfw(options => {
                options.InitialWindowWidth = 1280;
                options.InitialWindowHeight = 720;
            });
            // ... add other Piece Engine services
        })
        .Build()
        .Run();
    ```
2.  **C# to C++ Bridge:** During the C# `GameEngine`'s initialization, it resolves the configured C# factory wrappers for the C++ backends. It then uses P/Invoke to pass the raw C++ factory pointers (obtained from the native DLLs) and marshaled configuration options to the C++ `ServiceLocator` in the intermediate layer.
3.  **C++ Resolution:** The C++ intermediate layer then retrieves these pre-configured factories from its `ServiceLocator` to create the actual `IGraphicsDevice`, `IWindow`, and other core C++ components.

This approach provides a highly flexible and type-safe way to configure the engine, aligning perfectly with modern .NET development practices and the philosophy of **Modular Component Architecture** and interchangeable components.

---

## Related Documentation

*   [Versioning Strategy](../Development/VersioningStrategy.md): Details how releases are versioned, and how commit messages are structured to support automated releases.
*   [CI/CD Strategy](../Development/CiCdStrategy.md): Outlines the continuous integration and continuous delivery pipelines.
