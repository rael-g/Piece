#ifndef PIECE_RAL_INTERFACES_ISHADER_PROGRAM_H_
#define PIECE_RAL_INTERFACES_ISHADER_PROGRAM_H_

#include <cstdint> // For uint32_t
#include <string>

namespace Piece { namespace RAL {

class IShader; // Forward declaration

class IShaderProgram {
public:
    virtual ~IShaderProgram() = default;
    virtual bool Link(IShader* vertexShader, IShader* fragmentShader) = 0; // Add other shader types as needed
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual uint32_t GetRendererID() const = 0;

    // Uniform setting methods (examples)
    virtual void SetUniform1i(const std::string& name, int value) = 0;
    virtual void SetUniform1f(const std::string& name, float value) = 0;
    // virtual void SetUniformMat4f(const std::string& name, const glm::mat4& matrix) = 0;
    // virtual void SetUniformVec3f(const std::string& name, const glm::vec3& vector) = 0;
};

}} // namespace Piece::RAL

#endif  // PIECE_RAL_INTERFACES_ISHADER_PROGRAM_H_
