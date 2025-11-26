#ifndef PIECE_INTERMEDIATE_IWINDOW_FACTORY_H_
#define PIECE_INTERMEDIATE_IWINDOW_FACTORY_H_

#include <memory>

#include <wal/iwindow.h>
#include <piece_intermediate/native_interop_types.h>

namespace Piece { namespace Intermediate {

class IWindowFactory {
public:
    virtual ~IWindowFactory() = default;
    // Method to create an IWindow instance
    virtual std::unique_ptr<WAL::IWindow> CreateWindow(const NativeWindowOptions* options) = 0;
};

}} // namespace Piece::Intermediate

#endif  // PIECE_INTERMEDIATE_IWINDOW_FACTORY_H_
