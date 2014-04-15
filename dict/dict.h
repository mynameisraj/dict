//
//  dict.h
//  dict
//
//  Created by Raj Ramamurthy on 4/10/14.
//  Copyright (c) 2014 Raj Ramamurthy. All rights reserved.
//

#ifndef dict_dict_h
#define dict_dict_h

#include <stdlib.h>
#include "rbtree.h"

typedef int dict_type_t;

typedef struct dict {
    rbtree_t * tree;
    /* TODO: Add a lock here */
} dict_t;

dict_t * dict_create(comparator_t fn);
void dict_insert(dict_t * dict, const void * key, const void * value);
void dict_remove(dict_t * dict, const void * key);
const void * dict_find(dict_t * dict, const void * key);
int dict_destroy(dict_t * dict);
int dict_is_empty(dict_t * dict);

#endif
