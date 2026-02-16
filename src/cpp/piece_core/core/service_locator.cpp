/**
 * @file service_locator.cpp
 * @brief Implements the ServiceLocator class. Currently, this file is empty as the
 *        ServiceLocator is fully implemented in the header file.
 */
#include "service_locator.h"
#include "piece_core/logging_api.h" // Include logging_api for PIECE_INFO/TRACE

namespace Piece::Core
{

ServiceLocator &ServiceLocator::Get()
{
    PIECE_TRACE("ServiceLocator::Get()");
    static ServiceLocator instance; // Define static instance here
    return instance;
}

// Ensure the constructor for ServiceLocator is called only once
ServiceLocator::ServiceLocator()
{
    PIECE_INFO("ServiceLocator instance created.");
}

} // namespace Piece::Core
