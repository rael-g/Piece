#include "service_locator.h"

// Define the static instance if needed, but for a default constructor and
// Meyers' Singleton (static local variable), it's often not strictly necessary
// to have a separate .cpp for the instance definition.
// All logic is currently in the header for simplicity and inlining.

namespace Piece { namespace Intermediate {
// No explicit definitions needed here as per current design (Meyers' singleton and inlined methods).
}} // namespace Piece::Intermediate
