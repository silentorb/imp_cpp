create_library(solving)
require(imp_overworld)

if (DEBUG_SOLVER)
  add_definitions(-DDEBUG_SOLVER=${DEBUG_SOLVER})
endif ()