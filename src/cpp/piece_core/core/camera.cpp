#include "camera.h"
#include <iostream>

namespace Piece::Core
{

Camera::Camera()
{
    std::cout << "Camera created." << std::endl;
    UpdateViewMatrix();
    UpdateViewProjectionMatrix();
}

Camera::~Camera()
{
    std::cout << "Camera destroyed." << std::endl;
}

void Camera::SetPerspective(float fov_y, float aspect_ratio, float near_plane, float far_plane)
{
    projection_matrix_ = glm::perspective(glm::radians(fov_y), aspect_ratio, near_plane, far_plane);
    UpdateViewProjectionMatrix();
}

void Camera::UpdateViewMatrix()
{
    glm::mat4 rotate = glm::mat4_cast(rotation_);
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), -position_);
    view_matrix_ = rotate * translate;
    UpdateViewProjectionMatrix();
}

void Camera::UpdateViewProjectionMatrix()
{
    view_projection_matrix_ = projection_matrix_ * view_matrix_;
}

} // namespace Piece::Core
