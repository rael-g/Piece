#ifndef PIECE_CORE_LIGHT_H_
#define PIECE_CORE_LIGHT_H_

#include <glm/glm.hpp>

namespace Piece::Core
{

enum class LightType
{
    kDirectional,
    kPoint,
    kSpot,
};

/**
 * @brief Represents a light source in the scene.
 */
class Light
{
  public:
    Light();
    ~Light();

    void SetType(LightType type)
    {
        type_ = type;
    }
    void SetPosition(const glm::vec3 &position)
    {
        position_ = position;
    }
    void SetDirection(const glm::vec3 &direction)
    {
        direction_ = glm::normalize(direction);
    }
    void SetColor(const glm::vec3 &color)
    {
        color_ = color;
    }
    void SetIntensity(float intensity)
    {
        intensity_ = intensity;
    }

    [[nodiscard]] LightType GetType() const
    {
        return type_;
    }
    [[nodiscard]] const glm::vec3 &GetPosition() const
    {
        return position_;
    }
    [[nodiscard]] const glm::vec3 &GetDirection() const
    {
        return direction_;
    }
    [[nodiscard]] const glm::vec3 &GetColor() const
    {
        return color_;
    }
    [[nodiscard]] float GetIntensity() const
    {
        return intensity_;
    }

  private:
    LightType type_ = LightType::kDirectional;
    glm::vec3 position_ = glm::vec3(0.0f);
    glm::vec3 direction_ = glm::vec3(0.0f, -1.0f, 0.0f); // Downwards directional light
    glm::vec3 color_ = glm::vec3(1.0f);
    float intensity_ = 1.0f;
};

} // namespace Piece::Core

#endif // PIECE_CORE_LIGHT_H_
