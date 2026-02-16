#ifndef PIECE_CORE_RESOURCE_MANAGER_H_
#define PIECE_CORE_RESOURCE_MANAGER_H_

#include <memory>
#include <string>
#include <unordered_map>

#include <ral/igraphics_device.h>
#include <ral/interfaces/iindex_buffer.h>
#include <ral/interfaces/ishader_program.h>
#include <ral/interfaces/ivertex_buffer.h>
#include <ral/itexture.h>
#include <ral/ral_types.h>

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
class ResourceManager
{
  public:
    explicit ResourceManager(Piece::RAL::IGraphicsDevice *graphics_device);
    ~ResourceManager();

    std::shared_ptr<Material> LoadMaterial(const std::string &path);
    std::shared_ptr<Mesh> LoadMesh(const std::string &path);
    std::shared_ptr<Piece::RAL::ITexture> LoadTexture(const std::string &path);
    std::shared_ptr<Piece::RAL::IShaderProgram> LoadShaderProgram(const std::string &vertex_shader_path,
                                                           const std::string &fragment_shader_path);

  private:
    Piece::RAL::IGraphicsDevice *graphics_device_;
    std::unordered_map<std::string, std::shared_ptr<Material>> materials_;
    std::unordered_map<std::string, std::shared_ptr<Mesh>> meshes_;
    std::unordered_map<std::string, std::shared_ptr<Piece::RAL::ITexture>> textures_;
    std::unordered_map<std::string, std::shared_ptr<Piece::RAL::IShaderProgram>> shader_programs_;
};

} // namespace Piece::Core

#endif // PIECE_CORE_RESOURCE_MANAGER_H_
