#ifndef _BTREE_H_
#define _BTREE_H_

#include <stdlib.h>
#include <stdbool.h>

/* Énumération pour les couleurs des nœuds */
typedef enum { RED, BLACK } Color;

/* Type pointeur vers un nœud d'arbre bicolore */
typedef struct _BTreeNode *BTree;

/* Structure d'un nœud d'arbre bicolore */
struct _BTreeNode
{
    BTree left;          // Fils gauche
    BTree right;         // Fils droit
    BTree parent;        // Père (NOUVEAU pour arbre bicolore)
    Color color;         // Couleur du nœud (NOUVEAU pour arbre bicolore)
    char data[1];        // Données génériques (flexible array member)
};

/* ========== Fonctions de base ========== */

/**
 * Crée un arbre vide (NULL)
 */
BTree btree_new();

/**
 * Supprime récursivement un arbre et libère la mémoire
 * @param tree L'arbre à supprimer
 * @param delete Fonction de libération des données (NULL si données sur stack)
 */
void btree_delete(BTree tree, void (*delete)(void *));

/**
 * Crée un nouveau nœud avec les données fournies
 * @param data Pointeur vers les données à copier
 * @param size Taille des données en octets
 * @return Pointeur vers le nouveau nœud ou NULL si échec
 */
BTree btree_create(const void *data, size_t size);

/* ========== Accesseurs ========== */

BTree btree_get_left(BTree tree);
BTree btree_get_right(BTree tree);
BTree btree_get_parent(BTree tree);
Color btree_get_color(BTree tree);
void *btree_get_data(BTree tree);

/* ========== Mutateurs ========== */

bool btree_set_left(BTree tree, BTree left);
bool btree_set_right(BTree tree, BTree right);
bool btree_set_parent(BTree tree, BTree parent);
bool btree_set_color(BTree tree, Color color);
bool btree_set_data(BTree tree, const void *data, size_t size);

/* ========== Parcours ========== */

void btree_pre_order(BTree tree,
                     void (*func)(void *, void *),
                     void *extra_data);

void btree_in_order(BTree tree,
                    void (*func)(void *, void *),
                    void *extra_data);

void btree_post_order(BTree tree,
                      void (*func)(void *, void *),
                      void *extra_data);

/* ========== Propriétés ========== */

size_t btree_height(BTree tree);
size_t btree_size(BTree tree);
size_t btree_black_height(BTree tree);  // Hauteur noire (spécifique aux arbres bicolores)

/* ========== Opérations d'arbre bicolore ========== */

/**
 * Insère une donnée dans l'arbre bicolore avec rééquilibrage
 * @param ptree Pointeur vers la racine de l'arbre
 * @param data Données à insérer
 * @param size Taille des données
 * @param compare Fonction de comparaison (retourne -1, 0, ou 1)
 * @return true si insertion réussie, false sinon
 */
bool btree_insert(BTree *ptree,
                  const void *data,
                  size_t size,
                  int (*compare)(const void *, const void *));

/**
 * Recherche une donnée dans l'arbre bicolore
 * @param tree Arbre dans lequel chercher
 * @param data Donnée à chercher
 * @param compare Fonction de comparaison
 * @return Pointeur vers les données trouvées ou NULL
 */
void *btree_search(BTree tree,
                   const void *data,
                   int (*compare)(const void *, const void *));

/**
 * Supprime une donnée de l'arbre bicolore avec rééquilibrage
 * @param ptree Pointeur vers la racine de l'arbre
 * @param data Donnée à supprimer
 * @param compare Fonction de comparaison
 * @return true si suppression réussie, false sinon
 */
bool btree_delete_node(BTree *ptree,
                       const void *data,
                       int (*compare)(const void *, const void *));

/* ========== Fonctions auxiliaires ========== */

/**
 * Rotation à gauche autour d'un nœud
 */
void btree_rotate_left(BTree *root, BTree node);

/**
 * Rotation à droite autour d'un nœud
 */
void btree_rotate_right(BTree *root, BTree node);

/**
 * Trouve le nœud minimum dans un sous-arbre
 */
BTree btree_minimum(BTree tree);

/**
 * Trouve le nœud maximum dans un sous-arbre
 */
BTree btree_maximum(BTree tree);

/**
 * Vérifie si l'arbre respecte les propriétés des arbres bicolores
 * @return true si l'arbre est valide, false sinon
 */
bool btree_is_valid(BTree tree);

/**
 * Affiche l'arbre de manière visuelle (pour débogage)
 */
void btree_print(BTree tree, int level);

/* ========== Tri ========== */

/**
 * Trie un tableau en utilisant un arbre bicolore
 * @param array Tableau à trier
 * @param length Nombre d'éléments
 * @param size Taille d'un élément
 * @param compare Fonction de comparaison
 * @return true si tri réussi, false sinon
 */
int btree_sort(void *array,
               size_t length,
               size_t size,
               int (*compare)(const void *, const void *));

#endif /* _BTREE_H_ */