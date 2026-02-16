#ifndef RAL_OPENGL_OPENGL_SHADER_H_
#define RAL_OPENGL_OPENGL_SHADER_H_

#include <cstdint>
#include <ral/interfaces/ishader.h>
#include <ral/ral_types.h>
#include <string>

namespace Piece::RAL
{
class OpenGLShader : public IShader
{
  public:
    OpenGLShader(ShaderStage stage, const std::string &source);
    ~OpenGLShader() override;

  private:
    uint32_t renderer_id_ = 0;
    ShaderStage stage_;
};
} // namespace Piece::RAL

#endif // RAL_OPENGL_OPENGL_SHADER_H_
