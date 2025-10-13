#ifndef _BTREE_H_
#define _BTREE_H_

#include <stdlib.h>
#include <stdbool.h>

/* Énumération pour les couleurs des nœuds */
typedef enum { ROUGE, NOIR } Couleur;

typedef struct _BTreeNode *BTree;

struct _BTreeNode
{
    BTree left;
    BTree right;
    BTree parent;         // Pointeur vers le nœud père
    Couleur couleur;          //  Couleur du noeud (Rouge ou Noire)
    char data[1];
};

/* ========== Fonctions de base========== */

BTree btree_new();

void btree_delete(BTree btree, void (*delete)(void *));

BTree btree_create(const void *data, size_t size);

BTree btree_get_left(BTree btree);

BTree btree_get_right(BTree btree);

void *btree_get_data(BTree btree);

bool btree_set_left(BTree btree, BTree left);

bool btree_set_right(BTree btree, BTree right);

bool btree_set_data(BTree btree, const void *data, size_t size);

void btree_pre_order(BTree btree,
                     void (*func)(void *, void *),
                     void *extra_data);

void btree_in_order(BTree btree,
                    void (*func)(void *, void *),
                    void *extra_data);

void btree_post_order(BTree btree,
                      void (*func)(void *, void *),
                      void *extra_data);

size_t btree_height(BTree btree);

size_t btree_size(BTree btree);

/* ========== Nouvelles fonctions pour parent et couleur ========== */

BTree btree_get_parent(BTree btree);

Color btree_get_color(BTree btree);

bool btree_set_parent(BTree btree, BTree parent);

bool btree_set_color(BTree btree, Color color);

/* ========== Opérations arbre bicolore ========== */

/**
 * Insère une donnée dans l'arbre bicolore avec rééquilibrage
 * @param pbtree Pointeur vers la racine de l'arbre
 * @param data Données à insérer
 * @param size Taille des données
 * @param compare Fonction de comparaison (retourne -1, 0, ou 1)
 * @return true si insertion réussie, false sinon
 */
bool btree_insert_sorted(BTree *pbtree,
                         const void *data,
                         size_t size,
                         int (*compare)(const void *, const void *));

/**
 * Recherche une donnée dans l'arbre
 * @param btree Arbre dans lequel chercher
 * @param data Donnée à chercher
 * @param compare Fonction de comparaison
 * @return Pointeur vers les données trouvées ou NULL
 */
void *btree_search(BTree btree,
                   const void *data,
                   int (*compare)(const void *, const void *));

/**
 * Supprime une donnée de l'arbre bicolore avec rééquilibrage
 * @param pbtree Pointeur vers la racine de l'arbre
 * @param data Donnée à supprimer
 * @param compare Fonction de comparaison
 * @return true si suppression réussie, false sinon
 */
bool btree_delete_node(BTree *pbtree,
                       const void *data,
                       int (*compare)(const void *, const void *));

/* ========== Fonctions auxiliaires pour arbre bicolore ========== */

void btree_rotate_left(BTree *root, BTree node);

void btree_rotate_right(BTree *root, BTree node);

BTree btree_minimum(BTree btree);

BTree btree_maximum(BTree btree);

/* ========== Tri (conservé) ========== */

int btree_sort(void *array,
               size_t length,
               size_t size,
               int (*compare)(const void *, const void *));

#endif /* _BTREE_H_ */
