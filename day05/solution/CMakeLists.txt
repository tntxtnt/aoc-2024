add_library(${LIB}
    STATIC
        include/${DAY}_solution.h
        src/${DAY}_solution.cpp
)
find_package(fmt CONFIG REQUIRED)
target_link_libraries(${LIB}
    PRIVATE
        fmt::fmt
)
target_include_directories(${LIB} PUBLIC include)

# Enable sanitizers
# If you're getting error LNK2038: mismatch detected for 'annotate_vector': value '0' doesn't match value '1'
# either disable sanitizer or rebuild other libraries with sanitizer on. For more information see
# https://learn.microsoft.com/en-us/cpp/sanitizers/error-container-overflow?view=msvc-170
target_sanitize_options(${LIB} OPTIONS address undefined leak CONFIGS RelWithDebInfo)

target_fixit(${LIB})

# Use ${CMAKE_CURRENT_BINARY_DIR} for output dir
# configure_file(${resource_file} ${destination_file} COPYONLY)           # re-copy if source changes
# file(COPY ${resource_file_or_dir} DESTINATION ${destination_directory}) # copy only once
