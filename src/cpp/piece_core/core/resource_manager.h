#ifndef PIECE_CORE_RESOURCE_MANAGER_H_
#define PIECE_CORE_RESOURCE_MANAGER_H_

#include <memory>
#include <string>
#include <unordered_map>

#include <piece_core/piece_core_exports.h> // Explicitly include PIECE_CORE_API

#include <ral/igraphics_device.h>
#include <ral/interfaces/iindex_buffer.h>
#include <ral/interfaces/ishader_program.h>
#include <ral/interfaces/ivertex_buffer.h>
#include <ral/itexture.h>
#include <piece_core/core/iresource_manager.h> // Include the new interface

namespace Piece::Core
{

// Forward declarations of core classes that will use RAL resources
class Material;
class Mesh;

/**
 * @brief Manages the lifecycle and caching of graphics resources.
 * @details This class is responsible for loading assets from files and converting them into
 *          RAL resources, and maintaining a cache of these resources.
 */
class PIECE_CORE_API ResourceManager : public IResourceManager
{
  public:
    explicit ResourceManager(Piece::RAL::IGraphicsDevice *graphics_device);
    ~ResourceManager() override; // Mark destructor as override

    std::shared_ptr<Material> LoadMaterial(const std::string &path) override;
    std::shared_ptr<Mesh> LoadMesh(const std::string &path) override;
    std::shared_ptr<Piece::RAL::ITexture> LoadTexture(const std::string &path) override;
    std::shared_ptr<Piece::RAL::IShaderProgram> LoadShaderProgram(const std::string &path) override;

  private:
    Piece::RAL::IGraphicsDevice *graphics_device_;
    std::unordered_map<std::string, std::shared_ptr<Material>> materials_;
    std::unordered_map<std::string, std::shared_ptr<Mesh>> meshes_;
    std::unordered_map<std::string, std::shared_ptr<Piece::RAL::ITexture>> textures_;
    std::unordered_map<std::string, std::shared_ptr<Piece::RAL::IShaderProgram>> shader_programs_;
};

} // namespace Piece::Core

#endif // PIECE_CORE_RESOURCE_MANAGER_H_
