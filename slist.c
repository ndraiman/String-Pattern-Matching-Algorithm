#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "slist.H"


void slist_init(slist_t* list) {
  
  memset(list, 0, sizeof(list));
  slist_head(list) = (slist_node_t*)malloc(sizeof(slist_node_t));
  slist_tail(list) = slist_head(list);
  slist_size(list) = 0;
  
}


void slist_destroy(slist_t* list,slist_destroy_t freeData) {
    
    slist_node_t* p;
    int i;
    
    if(freeData) {
        
        for(p = slist_head(list); p != NULL; p = slist_next(p)) {
            free(slist_data(p));
        }
        
    }
    
}


void print(slist_t* list) {
    
    slist_node_t* p;
    
    for(p = slist_head(list); p != NULL; p = slist_next(p)) {
        
        printf("%s -> ", (char*)slist_data(p));
    }
    
    printf("|| \n");
    
}