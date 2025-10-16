#include <string.h>
#include "btree.h"
#include <stdbool.h>
#include "min-max.h"

/*--------------------------------------------------------------------*/
/* Fonctions de base */
/*--------------------------------------------------------------------*/

BTree btree_new()
{
    return NULL;
}

void btree_delete(BTree btree, void (*delete)(void *))
{
    if (btree)
    {
        btree_delete(btree->left, delete);
        btree_delete(btree->right, delete);
        if (delete)
            delete(btree->data);
        free(btree);
    }
}

BTree btree_create(const void *data, size_t size)
{
    BTree btree = (BTree)malloc(3 * sizeof(BTree) + sizeof(Couleur) + size);
    if (btree)
    {
        btree->left = NULL;
        btree->right = NULL;
        btree->parent = NULL;
        btree->couleur = ROUGE; // Un nouveau noeud est rouge par défaut
        memcpy(btree->data, data, size);
    }
    return btree;
}

BTree btree_get_left(BTree btree)
{
    if (btree)
        return btree->left;
    else
        return NULL;
}

BTree btree_get_right(BTree btree)
{
    if (btree)
        return btree->right;
    else
        return NULL;
}

void *btree_get_data(BTree btree)
{
    if (btree)
        return btree->data;
    else
        return NULL;
}

BTree btree_get_parent(BTree btree)
{
    if (btree)
        return btree->parent;
    else
        return NULL;
}

Couleur btree_get_couleur(BTree btree)
{
    if (btree)
        return btree->couleur;
    else
        return NOIR; // NULL est considéré comme noir
}

bool btree_set_left(BTree btree, BTree left)
{
    if (btree)
    {
        btree->left = left;
        if (left)
            left->parent = btree;
        return true;
    }
    else
        return false;
}

bool btree_set_right(BTree btree, BTree right)
{
    if (btree)
    {
        btree->right = right;
        if (right)
            right->parent = btree;
        return true;
    }
    else
        return false;
}

bool btree_set_data(BTree btree, const void *data, size_t size)
{
    if (btree)
    {
        memcpy(btree->data, data, size);
        return true;
    }
    else
        return false;
}

bool btree_set_parent(BTree btree, BTree parent)
{
    if (btree)
    {
        btree->parent = parent;
        return true;
    }
    else
        return false;
}

bool btree_set_couleur(BTree btree, Couleur couleur)
{
    if (btree)
    {
        btree->couleur = couleur;
        return true;
    }
    else
        return false;
}

/*--------------------------------------------------------------------*/
/* Parcours d'arbre */
/*--------------------------------------------------------------------*/

void btree_pre_order(BTree btree, void (*func)(void *, void *), void *extra_data)
{
    if (btree)
    {
        func(btree->data, extra_data);
        btree_pre_order(btree->left, func, extra_data);
        btree_pre_order(btree->right, func, extra_data);
    }
}

void btree_in_order(BTree btree, void (*func)(void *, void *), void *extra_data)
{
    if (btree)
    {
        btree_in_order(btree->left, func, extra_data);
        func(btree->data, extra_data);
        btree_in_order(btree->right, func, extra_data);
    }
}

void btree_post_order(BTree btree, void (*func)(void *, void *), void *extra_data)
{
    if (btree)
    {
        btree_post_order(btree->left, func, extra_data);
        btree_post_order(btree->right, func, extra_data);
        func(btree->data, extra_data);
    }
}

size_t btree_height(BTree btree)
{
    if (btree)
        return 1 + MAX(btree_height(btree->left), btree_height(btree->right));
    else
        return 0;
}

size_t btree_size(BTree btree)
{
    if (btree)
        return 1 + btree_size(btree->left) + btree_size(btree->right);
    else
        return 0;
}

/*--------------------------------------------------------------------*/
/* Fonctions auxiliaires pour arbre bicolore */
/*--------------------------------------------------------------------*/

void btree_rotate_left(BTree *root, BTree x)
{
    BTree y = x->right;
    x->right = y->left;
    
    if (y->left != NULL)
        y->left->parent = x;
    
    y->parent = x->parent;
    
    if (x->parent == NULL)
        *root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    
    y->left = x;
    x->parent = y;
}

void btree_rotate_right(BTree *root, BTree y)
{
    BTree x = y->left;
    y->left = x->right;
    
    if (x->right != NULL)
        x->right->parent = y;
    
    x->parent = y->parent;
    
    if (y->parent == NULL)
        *root = x;
    else if (y == y->parent->right)
        y->parent->right = x;
    else
        y->parent->left = x;
    
    x->right = y;
    y->parent = x;
}

BTree btree_minimum(BTree btree)
{
    while (btree && btree->left != NULL)
        btree = btree->left;
    return btree;
}

BTree btree_maximum(BTree btree)
{
    while (btree && btree->right != NULL)
        btree = btree->right;
    return btree;
}

/*--------------------------------------------------------------------*/
/* Insertion avec rééquilibrage */
/*--------------------------------------------------------------------*/

static void btree_insert_fixup(BTree *root, BTree z)
{
    while (z->parent && z->parent->couleur == ROUGE)
    {
        if (z->parent == z->parent->parent->left)
        {
            BTree y = z->parent->parent->right; // Oncle
            
            if (y && y->couleur == ROUGE)
            {
                // Cas 1: l'oncle est rouge
                z->parent->couleur = NOIR;
                y->couleur = NOIR;
                z->parent->parent->couleur = ROUGE;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->right)
                {
                    // Cas 2: z est un enfant droit
                    z = z->parent;
                    btree_rotate_left(root, z);
                }
                // Cas 3: z est un enfant gauche
                z->parent->couleur = NOIR;
                z->parent->parent->couleur = ROUGE;
                btree_rotate_right(root, z->parent->parent);
            }
        }
        else
        {
            BTree y = z->parent->parent->left; // Oncle
            
            if (y && y->couleur == ROUGE)
            {
                // Cas 1: l'oncle est rouge
                z->parent->couleur = NOIR;
                y->couleur = NOIR;
                z->parent->parent->couleur = ROUGE;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->left)
                {
                    // Cas 2: z est un enfant gauche
                    z = z->parent;
                    btree_rotate_right(root, z);
                }
                // Cas 3: z est un enfant droit
                z->parent->couleur = NOIR;
                z->parent->parent->couleur = ROUGE;
                btree_rotate_left(root, z->parent->parent);
            }
        }
    }
    (*root)->couleur = NOIR;
}

bool btree_insert(BTree *pbtree, const void *data, size_t size,
                         int (*compare)(const void *, const void *))
{
    BTree z = btree_create(data, size);
    if (!z)
        return false;
    
    BTree y = NULL;
    BTree x = *pbtree;
    
    // Recherche de la position d'insertion
    while (x != NULL)
    {
        y = x;
        if (compare(data, x->data) < 0)
            x = x->left;
        else
            x = x->right;
    }
    
    z->parent = y;
    
    if (y == NULL)
    {
        *pbtree = z; // L'arbre était vide
    }
    else if (compare(data, y->data) < 0)
    {
        y->left = z;
    }
    else
    {
        y->right = z;
    }
    
    z->left = NULL;
    z->right = NULL;
    z->couleur = ROUGE;
    
    btree_insert_fixup(pbtree, z);
    
    return true;
}

/*--------------------------------------------------------------------*/
/* Recherche */
/*--------------------------------------------------------------------*/

void *btree_search(BTree btree, const void *data,
                   int (*compare)(const void *, const void *))
{
    if (btree)
    {
        int cmp = compare(data, btree->data);
        if (cmp < 0)
            return btree_search(btree->left, data, compare);
        else if (cmp > 0)
            return btree_search(btree->right, data, compare);
        else
            return btree->data;
    }
    else
        return NULL;
}

/*--------------------------------------------------------------------*/
/* Suppression avec rééquilibrage */
/*--------------------------------------------------------------------*/

static void btree_transplant(BTree *root, BTree u, BTree v)
{
    if (u->parent == NULL)
        *root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    
    if (v != NULL)
        v->parent = u->parent;
}

static void btree_delete_fixup(BTree *root, BTree x, BTree x_parent)
{
    while (x != *root && (x == NULL || x->couleur == NOIR))
    {
        if (x == x_parent->left)
        {
            BTree w = x_parent->right;
            
            if (w && w->couleur == ROUGE)
            {
                w->couleur = NOIR;
                x_parent->couleur = ROUGE;
                btree_rotate_left(root, x_parent);
                w = x_parent->right;
            }
            
            if (w && (w->left == NULL || w->left->couleur == NOIR) &&
                (w->right == NULL || w->right->couleur == NOIR))
            {
                w->couleur = ROUGE;
                x = x_parent;
                x_parent = x->parent;
            }
            else if (w)
            {
                if (w->right == NULL || w->right->couleur == NOIR)
                {
                    if (w->left)
                        w->left->couleur = NOIR;
                    w->couleur = ROUGE;
                    btree_rotate_right(root, w);
                    w = x_parent->right;
                }
                
                w->couleur = x_parent->couleur;
                x_parent->couleur = NOIR;
                if (w->right)
                    w->right->couleur = NOIR;
                btree_rotate_left(root, x_parent);
                x = *root;
            }
            else
                break;
        }
        else
        {
            BTree w = x_parent->left;
            
            if (w && w->couleur == ROUGE)
            {
                w->couleur = NOIR;
                x_parent->couleur = ROUGE;
                btree_rotate_right(root, x_parent);
                w = x_parent->left;
            }
            
            if (w && (w->right == NULL || w->right->couleur == NOIR) &&
                (w->left == NULL || w->left->couleur == NOIR))
            {
                w->couleur = ROUGE;
                x = x_parent;
                x_parent = x->parent;
            }
            else if (w)
            {
                if (w->left == NULL || w->left->couleur == NOIR)
                {
                    if (w->right)
                        w->right->couleur = NOIR;
                    w->couleur = ROUGE;
                    btree_rotate_left(root, w);
                    w = x_parent->left;
                }
                
                w->couleur = x_parent->couleur;
                x_parent->couleur = NOIR;
                if (w->left)
                    w->left->couleur = NOIR;
                btree_rotate_right(root, x_parent);
                x = *root;
            }
            else
                break;
        }
    }
    
    if (x)
        x->couleur = NOIR;
}

bool btree_delete_node(BTree *pbtree, const void *data,
                       int (*compare)(const void *, const void *))
{
    BTree z = *pbtree;
    
    // Recherche du nœud à supprimer
    while (z != NULL)
    {
        int cmp = compare(data, z->data);
        if (cmp < 0)
            z = z->left;
        else if (cmp > 0)
            z = z->right;
        else
            break;
    }
    
    if (z == NULL)
        return false; // Élément non trouvé
    
    BTree y = z;
    BTree x;
    BTree x_parent;
    Couleur y_original_color = y->couleur;
    
    if (z->left == NULL)
    {
        x = z->right;
        x_parent = z->parent;
        btree_transplant(pbtree, z, z->right);
    }
    else if (z->right == NULL)
    {
        x = z->left;
        x_parent = z->parent;
        btree_transplant(pbtree, z, z->left);
    }
    else
    {
        y = btree_minimum(z->right);
        y_original_color = y->couleur;
        x = y->right;
        
        if (y->parent == z)
        {
            x_parent = y;
        }
        else
        {
            x_parent = y->parent;
            btree_transplant(pbtree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        
        btree_transplant(pbtree, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->couleur = z->couleur;
    }
    
    free(z);
    
    if (y_original_color == NOIR)
        btree_delete_fixup(pbtree, x, x_parent);
    
    return true;
}

/*--------------------------------------------------------------------*/
/* Tri utilisant l'arbre bicolore */
/*--------------------------------------------------------------------*/

static void set(void *data, void *array)
{
    static size_t size;
    static size_t offset;
    
    if (data)
    {
        memcpy(array + offset, data, size);
        offset += size;
    }
    else
    {
        offset = 0;
        size = *(size_t *)array;
    }
}

int btree_sort(void *array, size_t length, size_t size,
               int (*compare)(const void *, const void *))
{
    size_t i;
    BTree btree = btree_new();
    void *pointer;
    
    pointer = array;
    for (i = 0; i < length; i++)
    {
        if (btree_insert(&btree, pointer, size, compare))
            pointer += size;
        else
        {
            btree_delete(btree, NULL);
            return false;
        }
    }
    
    set(NULL, &size);
    btree_in_order(btree, set, array);
    btree_delete(btree, NULL);
    
    return true;
}