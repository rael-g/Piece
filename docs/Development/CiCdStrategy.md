# Piece Engine CI/CD Strategy

This document outlines the Continuous Integration (CI) and Continuous Delivery (CD) strategy for the Piece Engine, leveraging GitHub Actions to automate building, testing, and releasing the engine.

## 1. Core Principles

*   **Continuous Integration (CI):** Every code change will trigger an automated build and test process to ensure early detection of issues.
*   **Continuous Delivery (CD):** Verified changes on the `main` branch will automatically lead to the creation of new releases, including version bumping, changelog generation, and artifact publishing.
*   **Fast Feedback:** Pipelines are designed for efficiency to provide rapid feedback to developers.
*   **Reproducibility:** Builds and releases must be consistent and reproducible across different runs and environments.
*   **Platform Agnosticism:** Pipelines will aim to support builds and tests on relevant operating systems (Windows, Linux, macOS).

## 2. Tooling

*   **GitHub Actions:** The primary orchestration tool for defining and executing CI/CD pipelines.
*   **`vcpkg`:** For managing C++ dependencies, integrated with CMake.
*   **`CMake`:** For configuring and building C++ projects.
*   **`.NET SDK`:** For building and testing C# projects.
*   **`semantic-release`:** For automating semantic versioning, changelog generation, and GitHub Releases based on Conventional Commits.
*   **`commitlint` (recommended):** A tool to enforce Conventional Commits locally during the commit process, ensuring consistency before code is pushed.

## 3. Pipeline Stages

The CI/CD process is divided into two main pipelines: one for Pull Requests (CI) and another for the `main` branch (CD).

### 3.1. Pull Request / Feature Branch Pipeline (CI)

This pipeline ensures the quality and stability of code before it's merged into the `main` branch.

*   **Triggers:**
    *   `push` to any branch *except* `main`.
    *   `pull_request` targeting the `main` branch.

*   **Jobs:**

    1.  **Setup Environment:**
        *   Checkout the repository.
        *   Set up required development environments (e.g., `vcpkg`, `.NET SDK`, `Node.js`).

    2.  **Build C++ Components:**
        *   Configure CMake with appropriate settings (e.g., `Ninja` generator).
        *   Build all C++ targets (the "development fat build" as described in `BUILD_SYSTEM.md`) for selected platforms (e.g., Windows, Linux).

    3.  **Build C# Components:**
        *   Restore NuGet packages for C# projects.
        *   Build all C# projects using `dotnet build`.

    4.  **Run C++ Tests:**
        *   Execute C++ unit and integration tests (e.g., using `CTest`).

    5.  **Run C# Tests:**
        *   Execute C# unit and integration tests (e.g., using `dotnet test`).

    6.  **Linting & Static Analysis (Optional but Recommended):**
        *   Run code formatters (e.g., `clang-format`, `dotnet format --check`).
        *   Run static analysis tools (e.g., `clang-tidy` for C++).
        *   (If `commitlint` is not enforced via pre-push hooks, an additional job can validate commit messages here for PR merges).

    *   **Failure Action:** If any job fails, the PR check fails, blocking the merge until issues are resolved.

### 3.2. Main Branch Release Pipeline (CD)

This pipeline automates the release process for verified changes merged into the `main` branch.

*   **Triggers:**
    *   `push` events to the `main` branch (only after a successful PR merge).

*   **Jobs:**

    1.  **Perform CI Steps:**
        *   The initial CI steps (build and test) are implicitly or explicitly run again to ensure the `main` branch is always in a deployable state.

    2.  **Automated Versioning and Release (`semantic-release`):**
        *   Checkout the `main` branch.
        *   Run `semantic-release` which will:
            *   Analyze Conventional Commits made since the last release.
            *   Determine the next semantic version (`MAJOR`, `MINOR`, or `PATCH`).
            *   Generate and update the `CHANGELOG.md` file.
            *   Create a new Git tag for the determined version (e.g., `v1.2.3`).
            *   Create a new GitHub Release with the changelog.

    3.  **Production Packaging & Artifact Publishing:**
        *   Checkout the repository again, specifically using the newly created Git tag.
        *   Configure and build the C++ components for release (if not already done in a release configuration CI job).
        *   Execute the CMake `install` command to create "lean packages" (as per `BUILD_SYSTEM.md`), specifying desired components (e.g., specific rendering backends like OpenGL, Vulkan). This step might run multiple times to create different variations of the release package.
        *   Package the resulting `dist/` directories into deployable archives (e.g., `.zip`, `.tar.gz`).
        *   Attach these packaged artifacts to the GitHub Release created by `semantic-release`.
        *   **Optional:** Trigger deployment to a distribution platform or internal artifact repository.

## 4. Integration with Documentation

This CI/CD strategy works in conjunction with:
*   [Build System Design](../Development/BuildSystem.md): Provides the foundation for building and packaging.
*   [Versioning Strategy](../Development/VersioningStrategy.md): Leveraged by `semantic-release` for automated versioning.

By implementing this CI/CD strategy, the Piece Engine aims to maintain high code quality, accelerate delivery, and ensure transparent and automated release management.
