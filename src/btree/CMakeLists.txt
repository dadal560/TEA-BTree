cmake_minimum_required(VERSION 3.0)

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH true)

project(List C)
# add_executable(btree btree.c btree.h)
add_library(btree SHARED btree.c btree.h)

install(
	TARGETS btree
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
)

install(
	FILES btree.h
	DESTINATION include
)

# Ajout d'un fichier de configuration de type pkgconfig. Copie le 1er argument vers le 2ème. @ONLY = restreint le remplacement de variable dans btree.pc.in
# à celles qui ont le format @<var>@ pour éviter les conflits avec la syntaxe CMake ${<var>}.
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/btree.pc.in
	${CMAKE_CURRENT_BINARY_DIR}/btree.pc
	@ONLY
)
install(
	FILES ${CMAKE_CURRENT_BINARY_DIR}/btree.pc
	DESTINATION share/pkgconfig
	COMPONENT "PkgConfig"
)

#  Ajout d'un fichier de configuration de type cmake
include(CMakePackageConfigHelpers)
configure_package_config_file(
	${CMAKE_CURRENT_SOURCE_DIR}/BTreeConfig.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/BTreeConfig.cmake
	INSTALL_DESTINATION cmake
)
install(
	FILES ${CMAKE_CURRENT_BINARY_DIR}/BTreeConfig.cmake
	DESTINATION cmake
)

# Ajout d'un exécutable dépendant également de btree.h
add_executable(test-btree test-btree.c btree.h)
# Précision de l'ordre de construction: le programme de test doit se faire
# après la librairie
add_dependencies(test-btree btree)
# Aggrégation du programme de test avec la librairie btree
target_link_libraries(test-btree btree)

# Activation des tests
enable_testing()
# Ajout d'un test
add_test(test-btree ./test-btree)
