# options for primary and secondary solver backends
if(NOT PRIMARY_SOLVER_BACKEND)
	set(PRIMARY_SOLVER_BACKEND "glpk" CACHE STRING "Primary backend-solver. Options are: glpk, clp")
endif()
set_property(CACHE PRIMARY_SOLVER_BACKEND PROPERTY STRINGS "clp" "glpk")

if(NOT SECONDARY_SOLVER_BACKEND)
	set(SECONDARY_SOLVER_BACKEND "none" CACHE STRING "Secondary backend-solver used after invoking glpk. Options are: none, smtrat, z3, soplex")
endif()
set_property(CACHE SECONDARY_SOLVER_BACKEND PROPERTY STRINGS "none" "smtrat" "z3" "soplex")

# set according variables
if(${PRIMARY_SOLVER_BACKEND} STREQUAL "clp")
    set(HYPRO_USE_CLP "ON")
    set(HYPRO_PRIMARY_SOLVER "SOLVER_CLP")
elseif(${PRIMARY_SOLVER_BACKEND} STREQUAL "glpk")
    set(HYPRO_USE_GLPK "ON")
    set(HYPRO_PRIMARY_SOLVER "SOLVER_GLPK")
endif()

if(${SECONDARY_SOLVER_BACKEND} STREQUAL "smtrat")
    set(HYPRO_USE_SMTRAT "ON")
    set(HYPRO_SECONDARY_SOLVER SOLVER_SMTRAT)
elseif(${SECONDARY_SOLVER_BACKEND} STREQUAL "z3")
    set(HYPRO_USE_Z3 "ON")
    set(HYPRO_SECONDARY_SOLVER SOLVER_ZTHREE)
elseif(${SECONDARY_SOLVER_BACKEND} STREQUAL "soplex")
    set(HYPRO_USE_SOPLEX "ON")
    set(HYPRO_SECONDARY_SOLVER SOLVER_SOPLEX)
endif()

message(STATUS "Primary optimization backend: ${HYPRO_PRIMARY_SOLVER}")
message(STATUS "Secondary optimization backend: ${HYPRO_SECONDARY_SOLVER}")
