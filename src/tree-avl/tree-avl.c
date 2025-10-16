

#include <string.h>
#include "tree-avl.h"
#include <stdbool.h>
#include "min-max.h"

static Tree rotate_left(Tree A);
static Tree rotate_right(Tree A);
static Tree tree_balance(Tree root);

/*--------------------------------------------------------------------*/
Tree
tree_new ()
{
  return NULL;
}

void tree_delete (Tree tree, void (*delete) (void *))
{
  if (tree)
    {
      tree_delete (tree->left, delete);
      tree_delete (tree->right, delete);
      if (delete)
        delete (tree->data);
      free (tree);
    }
}

Tree tree_create(const void *data, size_t size) {
    // Allocation mémoire pour le nœud et les données
    Tree tree = (Tree)malloc(sizeof(struct _TreeNode) - sizeof(char) + size);
    if (tree) {
        tree->left = NULL;  // Pas de sous-arbre gauche
        tree->right = NULL; // Pas de sous-arbre droit
        tree->parent = NULL; // Pas de parent
        tree->balance = 0;   // Balance initiale de 0
        memcpy(tree->data, data, size);  // Copie les données dans le nœud
    }
    return tree;
}

Tree
tree_get_left (Tree tree)
{
  if (tree)
    return tree->left;
  else
    return NULL;
}

Tree
tree_get_right (Tree tree)
{
  if (tree)
    return tree->right;
  else
    return NULL;
}

void *
tree_get_data (Tree tree)
{
  if (tree)
    return tree->data;
  else
    return NULL;
}

bool
tree_set_left (Tree tree, Tree left)
{
  if (tree)
    {
      tree->left = left;
      return true;
    }
  else
    return false;
}

bool
tree_set_right (Tree tree, Tree right)
{
  if (tree)
    {
      tree->right = right;
      return true;
    }
  else
    return false;
}

bool
tree_set_data (Tree tree, const void *data, size_t
size)
{
  if (tree)
    {
      memcpy (tree->data, data, size);
      return true;
    }
  else
    return false;
}

void
tree_pre_order (Tree tree,
                void (*func) (void *, void *),
                void *extra_data)
{
  if (tree)
    {
      func (tree, extra_data);
      tree_pre_order (tree->left, func, extra_data);
      tree_pre_order (tree->right, func, extra_data);
    }
}


void
tree_in_order (Tree tree,
               void (*func) (void *, void *),
               void *extra_data)
{
  if (tree)
    {
      tree_in_order (tree->left, func, extra_data);
      func (tree, extra_data);
      tree_in_order (tree->right, func, extra_data);
    }
}

void
tree_post_order (Tree tree,
                 void (*func) (void *, void *),
                 void *extra_data)
{
  if (tree)
    {
      tree_post_order (tree->left, func, extra_data);
      tree_post_order (tree->right, func, extra_data);
      func (tree, extra_data);
    }
}

size_t
tree_height (Tree tree)
{
  if (tree)
    return 1 + MAX (tree_height (tree->left),
tree_height (tree->right));
  else
    return 0;
}

size_t
tree_size (Tree tree)
{
  if (tree)
    return 1 + tree_size (tree->left) + tree_size
(tree->right);
  else
    return 0;
}

bool
tree_insert_sorted (Tree * ptree,
                    const void *data,
                    size_t size,
                    int (*compare) (const void *, const
void *))
{
  if (*ptree)
    {
      switch (compare (data, (*ptree)->data))
        {
        case -1:
          return tree_insert_sorted (&(*ptree)->left,
data, size, compare);
        default:
          return tree_insert_sorted (&(*ptree)->right,
data, size, compare);
        }
    }
  else
    {
      Tree new = tree_create (data, size);
      if (new)
        {
          *ptree = new;
          return true;
        }
      else
        return false;
    }

}

void *
tree_search (Tree tree,
             const void *data,
             int (*compare) (const void *, const void
*))
{
  if (tree)
    {
      switch (compare (data, tree->data))
        {
        case -1:
          return tree_search (tree->left, data, compare);
        case 0:
          return tree->data;
        case 1:
          return tree_search (tree->right, data, compare);
        default:
            return NULL; // RAJOUTE CAR WARNING !!!
        }
    }
  else
    return NULL;
}

static void
set (void *data,void *array)
{
  static size_t size;
  static size_t offset;

  if (data)
    {
      memcpy (array + offset, data, size);
      offset += size;
    }
  else
    {
      offset = 0;
      size = *(size_t *) array;
    }
}

int
tree_sort (void *array,
           size_t length,
           size_t size,
           int (*compare) (const void *, const void *))
{
  size_t i;
  Tree tree = tree_new ();
  void *pointer;

  pointer = array;
  for (i = 0; i < length; i++)
    {
      if (tree_insert_sorted (&tree, pointer, size,
compare))
        pointer += size;
      else
        {
          tree_delete (tree, NULL);
          return false;
        }
    }
  set (NULL, &size);
  tree_in_order (tree, set, array);
  tree_delete (tree, NULL);
  return true;
}

//Fonctions AVL

/**
 * Insertion interne AVL.
 * `hauteur_a_augmenté` : true si la hauteur du sous-arbre a augmenté.
 */
static bool tree_insert_avl_interne(Tree *ptree, const void *data, int size,
                                   int (*compare)(const void *, const void *),
                                   bool *hauteur_a_augmenté) {
    if (!*ptree) {
        Tree new_node = tree_create(data, size);
        if (!new_node) return false;
        new_node->balance = 0;
        *ptree = new_node;
        *hauteur_a_augmenté = true;
        return true;
    }

    int cmp = compare(data, (*ptree)->data);

    if (cmp < 0) {
        if (!tree_insert_avl_interne(&(*ptree)->left, data, size, compare, hauteur_a_augmenté))
            return false;
        if ((*ptree)->left) (*ptree)->left->parent = *ptree;
        if (*hauteur_a_augmenté) (*ptree)->balance -= 1;
    } else if (cmp > 0) {
        if (!tree_insert_avl_interne(&(*ptree)->right, data, size, compare, hauteur_a_augmenté))
            return false;
        if ((*ptree)->right) (*ptree)->right->parent = *ptree;
        if (*hauteur_a_augmenté) (*ptree)->balance += 1;
    } else {
        *hauteur_a_augmenté = false;
        return true; // déjà présent
    }

    // Rééquilibrage si nécessaire
    if ((*ptree)->balance < -1 || (*ptree)->balance > 1) {
        *ptree = tree_balance(*ptree);
        *hauteur_a_augmenté = false;
    } else {
        if ((*ptree)->balance == 0)
            *hauteur_a_augmenté = false;
    }

    return true;
}

/* Wrapper public */
bool tree_insert_avl(Tree *ptree, const void *data, int size,
                     int (*compare)(const void *, const void *)) {
    bool hauteur_a_augmenté = false;
    return tree_insert_avl_interne(ptree, data, size, compare, &hauteur_a_augmenté);
}

/* Recherche AVL */
Tree tree_search_avl(Tree root, const void *data, int (*compare)(const void *, const void *)) {
    if (!root) return NULL;
    int cmp = compare(data, root->data);
    if (cmp == 0) return root;
    else if (cmp < 0) return tree_search_avl(root->left, data, compare);
    else return tree_search_avl(root->right, data, compare);
}

/* Suppression AVL */
Tree tree_delete_avl(Tree root,
                     const void *data,
                     int (*compare)(const void *, const void *),
                     void (*delete)(void *)) {
    if (!root) return NULL;
    int cmp = compare(data, root->data);

    if (cmp < 0) {
        root->left = tree_delete_avl(root->left, data, compare, delete);
        if (root->left) root->left->parent = root;
    } else if (cmp > 0) {
        root->right = tree_delete_avl(root->right, data, compare, delete);
        if (root->right) root->right->parent = root;
    } else {
        if (delete) delete(root->data);
        if (!root->left || !root->right) {
            Tree tmp = root->left ? root->left : root->right;
            if (!tmp) { free(root); return NULL; }
            tmp->parent = root->parent;
            free(root);
            return tmp;
        } else {
            Tree succ = root->right;
            while (succ->left) succ = succ->left;
            memcpy(root->data, succ->data, sizeof(int));
            root->right = tree_delete_avl(root->right, succ->data, compare, delete);
            if (root->right) root->right->parent = root;
        }
    }

    // Rééquilibrage
    if (root->balance < -1 || root->balance > 1)
        return tree_balance(root);

    return root;
}

/* Rotations */
static Tree rotate_left(Tree A) {
    Tree B = A->right;
    A->right = B->left;
    if (B->left) B->left->parent = A;
    B->left = A;
    B->parent = A->parent;
    A->parent = B;

    // Mise à jour balances
    int left_bal = A->balance;
    int right_bal = B->balance;
    A->balance = left_bal - 1 - (right_bal > 0 ? right_bal : 0);
    B->balance = right_bal - 1 + (A->balance < 0 ? A->balance : 0);

    return B;
}

static Tree rotate_right(Tree A) {
    Tree B = A->left;
    A->left = B->right;
    if (B->right) B->right->parent = A;
    B->right = A;
    B->parent = A->parent;
    A->parent = B;

    int left_bal = A->balance;
    int right_bal = B->balance;
    A->balance = left_bal + 1 - (right_bal < 0 ? right_bal : 0);
    B->balance = right_bal + 1 + (A->balance > 0 ? A->balance : 0);

    return B;
}

/* Rééquilibrage */
static Tree tree_balance(Tree root) {
    if (root->balance > 1) {
        if (root->right && root->right->balance < 0)
            root->right = rotate_right(root->right);
        return rotate_left(root);
    } else if (root->balance < -1) {
        if (root->left && root->left->balance > 0)
            root->left = rotate_left(root->left);
        return rotate_right(root);
    }
    return root;
}