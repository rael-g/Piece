#ifndef PIECE_CORE_MODEL_H_
#define PIECE_CORE_MODEL_H_

#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "material.h"
#include "mesh.h"
#include "piece_core_exports.h" // Include PIECE_CORE_API definition

namespace Piece::Core
{

/**
 * @brief Represents a renderable object in the 3D world, combining a Mesh, Material, and transformation.
 */
class PIECE_CORE_API Model
{
  public:
    Model();
    ~Model();

    void SetMesh(std::shared_ptr<Mesh> mesh)
    {
        mesh_ = std::move(mesh);
    }
    void SetMaterial(std::shared_ptr<Material> material)
    {
        material_ = std::move(material);
    }

    [[nodiscard]] const std::shared_ptr<Mesh> &GetMesh() const
    {
        return mesh_;
    }
    [[nodiscard]] const std::shared_ptr<Material> &GetMaterial() const
    {
        return material_;
    }

    void SetPosition(const glm::vec3 &position)
    {
        position_ = position;
        UpdateModelMatrix();
    }
    void SetRotation(const glm::quat &rotation)
    {
        rotation_ = rotation;
        UpdateModelMatrix();
    }
    void SetScale(const glm::vec3 &scale)
    {
        scale_ = scale;
        UpdateModelMatrix();
    }

    [[nodiscard]] const glm::vec3 &GetPosition() const
    {
        return position_;
    }
    [[nodiscard]] const glm::quat &GetRotation() const
    {
        return rotation_;
    }
    [[nodiscard]] const glm::vec3 &GetScale() const
    {
        return scale_;
    }
    [[nodiscard]] const glm::mat4 &GetModelMatrix() const
    {
        return model_matrix_;
    }

  private:
    void UpdateModelMatrix();

    std::shared_ptr<Mesh> mesh_;
    std::shared_ptr<Material> material_;

    glm::vec3 position_ = glm::vec3(0.0f);
    glm::quat rotation_ = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    glm::vec3 scale_ = glm::vec3(1.0f);
    glm::mat4 model_matrix_ = glm::mat4(1.0f);
};

} // namespace Piece::Core

#endif // PIECE_CORE_MODEL_H_
