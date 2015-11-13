#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "slist.h"


/****************************************************/
/****************************************************/

void slist_init(slist_t* list) {
  printf("slist_init() \n");//DEBUG

  memset(list, 0, sizeof(list));
  slist_head(list) = NULL;
  slist_tail(list) = NULL;

  slist_size(list) = 0;

}

/****************************************************/
/****************************************************/

void slist_destroy(slist_t* list,slist_destroy_t dealloc) {
    printf("slist_destroy() \n");//DEBUG

    slist_node_t* p, *q;
    int i;

    for(p = slist_head(list); p != NULL; p = q) {

        q = slist_next(p);

        if(dealloc)
            free(slist_data(p));


        free(p);
    }

}

/****************************************************/
/****************************************************/

void* slist_pop_first(slist_t* list) {
    printf("slist_pop_first() \n");//DEBUG

    if(slist_size(list) == 0) {
        return NULL;

    }


    void* data = slist_data(slist_head(list));

    if(slist_size(list) == 1) {

        free(slist_head(list));
        slist_head(list) = NULL;

    } else {

        slist_node_t* p = slist_head(list);
        slist_head(list) = slist_next(p);
        free(p);

    }

    slist_size(list)--;
    return data;
}

/****************************************************/
/****************************************************/

int slist_append(slist_t* list, void* data) {
    printf("slist_append() \n");//DEBUG

    slist_node_t* node = (slist_node_t*)malloc(sizeof(slist_node_t));
    if(node == NULL) {
      perror("Failed to allocate memory\n");
      exit(-1);
    }
    memset(node, 0, sizeof(node));

    slist_data(node) = data;
    slist_next(node) = NULL;

    slist_size(list)++;

    if(slist_head(list) == NULL) {
        printf("head == NULL \n");//DEBUG

        slist_head(list) = node;
        slist_tail(list) = node;
        return 0;

    }

    printf("adding after tail \n");//DEBUG
    slist_next(slist_tail(list)) = node;
    slist_tail(list) = node;

    return 0;
}

/****************************************************/
/****************************************************/

int slist_prepend(slist_t* list, void* data) {
  printf("slist_prepend() \n");//DEBUG

  slist_node_t* node = (slist_node_t*)malloc(sizeof(slist_node_t));
  if(node == NULL) {
    perror("Failed to allocate memory\n");
    exit(-1);
  }

  memset(node, 0, sizeof(node));

  slist_data(node) = data;
  slist_next(node) = NULL;

  slist_size(list)++;


  if(slist_head(list) == NULL) {

    printf("head == NULL \n");//DEBUG

    slist_head(list) = node;
    slist_tail(list) = node;

    return 0;

  }

  printf("adding before head \n");//DEBUG
  slist_next(node) = slist_head(list);
  slist_head(list) = node;

  return 0;
}

/****************************************************/
/****************************************************/


int slist_append_list(slist_t* to, slist_t* from) {
  printf("slist_append_list\n");//DEBUG
  slist_node_t* p;

  for(p = slist_head(from); p != NULL; p = slist_next(p)) {

    slist_append(to, slist_data(p));
  }

  return 0;
}

/****************************************************/
/****************************************************/
