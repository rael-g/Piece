#ifndef PIECE_RAL_INTERFACES_ISHADER_PROGRAM_H_
#define PIECE_RAL_INTERFACES_ISHADER_PROGRAM_H_

#include <cstdint>
#include <string>
#include <vector> // Required for IGraphicsDevice::CreateShaderProgram to pass std::vector<IShader*>

namespace Piece::RAL
{

class IShader; // Forward declaration

/**
 * @brief Interface for a shader program.
 * @details This class provides a pure virtual interface for managing a complete shader program,
 *          which consists of linked shader stages (e.g., vertex and fragment shaders) and their uniform definitions.
 */
class IShaderProgram
{
  public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~IShaderProgram() = default;

    /**
     * @brief Binds the shader program to the rendering pipeline.
     */
    virtual void Bind() const = 0;
    /**
     * @brief Unbinds the shader program from the rendering pipeline.
     */
    virtual void Unbind() const = 0;

    /**
     * @brief Sets an integer uniform variable.
     * @param name The name of the uniform.
     * @param value The integer value to set.
     */
    virtual void SetUniformInt(const std::string &name, int value) = 0;
    /**
     * @brief Sets a float uniform variable.
     * @param name The name of the uniform.
     * @param value The float value to set.
     */
    virtual void SetUniformFloat(const std::string &name, float value) = 0;
    /**
     * @brief Sets a 2-component float vector uniform variable.
     * @param name The name of the uniform.
     * @param v0 The first component.
     * @param v1 The second component.
     */
    virtual void SetUniformFloat2(const std::string &name, float v0, float v1) = 0;
    /**
     * @brief Sets a 3-component float vector uniform variable.
     * @param name The name of the uniform.
     * @param v0 The first component.
     * @param v1 The second component.
     * @param v2 The third component.
     */
    virtual void SetUniformFloat3(const std::string &name, float v0, float v1, float v2) = 0;
    /**
     * @brief Sets a 4-component float vector uniform variable.
     * @param name The name of the uniform.
     * @param v0 The first component.
     * @param v1 The second component.
     * @param v2 The third component.
     * @param v3 The fourth component.
     */
    virtual void SetUniformFloat4(const std::string &name, float v0, float v1, float v2, float v3) = 0;
    /**
     * @brief Sets a 4x4 float matrix uniform variable.
     * @param name The name of the uniform.
     * @param matrix A pointer to the first element of the 4x4 matrix (column-major or row-major depending on API).
     */
    virtual void SetUniformMat4(const std::string &name, const float *matrix) = 0;
};

} // namespace Piece::RAL

#endif // PIECE_RAL_INTERFACES_ISHADER_PROGRAM_H_
