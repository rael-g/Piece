#pragma once

#ifdef _WIN32
#ifdef RAL_OPENGL_BACKEND_BUILD_DLL
#define RAL_OPENGL_BACKEND_API __declspec(dllexport)
#else
#define RAL_OPENGL_BACKEND_API __declspec(dllimport)
#endif
#else
#define RAL_OPENGL_BACKEND_API
#endif
