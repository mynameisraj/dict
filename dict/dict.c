//
//  dict.c
//  dict
//
//  Created by Raj Ramamurthy on 4/10/14.
//  Copyright (c) 2014 Raj Ramamurthy. All rights reserved.
//

#include "dict.h"
#include <string.h>

dict_t * dict_create(comparator_t fn) {
    dict_t * d = (dict_t *) malloc(sizeof(dict_t));
    d->tree = rbtree_create(fn);
    return d;
}

void dict_insert(dict_t * dict, const void * key, const void * value)
{
    return rbtree_insert(dict->tree, key, value);
}

void dict_remove(dict_t * dict, const void * key)
{
    return rbtree_remove(dict->tree, key);
}

const void * dict_find(dict_t * dict, const void * key)
{
    return rbtree_find(dict->tree, key);
}

int dict_destroy(dict_t * dict)
{
    rbtree_destroy(dict->tree);
    free(dict);
    return 0;
}

int dict_is_empty(dict_t * dict)
{
    return rbtree_is_empty(dict->tree);
}