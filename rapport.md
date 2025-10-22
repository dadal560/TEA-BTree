# Rapport TEA : Arbres Équilibrés
## Étude de comparaison AVL / Bicolores

**Structures de Données Avancées - L3 Info Automne 2025**

---

Gwendal Henry

---

## 1. Introduction

Ce projet implémente et compare deux structures d'arbres binaires auto-équilibrés : les **arbres AVL** et les **arbres Rouge-Noir (bicolores)**. L'objectif est d'analyser leurs performances respectives en termes de temps d'exécution pour les opérations d'insertion, de recherche et de suppression sur différentes tailles de données.

## 2. Algorithmes implémentés

### 2.1 Arbres Bicolores (Rouge-Noir)

Les arbres Rouge-Noir sont des arbres binaires de recherche équilibrés qui respectent les propriétés suivantes :
1. Chaque nœud est soit rouge, soit noir
2. La racine est toujours noire
3. Les feuilles (NULL) sont considérées comme noires
4. Un nœud rouge ne peut avoir que des enfants noirs
5. Tous les chemins d'un nœud vers ses feuilles descendantes contiennent le même nombre de nœuds noirs

**Structure de données** :
```c
struct _BTreeNode {
    BTree left;
    BTree right;
    BTree parent;      // Pointeur vers le père
    Couleur couleur;   // ROUGE ou NOIR
    char data[1];      // Données génériques
};
```

**Insertion** :
1. Insertion classique comme dans un arbre binaire de recherche
2. Le nouveau nœud est coloré en rouge
3. Rééquilibrage via rotations et recolorations pour maintenir les propriétés

**Suppression** :
1. Recherche du nœud à supprimer
2. Remplacement par son successeur si nécessaire
3. Fixup pour rétablir les propriétés de l'arbre Rouge-Noir

**Rotations** :
- Rotation gauche et droite pour rééquilibrer l'arbre
- Complexité O(1) par rotation

### 2.2 Arbres AVL

Les arbres AVL maintiennent un facteur d'équilibre pour chaque nœud (différence de hauteur entre sous-arbre gauche et droit) compris entre -1 et 1.

**Structure de données** :
```c
struct _TreeNode {
    Tree left;
    Tree right;
    Tree parent;
    int balance;      // Facteur d'équilibre
    char data[1];
};
```

**Insertion et suppression** :
1. Opération standard d'arbre binaire de recherche
2. Mise à jour des facteurs d'équilibre
3. Rotations (simples ou doubles) si nécessaire pour rétablir l'équilibre

### 2.3 Complexité théorique

| Opération | AVL | Rouge-Noir |
|-----------|-----|------------|
| Recherche | O(log n) | O(log n) |
| Insertion | O(log n) | O(log n) |
| Suppression | O(log n) | O(log n) |

## 3. Tests effectués

### 3.1 Protocole de test

Les tests ont été réalisés sur :
- **Type entier** : Nombres aléatoires entre 0 et size×10
- **Type dictionnaire** : Structure {mot, définition} avec mot comme clé

Tailles testées : 10, 50, 100, 150, 200, 300, 500, 750, 1000 éléments

Pour chaque taille :
1. Génération de données aléatoires
2. Mesure du temps d'insertion pour AVL
3. Mesure du temps d'insertion pour Rouge-Noir
4. Libération de la mémoire

### 3.2 Environnement de test

- Compilateur : GCC avec optimisation standard
- Mesure du temps : `clock()` de `time.h`
- Unité : millisecondes (ms)
- Génération de graphiques : gnuplot

## 4. Résultats et analyse

### 4.1 Graphique des performances

Le graphique généré (`benchmark.png`) présente :
- Courbe bleue : AVL - Entiers
- Courbe rouge : Rouge-Noir - Entiers
- Courbe verte : AVL - Dictionnaire
- Courbe violette : Rouge-Noir - Dictionnaire
- Ligne noire : f(x) = x (complexité linéaire)

### 4.2 Observations

**Pour les entiers** :
- Les deux structures montrent des performances comparables
- Les arbres Rouge-Noir sont légèrement plus rapides en insertion pour les grandes tailles
- La courbe reste en dessous de la fonction linéaire f(x) = x, confirmant une complexité sous-linéaire

**Pour les dictionnaires** :
- Performance similaire entre AVL et Rouge-Noir
- Temps d'exécution légèrement supérieur aux entiers (coût de comparaison de chaînes)
- Comportement logarithmique maintenu

**Comparaison avec f(x) = x** :
- Les deux algorithmes sont largement plus performants qu'une complexité linéaire
- L'écart se creuse avec l'augmentation du nombre de données
- Confirmation de la complexité O(n log n) pour n insertions

### 4.3 Interprétation

1. **Équilibrage** :
   - AVL : Plus strictement équilibré (hauteur minimale)
   - Rouge-Noir : Équilibrage plus souple mais suffisant

2. **Performance pratique** :
   - Rouge-Noir : Moins de rotations lors de l'insertion (plus rapide en insertion intensive)
   - AVL : Recherche légèrement plus rapide grâce à un meilleur équilibrage

3. **Scalabilité** :
   - Les deux structures passent à l'échelle de manière logarithmique
   - Différence négligeable pour des ensembles de données de taille moyenne (< 1000)

## 5. Conclusion

Ce projet a permis d'implémenter et de comparer deux structures d'arbres auto-équilibrés majeures en informatique. Les résultats expérimentaux confirment les performances théoriques attendues :

- **Complexité logarithmique** : Toutes les opérations s'exécutent en O(log n)
- **Performance globale** : Les arbres Rouge-Noir sont légèrement plus performants pour l'insertion massive, tandis que les AVL offrent une recherche marginalement plus rapide
- **Généricité** : L'implémentation en C avec des pointeurs void* permet de manipuler tout type de données

**Choix pratique** :
- Utiliser les **arbres Rouge-Noir** pour des applications avec insertions/suppressions fréquentes (bases de données, systèmes de fichiers)
- Privilégier les **arbres AVL** pour des structures principalement consultées avec peu de modifications

L'écart de performance observé reste faible sur les tailles testées, rendant les deux structures viables pour la plupart des applications nécessitant un arbre binaire équilibré.

---

*Fin du rapport*
