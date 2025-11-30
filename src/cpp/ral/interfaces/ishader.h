/**
 * @file ishader.h
 * @brief Defines the IShader interface and ShaderType enum.
 */
#ifndef PIECE_RAL_INTERFACES_ISHADER_H_
#define PIECE_RAL_INTERFACES_ISHADER_H_

#include <string>

namespace Piece
{
namespace RAL
{

/**
 * @brief Specifies the type of a shader.
 */
enum class ShaderType
{
    Vertex,   /**< Vertex shader. */
    Fragment, /**< Fragment (or pixel) shader. */
    Geometry, /**< Geometry shader. */
    Compute,  /**< Compute shader. */
    Unknown   /**< Unknown shader type. */
};

/**
 * @brief Interface for a shader object.
 * @details This class provides a pure virtual interface for managing a single shader object,
 *          such as a vertex or fragment shader, including its compilation.
 */
class IShader
{
  public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~IShader() = default;
    /**
     * @brief Compiles the shader from a source string.
     * @param source The shader's source code.
     * @param type The type of the shader.
     * @return True if compilation was successful, false otherwise.
     */
    virtual bool Compile(const std::string &source, ShaderType type) = 0;
    /**
     * @brief Gets the native renderer ID of the shader object.
     * @return The renderer-specific ID.
     */
    virtual uint32_t GetRendererID() const = 0;
};

} // namespace RAL
} // namespace Piece

#endif // PIECE_RAL_INTERFACES_ISHADER_H_
