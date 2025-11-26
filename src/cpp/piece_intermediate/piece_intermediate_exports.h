#ifndef PIECE_INTERMEDIATE_EXPORTS_H_
#define PIECE_INTERMEDIATE_EXPORTS_H_

#ifdef _WIN32
    #ifdef PIECE_INTERMEDIATE_BUILD_DLL
        #define PIECE_INTERMEDIATE_API __declspec(dllexport)
    #else
        #define PIECE_INTERMEDIATE_API __declspec(dllimport)
    #endif
#else
    #define PIECE_INTERMEDIATE_API
#endif

#endif // PIECE_INTERMEDIATE_EXPORTS_H_
