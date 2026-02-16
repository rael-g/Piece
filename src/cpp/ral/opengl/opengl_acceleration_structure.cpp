#include "opengl_acceleration_structure.h"
#include <piece_core/logging_api.h>

// Note: OpenGL does not have a direct concept of Acceleration Structures for ray tracing
// This implementation will be largely a stub for now, or might throw an error if called.
// Ray tracing is typically a feature of more modern APIs like Vulkan/DXR.

namespace Piece::RAL
{
OpenGLAccelerationStructure::OpenGLAccelerationStructure(const AccelerationStructureBuildInfo &info)
{
    PIECE_INFO("OpenGLAccelerationStructure created (WARNING: Not natively supported in OpenGL).");
    (void)info; // Suppress unused parameter warning
    PIECE_WARN("Acceleration Structures are primarily for ray tracing APIs (e.g., Vulkan/DXR) and are not directly "
               "supported in OpenGL. This is a stub implementation.");
}

OpenGLAccelerationStructure::~OpenGLAccelerationStructure()
{
    PIECE_INFO("OpenGLAccelerationStructure destroyed.");
    // Clean up if any resources were allocated (unlikely for OpenGL AS)
}

void OpenGLAccelerationStructure::Build(const AccelerationStructureBuildInfo &info)
{
    PIECE_TRACE("OpenGLAccelerationStructure::Build called.");
    (void)info; // Suppress unused parameter warning
    PIECE_WARN("OpenGLAccelerationStructure::Build is a stub and has no effect.");
}

void OpenGLAccelerationStructure::Update(const AccelerationStructureBuildInfo &info)
{
    PIECE_TRACE("OpenGLAccelerationStructure::Update called.");
    (void)info; // Suppress unused parameter warning
    PIECE_WARN("OpenGLAccelerationStructure::Update is a stub and has no effect.");
}

} // namespace Piece::RAL
