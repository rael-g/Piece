#ifndef PIECE_RAL_IACCELERATION_STRUCTURE_H_
#define PIECE_RAL_IACCELERATION_STRUCTURE_H_

#include <cstdint>
#include <ral/ral_types.h>

namespace Piece::RAL
{

/**
 * @brief Interface for an acceleration structure for ray tracing.
 * @details Abstracts hierarchical data structures (e.g., BVH) to accelerate Ray Tracing queries.
 */
class IAccelerationStructure
{
  public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~IAccelerationStructure() = default;
    /**
     * @brief Builds the acceleration structure.
     * @param info Information required to build the acceleration structure.
     */
    virtual void Build(const AccelerationStructureBuildInfo &info) = 0;
    /**
     * @brief Updates the acceleration structure.
     * @param info Information required to update the acceleration structure.
     */
    virtual void Update(const AccelerationStructureBuildInfo &info) = 0;
};

} // namespace Piece::RAL

#endif // PIECE_RAL_IACCELERATION_STRUCTURE_H_
