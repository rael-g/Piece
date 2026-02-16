#ifndef RAL_OPENGL_OPENGL_SAMPLER_H_
#define RAL_OPENGL_OPENGL_SAMPLER_H_

#include <cstdint>
#include <ral/isampler.h>
#include <ral/ral_types.h>

namespace Piece::RAL
{
class OpenGLSampler : public ISampler
{
  public:
    OpenGLSampler(TextureFilter minFilter, TextureFilter magFilter, TextureWrap sWrap, TextureWrap tWrap);
    ~OpenGLSampler() override;

    void Bind(uint32_t slot = 0) const override;
    void Unbind() const override;

  private:
    uint32_t renderer_id_ = 0; // Placeholder for OpenGL Sampler Object ID
};
} // namespace Piece::RAL

#endif // RAL_OPENGL_OPENGL_SAMPLER_H_
