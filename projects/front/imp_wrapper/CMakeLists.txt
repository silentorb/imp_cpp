create_library(imp_wrapper)
require(
  imp_summoning
  imp_mirror
  imp_taskmaster
  imp_overworld
  solving
  cpp_stl
)

if (DEBUG_SOLVER)
  add_definitions(-DDEBUG_SOLVER=${DEBUG_SOLVER})
endif ()