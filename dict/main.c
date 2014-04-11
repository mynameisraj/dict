//
//  main.c
//  dict
//
//  Created by Raj Ramamurthy on 4/10/14.
//  Copyright (c) 2014 Raj Ramamurthy. All rights reserved.
//

/* TODO: add memory tests. Probably leaking somewhere */

#include <stdio.h>
#include "dict.h"
#include <assert.h>
#include <string.h>

static int * keys_int[10];
static int * values_int[10];
static int index_int = 0;

static char * keys_str[10];
static char * values_str[10];
static int index_str = 0;

static void test_int(void);
static void test_str(void);

static void create_test_value_int(int index)
{
    int *i = malloc(sizeof(int));
    int *j = malloc(sizeof(int));
    *i = index;
    *j = index*5;
    keys_int[index_int] = i;
    values_int[index_int] = j;
    index_int++;
}

static void create_test_value_str(const char *index)
{
    char *i = malloc(sizeof(int));
    char *j = malloc(sizeof(int));
    strcpy(i, index);
    strcpy(j, index);
    keys_str[index_str] = i;
    values_str[index_str] = j;
    index_str++;
}

int main(int argc, const char * argv[])
{
    printf("Running tests\n");
    test_int();
    test_str();

    return 0;
}

static void test_int(void)
{
    int i;
    printf("\ninteger tests\n------------------\n");
    dict_t *dict = dict_create(DICT_INT);
    assert(dict_is_empty(dict));
    printf("dict_is_empty: PASS\n");

    /* Test insertion */
    create_test_value_int(220);
    create_test_value_int(120);
    create_test_value_int(20);
    create_test_value_int(3);
    create_test_value_int(5);
    create_test_value_int(7);
    create_test_value_int(9);
    create_test_value_int(10);
    create_test_value_int(40);
    create_test_value_int(90);
    for (i = 0; i < index_int; i++) {
        dict_insert(dict, keys_int[i], values_int[i]);
    }
    assert(!dict_is_empty(dict));
    printf("insert: PASS\n");

    /* Test retrieval */
    for (i = 0; i < index_int; i++) {
        int * x = (int *) dict_find(dict, keys_int[i]);
        assert(*x == *values_int[i]);
        printf("find %d: PASS (%d)\n", i, *x);
    }

    /* Test remove */
    dict_remove(dict, keys_int[0]);
    assert(dict_find(dict, keys_int[0]) == NULL);
    printf("remove: PASS\n");

    /* Cleanup */
    for (i = 0; i < index_int; i++) {
        free(keys_int[i]);
        free(values_int[i]);
    }
}

static void test_str(void)
{
    int i;
    printf("\nstring tests\n------------------\n");
    dict_t *dict = dict_create(DICT_INT);
    assert(dict_is_empty(dict));
    printf("dict_is_empty: PASS\n");

    /* Test insertion */
    create_test_value_str("test");
    create_test_value_str("raj");
    for (i = 0; i < index_str; i++) {
        dict_insert(dict, keys_str[i], values_str[i]);
    }
    assert(!dict_is_empty(dict));
    printf("insert: PASS\n");

    /* Test retrieval */
    for (i = 0; i < index_str; i++) {
        const char * x = (char *) dict_find(dict, keys_str[i]);
        assert(strcmp(x, (const char *)values_str[i]) == 0);
        printf("find %d: PASS (%s)\n", i, x);
    }

    /* Test remove */
    dict_remove(dict, keys_str[0]);
    assert(dict_find(dict, keys_str[0]) == NULL);
    printf("remove: PASS\n");

    /* Cleanup */
    for (i = 0; i < index_str; i++) {
        free(keys_str[i]);
        free(values_str[i]);
    }
}