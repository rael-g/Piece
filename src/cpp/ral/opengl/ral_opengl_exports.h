#pragma once

#ifdef _WIN32
#ifdef RAL_OPENGL_BUILD_DLL
#define RAL_OPENGL_API __declspec(dllexport)
#else
#define RAL_OPENGL_API __declspec(dllimport)
#endif
#else
#define RAL_OPENGL_API
#endif
