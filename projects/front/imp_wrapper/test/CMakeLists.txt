create_executable(imp_wrapper_test)
require(imp_wrapper boost_filesystem googletest)

add_definitions(-DRESOURCE_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/resources/\")
add_definitions(-DOUTPUT_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/output/\")
add_definitions(-DDIFF_VIEWER_PATH=\"${DIFF_VIEWER_PATH}\")

if (DEFINED DISABLE_DIFF)
  add_definitions(-DDISABLE_DIFF)
endif ()