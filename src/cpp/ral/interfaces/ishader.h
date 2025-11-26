#ifndef PIECE_RAL_INTERFACES_ISHADER_H_
#define PIECE_RAL_INTERFACES_ISHADER_H_

#include <string>

namespace Piece { namespace RAL {

enum class ShaderType {
    Vertex,
    Fragment,
    Geometry,
    Compute,
    Unknown
};

class IShader {
public:
    virtual ~IShader() = default;
    virtual bool Compile(const std::string& source, ShaderType type) = 0;
    virtual uint32_t GetRendererID() const = 0;
};

}} // namespace Piece::RAL

#endif  // PIECE_RAL_INTERFACES_ISHADER_H_
