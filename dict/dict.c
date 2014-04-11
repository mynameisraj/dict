//
//  dict.c
//  dict
//
//  Created by Raj Ramamurthy on 4/10/14.
//  Copyright (c) 2014 Raj Ramamurthy. All rights reserved.
//

#include "dict.h"
#include <string.h>

int compare_int(const void * a, const void * b);
int compare_str(const void * a, const void * b);

dict_t * dict_create(dict_type_t type) {
    dict_t * d = (dict_t *) malloc(sizeof(dict_t));
    comparator_t fn = compare_int;
    switch (type) {
        case DICT_STR:
            fn = compare_int;
            break;
        default:
            break;
    }
    d->tree = rbtree_create(fn);
    d->type = type;
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

/* Various comparators */

int compare_int(const void * a, const void * b) {
    int * a_int = (int *) a;
    int * b_int = (int *) b;
    return (*a_int == *b_int) ? 0 : (*a_int < *b_int ? -1 : 1);
}

int compare_str(const void * a, const void * b) {
    return strcmp((const char *)a, (const char *)b);
}