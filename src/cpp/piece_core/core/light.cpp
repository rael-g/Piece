#include "light.h"
#include <iostream>

namespace Piece::Core
{

Light::Light()
{
    std::cout << "Light created." << std::endl;
}

Light::~Light()
{
    std::cout << "Light destroyed." << std::endl;
}

} // namespace Piece::Core
