create_executable(imp_mirror_test)
require(imp_summoning imp_mirror googletest)

add_definitions(-DRESOURCE_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/resources/\")