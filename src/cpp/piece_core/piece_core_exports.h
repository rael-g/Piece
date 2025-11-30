/**
 * @file piece_core_exports.h
 * @brief Defines macros for exporting and importing symbols from the Piece Core library (DLL).
 *        This is essential for creating shared libraries that can be used by other applications.
 */
#ifndef PIECE_CORE_EXPORTS_H_
#define PIECE_CORE_EXPORTS_H_

#ifdef _WIN32
#ifdef PIECE_CORE_BUILD_DLL
/**
 * @def PIECE_CORE_API
 * @brief Exports symbols from the DLL on Windows when building the DLL.
 */
#define PIECE_CORE_API __declspec(dllexport)
#else
/**
 * @def PIECE_CORE_API
 * @brief Imports symbols from the DLL on Windows when using the DLL.
 */
#define PIECE_CORE_API __declspec(dllimport)
#endif
#else // Non-Windows platforms
#ifdef PIECE_CORE_BUILD_DLL
/**
 * @def PIECE_CORE_API
 * @brief Sets default visibility for symbols on non-Windows platforms when building the shared library.
 */
#define PIECE_CORE_API __attribute__((visibility("default")))
#else
/**
 * @def PIECE_CORE_API
 * @brief Defined as empty on non-Windows platforms when using the shared library.
 */
#define PIECE_CORE_API
#endif
#endif

#endif // PIECE_CORE_EXPORTS_H_
