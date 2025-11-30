/**
 * @file iwindow_factory.h
 * @brief Defines the IWindowFactory interface for creating window instances.
 */
#ifndef PIECE_CORE_IWINDOW_FACTORY_H_
#define PIECE_CORE_IWINDOW_FACTORY_H_

#include <piece_core/native_interop_types.h>
#include <wal/iwindow.h>

#include <memory>

namespace Piece
{
namespace Core
{

/**
 * @brief Abstract factory interface for creating IWindow instances.
 */
class IWindowFactory
{
  public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~IWindowFactory() = default;

    /**
     * @brief Creates a new IWindow instance.
     * @param options Configuration options for the window.
     * @return A unique_ptr to the newly created IWindow instance.
     */
    virtual std::unique_ptr<WAL::IWindow> CreateWindow(const NativeWindowOptions *options) = 0;
};

} // namespace Core
} // namespace Piece

#endif // PIECE_CORE_IWINDOW_FACTORY_H_
