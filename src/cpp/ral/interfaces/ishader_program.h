/**
 * @file ishader_program.h
 * @brief Defines the IShaderProgram interface for managing linked shader programs.
 */
#ifndef PIECE_RAL_INTERFACES_ISHADER_PROGRAM_H_
#define PIECE_RAL_INTERFACES_ISHADER_PROGRAM_H_

#include <cstdint>
#include <string>
#include <glm/glm.hpp> // Required for glm::mat4 and glm::vec3

namespace Piece
{
namespace RAL
{

class IShader;

/**
 * @brief Interface for a shader program.
 * @details This class provides a pure virtual interface for managing a complete shader program,
 *          which consists of linked shader stages (e.g., vertex and fragment shaders).
 */
class IShaderProgram
{
  public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~IShaderProgram() = default;
    /**
     * @brief Links vertex and fragment shaders into a shader program.
     * @param vertexShader The vertex shader to link.
     * @param fragmentShader The fragment shader to link.
     * @return True if linking was successful, false otherwise.
     */
    virtual bool Link(IShader *vertexShader, IShader *fragmentShader) = 0;
    /**
     * @brief Binds the shader program to the rendering pipeline.
     */
    virtual void Bind() const = 0;
    /**
     * @brief Unbinds the shader program from the rendering pipeline.
     */
    virtual void Unbind() const = 0;
    /**
     * @brief Gets the native renderer ID of the shader program.
     * @return The renderer-specific ID.
     */
    virtual uint32_t GetRendererID() const = 0;

    /**
     * @brief Sets an integer uniform variable.
     * @param name The name of the uniform.
     * @param value The integer value to set.
     */
    virtual void SetUniform1i(const std::string &name, int value) = 0;
    /**
     * @brief Sets a float uniform variable.
     * @param name The name of the uniform.
     * @param value The float value to set.
     */
    virtual void SetUniform1f(const std::string &name, float value) = 0;
    /**
     * @brief Sets a 4x4 float matrix uniform variable.
     * @param name The name of the uniform.
     * @param matrix The matrix to set.
     */
    virtual void SetUniformMat4f(const std::string& name, const glm::mat4& matrix) = 0;
    /**
     * @brief Sets a 3-component float vector uniform variable.
     * @param name The name of the uniform.
     * @param vector The vector to set.
     */
    virtual void SetUniformVec3f(const std::string& name, const glm::vec3& vector) = 0;
};

} // namespace RAL
} // namespace Piece

#endif // PIECE_RAL_INTERFACES_ISHADER_PROGRAM_H_
