cmake_minimum_required(VERSION 3.5)

project(plotcpp)

set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O3 -Werror -Wall -Wextra -Wpedantic -Wconversion")

set(INCLUDE include)
set(SRC src)
set(CLI cli)
set(TEST test)

find_package(PkgConfig REQUIRED)
find_package(LibXml2 REQUIRED)
pkg_check_modules(RSVG2 REQUIRED librsvg-2.0)
find_package(fmt REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)
find_package(Doxygen REQUIRED)
find_package(GTest REQUIRED)

include_directories(${INCLUDE})
include_directories(${LIBXML2_INCLUDE_DIR})
include_directories(${RSVG2_INCLUDE_DIRS})
include_directories(${GTEST_INCLUDE_DIR})

# Shared library
set(
    LIB_SOURCES
    ${SRC}/BarPlot.cpp
    ${SRC}/BarPlotBase.cpp
    ${SRC}/DisplayService.cpp
    ${SRC}/Figure.cpp
    ${SRC}/fonts.cpp
    ${SRC}/HistogramPlot.cpp
    ${SRC}/Plot2D.cpp
    ${SRC}/svg.cpp
    ${SRC}/version.cpp
    ${SRC}/components/Frame.cpp
    ${SRC}/components/Legend.cpp
)

set(
    LIB_LINKED_LIBRARIES
    ${CMAKE_THREAD_LIBS_INIT}
    ${LIBXML2_LIBRARIES}
    ${RSVG2_LIBRARIES}
    ${OPENGL_LIBRARIES}
    glfw
    fmt::fmt
)

add_library(plotcpp SHARED ${LIB_SOURCES})
target_link_libraries(plotcpp ${LIB_LINKED_LIBRARIES})

# CLI
set(CLI_MAIN ${CLI}/plotcpp.cpp)
set(
    CLI_SOURCES
    ${CLI}/SplitStringView.cpp
	${CLI}/csv.cpp
	${CLI}/Plot2DHandler.cpp
	${CLI}/HistogramHandler.cpp
	${CLI}/BarPlotHandler.cpp
)

add_executable(cli ${CLI_MAIN} ${CLI_SOURCES})
target_link_libraries(cli -L./${CMAKE_CURRENT_BINARY_DIR} plotcpp)
set_target_properties(cli PROPERTIES OUTPUT_NAME plotcpp)

add_executable(cli_static ${CLI_MAIN} ${CLI_SOURCES} ${LIB_SOURCES})
target_link_libraries(cli_static ${LIB_LINKED_LIBRARIES})
set_target_properties(cli_static PROPERTIES OUTPUT_NAME plotcpp_static)

# Figure tester
add_executable(figure_tester ${LIB_SOURCES} examples/figure_tester.cpp)
set_target_properties(figure_tester PROPERTIES COMPILE_FLAGS "-g3")
target_link_libraries(figure_tester ${LIB_LINKED_LIBRARIES})

# Tests
set(
    TEST_SOURCES
    ${LIB_SOURCES}
    ${CLI_SOURCES}
    ${TEST}/AxisPartitionTest.cpp
    ${TEST}/UtilityTest.cpp
)

add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests ${LIB_LINKED_LIBRARIES} gtest gtest_main)

# Doxygen
add_custom_target(
    doc ALL
    COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Generating documentation with Doxygen"
    VERBATIM)
