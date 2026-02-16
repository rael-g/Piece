#ifndef PIECE_RAL_ISAMPLER_H_
#define PIECE_RAL_ISAMPLER_H_

#include <cstdint>
#include <ral/ral_types.h>

namespace Piece::RAL
{

/**
 * @brief Interface for a texture sampler.
 * @details Defines how a texture is sampled by shaders (filtering, wrap modes).
 */
class ISampler
{
  public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~ISampler() = default;
    /**
     * @brief Binds the sampler to a texture unit.
     * @param slot The texture unit to bind the sampler to.
     */
    virtual void Bind(uint32_t slot = 0) const = 0;
    /**
     * @brief Unbinds the sampler from a texture unit.
     */
    virtual void Unbind() const = 0;
};

} // namespace Piece::RAL

#endif // PIECE_RAL_ISAMPLER_H_
