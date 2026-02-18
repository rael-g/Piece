#ifndef PIECE_CORE_IENGINE_CORE_FACTORY_H_
#define PIECE_CORE_IENGINE_CORE_FACTORY_H_

#include <memory>
// #include <piece_core/engine_core.h> // Remove this include

namespace Piece::Core {
    class EngineCore; // Forward declaration
}

namespace Piece::Core
{

class IEngineCoreFactory
{
public:
    virtual ~IEngineCoreFactory() = default;
    virtual std::unique_ptr<EngineCore> CreateEngineCore() = 0;
};

} // namespace Piece::Core

#endif // PIECE_CORE_IENGINE_CORE_FACTORY_H_
