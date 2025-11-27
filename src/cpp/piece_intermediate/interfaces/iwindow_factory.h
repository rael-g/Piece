#ifndef PIECE_INTERMEDIATE_IWINDOW_FACTORY_H_
#define PIECE_INTERMEDIATE_IWINDOW_FACTORY_H_

#include <piece_intermediate/native_interop_types.h>
#include <wal/iwindow.h>

#include <memory>

namespace Piece
{
namespace Intermediate
{

class IWindowFactory
{
  public:
    virtual ~IWindowFactory() = default;

    virtual std::unique_ptr<WAL::IWindow> CreateWindow(const NativeWindowOptions *options) = 0;
};

} // namespace Intermediate
} // namespace Piece

#endif // PIECE_INTERMEDIATE_IWINDOW_FACTORY_H_
