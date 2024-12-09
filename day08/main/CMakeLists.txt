add_executable(${MAIN} src/${DAY}_main.cpp)

find_package(doctest CONFIG REQUIRED)

target_link_libraries(${MAIN}
    PRIVATE
        ${LIB}
        doctest::doctest
        AdventOfCode::Session
        AdventOfCode::Utils
)

# Enable sanitizers
# If you're getting error LNK2038: mismatch detected for 'annotate_vector': value '0' doesn't match value '1'
# either disable sanitizer or rebuild other libraries with sanitizer on. For more information see
# https://learn.microsoft.com/en-us/cpp/sanitizers/error-container-overflow?view=msvc-170
target_sanitize_options(${MAIN} OPTIONS address undefined leak CONFIGS RelWithDebInfo)

target_fixit(${MAIN})

# Use ${CMAKE_CURRENT_BINARY_DIR} for output dir
# configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../${DAY}.txt ${CMAKE_CURRENT_BINARY_DIR}/${DAY}.txt COPYONLY)           # re-copy if source changes
# file(COPY ${resource_file_or_dir} DESTINATION ${destination_directory}) # copy only once
