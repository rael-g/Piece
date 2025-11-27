#ifndef WAL_GLFW_BACKEND_EXPORTS_H_
#define WAL_GLFW_BACKEND_EXPORTS_H_

#ifdef _WIN32
#ifdef WAL_GLFW_BACKEND_BUILD_DLL
#define WAL_GLFW_BACKEND_API __declspec(dllexport)
#else
#define WAL_GLFW_BACKEND_API __declspec(dllimport)
#endif
#else // Non-Windows platforms
#ifdef WAL_GLFW_BACKEND_BUILD_DLL
#define WAL_GLFW_BACKEND_API __attribute__((visibility("default")))
#else
#define WAL_GLFW_BACKEND_API
#endif
#endif

#endif // WAL_GLFW_BACKEND_EXPORTS_H_
