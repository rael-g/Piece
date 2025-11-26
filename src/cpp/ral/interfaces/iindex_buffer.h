#ifndef PIECE_RAL_INTERFACES_IINDEX_BUFFER_H_
#define PIECE_RAL_INTERFACES_IINDEX_BUFFER_H_

#include <cstdint> // For uint32_t

namespace Piece { namespace RAL {

class IIndexBuffer {
public:
    virtual ~IIndexBuffer() = default;
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual uint32_t GetCount() const = 0;
};

}} // namespace Piece::RAL

#endif  // PIECE_RAL_INTERFACES_IINDEX_BUFFER_H_
