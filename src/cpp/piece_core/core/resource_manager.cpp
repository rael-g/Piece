#include "resource_manager.h"

#include <piece_core/core/material.h> // Assuming these classes exist
#include <piece_core/core/mesh.h>     // Assuming these classes exist
#include <piece_core/logging_api.h>

namespace Piece::Core
{
using namespace Piece::RAL;

ResourceManager::ResourceManager(IGraphicsDevice *graphics_device) : graphics_device_(graphics_device)
{
    PIECE_INFO("ResourceManager created with GraphicsDevice: {0}.", fmt::ptr(graphics_device));
}

ResourceManager::~ResourceManager()
{
    PIECE_INFO("ResourceManager destroyed.");
}

std::shared_ptr<Material> ResourceManager::LoadMaterial(const std::string &path)
{
    PIECE_TRACE("ResourceManager::LoadMaterial(path: '{0}')", path);
    PIECE_INFO("Loading material: {0}", path);
    // Stub: In a real scenario, load material data from file and create RAL resources
    // For now, return a placeholder
    return std::make_shared<Material>();
}

std::shared_ptr<Mesh> ResourceManager::LoadMesh(const std::string &path)
{
    PIECE_TRACE("ResourceManager::LoadMesh(path: '{0}')", path);
    PIECE_INFO("Loading mesh: {0}", path);
    // Stub: In a real scenario, load mesh data from file and create RAL vertex/index buffers
    // For now, return a placeholder
    return std::make_shared<Mesh>(graphics_device_);
}

std::shared_ptr<RAL::ITexture> ResourceManager::LoadTexture(const std::string &path)
{
    PIECE_TRACE("ResourceManager::LoadTexture(path: '{0}')", path);
    PIECE_INFO("Loading texture: {0}", path);
    // Stub: In a real scenario, load image data and create RAL ITexture
    // For now, return a placeholder (dummy 1x1 texture)
    return graphics_device_->CreateTexture(RAL::TextureType::kTexture2D, 1, 1, 1, RAL::TextureFormat::kR8G8B8A8Unorm);
}

std::shared_ptr<RAL::IShaderProgram> ResourceManager::LoadShaderProgram(const std::string &path)
{
    PIECE_TRACE("ResourceManager::LoadShaderProgram(path: '{0}')", path);
    PIECE_INFO("Loading shader program from path: {0}", path);
    // Stub: In a real scenario, parse path to determine shader type, load source, create IShader, then IShaderProgram
    // For now, return nullptr
    return nullptr;
}

} // namespace Piece::Core
