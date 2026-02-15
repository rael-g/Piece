#ifndef RAL_OPENGL_RAL_OPENGL_EXPORTS_H_
#define RAL_OPENGL_RAL_OPENGL_EXPORTS_H_

#ifdef _WIN32
#ifdef RAL_OPENGL_BUILD_DLL
#define RAL_OPENGL_API __declspec(dllexport)
#else
#define RAL_OPENGL_API __declspec(dllimport)
#endif
#else // Non-Windows platforms
#ifdef RAL_OPENGL_BUILD_DLL
#define RAL_OPENGL_API __attribute__((visibility("default")))
#else
#define RAL_OPENGL_API
#endif
#endif

#endif // RAL_OPENGL_RAL_OPENGL_EXPORTS_H_