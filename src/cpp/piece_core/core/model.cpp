#include "model.h"
#include <iostream>

namespace Piece::Core
{

Model::Model()
{
    std::cout << "Model created." << std::endl;
    UpdateModelMatrix();
}

Model::~Model()
{
    std::cout << "Model destroyed." << std::endl;
}

void Model::UpdateModelMatrix()
{
    auto transform = glm::mat4(1.0f);
    transform = glm::translate(transform, position_);
    transform = transform * glm::mat4_cast(rotation_); // Apply rotation
    transform = glm::scale(transform, scale_);
    model_matrix_ = transform;
}

} // namespace Piece::Core
