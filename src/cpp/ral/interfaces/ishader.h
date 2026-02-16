#ifndef PIECE_RAL_INTERFACES_ISHADER_H_
#define PIECE_RAL_INTERFACES_ISHADER_H_

namespace Piece::RAL
{

/**
 * @brief Interface for a shader module.
 * @details This class provides a pure virtual interface for a single compiled shader stage (e.g., Vertex, Fragment,
 * Compute).
 */
class IShader
{
  public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~IShader() = default;
};

} // namespace Piece::RAL

#endif // PIECE_RAL_INTERFACES_ISHADER_H_
