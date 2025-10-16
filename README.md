# TEA-BTree
Implémentation et comparaison de performances entre arbres bicolores (Red-Black) et AVL en C - Projet L3 Informatique 2025


## Compilation

### Avec CMake

```bash
mkdir debug
cd debug
cmake ../../src/tree-avl/ -DCMAKE_INSTALL_PREFIX=../tmp -DCMAKE_BUILD_TYPE=Debug
make
```

### Installation de la bibliothèque

```bash
make install
```