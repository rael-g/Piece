#pragma once

#include <piece_core/core/iresource_manager_factory.h>
#include <piece_core/core/resource_manager.h> // Include the concrete ResourceManager

namespace Piece
{
    namespace Core
    {
        /// @brief Concrete factory for creating ResourceManager instances.
        class ResourceManagerFactory : public IResourceManagerFactory
        {
          public:
            /// @brief Creates an instance of ResourceManager.
            /// @param graphics_device A raw pointer to the IGraphicsDevice.
            /// @return A unique_ptr to a new ResourceManager instance.
            std::unique_ptr<IResourceManager> CreateResourceManager(RAL::IGraphicsDevice *graphics_device) override
            {
                return std::make_unique<ResourceManager>(graphics_device);
            }
        };
    } // namespace Core
} // namespace Piece