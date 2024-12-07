set(DAY day01)
set(MAIN ${DAY}_main)
set(LIB ${DAY}_lib)
set(VISUALIZATION ${DAY}_visualization)

add_subdirectory(solution)
add_subdirectory(main)
add_subdirectory(visualization)

# find_package(fmt CONFIG REQUIRED)
# target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)

# Enable sanitizers
# If you're getting error LNK2038: mismatch detected for 'annotate_vector': value '0' doesn't match value '1'
# either disable sanitizer or rebuild other libraries with sanitizer on. For more information see
# https://learn.microsoft.com/en-us/cpp/sanitizers/error-container-overflow?view=msvc-170
# target_sanitize_options(${PROJECT_NAME} OPTIONS address undefined leak CONFIGS RelWithDebInfo)

# target_fixit(${PROJECT_NAME})

# Use ${CMAKE_CURRENT_BINARY_DIR} for output dir
# configure_file(${resource_file} ${destination_file} COPYONLY)           # re-copy if source changes
# file(COPY ${resource_file_or_dir} DESTINATION ${destination_directory}) # copy only once
