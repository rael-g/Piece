#ifndef RAL_OPENGL_OPENGL_TEXTURE_H_
#define RAL_OPENGL_OPENGL_TEXTURE_H_

#include <cstdint>
#include <ral/itexture.h>
#include <ral/ral_types.h>

namespace Piece::RAL
{
class OpenGLTexture : public ITexture
{
  public:
    OpenGLTexture(TextureType type, uint32_t width, uint32_t height, uint32_t depth, TextureFormat format,
                  const void *data = nullptr);
    ~OpenGLTexture() override;

    void Bind(uint32_t slot = 0) const override;
    void Unbind() const override;
    [[nodiscard]] uint32_t GetWidth() const override;
    [[nodiscard]] uint32_t GetHeight() const override;
    [[nodiscard]] void *GetNativeID() const override;

  private:
    uint32_t renderer_id_ = 0; // Placeholder for OpenGL Texture ID
    uint32_t width_ = 0;
    uint32_t height_ = 0;
    // Note: Depth and Type are stored during creation but not explicitly exposed in getters for simplicity.
};
} // namespace Piece::RAL

#endif // RAL_OPENGL_OPENGL_TEXTURE_H_
