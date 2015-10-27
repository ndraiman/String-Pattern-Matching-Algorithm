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
    
    return 0;
}





void print_list(slist_t* list) {
    printf("print_list \n");
    
    slist_node_t* p;
    
    for(p = slist_head(list); p != NULL; p = slist_next(p)) {
        
        
        printf("%s -> ", (char*)slist_data(p));
    }
    
    printf("|| \n");
    
}