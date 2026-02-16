#ifndef PIECE_RAL_ICOMPUTE_BUFFER_H_
#define PIECE_RAL_ICOMPUTE_BUFFER_H_

#include <cstdint>
#include <ral/ral_types.h>

namespace Piece::RAL
{

/**
 * @brief Interface for a compute buffer (storage buffer).
 * @details Manages GPU memory buffers that can be read and written by compute shaders.
 */
class IComputeBuffer
{
  public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~IComputeBuffer() = default;
    /**
     * @brief Sets the data of the compute buffer.
     * @param data Pointer to the buffer data.
     * @param size Size of the buffer data in bytes.
     */
    virtual void SetData(const void *data, uint32_t size) = 0;
    /**
     * @brief Gets the size of the compute buffer in bytes.
     * @return The size of the compute buffer.
     */
    [[nodiscard]] virtual uint32_t GetSize() const = 0;
};

} // namespace Piece::RAL

#endif // PIECE_RAL_ICOMPUTE_BUFFER_H_
