#ifndef PIECE_RAL_IUNIFORM_BUFFER_H_
#define PIECE_RAL_IUNIFORM_BUFFER_H_

#include <cstdint>

namespace Piece::RAL
{

/**
 * @brief Interface for a uniform buffer.
 * @details Provides a block of data for shaders that can be updated efficiently.
 */
class IUniformBuffer
{
  public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~IUniformBuffer() = default;
    /**
     * @brief Sets the data of the uniform buffer.
     * @param data Pointer to the buffer data.
     * @param size Size of the buffer data in bytes.
     */
    virtual void SetData(const void *data, uint32_t size) = 0;
    /**
     * @brief Gets the size of the uniform buffer in bytes.
     * @return The size of the uniform buffer.
     */
    [[nodiscard]] virtual uint32_t GetSize() const = 0;
};

} // namespace Piece::RAL

#endif // PIECE_RAL_IUNIFORM_BUFFER_H_
