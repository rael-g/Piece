# Project-specific engineering guidelines

> **Status:** Authoritative
>
> This document defines **project-level technical standards** specific to this engine. It complements `AGENTS.md` and MUST be followed by both human contributors and AI agents.
>
> In case of conflict, this document takes precedence over agent adaptation, personal preference, or inferred conventions.

---

## Project overview

This project is a **highly modular hybrid game engine**, named **Piece Engine**, composed of:

* A **C++ core** responsible for low-level systems (rendering, platform abstraction, performance-critical paths)
* A **C# layer** providing higher-level orchestration, tooling, and user-facing APIs

The architecture prioritizes:

* Explicit boundaries between modules
* Predictable ownership and lifetimes
* Long-term maintainability over short-term convenience

---

## Guiding principles

* Consistency beats cleverness
* Explicit is better than implicit
* Readability and predictability outweigh brevity
* Code is written for other engineers first, compilers second

If a choice exists between two valid approaches, the one that reduces ambiguity and variation MUST be preferred.

---

## Language-specific expectations

### C# (baseline)

C# follows standard, well-known conventions:

* .NET naming guidelines
* PascalCase for public APIs
* camelCase for locals and parameters
* XML documentation for public/protected members

Unless explicitly stated otherwise, **standard C# conventions are assumed and must not be redefined here**.

---

## C++ guidelines (strict)

C++ is treated as a **high-discipline language**. Freedom of expression is intentionally restricted to avoid fragmentation and inconsistency.

### General philosophy

* There is ONE preferred way to write C++ in this project
* Personal style, editor defaults, or external codebases are not justification
* When in doubt, choose the most boring, explicit, and review-friendly option

If an engineer or agent is unsure, they MUST ask rather than invent a convention.

---

## Namespaces

* All code MUST live inside a project namespace (e.g. `Piece`)
* Nested namespaces MUST reflect module boundaries
* Anonymous namespaces are allowed **only** in `.cpp` files for internal linkage

Example:

```
namespace Piece::Ral
{
}
```

Rules:

* `using namespace` is FORBIDDEN in headers
* `using namespace` is FORBIDDEN at global scope
* Prefer fully-qualified names in headers

---

## Headers and source files

### Header rules

* Headers MUST be self-contained
* Including a header MUST NOT require prior includes
* Headers MUST NOT include unnecessary dependencies

Headers MUST:

* Contain declarations only
* Avoid implementation details
* Use forward declarations where possible

Headers MUST NOT:

* Contain `using namespace`
* Contain non-inline function definitions
* Include heavy system headers unless strictly required

### Include guards

* All header files MUST use traditional include guards (`#ifndef`/`#define`/`#endif`).


---

## Includes

* Includes MUST be ordered logically:

  1. Corresponding header
  2. Project headers
  3. Third-party headers
  4. Standard library headers

* Includes MUST be minimal

* Transitive includes MUST NOT be relied upon

If a `.cpp` compiles only because another header happens to include something, this is a defect.

---

## Directory & Module Organization

* Directory structure MUST reflect architectural boundaries
* Modules MUST NOT leak responsibilities across directories
* Avoid "misc", "common", "utils" dumping grounds

The project follows a structure that separates C++ and C# code, as well as tests and documentation.

*   `/src/cpp` -> C++ source code, organized by layers (pal, ral, wal, piece_core).
*   `/src/csharp` -> C# source code, with each project in its own directory.
*   `/tests` -> Contains both C++ and C# tests, in separate subdirectories.
*   `/docs` -> All project documentation.
*   `/cmake` -> CMake utility modules.
*   `/build` -> Build output directory (ignored by Git).

Rules:

* Public headers for C++ modules are located alongside the source files.
* Cross-module includes MUST be deliberate and reviewable.
* Directory names MUST represent stable concepts, not temporary tasks.

---

## File structure

* One primary type per header/source pair
* File names MUST match the primary type name
* Avoid monolithic headers

---

## Naming conventions (C++)

This project strictly adheres to the **Google C++ Style Guide** for all C++ naming conventions. This ensures clarity, consistency, and universality across the C++ codebase. Developers and AI agents must consult the official Google C++ Style Guide for specific rules regarding types, functions, variables, member variables, and constants.

Abbreviations MUST be avoided unless industry-standard.

---

## Build System & CMake (strict)

CMake is treated as part of the engineering architecture, not a convenience script.

### General rules

* CMake logic MUST be deterministic and explicit
* Avoid implicit global state where possible
* Prefer target-based configuration

### Target design

* Every module MUST define its own target
* Avoid monolithic targets
* `target_link_libraries` MUST express real dependencies only

Forbidden patterns:

* Global include_directories
* Global compile definitions
* Hidden transitive dependency reliance

### Dependencies

* External dependencies MUST be managed via vcpkg manifest mode
* No ad-hoc FetchContent unless explicitly justified

### Compile options

* Compile options MUST be target-scoped
* Avoid global flags affecting unrelated modules

### Readability

* CMakeLists MUST be readable and reviewable
* Avoid overly clever macros or meta-CMake abstractions

If CMake becomes hard to understand, it is considered a design defect.

---

## Formatting and indentation

C++ code formatting adheres to the **Microsoft C++ style**, as enforced by `clang-format`. This ensures consistent indentation, brace style, and overall code presentation across the project.

---

## Const-correctness

* Use `const` aggressively and intentionally
* If something does not modify state, it SHOULD be `const`
* Missing `const` is considered technical debt

---

## Memory and ownership

* Ownership MUST be explicit
* Raw pointers represent non-owning references unless documented otherwise
* Prefer RAII and value semantics

Implicit ownership assumptions are forbidden.

---

## Error handling

* Errors MUST be explicit
* Avoid hidden control flow

> **TODO:** A consistent error handling strategy for the C++ core has not been established yet. A clear strategy (e.g., using exceptions, error codes, or a result type like `std::expected`) should be defined and consistently applied.


---

## Templates and metaprogramming

* Templates MUST be used sparingly
* Prefer readability over compile-time cleverness
* If template code is hard to read, it is likely unacceptable

---

## Comments (C++)

* Comments explain **why**, not **what**
* Comments MUST NOT narrate changes
* Comments MUST NOT restate code

If a comment is only useful to explain how the code was written, it should not exist.

---

## Interop boundaries (C++ / C#)

* Boundaries MUST be explicit
* Data ownership across language boundaries MUST be documented
* Avoid leaking C++ abstractions into C# APIs

---

## AI-specific enforcement

Given the tendency of AI agents to:

* Invent conventions
* Mix styles across files
* Mirror low-quality C++ examples

The following rules apply:

* Never infer style from existing files alone
* Always follow this document over observed patterns
* If inconsistency exists, STOP and report

---

## Final check (C++)

Before delivering C++ code, the agent MUST ask:

* Would a senior C++ engineer immediately trust this code?
* Is the style consistent with every other file?
* Are ownership, lifetimes, and dependencies obvious?
* Could this survive a strict code review?

If the answer to any is "no", the code must be revised.
