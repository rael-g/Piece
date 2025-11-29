# Design Document - Visual Editor of the Piece Engine

## 1. Introduction

This document details the architectural design of the Visual Editor for the Piece engine. Developed in C# (.NET), the editor will act as a powerful and user-friendly tool for game programmers and animators, consuming the high-level APIs of the Piece engine's C# framework. Its objective is to abstract the complexity of the underlying engine, provide an intuitive interface for scene manipulation, asset management, and content authoring, ensuring an efficient development experience.

## 2. Design Philosophy

*   **Data-Driven and Component-Oriented:** The editor will reflect the engine's node-based and component-oriented architecture. The UI will be dynamically generated based on the properties and components of objects.
*   **Extensibility:** Allow developers to extend the editor with custom tools, panels, and behaviors using C#.
*   **Real-time Visualization:** The editor's viewport will render the scene using the Piece engine itself, providing instant visual feedback.
*   **Non-Destructive Workflow:** Favor edits that can be undone/redone and that do not permanently alter original data (e.g., modifiers).
*   **Modern and Cross-Platform UI:** Utilize a UI framework that supports cross-platform (e.g., AvaloniaUI) to ensure accessibility on different operating systems.

## 3. Interaction with the Engine Core via Interfaces

A core architectural principle of the editor is its **decoupling from any concrete engine implementation**. The editor communicates with the engine core exclusively through a well-defined set of C# interfaces that represent the high-level API (e.g., `IEngine`, `IScene`, `INode`, `IComponent`).

This interface-driven design ensures that the editor remains completely agnostic to the underlying implementation of the engine logic. It can seamlessly operate on:

*   The default, feature-rich **C# high-level framework**.
*   A potential future **C++/Lua high-level framework** (exposed to C# via a wrapper that implements the same interfaces), which might be used for console-targeted builds.
*   Any other engine backend that adheres to the required interface contract.

This approach fully realizes the engine's philosophy of modularity, allowing the high-level "game logic" layer to be a truly swappable component without requiring any changes to the editor itself. The specific implementation of the engine interfaces is resolved at runtime using the .NET Dependency Injection container.

## 4. Main Components of the Visual Editor (C#)

### 4.1. `EditorApplication`

*   **Responsibilities:** The main entry point of the editor.
    *   Manages the editor's lifecycle.
    *   Orchestrates the initialization and layout of UI panels.
    *   Hosts the `GameEngine` instance (from the high-level framework) for real-time rendering.
    *   Manages project loading/saving.

### 4.2. `EditorState`

*   **Responsibilities:** Manages the global state of the editor.
    *   `CurrentProject`: Path of the open project.
    *   `ActiveScene`: The currently loaded and edited `Scene`.
    *   `SelectedObjects`: List of selected `Node`s or `Component`s.
    *   `CommandHistory`: Stack for Undo/Redo operations.

### 4.3. UI Panels (Editor Visual Components)

*   **`ViewportPanel`:**
    *   **Purpose:** Display the game scene in real-time.
    *   **Interaction:** Hosts the `Scene` rendering from the engine. Renders editing gizmos (move, rotate, scale) using UI overlays. Manages editor input (camera navigation, object selection).
    *   **Dependencies:** `GameEngine` (for rendering), `EditorState` (for `SelectedObjects`).
*   **`HierarchyPanel`:**
    *   **Purpose:** Visualize and manipulate the `Node` hierarchy of the `ActiveScene`.
    *   **Interaction:** Displays the `Node` tree. Allows drag-and-drop for re-parenting, adding/removing `Node`s, renaming.
    *   **Dependencies:** `EditorState` (for `ActiveScene`, `SelectedObjects`).
*   **`InspectorPanel`:**
    *   **Purpose:** Display and allow editing of properties of selected `Node`s and `Component`s.
    *   **Interaction:** Inspects the `SelectedObject` from `EditorState`. Uses C# reflection to display and edit properties (integers, floats, strings, enums, asset references). Allows adding/removing `Component`s.
    *   **Dependencies:** `EditorState` (for `SelectedObjects`).
*   **`AssetBrowserPanel`:**
    *   **Purpose:** Browse, view, and import project assets.
    *   **Interaction:** Displays project files. Allows previewing assets (textures, models). Drag-and-drop to instantiate `Node`s in the scene.
    *   **Dependencies:** `AssetManager` (for loading, importing), `EditorState`.
*   **`ConsolePanel`:**
    *   **Purpose:** Display logs, error messages, and debug information from the engine and editor.
    *   **Interaction:** Receives messages from the engine's logging system.
*   **`Toolbar` and `MenuBar`:**
    *   **Purpose:** Provide access to global actions (Save, Load, Play, Stop, Editing Tools).
    *   **Interaction:** Call methods in `EditorApplication` or `EditorSystems`.

### 4.4. `EditorSystems` (Logic for the Editor)

*   **`UndoRedoSystem`:** Manages the command stack for undo/redo editor operations.
*   **`SelectionSystem`:** Selection logic (click, drag-box, multi-selection).
*   **`GizmoSystem`:** Renders and manipulates gizmos (move, rotate, scale).
*   **`SerializationSystem`:** Saves and loads `Scene`s and project data to/from files.

## 5. High-Level C# Interface Expansion (if necessary)

To support editor functionalities, the following additions/modifications will be necessary in the Piece engine's high-level C# framework APIs:

*   **`Node` / `Component`:**
    *   **`GetProperties()`:** A method or interface that returns a list of editable properties (e.g., `EditorPropertyInfo` struct with name, type, value, min/max, etc.).
    *   **`SetProperty(string name, object value)`:** For generic property editing.
    *   **`Serialize()` / `Deserialize()`:** Methods to serialize/deserialize the complete state of the node/component.
    *   **`Node.AddComponent<T>()`, `Node.RemoveComponent<T>()`:** Methods for dynamic component manipulation.
    *   **`Node.GetTransform()`:** Returns the associated `TransformComponent`.
*   **`Scene`:**
    *   **`Scene.Save(path)` / `Scene.Load(path)`:** For scene persistence.
    *   **`Scene.RootNode`:** Property to access the root node of the hierarchy.
*   **`AssetManager`:**
    *   **`ImportAsset(sourcePath, destPath)`:** For the Asset Browser to import raw files and transform them into engine assets.
    *   **`GetAssetMetadata(path)`:** To display information in the Asset Browser.
*   **`Camera`:** Methods or properties to configure an editor camera (e.g., `SetEditorMode(bool isEditor)`).
*   **`RenderManager`:**
    *   **`DrawEditorGizmos(RenderContext renderContext)`:** To render gizmos (e.g., bounding boxes, light icons, manipulators) within the editor viewport.
*   **New Components (if applicable):**
    *   `AnimationComponent`: Manages animation states, linking to an `Animator` (C# or C++).

## 6. Editor Game Loop (Simplified)

1.  **`EditorApplication.Run()`:**
    *   Main editor loop.
    *   Processes UI events.
    *   Updates `EditorState`.
    *   **Simulation Loop (if Play Mode active):**
        *   `GameEngine.Update()`
        *   `GameEngine.Draw()` (for game viewport)
    *   **Editor Drawing:**
        *   Draws all UI panels.
        *   `ViewportPanel.Draw()`:
            *   Calls `GameEngine.Draw()` (for scene rendering).
            *   Calls `RenderManager.DrawEditorGizmos()` (to draw editor overlays).

## 7. Challenges and Considerations

*   **Editor-Engine Synchronization:** Ensure that changes in the editor are instantly reflected in the `GameEngine` and vice-versa, especially in Play Mode.
*   **Performance:** The editor needs to be responsive even with complex scenes. Viewport rendering must be efficient.
*   **Robust Serialization:** The serialization system for scenes and assets must be robust and flexible to handle version changes.
*   **UI Extensibility:** The UI framework must allow easy creation of custom property editors for components.

---
**Note:** This document focuses on the C# editor architecture. Communication with the intermediate C++ layer will be through C# wrappers and C-compatible APIs, as detailed in the intermediate layer design document.
