#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "slist.H"


void slist_init(slist_t* list) {
  printf("slist_init() \n");
  
  memset(list, 0, sizeof(list));
  slist_head(list) = NULL;
  slist_tail(list) = slist_head(list);
  slist_size(list) = 0;
  
}


void slist_destroy(slist_t* list,slist_destroy_t freeData) {
    printf("slist_destroy() \n");
    
    slist_node_t* p;
    int i;
    
    for(p = slist_head(list); p != NULL; p = slist_next(p)) {
        if(freeData)
            free(slist_data(p));
            
            
    }
    
}


int slist_append(slist_t* list, void* newData) {
    printf("slist_append() \n");
    
    if(slist_head(list) == NULL) {
        slist_data(slist_head(list)) = newData;
    }
    
}

