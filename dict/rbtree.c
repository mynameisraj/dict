/* THIS TREE IS NOT THREADSAFE
   TODOS:
    - custom comparator
    - serialize from disk
    - write to disk
    - print tree to screen
 */

#include <stdlib.h>
#include "rbtree.h"

#define RBTREE_BLACK 0
#define RBTREE_RED   1

#define LESS_THAN   -1
#define GREATER_THAN 1
#define EQUAL_TO     0

/* Node creation */
static rbtreenode_t * create_node_empty(rbtree_t * tree);
static rbtreenode_t * create_node(rbtree_t * tree, const void * key, const void * value);

/* Insertion */
static void insert(rbtree_t * tree, rbtreenode_t * z);
static void insert_fix(rbtree_t * tree, rbtreenode_t * z);

/* Rotations */
static void left_rotate(rbtree_t * tree, rbtreenode_t * x);
static void right_rotate(rbtree_t * tree, rbtreenode_t * x);
static void transplant(rbtree_t * tree, rbtreenode_t * u, rbtreenode_t * v);

/* Removal */
static void remove_node(rbtree_t * tree, rbtreenode_t * z);
static void fix_remove(rbtree_t * tree, rbtreenode_t * x);

/* Retrieval */
static rbtreenode_t * find_node(rbtree_t * tree, rbtreenode_t * x, const void * key);
static rbtreenode_t * get_minimum(rbtree_t * tree, rbtreenode_t * x);
static rbtreenode_t * get_maximum(rbtree_t * tree, rbtreenode_t * x);

int compare_less_than(comparator_t comparator, void * a, void * b)
{
    return comparator(a, b) == LESS_THAN;
}

int compare_equal(comparator_t comparator, void * a, void * b)
{
    return comparator(a, b) == EQUAL_TO;
}

int compare_greater_than(comparator_t comparator, void * a, void * b)
{
    return comparator(a, b) == GREATER_THAN;
}

/* No-argument constructor */
rbtree_t * rbtree_create(comparator_t comparator)
{
    rbtree_t * tree = (rbtree_t *) malloc(sizeof(rbtree_t));
    if (tree == NULL) {
        return NULL;
    }
    rbtreenode_t * nil = (rbtreenode_t *) malloc(sizeof(rbtreenode_t));
    nil->color = RBTREE_BLACK;
    tree->nil_sentinel = nil;
    tree->root = tree->nil_sentinel;
    tree->comparator = comparator;
    return tree;
}

/* Creates a new empty node. You must free this */
static rbtreenode_t * create_node_empty(rbtree_t * tree)
{
    rbtreenode_t * node = (rbtreenode_t *) malloc(sizeof(rbtreenode_t));
    if (node == NULL) {
        return NULL;
    }
    node->key = NULL;
    node->value = NULL;
    node->left = tree->nil_sentinel;
    node->right = tree->nil_sentinel;
    node->parent = tree->nil_sentinel;
    node->color = RBTREE_BLACK;
    return node;
}

/* Creates a node with the specified key and value. */
static rbtreenode_t * create_node(rbtree_t * tree, const void * key, const void * value)
{
    rbtreenode_t * node = create_node_empty(tree);
    node->key = key;
    node->value = value;
    return node;
}

/* Clear recursive helper */
static void clear(rbtreenode_t * node)
{
    if (node == NULL) {
        return;
    }
    clear(node->left);
    clear(node->right);
    free(node);
}

/* Destructor. */
int rbtree_destroy(rbtree_t * tree)
{
    if (tree->root != tree->nil_sentinel) {
        clear(tree->root);
    }
    free(tree);
    return 0;
}

/* Inserts a given key and value into the tree */
void rbtree_insert(rbtree_t * tree, const void * key, const void * value)
{
    rbtreenode_t* node = create_node(tree, key, value);
    insert(tree, node);
}

/* Insert given node into the tree */
static void insert(rbtree_t * tree, rbtreenode_t * z)
{
    rbtreenode_t * y = tree->nil_sentinel;
    rbtreenode_t * x = tree->root;
    if (x == y) {
        /* Insertion into empty tree */
        tree->root = z;
        return;
    }
    while (x != tree->nil_sentinel) {
        y = x;
        x = (tree->comparator(z->key, x->key) == LESS_THAN) ? x->left : x->right;
    }
    z->parent = y;
    if (y == tree->nil_sentinel) {
        tree->root = z;
    } else if (tree->comparator(z->key, y->key) == LESS_THAN) {
        y->left = z;
    } else {
        y->right = z;
    }
    z->color = RBTREE_RED;
    insert_fix(tree, z);
}

/* Fix for insert as documented in CLRS */
/* TODO: check the equals here to see if they need to use the comparator */
static void insert_fix(rbtree_t * tree, rbtreenode_t * z)
{
    rbtreenode_t * y;
    while (z->parent->color == RBTREE_RED) {
        if (z->parent == z->parent->parent->left) {
            y = z->parent->parent->right;
            if (y->color == RBTREE_RED) {
                z->parent->color = RBTREE_BLACK;
                y->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                z = z->parent->parent;
            }
            else if (z == z->parent->right) {
                z = z->parent;
                left_rotate(tree, z);
                z->parent->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                right_rotate(tree, z->parent->parent);
            } else {
                break;
            }
        } else {
            y = z->parent->parent->left;
            if (y->color == RBTREE_RED) {
                z->parent->color = RBTREE_BLACK;
                y->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                z = z->parent->parent;
            } else if (z == z->parent->left) {
                z = z->parent;
                right_rotate(tree, z);
                z->parent->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                left_rotate(tree, z->parent->parent);
            } else {
                break;
            }
        }
    }
    tree->root->color = RBTREE_BLACK;
}

/* Performs a left rotate on the given node. */
static void left_rotate(rbtree_t * tree, rbtreenode_t * x)
{
    rbtreenode_t * y = x->right;
    x->right = y->left;
    if (y->left != tree->nil_sentinel) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == tree->nil_sentinel) {
        tree->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

/* Performs a right rotate on the given node. */
static void right_rotate(rbtree_t * tree, rbtreenode_t * x)
{
    rbtreenode_t * y = x->left;
    x->left = y->right;
    if (y->right != tree->nil_sentinel) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == tree->nil_sentinel) {
        tree->root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}

/* Transplants two nodes */
static void transplant(rbtree_t * tree, rbtreenode_t * u, rbtreenode_t * v)
{
    if (u->parent == tree->nil_sentinel) {
        tree->root = v;
    } else if (u == (u->parent)->left) {
        (u->parent)->left = v;
    } else {
        (u->parent)->right = v;
    }
    v->parent = u->parent;
}

/* Public remove */
void rbtree_remove(rbtree_t * tree, const void * key)
{
    remove_node(tree, find_node(tree, tree->root, key));
}

/* Removes a node */
static void remove_node(rbtree_t * tree, rbtreenode_t * z)
{
    rbtreenode_t *y, *x;
    y = z;
    int old_color = y->color;
    if (z->left == tree->nil_sentinel) {
        x = z->right;
        transplant(tree, z, z->right);
    } else if (z->right == tree->nil_sentinel) {
        x = z->left;
        transplant(tree, z, z->left);
    } else {
        y = get_minimum(tree, z->right);
        old_color= y->color;
        x = y->right;
        if (y->parent == z)
            x->parent = y;
        else
        {
            transplant(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
            
        }
        transplant(tree, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    if (old_color == RBTREE_BLACK) {
        fix_remove(tree, x);
    }
}

/* Remove helper function */
static void fix_remove(rbtree_t * tree, rbtreenode_t * x)
{
    rbtreenode_t* w;
    while (x != tree->root && x->color == RBTREE_BLACK) {
        if (x == x->parent->left)
        {
            w = x->parent->right;
            if (w->color == RBTREE_RED)
            {
                w->color = RBTREE_BLACK;
                x->parent->color = RBTREE_RED;
                left_rotate(tree, x->parent);
                w = (x->parent)->right;
            }
            if (w->right->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK)
            {
                w->color = RBTREE_RED;
                x = x->parent;
            }
            else if (w->right->color == RBTREE_BLACK)
            {
                w->left->color = RBTREE_BLACK;
                w->color = RBTREE_RED;
                right_rotate(tree, w);
                w = x->parent->right;
            }
            w->color = x->parent->color;
            x->parent->color = RBTREE_BLACK;
            w->right->color = RBTREE_BLACK;
            left_rotate(tree, x->parent);
            x = tree->root;
        }
        else
        {
            w = x->parent->left;
            if (w->color == RBTREE_RED)
            {
                w->color = RBTREE_BLACK;
                x->parent->color = RBTREE_RED;
                right_rotate(tree, x->parent);
                w = x->parent->left;
            }
            if (w->left->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK)
            {
                w->color = RBTREE_RED;
                x = x->parent;
            }
            else if (w->left->color == RBTREE_BLACK)
            {
                w->right->color = RBTREE_BLACK;
                w->color = RBTREE_RED;
                left_rotate(tree, w);
                w = x->parent->left;
            }
            w->color = x->parent->color;
            x->parent->color = RBTREE_BLACK;
            w->left->color = RBTREE_BLACK;
            right_rotate(tree, x->parent);
            x = tree->root;
        }
    }
    x->color = RBTREE_BLACK;
}

/* Is this tree empty? */
int rbtree_is_empty(rbtree_t * tree) {
    return tree->root == tree->nil_sentinel;
}

/* Public find */
const void * rbtree_find(rbtree_t * tree, const void * key)
{
    rbtreenode_t * found = find_node(tree, tree->root, key);
    if (found == tree->nil_sentinel) return NULL;
    return found->value;
}

/* Searches the tree for a given key */
static rbtreenode_t * find_node(rbtree_t * tree, rbtreenode_t * x, const void * key)
{
    if (x == tree->nil_sentinel) return x;      // Key not found
    if (tree->comparator(key, x->key) == EQUAL_TO) return x;
    if (tree->comparator(key, x->key) == LESS_THAN) {
        return find_node(tree, x->left, key);
    }
    return find_node(tree, x->right, key);
}

/* Returns minimum child of given node */
static rbtreenode_t * get_minimum(rbtree_t * tree, rbtreenode_t * x)
{
    while (x->left != tree->nil_sentinel) {
        x = x->left;
    }
    return x;
}

/* Returns maximum child of given node */
static rbtreenode_t * get_maximum(rbtree_t * tree, rbtreenode_t * x)
{
    while (x->right != tree->nil_sentinel) {
        x = x->right;
    }
    return x;
}
