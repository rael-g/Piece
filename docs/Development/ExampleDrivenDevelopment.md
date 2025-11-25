# Example-Driven Development (EDD) for the Piece Engine

## 1. Introduction to Example-Driven Development

Example-Driven Development (EDD) is a development methodology that focuses on defining clear, executable examples to guide the implementation process. Similar to Test-Driven Development (TDD), EDD emphasizes a tight feedback loop, but instead of starting with unit tests, it begins with concrete, user-centric examples that demonstrate a desired functionality.

In the context of the Piece Engine, EDD will be used to progressively build out the engine's features, starting from the lowest layers and moving upwards. This approach ensures that functionality is always tied to a tangible use case, fostering a practical understanding of the engine's capabilities and constraints.

## 2. The EDD Workflow

The EDD workflow for the Piece Engine follows an iterative and incremental process:

### Step 2.1: Define the Example

Each development cycle begins with defining a simple, yet illustrative example. Examples should:
*   **Increase in Complexity:** Start with very basic functionalities and gradually introduce more complex scenarios.
*   **Be Concrete:** Describe a specific action or visual outcome (e.g., "Open a window," "Draw a red triangle," "Load a 3D model").
*   **Focus on a Single Goal:** Avoid examples that try to achieve too many things at once.

**Initial Examples (Illustrative):**
*   **Example 1: Basic Window Creation:** Open a native window with a specified title and dimensions. The window should be visible and closeable.
*   **Example 2: Clear Screen:** Open a window and clear its background to a specific color (e.g., blue) in each frame.
*   **Example 3: Draw a Basic Triangle:** Open a window, clear the background, and draw a simple 2D or 3D triangle.
*   **Example 4: Draw a Textured Quad:** Open a window, and draw a 2D quad with a loaded texture.
*   **Example 5: Draw a Rotating Cube:** Open a window, and draw a 3D cube that rotates over time.
*   **Example 6: Camera Control:** Draw a 3D scene where the camera can be moved and rotated by user input.
*   **Example 7: Basic Light Source:** Add a simple light source to a 3D scene and observe its effect on objects.

### Step 2.2: Implement the Minimum Necessary Software

For the chosen example, implement *only the minimum amount of software functionality* required to make that example runnable and visually demonstrate its success.

*   **"Just Enough" Principle:** Avoid over-engineering or implementing features that are not directly required by the current example.
*   **Focus on the Current Layer:** If the example targets a low-level feature (e.g., window creation), the implementation should primarily focus on the WAL/RAL layers. If it targets a high-level feature, it will utilize the lower layers.
*   **Initial Backend:** The initial implementation will use `GLFW` for the WAL and `OpenGL` for the RAL.

### Step 2.3: Develop Tests Concurrently

As the software functionality is implemented, *develop tests in conjunction with it*. These tests are based on the defined requirements for the example and the emerging understanding of the implementation. The Piece Engine employs a comprehensive testing strategy across its layers:

*   **Unit Tests:** Focus on verifying the correctness of individual components, functions, or classes within each layer (Low-Level C++, Intermediate C++, High-Level C#). This includes testing specific API implementations (e.g., `IWindow`, `IGraphicsDevice`), resource creation, and core logic.
*   **Integration Tests:** Verify the correct interaction and communication between multiple components or layers. Examples include testing window initialization with graphics devices, rendering contexts with resources, asset loading workflows, and the lifecycle of the game engine itself.
*   **End-to-End Tests:** Cover full system functionality, simulating user scenarios to ensure the entire engine operates as expected. These tests validate the full game loop execution, specific rendering scenarios (e.g., PBR materials, shadow mapping), platform compatibility, and performance metrics (e.g., frame rate, memory usage).

### Step 2.4: Refine and Expand Tests

Once an example is successfully implemented and its core functionality is working, take the knowledge gained during its implementation to *create more comprehensive and specific tests*.
*   **Edge Cases:** Consider boundary conditions, error handling, and unusual inputs.
*   **Performance:** Add basic performance checks if relevant to the example.
*   **Regression:** Ensure that new tests are added to prevent regressions in the future.

### Step 2.5: Document the Example and Learnings

Document the implemented example, including code snippets, explanation of the new features, and any significant architectural decisions or challenges encountered. This documentation feeds back into the overall engine design.

## 3. Benefits of EDD

*   **Clear Goals:** Each example provides a concrete target, making development focused.
*   **Incremental Progress:** Builds functionality piece by piece, reducing complexity.
*   **Early Feedback:** Executable examples provide immediate feedback on progress.
*   **Practical Testing:** Tests are developed in a practical context, ensuring relevance.
*   **Guided Architecture:** The examples naturally drive the evolution of the engine's architecture.



## Related Documentation

*   [Versioning Strategy](../Development/VersioningStrategy.md): Details how releases are versioned, and how commit messages are structured to support automated releases.
*   [CI/CD Strategy](../Development/CiCdStrategy.md): Outlines the continuous integration and continuous delivery pipelines.