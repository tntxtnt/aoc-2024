add_subdirectory(session)
add_library(AdventOfCode::Session ALIAS aoc_session)
add_subdirectory(utils)
add_library(AdventOfCode::Utils ALIAS aoc_utils)