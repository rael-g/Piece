#ifndef PIECE_RAL_IFRAMEBUFFER_H_
#define PIECE_RAL_IFRAMEBUFFER_H_

#include <cstdint>
#include <memory>

namespace Piece::RAL
{

class ITexture; // Forward declaration

/**
 * @brief Interface for a framebuffer object.
 * @details Manages off-screen rendering targets.
 */
class IFrameBuffer
{
  public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~IFrameBuffer() = default;
    /**
     * @brief Binds the framebuffer.
     */
    virtual void Bind() const = 0;
    /**
     * @brief Unbinds the framebuffer.
     */
    virtual void Unbind() const = 0;
    /**
     * @brief Gets a pointer to the color attachment texture.
     * @param index The index of the color attachment.
     * @return A pointer to the ITexture.
     */
    [[nodiscard]] virtual ITexture *GetColorAttachment(uint32_t index = 0) const = 0;
    /**
     * @brief Gets a pointer to the depth attachment texture.
     * @return A pointer to the ITexture.
     */
    [[nodiscard]] virtual ITexture *GetDepthAttachment() const = 0;
    /**
     * @brief Resizes the framebuffer.
     * @param width New width.
     * @param height New height.
     */
    virtual void Resize(uint32_t width, uint32_t height) = 0;
};

} // namespace Piece::RAL

#endif // PIECE_RAL_IFRAMEBUFFER_H_
