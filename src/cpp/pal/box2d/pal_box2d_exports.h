#pragma once

#ifdef _WIN32
#ifdef PAL_BOX2D_BUILD_DLL
#define PAL_BOX2D_API __declspec(dllexport)
#else
#define PAL_BOX2D_API __declspec(dllimport)
#endif
#else // Non-Windows platforms
#ifdef PAL_BOX2D_BUILD_DLL
#define PAL_BOX2D_API __attribute__((visibility("default")))
#else
#define PAL_BOX2D_API
#endif
#endif
