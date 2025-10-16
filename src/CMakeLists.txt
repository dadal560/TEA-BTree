cmake_minimum_required(VERSION 3.0)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

project(List C)

# -----------------------------
# Librairie btree
# -----------------------------
add_library(btree SHARED btree/btree.c btree/btree.h)
install(
    TARGETS btree
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)
install(
    FILES btree/btree.h
    DESTINATION include
)

# -----------------------------
# pkg-config
# -----------------------------
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/btree/btree.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/btree.pc
    @ONLY
)
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/btree.pc
    DESTINATION share/pkgconfig
    COMPONENT "PkgConfig"
)

# -----------------------------
# Fichier CMake config
# -----------------------------
include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/btree/BTreeConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/BTreeConfig.cmake
    INSTALL_DESTINATION cmake
)
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/BTreeConfig.cmake
    DESTINATION cmake
)

# -----------------------------
# Programme de test
# -----------------------------
add_executable(test-btree btree/test-btree.c btree/btree.h)
add_dependencies(test-btree btree)
target_link_libraries(test-btree btree)

enable_testing()
add_test(NAME test-btree COMMAND test-btree)

# -----------------------------
# Benchmark
# -----------------------------
add_executable(benchmark benchmark/benchmark.c tree-avl/tree-avl.c)
add_dependencies(benchmark btree)
target_link_libraries(benchmark btree)
