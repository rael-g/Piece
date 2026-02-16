#ifndef PIECE_RAL_IINDIRECT_DRAW_BUFFER_H_
#define PIECE_RAL_IINDIRECT_DRAW_BUFFER_H_

#include <cstdint>
#include <ral/ral_types.h>

namespace Piece::RAL
{

/**
 * @brief Interface for an indirect draw buffer.
 * @details Stores parameters for drawing calls that are issued by the GPU (GPU-Driven Rendering).
 */
class IIndirectDrawBuffer
{
  public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~IIndirectDrawBuffer() = default;
    /**
     * @brief Sets the data of the indirect draw buffer.
     * @param data Pointer to the buffer data.
     * @param size Size of the buffer data in bytes.
     */
    virtual void SetData(const void *data, uint32_t size) = 0;
    /**
     * @brief Gets the size of the indirect draw buffer in bytes.
     * @return The size of the indirect draw buffer.
     */
    [[nodiscard]] virtual uint32_t GetSize() const = 0;
};

} // namespace Piece::RAL

#endif // PIECE_RAL_IINDIRECT_DRAW_BUFFER_H_
