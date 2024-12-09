add_executable(${VISUALIZATION} WIN32
    src/${VISUALIZATION}.h
    src/${VISUALIZATION}.cpp
    src/constants.h
    src/imgui_glfw3_opengl3.cpp
    src/custom_imconfig.h
    ${VISUALIZATION}.rc
)

if (WIN32)
    # /ENTRY:mainCRTStartup keeps the same "main" function instead of requiring "WinMain"
    set(SUBSYSTEM_LINKER_OPTIONS /ENTRY:mainCRTStartup)
else()
    set(SUBSYSTEM_LINKER_OPTIONS "-mwindows")
endif()
target_link_options(${VISUALIZATION} PRIVATE ${SUBSYSTEM_LINKER_OPTIONS})

find_package(fmt CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(implot CONFIG REQUIRED)
find_package(lodepng CONFIG REQUIRED)

target_link_libraries(${VISUALIZATION}
    PRIVATE
        AdventOfCode::Session
        AdventOfCode::Utils
        glfw
        OpenGL::GL
        imgui::imgui
        implot::implot
        lodepng
)
target_compile_definitions(${VISUALIZATION}
    PRIVATE
        IMGUI_USER_CONFIG=\"custom_imconfig.h\"
)

# Enable sanitizers
# If you're getting error LNK2038: mismatch detected for 'annotate_vector': value '0' doesn't match value '1'
# either disable sanitizer or rebuild other libraries with sanitizer on. For more information see
# https://learn.microsoft.com/en-us/cpp/sanitizers/error-container-overflow?view=msvc-170
target_sanitize_options(${VISUALIZATION} OPTIONS address undefined leak CONFIGS RelWithDebInfo)

target_fixit(${VISUALIZATION})

# Use ${CMAKE_CURRENT_BINARY_DIR} for output dir
# configure_file(${resource_file} ${destination_file} COPYONLY)           # re-copy if source changes
# file(COPY ${resource_file_or_dir} DESTINATION ${destination_directory}) # copy only once
file(COPY ${CMAKE_SOURCE_DIR}/static DESTINATION ${CMAKE_CURRENT_BINARY_DIR}) 
