create_test(imp_wrapper_test)
require(imp_wrapper boost_filesystem)

add_definitions(-DRESOURCE_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/resources/\")
add_definitions(-DOUTPUT_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/output/\")