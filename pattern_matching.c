#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pattern_matching.h"
// #include "slist.h"

void print_list2(slist_t*);//DEBUG
void print_list3(slist_t*);//DEBUG
int init_state(pm_t*, pm_state_t*, pm_int_t);

int pm_init(pm_t *pm) {
	printf("pm_init\n");//DEBUG

	pm->zerostate = (pm_state_t*)malloc(sizeof(pm_state_t));
	if(pm->zerostate == NULL) {
		perror("Failed to allocate memory\n");
		exit(-1);
	}

	// return init_state(pm, pm->zerostate, 0);
	int result = init_state(pm, pm->zerostate, 0);
	pm->zerostate->fail = pm->zerostate;

	return result;
}

int init_state(pm_t* pm, pm_state_t* state, pm_int_t depth) {
	printf("init_state\n");//DEBUG
	memset(state, 0, sizeof(pm_state_t));

	state->depth = depth;
	state->id = pm->newstate;
	state->fail = NULL;
	//TODO should fail default to zerostate or NULL?

	state->output = (slist_t*)malloc(sizeof(slist_t));
	state->_transitions = (slist_t*)malloc(sizeof(slist_t));
	if(state->output == NULL || state->_transitions == NULL) {
		perror("Failed to allocate memory\n");
		exit(-1);
	}
	memset(state->output, 0, sizeof(slist_t));
	memset(state->_transitions, 0, sizeof(slist_t));
	slist_init(state->output);
	slist_init(state->_transitions);

	pm->newstate++;

	return 0;
}

/****************************************************/
/****************************************************/

int pm_addstring(pm_t *pm, unsigned char *c, size_t n) {
	printf("pm_addstring\n");//DEBUG
	int i;
	pm_state_t* currentState = pm->zerostate;
	pm_state_t* nextState;

	for(i=0; i < n; i++) {
		printf("pm_addstring for-loop, c[i] = %c\n", c[i]);//DEBUG
		if((nextState = pm_goto_get(currentState, c[i])) != NULL) {
			currentState = nextState;

		} else {

			pm_state_t* newState = (pm_state_t*)malloc(sizeof(pm_state_t));
			if(newState == NULL) {
				perror("Failed to allocate memory\n");
				exit(-1);
			}

			printf("Allocating state %d\n", pm->newstate);
			if(init_state(pm, newState, (currentState->depth) + 1) == -1) {
				return -1;
			}

			if(pm_goto_set(currentState, c[i], newState) == -1) {
				return -1;
			}

			currentState = newState;
		}
	}
	printf("appending output\n");//DEBUG
	slist_append(currentState->output, c);
	printf("checking output\n");
	print_list2(currentState->output);
	return 0;
}

/****************************************************/
/****************************************************/

int pm_makeFSM(pm_t *pm) {
	printf("pm_makeFSM\n");//DEBUG
	slist_t* queue = (slist_t*)malloc(sizeof(slist_t));
	if(queue == NULL) {
		perror("Failed to allocate memory\n");
		exit(-1);
	}
	printf("setting zerostate's transition failures\n");//DEBUG
	slist_node_t* p;
	for(p = pm->zerostate->slist_head(_transitions); p != NULL; p = slist_next(p)) {

		pm_state_t* state = ((pm_labeled_edge_t*)slist_data(p))->state;
		slist_append(queue, state);
		state->fail = pm->zerostate;
		printf("state id = %d\n", state->id);//DEBUG
	}

	printf("Working on queue\n");//DEBUG

	while(slist_size(queue) != 0) {
		printf("***********************\n");//DEBUG
		pm_state_t* r = slist_pop_first(queue);
		printf("!!! r popped: id=%d\n", r->id);//DEBUG
		for(p = r->slist_head(_transitions); p != NULL; p = slist_next(p)) {

			pm_labeled_edge_t* edge = ((pm_labeled_edge_t*)slist_data(p));
			pm_state_t* current = edge->state;
			printf("!!! current: id=%d\n", current->id); //DEBUG
			slist_append(queue, current);
			pm_state_t* state = r->fail;
			printf("state = r->fail, state id = %d\n", state->id); //DEBUG

			pm_state_t* failState;

			while((failState = pm_goto_get(state, edge->label)) == NULL) {

				state = state->fail;

			}

			current->fail = failState;
			printf("Settings f(%d) = %d\n", current->id, current->fail->id);
			slist_append_list(current->output, current->fail->output);

			printf("printing output\n");//DEBUG
			print_list2(current->output);
		}
	}
}

/****************************************************/
/****************************************************/

int pm_goto_set(pm_state_t *from_state,
	unsigned char symbol,
	pm_state_t *to_state) {
		printf("pm_goto_set\n");//DEBUG
		printf("%d -> %c -> %d\n", from_state->id, symbol, to_state->id);

		pm_labeled_edge_t* edge = (pm_labeled_edge_t*)malloc(sizeof(pm_labeled_edge_t));
		if(edge == NULL) {
			perror("Failed to allocate memory\n");
			exit(-1);
		}
		memset(edge, 0, sizeof(pm_labeled_edge_t));

		edge->label = symbol;
		edge->state = to_state;

		return slist_append(from_state->_transitions, edge);
	}

/****************************************************/
/****************************************************/

pm_state_t* pm_goto_get(pm_state_t *state,
  unsigned char symbol) {
		printf("pm_goto_get\n");//DEBUG
		if(state == NULL) {
			printf("state is NULL, returning NULL\n");//DEBUG
			return NULL;
		}
		printf("state id = %d, symbol = %c\n", state->id, symbol);//DEBUG
		slist_node_t* p;

		for(p = state->slist_head(_transitions); p != NULL; p = slist_next(p)) {

			pm_labeled_edge_t* edge = ((pm_labeled_edge_t*)slist_data(p));
			printf("label = %c, symbol = %c\n", edge->label, symbol);//DEBUG
			if(edge->label == symbol) {
				printf("returning state %d\n", edge->state->id);//DEBUG
				return edge->state;
			}
		}

		printf("returning NULL\n");//DEBUG
		return NULL;
	}


/****************************************************/
/****************************************************/


slist_t* pm_fsm_search(pm_t *pm,
	unsigned char *c,
	size_t n) {
		printf("pm_fsm_search\n");//DEBUG

		slist_t* matched_list = (slist_t*)malloc(sizeof(slist_t));
		if(matched_list == NULL) {
			perror("Failed to allocate memory\n");
			exit(-1);
		}
		slist_init(matched_list);

		int j;
		pm_state_t* state;
		pm_state_t* matched_state;

		// printf("before loop\n");//DEBUG
		// int x;//DEBUG
		// scanf("%d", &x);//DEBUG
		state = pm->zerostate;

		for(j = 0; j < n; j++) {

			// printf("in loop\n");//DEBUG
			// int x;//DEBUG
			// scanf("%d", &x);//DEBUG

			// state = pm->zerostate; //Should i get pm_state_t or pm_t?

			while((matched_state = pm_goto_get(state, c[j])) == NULL) {
				// slist_node_t* p = state->slist_head(_transitions);//DEBUG
				// char c = ((pm_labeled_edge_t*)slist_data(p))->label;//DEBUG
				printf("state id = %d, c[%d] = %c\n*******\n", state->id, j, c[j]);//DEBUG
				state = state->fail;

				if(state->id == 0) {
					j++;
					printf("Its zerostate, increasting J to %d\n", j);//DEBUG
					if(j == n) {
						printf("I reached the end of the string, returning...\n");//DEBUG
						return matched_list;
					}

				}
			}

			state = matched_state; //pm_goto_get(state, c[j]);
			// if(state == NULL) {
			// 	continue;
			// }

			if(state->slist_size(output) > 0) {
				printf("!!!!! adding output to matched list \n");//DEBUG
				slist_node_t* p;
				for(p = state->slist_head(output); p != NULL; p = slist_next(p)) {

					pm_match_t* match = (pm_match_t*)malloc(sizeof(pm_match_t));
					if(match == NULL) {
						perror("Failed to allocate memory\n");
						exit(-1);
					}
					memset(match, 0, sizeof(match));

					char* pattern = (char*)slist_data(p);

					match->pattern = pattern;
					match->start_pos = (int)(j - (strlen(pattern) - 1));
					match->end_pos = j;
					match->fstate = state;

					printf("Pattern: %s, start at: %d, ends at: %d, last state = %d\n",
						match->pattern, match->start_pos, match->end_pos, match->fstate->id);

					slist_append(matched_list, match);

				}
				// slist_append_list(matched_list, state->output);

			} else {
				printf("!!! NOT adding output!!!!\n");//DEBUG
			}

		}

		return matched_list;
}

/****************************************************/
/****************************************************/

void pm_destroy(pm_t* pm) {
	printf("pm_destroy\n");//DEBUG


}


/****************************************************/
/****************************************************/

//DEBUG:
void print_list2(slist_t* list) {
    printf("print_list() \n");

    slist_node_t* p, q;
    int i = 0;

    for(p = slist_head(list); p != NULL; p = slist_next(p)) {

        i++;
        printf("[%s] -> ", (char*)slist_data(p));
    }

    printf("|| \n");

}

void print_list3(slist_t* list) {
    printf("print_list() \n");

    slist_node_t* p, q;
    int i = 0;

    for(p = slist_head(list); p != NULL; p = slist_next(p)) {
				pm_match_t* match = (pm_match_t*)slist_data(p);
        i++;
        printf("[%s] -> ", match->pattern);
    }

    printf("|| \n");

}
