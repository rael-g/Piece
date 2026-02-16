#include "render_system.h"
#include <array>
#include <glm/gtc/type_ptr.hpp> // Added for glm::value_ptr
#include <piece_core/logging_api.h>

namespace Piece::Core
{

RenderSystem::RenderSystem(RAL::IGraphicsDevice *graphics_device) : graphics_device_(graphics_device)
{
    PIECE_INFO("RenderSystem created with GraphicsDevice: {0}.", fmt::ptr(graphics_device));
    render_context_ = graphics_device_->GetImmediateContext();
    if (!render_context_)
    {
        PIECE_ERROR("Failed to get immediate render context from graphics device.");
    }
}

RenderSystem::~RenderSystem()
{
    PIECE_INFO("RenderSystem destroyed.");
}

void RenderSystem::RenderFrame(const Camera &camera, const Light &light,
                               const std::vector<std::shared_ptr<Model>> &models)
{
    PIECE_TRACE("RenderSystem::RenderFrame (Camera: {0}, Light: {1}, Models count: {2})", fmt::ptr(&camera),
                fmt::ptr(&light), models.size());
    graphics_device_->BeginFrame();

    render_context_->Clear(0.1f, 0.1f, 0.1f, 1.0f); // Clear to a dark grey
    // Assuming viewport is set externally or by the Init of IGraphicsDevice

    // For simplicity, directly bind the camera's view-projection matrix to shaders
    // In a real scenario, this would be in a uniform buffer.
    std::array<float, 16> view_proj_matrix_data;
    const glm::mat4 &view_proj = camera.GetViewProjectionMatrix();
    std::copy(glm::value_ptr(view_proj), glm::value_ptr(view_proj) + 16, view_proj_matrix_data.begin());

    for (const auto &model : models)
    {
        if (!model || !model->GetMesh() || !model->GetMaterial() || !model->GetMaterial()->GetShaderProgram())
        {
            PIECE_WARN("Skipping rendering for an incomplete model.");
            continue;
        }

        auto shader_program = model->GetMaterial()->GetShaderProgram();
        shader_program->Bind();
        PIECE_TRACE("Bound shader program: {0}", fmt::ptr(shader_program));

        // Pass camera uniforms
        shader_program->SetUniformMat4("u_ViewProjection", view_proj_matrix_data.data());

        // Pass model matrix
        std::array<float, 16> model_matrix_data;
        const glm::mat4 &model_mat = model->GetModelMatrix();
        std::copy(glm::value_ptr(model_mat), glm::value_ptr(model_mat) + 16, model_matrix_data.begin());
        shader_program->SetUniformMat4("u_Model", model_matrix_data.data());

        // Pass light uniforms (very basic for now)
        shader_program->SetUniformFloat3("u_LightDirection", light.GetDirection().x, light.GetDirection().y,
                                         light.GetDirection().z);
        shader_program->SetUniformFloat3("u_LightColor", light.GetColor().x, light.GetColor().y, light.GetColor().z);
        shader_program->SetUniformFloat("u_LightIntensity", light.GetIntensity());

        // Bind textures and samplers (Material handles this)
        // ... (iterating through material's textures and samplers and calling SetTexture/SetSampler on context)

        // Bind and draw mesh
        auto mesh = model->GetMesh();
        mesh->GetVertexBuffer()->Bind();
        mesh->GetIndexBuffer()->Bind();
        PIECE_TRACE("Drawing mesh with {0} indices.", mesh->GetIndexBuffer()->GetCount());

        render_context_->DrawIndexed(mesh->GetIndexBuffer()->GetCount(), 0, 0);

        mesh->GetIndexBuffer()->Unbind();
        mesh->GetVertexBuffer()->Unbind();
        shader_program->Unbind();
        PIECE_TRACE("Unbound shader program: {0}", fmt::ptr(shader_program));
    }

    graphics_device_->EndFrame();
    PIECE_TRACE("RenderSystem::RenderFrame finished.");
}

} // namespace Piece::Core
