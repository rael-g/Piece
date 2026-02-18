#ifndef PIECE_CORE_ENGINE_CORE_FACTORY_H_
#define PIECE_CORE_ENGINE_CORE_FACTORY_H_

#include "iengine_core_factory.h"
#include <piece_core/engine_core.h> // Ensure EngineCore is included

namespace Piece::Core
{

class EngineCoreFactory : public IEngineCoreFactory
{
public:
    std::unique_ptr<EngineCore> CreateEngineCore() override {
        return std::make_unique<EngineCore>();
    }
};

} // namespace Piece::Core

#endif // PIECE_CORE_ENGINE_CORE_FACTORY_H_
