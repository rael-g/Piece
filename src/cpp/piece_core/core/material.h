#ifndef PIECE_CORE_MATERIAL_H_
#define PIECE_CORE_MATERIAL_H_

#include <array>
#include <memory>
#include <string>
#include <unordered_map>

#include <ral/interfaces/ishader_program.h>
#include <ral/isampler.h>
#include <ral/itexture.h>
#include <ral/iuniform_buffer.h>

namespace Piece::Core
{

/**
 * @brief Represents a material, encapsulating shader program, textures, and uniform parameters.
 */
class Material
{
  public:
    Material();
    ~Material();

    void SetShaderProgram(std::shared_ptr<Piece::RAL::IShaderProgram> shader_program);
    void SetTexture(const std::string &name, std::shared_ptr<Piece::RAL::ITexture> texture);
    void SetSampler(const std::string &name, std::shared_ptr<Piece::RAL::ISampler> sampler);
    void SetUniformBuffer(const std::string &name, std::shared_ptr<Piece::RAL::IUniformBuffer> uniform_buffer);

    void SetFloat(const std::string &name, float value);
    void SetFloat2(const std::string &name, float v0, float v1);
    void SetFloat3(const std::string &name, float v0, float v1, float v2);
    void SetFloat4(const std::string &name, float v0, float v1, float v2, float v3);
    void SetMat4(const std::string &name, const float *matrix);
    void SetInt(const std::string &name, int value);

    [[nodiscard]] std::shared_ptr<Piece::RAL::IShaderProgram> GetShaderProgram() const
    {
        return shader_program_;
    }

  private:
    std::shared_ptr<Piece::RAL::IShaderProgram> shader_program_;
    std::unordered_map<std::string, std::shared_ptr<Piece::RAL::ITexture>> textures_;
    std::unordered_map<std::string, std::shared_ptr<Piece::RAL::ISampler>> samplers_;
    std::unordered_map<std::string, std::shared_ptr<Piece::RAL::IUniformBuffer>> uniform_buffers_;

    // Simple storage for uniform values for now.
    // In a real engine, these would be packed into uniform buffers.
    std::unordered_map<std::string, float> float_params_;
    std::unordered_map<std::string, std::array<float, 2>> float2_params_;
    std::unordered_map<std::string, std::array<float, 3>> float3_params_;
    std::unordered_map<std::string, std::array<float, 4>> float4_params_;
    std::unordered_map<std::string, std::array<float, 16>> mat4_params_;
    std::unordered_map<std::string, int> int_params_;
};

} // namespace Piece::Core

#endif // PIECE_CORE_MATERIAL_H_
