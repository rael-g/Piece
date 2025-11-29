#ifndef PIECE_CORE_EXPORTS_H_
#define PIECE_CORE_EXPORTS_H_

#ifdef _WIN32
#ifdef PIECE_CORE_BUILD_DLL
#define PIECE_CORE_API __declspec(dllexport)
#else
#define PIECE_CORE_API __declspec(dllimport)
#endif
#else // Non-Windows platforms
#ifdef PIECE_CORE_BUILD_DLL
#define PIECE_CORE_API __attribute__((visibility("default")))
#else
#define PIECE_CORE_API
#endif
#endif

#endif // PIECE_CORE_EXPORTS_H_
