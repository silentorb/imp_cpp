create_executable(cpp_scanning_test)
require(cpp_scanning boost_filesystem googletest cpp_stl)

add_definitions(-DIMP_ROOT=\"${imp_root}\")
