#pragma once

#ifdef _WIN32
#ifdef PAL_BOX2D_BUILD_DLL
#define PAL_BOX2D_API __declspec(dllexport)
#else
#define PAL_BOX2D_API __declspec(dllimport)
#endif
#else
#define PAL_BOX2D_API
#endif
