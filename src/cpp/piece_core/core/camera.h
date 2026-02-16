#ifndef PIECE_CORE_CAMERA_H_
#define PIECE_CORE_CAMERA_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Piece::Core
{

/**
 * @brief Defines the rendering perspective.
 */
class Camera
{
  public:
    Camera();
    ~Camera();

    void SetPosition(const glm::vec3 &position)
    {
        position_ = position;
        UpdateViewMatrix();
    }
    void SetRotation(const glm::quat &rotation)
    {
        rotation_ = rotation;
        UpdateViewMatrix();
    }
    void SetPerspective(float fov_y, float aspect_ratio, float near_plane, float far_plane);

    [[nodiscard]] const glm::vec3 &GetPosition() const
    {
        return position_;
    }
    [[nodiscard]] const glm::quat &GetRotation() const
    {
        return rotation_;
    }
    [[nodiscard]] const glm::mat4 &GetViewMatrix() const
    {
        return view_matrix_;
    }
    [[nodiscard]] const glm::mat4 &GetProjectionMatrix() const
    {
        return projection_matrix_;
    }
    [[nodiscard]] const glm::mat4 &GetViewProjectionMatrix() const
    {
        return view_projection_matrix_;
    }

  private:
    void UpdateViewMatrix();
    void UpdateViewProjectionMatrix();

    glm::vec3 position_ = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::quat rotation_ = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // Identity quaternion

    glm::mat4 view_matrix_ = glm::mat4(1.0f);
    glm::mat4 projection_matrix_ = glm::mat4(1.0f);
    glm::mat4 view_projection_matrix_ = glm::mat4(1.0f);
};

} // namespace Piece::Core

#endif // PIECE_CORE_CAMERA_H_
