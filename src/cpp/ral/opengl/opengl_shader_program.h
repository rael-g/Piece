#ifndef RAL_OPENGL_OPENGL_SHADER_PROGRAM_H_
#define RAL_OPENGL_OPENGL_SHADER_PROGRAM_H_

#include <cstdint>
#include <ral/interfaces/ishader.h>
#include <ral/interfaces/ishader_program.h>
#include <string>
#include <vector>

namespace Piece::RAL
{
class OpenGLShaderProgram : public IShaderProgram
{
  public:
    explicit OpenGLShaderProgram(const std::vector<IShader *> &shaderModules);
    ~OpenGLShaderProgram() override;

    void Bind() const override;
    void Unbind() const override;

    void SetUniformInt(const std::string &name, int value) override;
    void SetUniformFloat(const std::string &name, float value) override;
    void SetUniformFloat2(const std::string &name, float v0, float v1) override;
    void SetUniformFloat3(const std::string &name, float v0, float v1, float v2) override;
    void SetUniformFloat4(const std::string &name, float v0, float v1, float v2, float v3) override;
    void SetUniformMat4(const std::string &name, const float *matrix) override;

  private:
    uint32_t renderer_id_ = 0;
};
} // namespace Piece::RAL

#endif // RAL_OPENGL_OPENGL_SHADER_PROGRAM_H_
