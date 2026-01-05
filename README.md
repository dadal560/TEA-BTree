# Étude Comparative : Arbres Rouge-Noir vs Arbres AVL
## Implémentation en C Générique et Analyse de Performance

### Présentation du Projet
Ce projet implémente et compare deux structures d'arbres binaires de recherche auto-équilibrés : les Arbres Rouge-Noir (Red-Black Trees) et les Arbres AVL. L'objectif principal est d'analyser leurs performances respectives en termes de temps d'exécution (CPU) pour des opérations d'insertion et de suppression sur de grands volumes de données.

Le code est réalisé en langage C et se distingue par une conception **générique**, utilisant des pointeurs `void*` et des pointeurs de fonctions pour manipuler n'importe quel type de données (entiers, structures, dictionnaires).

### Architecture Technique

#### 1. Arbres Bicolores (Rouge-Noir)
L'arbre Rouge-Noir est une structure relaxée qui garantit l'équilibre via un attribut de couleur dans chaque nœud.
* **Structure de données :** Utilisation d'une structure `struct` contenant un pointeur parent et une énumération de couleur (ROUGE/NOIR).
* **Insertion :** Insertion standard suivie d'une phase de réparation (`Fixup`) utilisant principalement des recolorations et, si nécessaire, des rotations.
* **Suppression :** Implémentation avancée gérant la suppression d'un nœud et la restauration de la hauteur noire via 4 cas de réparation distincts.

#### 2. Arbres AVL
L'arbre AVL maintient un équilibre strict en calculant un facteur d'équilibre pour chaque nœud (différence de hauteur entre sous-arbres).
* **Stratégie :** Applique des rotations (simples ou doubles) dès que le facteur d'équilibre sort de l'intervalle [-1, 1].

#### 3. Conception Générique (C ANSI)
Le projet démontre une maîtrise de la gestion mémoire bas niveau :
* Utilisation de `void*` pour l'abstraction des données.
* Pointeurs de fonctions pour la comparaison (`compare`) et la libération mémoire (`delete`).
* Gestion manuelle de l'allocation dynamique (`malloc`, `free`).

### Résultats du Benchmark

Les tests de performance ont été réalisés en mesurant le temps processeur (via `clock()`) pour l'insertion de jeux de données allant de 10 à 100 000 éléments.

**Résultats critiques (Insertion de 100 000 entiers) :**

| Structure | Temps d'Insertion | Analyse |
| :--- | :--- | :--- |
| **Arbre Rouge-Noir** | **25.948 ms** | Performance supérieure pour l'écriture. |
| **Arbre AVL** | **55.202 ms** | Plus lent en raison des rotations strictes. |

### Analyse et Interprétation

Les résultats démontrent que l'Arbre Rouge-Noir est environ **deux fois plus rapide** que l'AVL sur des charges massives d'insertion.

**Explication technique :**
L'AVL impose une contrainte de hauteur stricte, ce qui déclenche des rotations fréquentes (opérations coûteuses en manipulation de pointeurs). À l'inverse, l'algorithme de réparation de l'Arbre Rouge-Noir privilégie les **recolorations** (changement d'un bit/enum), qui sont beaucoup moins coûteuses en cycles CPU que les restructurations topologiques.

**Conclusion pratique :**
* Utiliser **Rouge-Noir** pour les systèmes à forte intensité d'écriture (SGBD, Ordonnanceurs OS).
* Utiliser **AVL** pour les systèmes à forte intensité de lecture où la hauteur minimale de l'arbre est la priorité absolue.

### Compilation et Utilisation

Ce projet utilise un `Makefile` pour automatiser la compilation.

**Prérequis :**
* Compilateur GCC
* Make

**Compilation :**

### Avec CMake

```bash
mkdir debug
cd debug
cmake ../src/btree/ -DCMAKE_INSTALL_PREFIX=../tmp -DCMAKE_BUILD_TYPE=Debug
make
```

### Installation de la bibliothèque

```bash
make install
```

### Test

```bash
./test-btree
./benchmark
```
