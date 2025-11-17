cmake_minimum_required(VERSION 3.31)
project(north_bridge  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)


include_directories(/usr/local/include inc obj)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set(NB_SRC_DIR      "${PROJECT_SOURCE_DIR}/src")
set(NB_HDL_DIR      "${PROJECT_SOURCE_DIR}/HDL")
set(NB_INCLUDE_DIR  "${PROJECT_SOURCE_DIR}/inc")
set(NB_OBJ_DIR      "${PROJECT_SOURCE_DIR}/obj")

set(CPP_FLAGS
)

set(CPP_SOURCES
    ${NB_SRC_DIR}/main.cpp
    ${NB_SRC_DIR}/tests.cpp
)

set(VERILOG_SOURCE
	${NB_HDL_DIR}/north_bridge.v
)

find_program(VERILATOR verilator)

find_package(verilator HINTS $ENV{VERILATOR_ROOT})

add_executable(north_bridge_sim ${CPP_SOURCES})

verilate(
	north_bridge_sim
	SOURCES ${VERILOG_SOURCE}
	TRACE_VCD
)
