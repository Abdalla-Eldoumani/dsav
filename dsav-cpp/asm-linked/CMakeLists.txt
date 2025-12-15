# Assembly-Linked Version - C++ Visualization with ARMv8 Assembly Backend
# Requires ARM64 architecture or ARM cross-compiler

cmake_minimum_required(VERSION 3.16)

message(STATUS "Building asm-linked version (ARM backend)")

# ===== Assembly Object Files =====

# Path to assembly directory (one level up from dsav-cpp)
set(ASM_DIR "${CMAKE_SOURCE_DIR}/../dsav-assembly")

# Check if assembly directory exists
if(NOT EXISTS "${ASM_DIR}")
    message(FATAL_ERROR "Assembly directory not found at ${ASM_DIR}")
endif()

# List of assembly object files we need
set(ASM_OBJECTS
    ${ASM_DIR}/ansi.o
    ${ASM_DIR}/display.o
    ${ASM_DIR}/stack_viz.o
    ${ASM_DIR}/queue_viz.o
    ${ASM_DIR}/linkedlist_viz.o
    ${ASM_DIR}/bst_viz.o
    ${ASM_DIR}/sort_viz.o
    ${ASM_DIR}/search_viz.o
    ${ASM_DIR}/utils.o
)

# Custom target to build assembly objects
add_custom_target(asm_objects
    COMMAND $(MAKE) -C ${ASM_DIR} clean
    COMMAND $(MAKE) -C ${ASM_DIR}
    BYPRODUCTS ${ASM_OBJECTS}
    COMMENT "Building ARMv8 assembly object files"
    VERBATIM
)

# ===== Executable =====

add_executable(dsav-asm-linked
    src/main.cpp
    src/visualizers/asm_stack_visualizer.cpp
)

# Depend on assembly objects being built first
add_dependencies(dsav-asm-linked asm_objects)

# ===== Include Directories =====

target_include_directories(dsav-asm-linked PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/common/include
)

# ===== Link Libraries =====

# Link common library
target_link_libraries(dsav-asm-linked PRIVATE dsav-common)

# Link assembly object files
target_link_libraries(dsav-asm-linked PRIVATE ${ASM_OBJECTS})

# Link system libraries (C library functions used by assembly)
target_link_libraries(dsav-asm-linked PRIVATE
    m        # Math library
    c        # C standard library
)

# ===== Compiler Options =====

target_compile_features(dsav-asm-linked PRIVATE cxx_std_17)

target_compile_options(dsav-asm-linked PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    $<$<CONFIG:Debug>:-g -O0>
    $<$<CONFIG:Release>:-O3 -DNDEBUG>
)

# ===== Post-Build Message =====

add_custom_command(TARGET dsav-asm-linked POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "======================================"
    COMMAND ${CMAKE_COMMAND} -E echo "Built: dsav-asm-linked"
    COMMAND ${CMAKE_COMMAND} -E echo "Run with: ./dsav-asm-linked"
    COMMAND ${CMAKE_COMMAND} -E echo "Uses ARMv8 assembly for data structures"
    COMMAND ${CMAKE_COMMAND} -E echo "======================================"
    VERBATIM
)
