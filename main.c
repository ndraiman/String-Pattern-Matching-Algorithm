#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "slist.H"

void print_list(slist_t*);



int main() {
  
  printf("\n********************\n**** Main START ****\n********************\n");
    
    slist_t* list = (slist_t*)malloc(sizeof(slist_t));
    slist_t* list2 = (slist_t*)malloc(sizeof(slist_t));
    
    slist_init(list);
    slist_init(list2);
    
    int x = 5;
    int y = 8;
    
    printf("\n*** adding to list ***\n");
    slist_append(list, &x);
    slist_prepend(list, &y);
    printf("\n*** adding to list2 ***\n");
    slist_append(list2, &x);
    slist_prepend(list2, &y);
    
    printf("\n*** Printing list ***\n");
    print_list(list);
    printf("\n*** Printing list2 ***\n");
    print_list(list2);
    
    printf("\n*** Pop list ***\n");
    slist_pop_first(list);
    
    printf("\n*** Printing list ***\n");
    print_list(list);
    
    printf("\n*** Appending lists ***\n");
    slist_append_list(list, list2);
    
    printf("\n*** Printing list ***\n");
    print_list(list);
    printf("\n*** Printing list2 ***\n");
    print_list(list2);
    
    printf("\n*** Destroying lists ***\n");
    slist_destroy(list, SLIST_LEAVE_DATA);
    slist_destroy(list2, SLIST_LEAVE_DATA);
    
    printf("\n********************\n***** Main END *****\n********************\n");
    
    return 0;
}





void print_list(slist_t* list) {
    printf("print_list() \n");
    
    slist_node_t* p, q;
    int i = 0;
    
    for(p = slist_head(list); p != NULL; p = slist_next(p)) {
        
        i++;
        printf("[%d] -> ", *(int*)slist_data(p));
    }
    
    printf("|| \n");
    
}