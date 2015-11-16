#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "slist.h"
#include "pattern_matching.h"

void print_list(slist_t*);



int main() {

  printf("\n********************\n**** Main START ****\n********************\n");

    // slist_t* list = (slist_t*)malloc(sizeof(slist_t));
    // slist_t* list2 = (slist_t*)malloc(sizeof(slist_t));
    //
    // slist_init(list);
    // slist_init(list2);
    //
    // int x = 5;
    // int y = 8;
    //
    // printf("\n*** adding to list ***\n");
    // slist_append(list, &x);
    // slist_prepend(list, &y);
    // printf("\n*** adding to list2 ***\n");
    // slist_append(list2, &x);
    // slist_prepend(list2, &y);
    //
    // printf("\n*** Printing list ***\n");
    // print_list(list);
    // printf("\n*** Printing list2 ***\n");
    // print_list(list2);
    //
    // printf("\n*** Pop list ***\n");
    // slist_pop_first(list);
    //
    // printf("\n*** Printing list ***\n");
    // print_list(list);
    //
    // printf("\n*** Appending lists ***\n");
    // slist_append_list(list, list2);
    //
    // printf("\n*** Printing list ***\n");
    // print_list(list);
    // printf("\n*** Printing list2 ***\n");
    // print_list(list2);
    //
    // printf("\n*** Destroying lists ***\n");
    // slist_destroy(list, SLIST_LEAVE_DATA);
    // slist_destroy(list2, SLIST_LEAVE_DATA);
    //
    // free(list);
    // free(list2);

    printf("\n********************\n***** Testing pattern_matching *****\n********************\n");

    pm_t* fsm = (pm_t*)malloc(sizeof(pm_t));

    int returned;
    returned = pm_init(fsm);
    printf("pm_init returned %d\n", returned);

    unsigned char* string;

    printf("*******************************\n");
    string = "a";
    printf("adding string: %s\n", string);
    returned = pm_addstring(fsm, string, 1);
    printf("pm_addstring returned %d\n", returned);

    printf("*******************************\n");

    string = "abc";
    printf("adding string: %s\n", string);
    returned = pm_addstring(fsm, string, 3);
    printf("pm_addstring returned %d\n", returned);

    printf("*******************************\n");

    string = "bca";
    printf("adding string: %s\n", string);
    returned = pm_addstring(fsm, string, 3);
    printf("pm_addstring returned %d\n", returned);

    printf("*******************************\n");

    string = "cab";
    printf("adding string: %s\n", string);
    returned = pm_addstring(fsm, string, 3);
    printf("pm_addstring returned %d\n", returned);

    printf("*******************************\n");

    string = "acb";
    printf("adding string: %s\n", string);
    returned = pm_addstring(fsm, string, 3);
    printf("pm_addstring returned %d\n", returned);

    printf("*******************************\n");
    printf("*******************************\n");
    printf("*******************************\n");

    printf("computing failure functions\n");
    returned = pm_makeFSM(fsm);
    printf("pm_makeFSM returned %d\n", returned);

    // slist_node_t* p = fsm->zerostate->slist_head(_transitions);
    // pm_state_t* stateA = ((pm_labeled_edge_t*)slist_data(p))->state;
    // printf("stateA id = %d, output = %s\n", stateA->id, (char*)stateA->slist_data(slist_head(output)));

    printf("*******************************\n");

    string = "acbcabacbacbacbacbacbacbabacb";
    printf("searching for string \"%s\"\n", string);
    slist_t* matched = pm_fsm_search(fsm->zerostate, string, strlen(string));
    printf("matched list:\n");
    // print_list3(matched);
    printf("destroying matched list...\n");
    slist_destroy(matched, SLIST_FREE_DATA);

    printf("*******************************\n");
    printf("*******************************\n");

    printf("Destroying fsm\n");
    pm_destroy(fsm);
    printf("pm_destroy finished\n");

    printf("\n********************\n***** Main END *****\n********************\n");

    return 0;
}





// void print_list(slist_t* list) {
//     printf("print_list() \n");
//
//     slist_node_t* p;
//     int i = 0;
//
//     for(p = slist_head(list); p != NULL; p = slist_next(p)) {
//
//         i++;
//         printf("[%d] -> ", *(int*)slist_data(p));
//     }
//
//     printf("|| \n");
//
// }
