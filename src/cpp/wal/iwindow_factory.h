/**
 * @file iwindow_factory.h
 * @brief Defines the IWindowFactory interface for creating window instances.
 */
#ifndef WAL_IWINDOW_FACTORY_H_
#define WAL_IWINDOW_FACTORY_H_

#include <wal/iwindow.h>
#include "native_window_options.h" // New include

#include <memory>

namespace Piece
{
namespace WAL
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

} // namespace WAL
} // namespace Piece

#endif // WAL_IWINDOW_FACTORY_H_