# Pure C++ version - All data structures and algorithms implemented in C++

add_executable(dsav-pure
    src/main.cpp
    src/visualizers/stack_visualizer.cpp
    src/visualizers/queue_visualizer.cpp
    src/visualizers/array_visualizer.cpp
    src/visualizers/linked_list_visualizer.cpp
    src/visualizers/bst_visualizer.cpp
    src/visualizers/rbtree_visualizer.cpp
    src/visualizers/sorting_visualizer.cpp
    src/visualizers/searching_visualizer.cpp
    src/algorithms/sorting.cpp
    src/algorithms/searching.cpp
)

target_include_directories(dsav-pure PRIVATE
    include
)

target_link_libraries(dsav-pure PRIVATE
    dsav-common
)

# Copy assets to build directory for easier access
add_custom_command(TARGET dsav-pure POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:dsav-pure>/assets
    COMMENT "Copying assets to build directory"
)
