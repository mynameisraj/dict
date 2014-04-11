#ifndef _RBTREE_H_
#define _RBTREE_H_

typedef int (*comparator_t)(const void *, const void *);

typedef struct rbtreenode {
    const void * key;
    const void * value;
    struct rbtreenode * left;
    struct rbtreenode * right;
    struct rbtreenode * parent;
    int color;
} rbtreenode_t;

typedef struct rbtree_t {
    rbtreenode_t* root;
    rbtreenode_t* nil_sentinel;
    comparator_t comparator;
} rbtree_t;

rbtree_t * rbtree_create(comparator_t comparator);
void rbtree_insert(rbtree_t * tree, const void * key, const void * value);
void rbtree_remove(rbtree_t * tree, const void * key);
const void * rbtree_find(rbtree_t * tree, const void * key);
int rbtree_destroy(rbtree_t * tree);
int rbtree_is_empty(rbtree_t * tree);

#endif
