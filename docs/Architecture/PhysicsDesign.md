# Design Document - Piece Engine Physics Abstraction Layer (PAL)

## 1. Introduction

This document defines the architectural specifications and guidelines for the Physics Abstraction Layer (PAL) within the Piece Engine. The PAL's primary goal is to provide a highly modular, engine-agnostic interface for physics simulation, allowing for the integration of various concrete physics engines (e.g., PhysX, Jolt, Box2D, Bullet) while maintaining a stable and minimal API. This includes explicit support for both 2D and 3D physics simulations.

Inspired by a philosophy that prioritizes explicit boundaries and freedom for concrete implementations, the PAL aims to avoid "Frankenstein API growth" by exposing a universal core while allowing direct access to engine-specific features where intentionally desired. This approach ensures maximum control and extensibility without artificial abstractions.

## 2. Design Philosophy

The PAL's architecture is founded on a rigid separation of concerns:

*   **Universal API**: Defines fundamental, stable concepts.
*   **Concrete Implementations**: Provide advanced features and specific optimizations.

The engine does not attempt to hide the underlying physics engines. Instead, it offers:

*   A common, stable core for fundamental physics operations.
*   Stable contracts for interchangeability.
*   Optional portability across different physics implementations.
*   Full freedom for specific engine customizations without being limited by the universal API.

This aligns with the Piece Engine's overall philosophy of a Modular Component Architecture, where low-level implementations are swappable and extensible units.

## 3. Interaction with the Engine Core

The PAL defines C++ interfaces that concrete physics engines must implement. These implementations are compiled as standalone dynamic libraries (e.g., `physics_jolt.dll`, `physics_box2d.dll`) and are completely agnostic to `piece_core` C++.

The `piece_core` C++ acts as a passive service container and orchestrator. It receives already-created C++ `IPhysicsWorldFactory` instances (typically via P/Invoke from C#) and makes them available through its `ServiceLocator`.

The `Piece.Framework` (C#) layer is the intelligent orchestrator, using .NET Dependency Injection to manage and integrate these C++ low-level PAL implementations. It also handles the marshaling of configuration data (e.g., `NativePhysicsOptions`) from C# to C++.

## 4. Usage Model

The PAL supports two primary usage levels:

### 4.1. Universal Level (Gameplay Agnostic)

Code that depends only on the universal PAL interfaces. This code is inherently portable, stable, and decoupled from any specific physics engine.

**Example (C#):**
```csharp
world.Step(dt);
body.AddForce(force);
var hits = world.Raycast(query);
```

**Example (C++):**
```cpp
world->Step(dt);
body->AddForce(force);
RaycastHits hits = world->Raycast(query);
```

### 4.2. Engine-Specific Level (Advanced Access)

When explicit, deliberate, and conscious access to engine-specific features is required, code can directly interact with the concrete physics engine implementation. This approach avoids artificial abstractions, provides maximum control, and circumvents limitations imposed by the universal API.

**Capability Detection (Official):**
Using pattern matching or type checks to safely downcast to a specific implementation.

**Example (C#):**
```csharp
if (world is JoltPhysicsWorld joltWorld)
{
    joltWorld.SetSolverIterations(64);
}
```

**Example (C++):**
```cpp
if (auto* joltWorld = dynamic_cast<JoltPhysicsWorld*>(world))
{
    joltWorld->SetSolverIterations(64);
}
```

This method is architecturally sound, simple, and predictable, avoiding artificial indirections or unnecessary complexity.

## 5. Modular Structure

The PAL conceptual structure consists of:

*   **Core API**: The stable, minimal, and fundamental contract (interfaces).
*   **Implementations**: Concrete physics engines that adhere to the Core API and can expose their specific features.

## 6. Core API (Fundamental Contract)

The core API is deliberately small, semantically pure, and designed to be almost immutable. It defines the foundational interfaces for physics simulation, explicitly differentiating between 2D and 3D contexts.

### 6.1. `IPhysicsWorld2D` and `IPhysicsWorld3D`

**Responsibilities:**
*   Advance the physics simulation.
*   Perform spatial queries (raycasts, sweeps).
*   Create and manage physics bodies.

#### `IPhysicsWorld2D`

**C++ Interface:**
```cpp
namespace Piece::PAL
{
class IPhysicsBody2D; // Forward declaration
struct RaycastQuery2D;
struct RaycastHits2D;
struct SweepQuery2D;
struct SweepHits2D;
struct BodyDesc2D;

class IPhysicsWorld2D
{
public:
    virtual ~IPhysicsWorld2D() = default;

    virtual void Step(float dt) = 0;

    virtual RaycastHits2D Raycast(const RaycastQuery2D& query) const = 0;
    virtual SweepHits2D Sweep(const SweepQuery2D& query) const = 0;

    virtual std::unique_ptr<IPhysicsBody2D> CreateBody(const BodyDesc2D& desc) = 0;
};
} // namespace Piece::PAL
```

**C# Interface:**
```csharp
public interface IPhysicsWorld2D
{
    void Step(float dt);

    RaycastHits2D Raycast(in RaycastQuery2D query);
    SweepHits2D Sweep(in SweepQuery2D query);

    IPhysicsBody2D CreateBody(in BodyDesc2D desc);
}
```

#### `IPhysicsWorld3D`

**C++ Interface (from `src/cpp/pal/iphysics_world.h` - conceptually):**
```cpp
namespace Piece::PAL
{
class IPhysicsBody3D; // Forward declaration
struct RaycastQuery3D;
struct RaycastHits3D;
struct SweepQuery3D;
struct SweepHits3D;
struct BodyDesc3D;

class IPhysicsWorld3D
{
public:
    virtual ~IPhysicsWorld3D() = default;

    virtual void Step(float dt) = 0;

    virtual RaycastHits3D Raycast(const RaycastQuery3D& query) const = 0;
    virtual SweepHits3D Sweep(const SweepQuery3D& query) const = 0;

    virtual std::unique_ptr<IPhysicsBody3D> CreateBody(const BodyDesc3D& desc) = 0;
};
} // namespace Piece::PAL
```

**C# Interface:**
```csharp
public interface IPhysicsWorld3D
{
    void Step(float dt);

    RaycastHits3D Raycast(in RaycastQuery3D query);
    SweepHits3D Sweep(in SweepQuery3D query);

    IPhysicsBody3D CreateBody(in BodyDesc3D desc);
}
```

### 6.2. `IPhysicsBody2D` and `IPhysicsBody3D`

**Responsibilities:**
*   Apply forces and impulses.
*   Manage transformations (position, rotation).
*   Manage linear and angular velocities.

#### `IPhysicsBody2D`

**C++ Interface:**
```cpp
namespace Piece::PAL
{
struct Vec2; // Placeholder, assuming a math library vector type
struct Transform2D; // Placeholder, assuming a math library transform type

class IPhysicsBody2D
{
public:
    virtual ~IPhysicsBody2D() = default;

    virtual void AddForce(const Vec2& force) = 0;
    virtual void AddImpulse(const Vec2& impulse) = 0;

    virtual void SetTransform(const Transform2D& transform) = 0;
    virtual Transform2D GetTransform() const = 0;

    virtual void SetLinearVelocity(const Vec2& velocity) = 0;
    virtual Vec2 GetLinearVelocity() const = 0;

    // Additional methods for collider, material, etc., expected from src/cpp/pal/icollider_shape.h, etc.
};
} // namespace Piece::PAL
```

**C# Interface:**
```csharp
public interface IPhysicsBody2D
{
    void AddForce(in Vec2 force);
    void AddImpulse(in Vec2 impulse);

    void SetTransform(in Transform2D transform);
    Transform2D GetTransform();

    void SetLinearVelocity(in Vec2 velocity);
    Vec2 GetLinearVelocity();
}
```

#### `IPhysicsBody3D`

**C++ Interface (from `src/cpp/pal/iphysics_body.h` - conceptually):**
```cpp
namespace Piece::PAL
{
struct Vec3; // Placeholder, assuming a math library vector type
struct Transform3D; // Placeholder, assuming a math library transform type

class IPhysicsBody3D
{
public:
    virtual ~IPhysicsBody3D() = default;

    virtual void AddForce(const Vec3& force) = 0;
    virtual void AddImpulse(const Vec3& impulse) = 0;

    virtual void SetTransform(const Transform3D& transform) = 0;
    virtual Transform3D GetTransform() const = 0;

    virtual void SetLinearVelocity(const Vec3& velocity) = 0;
    virtual Vec3 GetLinearVelocity() const = 0;

    // Additional methods for collider, material, etc., expected from src/cpp/pal/icollider_shape.h, etc.
};
} // namespace Piece::PAL
```

**C# Interface:**
```csharp
public interface IPhysicsBody3D
{
    void AddForce(in Vec3 force);
    void AddImpulse(in Vec3 impulse);

    void SetTransform(in Transform3D transform);
    Transform3D GetTransform();

    void SetLinearVelocity(in Vec3 velocity);
    Vec3 GetLinearVelocity();
}
```

### 6.3. Other Core Interfaces (from `src/cpp/pal`):

To maintain consistency with the 2D/3D physics worlds and bodies, other core interfaces will also need to be defined with explicit dimensionality or a mechanism to handle it.

*   `IColliderShape2D` / `IColliderShape3D`: Abstract interfaces for various collider shapes (e.g., circle, box for 2D; sphere, box, mesh for 3D).
*   `IJoint2D` / `IJoint3D`: Abstract interfaces for physics joints.
*   `IPhysicsMaterial`: Abstract interface for defining material properties (friction, restitution). This might remain universal as material properties are often dimension-agnostic, but could have 2D/3D specific aspects if needed.
*   `IPhysicsWorldFactory2D` / `IPhysicsWorldFactory3D`: Factory interfaces for creating `IPhysicsWorld2D` and `IPhysicsWorld3D` instances, respectively. These will be the entry points for `piece_core` to obtain physics worlds from specific PAL implementations.

## 7. Descriptor Structs (Universal Contracts)

Essential for binary stability and semantic clarity, these structs define data for queries and object creation without leaking engine-specific details. They are defined in `pal_types.h` and other relevant headers within `src/cpp/pal`, with distinct versions for 2D and 3D contexts.

**Example: `RaycastQuery2D` / `RaycastQuery3D`**
```cpp
// 2D
struct RaycastQuery2D
{
    Vec2 Origin;
    Vec2 Direction;
    float MaxDistance;
    CollisionMask2D Mask; // Placeholder, assuming an enum/bitmask
};

// 3D
struct RaycastQuery3D
{
    Vec3 Origin;
    Vec3 Direction;
    float MaxDistance;
    CollisionMask3D Mask; // Placeholder, assuming an enum/bitmask
};
```

**Example: `RaycastHit2D` / `RaycastHit3D`**
```cpp
// 2D
struct RaycastHit2D
{
    IPhysicsBody2D* Body;
    Vec2 Position;
    Vec2 Normal;
    float Distance;
};

// 3D
struct RaycastHit3D
{
    IPhysicsBody3D* Body;
    Vec3 Position;
    Vec3 Normal;
    float Distance;
};
```

Similar 2D/3D distinctions will apply to `SweepQuery`, `SweepHits`, `BodyDesc`, `Vec` types (e.g., `Vec2`, `Vec3`), and `CollisionMask` (e.g., `CollisionMask2D`, `CollisionMask3D`).

## 8. Event System

The event system focuses on fundamental, engine-agnostic interactions, avoiding internal tuning specifics, and providing distinct events for 2D and 3D contexts.

**C++ Interface (`IPhysicsEventListener`):**
```cpp
class IPhysicsEventListener
{
public:
    virtual ~IPhysicsEventListener() = default;

    virtual void OnCollisionEnter2D(const CollisionEvent2D& event) {}
    virtual void OnCollisionStay2D(const CollisionEvent2D& event) {}
    virtual void OnCollisionExit2D(const CollisionEvent2D& event) {}

    virtual void OnCollisionEnter3D(const CollisionEvent3D& event) {}
    virtual void OnCollisionStay3D(const CollisionEvent3D& event) {}
    virtual void OnCollisionExit3D(const CollisionEvent3D& event) {}
};

struct CollisionEvent2D
{
    IPhysicsBody2D* BodyA;
    IPhysicsBody2D* BodyB;

    Vec2 ContactPoint;
    Vec2 ContactNormal;
};

struct CollisionEvent3D
{
    IPhysicsBody3D* BodyA;
    IPhysicsBody3D* BodyB;

    Vec3 ContactPoint;
    Vec3 ContactNormal;
};
```
This avoids complex manifolds or engine-specific payloads in the universal API.

## 9. Ownership & Lifetime Model (Critical)

A strict ownership model is enforced to ensure stability, prevent double-free issues, and avoid ambiguous ownership across the C++ and C# boundary.

*   **Absolute Rule**: The C++ layer is the sole owner of the physics simulation and its native resources. C# layers only hold references (e.g., `IntPtr` to native handles).

### 9.1. C++ Side

*   Leverages RAII (Resource Acquisition Is Initialization) for automatic resource management.
*   `std::unique_ptr` is the preferred mechanism for owning dynamically allocated physics objects (e.g., `IPhysicsBody` instances created by `IPhysicsWorld`).
*   The `IPhysicsWorld` instance is responsible for the lifetime of the `IPhysicsBody` instances it creates and manages.

### 9.2. C# Side

*   Utilizes lightweight wrappers that hold an `IntPtr` to the native C++ object.
*   C# wrappers explicitly **never** own native memory. Their finalizers or `IDisposable` implementations (if any) would primarily be responsible for notifying the C++ side to release a reference, or they would simply be released when the C++ owner dictates.

**Example (C#):**
```csharp
public sealed class PhysicsBody2D : IPhysicsBody2D, IDisposable
{
    internal IntPtr NativeHandle;
    // ...
    public void Dispose()
    {
        // Notify C++ to decrement reference count or mark for deletion if this is the last C# reference
        // Or simply do nothing, relying solely on C++ ownership.
    }
}

public sealed class PhysicsBody3D : IPhysicsBody3D, IDisposable
{
    internal IntPtr NativeHandle;
    // ...
    public void Dispose()
    {
        // Notify C++ to decrement reference count or mark for deletion if this is the last C# reference
        // Or simply do nothing, relying solely on C++ ownership.
    }
}
```

**Guarantees:**
*   No double-free issues.
*   No ambiguous ownership.
*   No garbage collector interference with the native physics engine's memory management.

## 10. Concrete Implementations

Each concrete physics engine implementation (e.g., Box2D, Minimal PAL) must:

*   Implement the universal Core API interfaces (e.g., `IPhysicsWorld2D`/`IPhysicsWorld3D`, `IPhysicsBody2D`/`IPhysicsBody3D`, etc.).
*   Be free to expose its own engine-specific API and features without restrictions or the need to register extensions with the universal PAL.

**Example (2D Implementation):**
```cpp
class Box2DPhysicsWorld : public Piece::PAL::IPhysicsWorld2D
{
public:
    // IPhysicsWorld2D interface implementation
    // ...

    // Box2D-specific methods
    void SetGravity(const b2Vec2& gravity);
    void SetContactListener(b2ContactListener* listener);
};
```
The `src/cpp/pal/minimal` directory serves as an example of a simple, minimal implementation of these interfaces, demonstrating how a concrete implementation adheres to the PAL contracts.

## 11. Anti-Frankenstein Rules

To prevent API bloat and maintain clarity, a feature is only integrated into the universal Core API if it meets the following criteria:

*   It exists in multiple, diverse physics engines.
*   It represents a fundamental concept crucial for general gameplay mechanics.
*   It does not expose internal details of any specific physics engine.
*   It is semantically stable and universally understood.
*   It is not related to engine-specific tuning or optimization.

### 11.1. What Belongs to the Core API (for both 2D and 3D):

*   Step simulation
*   Basic physics bodies (creation, destruction)
*   Application of forces and impulses
*   Transformation management (position, rotation)
*   Velocity management (linear, angular)
*   Spatial queries (raycasts, sweeps)
*   Basic collision events (enter, stay, exit)
*   Basic collider shapes (e.g., Circle, Box for 2D; Sphere, Box, Mesh for 3D)
*   Basic physics materials (friction, restitution)

### 11.2. What NEVER Enters the Core API:

*   Solver iterations and sub-step tuning
*   Broadphase internal configurations
*   Continuous Collision Detection (CCD) specific knobs
*   Debug rendering internals
*   Engine-specific heuristics or optimizations

These belong exclusively to the concrete physics engine implementations.

## 12. Challenges and Considerations

*   **Error Handling**: The `ProjectGuidelines.md` indicates that a consistent error handling strategy for the C++ core is yet to be established. This needs to be defined and consistently applied across PAL interfaces and implementations.
*   **Performance**: Ensuring minimal overhead for the abstraction layer while allowing concrete engines to perform optimally.
*   **Cross-Language Interoperability**: Robust marshaling of complex data structures (e.g., `BodyDesc`, `RaycastQuery`) between C++ and C# is crucial. `pal_types.h` and `native_physics_options.h` play a key role here.
*   **API Evolution**: Maintaining the stability of the Core API while allowing for necessary extensions and updates in concrete implementations.
