#pragma once

#include <string>
#include <memory>
#include <vector>

// Forward declarations
namespace Piece
{
    namespace RAL
    {
        class IGraphicsDevice;
        class ITexture;
        class IShaderProgram;
    } // namespace RAL
    namespace Core
    {
        class Mesh;
        class Material;
    } // namespace Core
} // namespace Piece

namespace Piece
{
    namespace Core
    {
        /// @brief Interface for managing resources such as meshes, materials, and textures.
        ///
        /// This interface provides a contract for resource loading and management, allowing
        /// different implementations to be swapped out (e.g., for testing or different
        /// resource formats).
        class IResourceManager
        {
          public:
            virtual ~IResourceManager() = default;

            /// @brief Loads a mesh from the specified file path.
            /// @param path The path to the mesh file.
            /// @return A shared pointer to the loaded Mesh object, or nullptr if loading fails.
            virtual std::shared_ptr<Mesh> LoadMesh(const std::string &path) = 0;

            /// @brief Loads a material from the specified file path.
            /// @param path The path to the material file.
            /// @return A shared pointer to the loaded Material object, or nullptr if loading fails.
            virtual std::shared_ptr<Material> LoadMaterial(const std::string &path) = 0;

            /// @brief Loads a texture from the specified file path.
            /// @param path The path to the texture file.
            /// @return A shared pointer to the loaded ITexture object, or nullptr if loading fails.
            virtual std::shared_ptr<RAL::ITexture> LoadTexture(const std::string &path) = 0;

            /// @brief Loads a shader program from the specified file path.
            /// @param path The path to the shader program file.
            /// @return A shared pointer to the loaded IShaderProgram object, or nullptr if loading fails.
            virtual std::shared_ptr<RAL::IShaderProgram> LoadShaderProgram(const std::string &path) = 0;
        };
    } // namespace Core
} // namespace Piece