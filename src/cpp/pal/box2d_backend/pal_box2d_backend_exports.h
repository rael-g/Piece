#pragma once

#ifdef _WIN32
#ifdef PAL_BOX2D_BACKEND_BUILD_DLL
#define PAL_BOX2D_BACKEND_API __declspec(dllexport)
#else
#define PAL_BOX2D_BACKEND_API __declspec(dllimport)
#endif
#else
#define PAL_BOX2D_BACKEND_API
#endif
