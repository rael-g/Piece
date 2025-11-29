#ifndef PIECE_CORE_IWINDOW_FACTORY_H_
#define PIECE_CORE_IWINDOW_FACTORY_H_

#include <piece_core/native_interop_types.h>
#include <wal/iwindow.h>

#include <memory>

namespace Piece
{
namespace Core
{

class IWindowFactory
{
  public:
    virtual ~IWindowFactory() = default;

    virtual std::unique_ptr<WAL::IWindow> CreateWindow(const NativeWindowOptions *options) = 0;
};

} // namespace Core
} // namespace Piece

#endif // PIECE_CORE_IWINDOW_FACTORY_H_
