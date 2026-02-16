#ifndef RAL_OPENGL_OPENGL_ACCELERATION_STRUCTURE_H_
#define RAL_OPENGL_OPENGL_ACCELERATION_STRUCTURE_H_

#include <ral/iacceleration_structure.h>
#include <ral/ral_types.h>

namespace Piece::RAL
{
class OpenGLAccelerationStructure : public IAccelerationStructure
{
  public:
    explicit OpenGLAccelerationStructure(const AccelerationStructureBuildInfo &info);
    ~OpenGLAccelerationStructure() override;

    void Build(const AccelerationStructureBuildInfo &info) override;
    void Update(const AccelerationStructureBuildInfo &info) override;

  private:
    uint32_t renderer_id_ = 0; // Placeholder for OpenGL-specific ID
};
} // namespace Piece::RAL

#endif // RAL_OPENGL_OPENGL_ACCELERATION_STRUCTURE_H_
