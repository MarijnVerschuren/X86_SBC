cmake_minimum_required(VERSION 3.31)
project(north_bridge  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set(NB_BIN_DIR		"${PROJECT_SOURCE_DIR}/bin")
set(NB_SRC_DIR      "${PROJECT_SOURCE_DIR}/src")
set(NB_HDL_DIR      "${PROJECT_SOURCE_DIR}/HDL")
set(NB_INC_DIR  	"${PROJECT_SOURCE_DIR}/inc")
set(NB_LIB_DIR      "${PROJECT_SOURCE_DIR}/lib")

link_directories(
	${NB_LIB_DIR}/CUTIL/cprintf/bin
)

include_directories(
	/usr/local/include
	${NB_INC_DIR}
	${NB_LIB_DIR}/CUTIL/cprintf/inc
)

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
target_link_libraries(north_bridge_sim PRIVATE cprintf_static)

verilate(
	north_bridge_sim
	SOURCES ${VERILOG_SOURCE}
	TRACE_VCD
)
