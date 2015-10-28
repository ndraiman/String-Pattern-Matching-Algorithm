#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "slist.H"

void print_list(slist_t*);



int main() {
    
    slist_t* list = (slist_t*)malloc(sizeof(slist_t));
    
    slist_init(list);
    int x = 5;
    slist_append(list, &x);
    print_list(list);
    slist_destroy(list, SLIST_LEAVE_DATA);
    
    return 0;
}





void print_list(slist_t* list) {
    printf("print_list \n");
    
    slist_node_t* p, q;
    int i = 0;
    
    for(p = slist_head(list); p != NULL; p = slist_next(p)) {
        
        i++;
        printf("Node #%d -> ", i);
    }
    
    printf("|| \n");
    
}