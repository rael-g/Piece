/**
 * @file wal_glfw_exports.h
 * @brief Defines macros for exporting and importing symbols from the WAL GLFW implementation library.
 */
#ifndef WAL_GLFW_EXPORTS_H_
#define WAL_GLFW_EXPORTS_H_

#ifdef _WIN32
#ifdef WAL_GLFW_BUILD_DLL
/**
 * @def WAL_GLFW_API
 * @brief Exports symbols from the DLL on Windows when building the DLL.
 */
#define WAL_GLFW_API __declspec(dllexport)
#else
/**
 * @def WAL_GLFW_API
 * @brief Imports symbols from the DLL on Windows when using the DLL.
 */
#define WAL_GLFW_API __declspec(dllimport)
#endif
#else // Non-Windows platforms
#ifdef WAL_GLFW_BUILD_DLL
/**
 * @def WAL_GLFW_API
 * @brief Sets default visibility for symbols on non-Windows platforms when building the shared library.
 */
#define WAL_GLFW_API __attribute__((visibility("default")))
#else
/**
 * @def WAL_GLFW_API
 * @brief Defined as empty on non-Windows platforms when using the shared library.
 */
#define WAL_GLFW_API
#endif
#endif

#endif // WAL_GLFW_EXPORTS_H_