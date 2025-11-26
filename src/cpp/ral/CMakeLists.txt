add_library(ral INTERFACE)

target_include_directories(ral INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/interfaces"
)
