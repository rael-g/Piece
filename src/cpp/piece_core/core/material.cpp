#include "material.h"
#include <iostream>

namespace Piece::Core
{
using namespace Piece::RAL;

Material::Material()
{
    std::cout << "Material created." << std::endl;
}

Material::~Material()
{
    std::cout << "Material destroyed." << std::endl;
}

void Material::SetShaderProgram(std::shared_ptr<RAL::IShaderProgram> shader_program)
{
    shader_program_ = std::move(shader_program);
}

void Material::SetTexture(const std::string &name, std::shared_ptr<RAL::ITexture> texture)
{
    textures_[name] = std::move(texture);
}

void Material::SetSampler(const std::string &name, std::shared_ptr<RAL::ISampler> sampler)
{
    samplers_[name] = std::move(sampler);
}

void Material::SetUniformBuffer(const std::string &name, std::shared_ptr<RAL::IUniformBuffer> uniform_buffer)
{
    uniform_buffers_[name] = std::move(uniform_buffer);
}

void Material::SetFloat(const std::string &name, float value)
{
    float_params_[name] = value;
    if (shader_program_)
    {
        shader_program_->SetUniformFloat(name, value);
    }
}

void Material::SetFloat2(const std::string &name, float v0, float v1)
{
    float2_params_[name] = {v0, v1};
    if (shader_program_)
    {
        shader_program_->SetUniformFloat2(name, v0, v1);
    }
}

void Material::SetFloat3(const std::string &name, float v0, float v1, float v2)
{
    float3_params_[name] = {v0, v1, v2};
    if (shader_program_)
    {
        shader_program_->SetUniformFloat3(name, v0, v1, v2);
    }
}

void Material::SetFloat4(const std::string &name, float v0, float v1, float v2, float v3)
{
    float4_params_[name] = {v0, v1, v2, v3};
    if (shader_program_)
    {
        shader_program_->SetUniformFloat4(name, v0, v1, v2, v3);
    }
}

void Material::SetMat4(const std::string &name, const float *matrix)
{
    if (matrix)
    {
        std::copy(matrix, matrix + 16, mat4_params_[name].begin());
        if (shader_program_)
        {
            shader_program_->SetUniformMat4(name, matrix);
        }
    }
}

void Material::SetInt(const std::string &name, int value)
{
    int_params_[name] = value;
    if (shader_program_)
    {
        shader_program_->SetUniformInt(name, value);
    }
}

} // namespace Piece::Core
