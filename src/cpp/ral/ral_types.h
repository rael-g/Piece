#ifndef PIECE_RAL_RAL_TYPES_H_
#define PIECE_RAL_RAL_TYPES_H_

#include <cstdint>
#include <string> // Added for std::string
#include <vector>

namespace Piece::RAL
{

/**
 * @brief Enumerates the different types of shader stages.
 */
enum class ShaderStage : uint8_t
{
    kVertex,
    kFragment,
    kCompute,
    kTask,
    kMesh,
    kRayGen,
    kMiss,
    kClosestHit,
    kAnyHit,
    kIntersection,
};

/**
 * @brief Enumerates the different types of textures.
 */
enum class TextureType : uint8_t
{
    kTexture1D,
    kTexture2D,
    kTexture3D,
    kTextureCube,
    kTexture1DArray,
    kTexture2DArray,
    kTextureCubeArray,
};

/**
 * @brief Enumerates the different pixel formats for textures.
 */
enum class TextureFormat : uint8_t
{
    kUnknown,
    kR8G8B8A8Unorm,
    kR8G8B8A8Srgb,
    kR32G32B32A32Float,
    kR32G32B32Float,
    kR32Float,
    kD24UnormS8Uint, // Depth/Stencil
};

/**
 * @brief Enumerates the different usage hints for buffers.
 */
enum class BufferUsage : uint8_t
{
    kStatic,  // Data is set once and rarely changed
    kDynamic, // Data is updated frequently
    kStream,  // Data is updated almost every frame
};

/**
 * @brief Enumerates the data types for vertex attributes.
 */
enum class VertexDataType : uint8_t
{
    kFloat,
    kFloat2,
    kFloat3,
    kFloat4,
    kInt,
    kInt2,
    kInt3,
    kInt4,
    kBool,
};

/**
 * @brief Describes a single element within a vertex layout.
 */
struct VertexLayoutElement
{
    std::string name;
    VertexDataType type;
    uint32_t size;
    uint32_t offset;
    bool normalized;

    VertexLayoutElement(const std::string &name, VertexDataType type, bool normalized = false);
};

/**
 * @brief Describes the layout of vertex data.
 */
class VertexLayout
{
  public:
    VertexLayout() = default;

    void AddElement(const std::string &name, VertexDataType type, bool normalized = false);

    [[nodiscard]] uint32_t GetStride() const
    {
        return stride_;
    }
    [[nodiscard]] const std::vector<VertexLayoutElement> &GetElements() const
    {
        return elements_;
    }

  private:
    std::vector<VertexLayoutElement> elements_;
    uint32_t stride_ = 0;

    void CalculateOffsetsAndStride();
};

/**
 * @brief Enumerates functions for depth comparison.
 */
enum class DepthFunc : uint8_t
{
    kAlways,
    kNever,
    kLess,
    kEqual,
    kLessEqual,
    kGreater,
    kNotEqual,
    kGreaterEqual,
};

/**
 * @brief Enumerates blend modes for alpha blending.
 */
enum class BlendMode : uint8_t
{
    kDisabled,
    kAlpha,
    kAdditive,
};

/**
 * @brief Enumerates cull modes for back-face culling.
 */
enum class CullMode : uint8_t
{
    kNone,
    kFront,
    kBack,
    kFrontAndBack,
};

/**
 * @brief Enumerates filter modes for texture minification/magnification.
 */
enum class TextureFilter : uint8_t
{
    kNearest,
    kLinear,
};

/**
 * @brief Enumerates wrap modes for texture coordinates.
 */
enum class TextureWrap : uint8_t
{
    kRepeat,
    kMirroredRepeat,
    kClampToEdge,
    kClampToBorder,
};

/**
 * @brief Information required to build an acceleration structure.
 */
struct AccelerationStructureBuildInfo
{
    // Placeholder for actual build information (e.g., geometry data, flags)
    // This would contain more detailed data depending on the RT API (e.g., VkAccelerationStructureBuildGeometryInfoKHR)
    uint32_t dummy_data = 0;
};

/**
 * @brief Settings for Variable Rate Shading (VRS).
 */
struct VRSSettings
{
    // Placeholder for VRS settings (e.g., shading rate, combiners)
    uint32_t dummy_data = 0;
};

} // namespace Piece::RAL

#endif // PIECE_RAL_RAL_TYPES_H_
