#pragma once

#include <memory>
#include <piece_core/core/iresource_manager.h> // Include the new interface

namespace Piece
{
    namespace Core
    {
        /// @brief Interface for creating IResourceManager instances.
        ///
        /// This factory interface allows for dependency injection of resource manager
        /// implementations, enabling easier testing and flexibility in resource handling.
        class IResourceManagerFactory
        {
          public:
            virtual ~IResourceManagerFactory() = default;

            /// @brief Creates an instance of IResourceManager.
            /// @param graphics_device A raw pointer to the IGraphicsDevice, required by ResourceManager's constructor.
            ///                        Ownership of graphics_device is NOT transferred.
            /// @return A unique_ptr to a new IResourceManager instance.
            virtual std::unique_ptr<IResourceManager> CreateResourceManager(RAL::IGraphicsDevice *graphics_device) = 0;
        };
    } // namespace Core
} // namespace Piece