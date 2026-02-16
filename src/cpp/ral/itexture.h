#ifndef PIECE_RAL_ITEXTURE_H_
#define PIECE_RAL_ITEXTURE_H_

#include <cstdint>
#include <ral/ral_types.h>

namespace Piece::RAL
{

/**
 * @brief Interface for a texture.
 * @details Stores image data on the GPU. Does not manage sampling states.
 */
class ITexture
{
  public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~ITexture() = default;
    /**
     * @brief Binds the texture to a texture unit.
     * @param slot The texture unit to bind the texture to.
     */
    virtual void Bind(uint32_t slot = 0) const = 0;
    /**
     * @brief Unbinds the texture from a texture unit.
     */
    virtual void Unbind() const = 0;
    /**
     * @brief Gets the width of the texture.
     * @return The width.
     */
    [[nodiscard]] virtual uint32_t GetWidth() const = 0;
    /**
     * @brief Gets the height of the texture.
     * @return The height.
     */
    [[nodiscard]] virtual uint32_t GetHeight() const = 0;
    /**
     * @brief Gets the native renderer ID of the texture object.
     * @return The renderer-specific ID.
     */
    [[nodiscard]] virtual void *GetNativeID() const = 0;
};

} // namespace Piece::RAL

#endif // PIECE_RAL_ITEXTURE_H_
