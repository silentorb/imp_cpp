cmake_minimum_required(VERSION 3.6)
enable_language(CXX) # SINCE Compiler/Platform variables do not normally exist before a project command in CMake.

get_filename_component(PARENT_DIR ${CMAKE_SOURCE_DIR} DIRECTORY)
get_filename_component(CPP_ROOT "../.." ABSOLUTE)

include(config.cmake)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
include("${CMAKE_TOOLS_PATH}/scripts/generator-key.cmake")

set(MYTHIC_DEPENDENCIES "${CPP_ROOT}/mythic/dependencies/${GENERATOR_KEY}")

set(LOCAL_HEADER 1)
include(local-config.cmake OPTIONAL)

if (NOT MINGW)
  project(imp)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
else ()
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif ()

include("${CMAKE_TOOLS_PATH}/cmake-tools.cmake")

add_subdirectory(${CMAKE_BOOST_DIR}/projects/system projects/system)
add_subdirectory(${CMAKE_BOOST_DIR}/projects/filesystem projects/filesystem)

add_subdirectory(projects/parse/runic)
add_subdirectory(projects/parse/runic_cpp)
add_subdirectory(projects/parse/runic_imp)
add_subdirectory(projects/model/graphing)
add_subdirectory(projects/model/solving)
#add_subdirectory(projects/model/imp_graph)
add_subdirectory(projects/model/imp_underworld)
add_subdirectory(projects/model/imp_overworld)
add_subdirectory(projects/parse/imp_summoning)
add_subdirectory(projects/model/imp_mirror)
add_subdirectory(projects/render/imp_artisan)
add_subdirectory(projects/render/imp_rendering)
add_subdirectory(projects/front/imp_taskmaster)
add_subdirectory(projects/front/imp_wrapper)

add_subdirectory(projects/parse/runic_imp/test)
add_subdirectory(projects/parse/imp_summoning/test)
add_subdirectory(projects/model/imp_mirror/test)
add_subdirectory(projects/front/imp_wrapper/test)

finish_cmake()
