function(_set_san_compile_opts prefix valid_opts)
  set(san_opts "")
  foreach(opt IN LISTS valid_opts)
    if(${opt} IN_LIST SANITIZE_OPTIONS)
      list(APPEND san_opts ${opt})
    endif()
  endforeach()
  if("address" IN_LIST SANITIZE_OPTIONS)
    set(LINK_OPTIONS "clang_rt.asan_dynamic-x86_64.lib" "clang_rt.asan_dynamic_runtime_thunk-x86_64.lib")
  endif()
  string(JOIN "," san_opts_str ${san_opts})
  set(COMPILE_OPTIONS "${prefix}fsanitize=${san_opts_str}" PARENT_SCOPE)
endfunction()

function(target_sanitize_options target_name)
  set(multiValueArgs CONFIGS OPTIONS)
  cmake_parse_arguments(SANITIZE "" "" "${multiValueArgs}" ${ARGN})

  set(COMPILE_OPTIONS "")
  set(LINK_OPTIONS "")
  if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC") # cl
    _set_san_compile_opts("/" "address")
    message(STATUS "cl compiler detected, enable '${COMPILE_OPTIONS}'")
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_CXX_SIMULATE_ID MATCHES "MSVC") # clang-cl
    _set_san_compile_opts("-" "address;undefined")
    if("address" IN_LIST SANITIZE_OPTIONS)
      set(LINK_OPTIONS "clang_rt.asan_dynamic-x86_64.lib" "clang_rt.asan_dynamic_runtime_thunk-x86_64.lib")
    endif()
    message(STATUS "clang-cl compiler detected, enable '${COMPILE_OPTIONS}'")
  elseif(WIN32)
    message(AUTHOR_WARNING "Other compiler on Windows detected, sanitizer not supported")
  else()
    _set_san_compile_opts("-" "address;undefined;leak")
    set(LINK_OPTIONS "${COMPILE_OPTIONS}")
    message(STATUS "Other compiler detected, not on Windows, enable '${COMPILE_OPTIONS}'")
  endif()

  foreach(conf IN LISTS SANITIZE_CONFIGS)
    if(NOT COMPILE_OPTIONS STREQUAL "")
      target_compile_options(${target_name} PRIVATE $<$<CONFIG:${conf}>:${COMPILE_OPTIONS}>)
    endif()
    if(NOT LINK_OPTIONS STREQUAL "")
      target_link_options(${target_name} PRIVATE $<$<CONFIG:${conf}>:${LINK_OPTIONS}>)
    endif()
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_CXX_SIMULATE_ID MATCHES "MSVC") # clang-cl
      set_property(TARGET ${target_name} PROPERTY MSVC_RUNTIME_LIBRARY "$<$<CONFIG:${conf}>:>") # Remove -MDd from this config
    endif()
  endforeach()
endfunction()