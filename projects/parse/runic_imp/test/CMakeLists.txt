create_executable(runic_imp_test)
require(runic_imp googletest)

add_definitions(-DRESOURCE_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/resources/\")