# Handles runtime dependency propagation for executable targets
function(enable_runtime_dependencies target)
    if(WIN32)
        get_target_property(_target_type ${target} TYPE)
        if(NOT (_target_type STREQUAL "EXECUTABLE" OR _target_type STREQUAL "MODULE_LIBRARY"))
            message(WARNING "enable_runtime_dependencies: Target '${target}' is not an executable or module library, skipping.")
            return()
        endif()

        # Get the destination directory for runtime dependencies
        get_target_property(_destination_dir ${target} RUNTIME_OUTPUT_DIRECTORY)
        if(NOT _destination_dir)
            set(_destination_dir $<TARGET_FILE_DIR:${target}>)
        endif()

        # Get the list of runtime DLLs and process them one by one
        # Split the string with semicolons into a CMake list
        string(REPLACE ";" " " _runtime_dll_list "$<TARGET_RUNTIME_DLLS:${target}>")
        
        foreach(_dll IN LISTS _runtime_dll_list)
            if(EXISTS "${_dll}")
                add_custom_command(TARGET ${target} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        "${_dll}"
                        "${_destination_dir}"
                    COMMENT "Copying runtime DLL: ${CMAKE_CURRENT_SOURCE_DIR}/${_dll} to ${_destination_dir}"
                    VERBATIM # Ensures that arguments are passed as-is without further interpretation by the shell
                )
            else()
                message(WARNING "enable_runtime_dependencies: Runtime DLL '${_dll}' not found for target '${target}', skipping copy.")
            endif()
        endforeach()
    endif()
endfunction()
