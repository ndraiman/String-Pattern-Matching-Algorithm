#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pattern_matching.h"

//Private Methods Declarations
int init_state(pm_t*, pm_state_t*, pm_int_t);
void pm_destroy_state(pm_state_t*); //Destory States by recursion
int contains(slist_t*, char*); //check if list contains string

/****************************************************/
/****************************************************/


int pm_init(pm_t *pm) {

	if(pm == NULL) {
		return -1;
	}

	memset(pm, 0, sizeof(pm_t));

	pm->zerostate = (pm_state_t*)malloc(sizeof(pm_state_t));
	if(pm->zerostate == NULL) {
		perror("Failed to allocate memory\n");
		exit(-1);
	}

	int result = init_state(pm, pm->zerostate, 0);
	pm->zerostate->fail = pm->zerostate;

	return result;
}

//Private Method
int init_state(pm_t* pm, pm_state_t* state, pm_int_t depth) {

	if(pm == NULL || state == NULL) {
		return -1;
	}

	memset(state, 0, sizeof(pm_state_t));

	state->depth = depth;
	state->id = pm->newstate;
	state->fail = NULL;

	state->output = (slist_t*)malloc(sizeof(slist_t));
	state->_transitions = (slist_t*)malloc(sizeof(slist_t));
	if(state->output == NULL || state->_transitions == NULL) {
		perror("Failed to allocate memory\n");
		exit(-1);
	}

	slist_init(state->output);
	slist_init(state->_transitions);

	pm->newstate++;

	return 0;
}

/****************************************************/
/****************************************************/

int pm_addstring(pm_t *pm, unsigned char *c, size_t n) {

	if(pm == NULL || c == NULL) {
		return -1;
	}

	int i;
	pm_state_t* currentState = pm->zerostate;
	pm_state_t* nextState;

	for(i=0; i < n; i++) {

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

	if(contains(currentState->output, c) == 0) {

		slist_append(currentState->output, c);

	} 

	return 0;
}


//Private Method
int contains(slist_t* list, char* string) {

	if(list == NULL) {
		return -1;
	}

	slist_node_t* p;
	for(p = slist_head(list); p != NULL; p = slist_next(p)) {

		if(slist_data(p) == string) {
			return -1;
		}

	}

	return 0;
}

/****************************************************/
/****************************************************/

int pm_makeFSM(pm_t *pm) {

	if(pm == NULL) {
		return -1;
	}

	slist_t* queue = (slist_t*)malloc(sizeof(slist_t));
	if(queue == NULL) {
		perror("Failed to allocate memory\n");
		exit(-1);
	}
	slist_init(queue);

	slist_node_t* p;
	for(p = pm->zerostate->slist_head(_transitions); p != NULL; p = slist_next(p)) {

		pm_state_t* state = ((pm_labeled_edge_t*)slist_data(p))->state;
		if(slist_append(queue, state) == -1) {
			return -1;
		}
		state->fail = pm->zerostate;

	}



	while(slist_size(queue) != 0) {


		pm_state_t* r = slist_pop_first(queue);

		for(p = r->slist_head(_transitions); p != NULL; p = slist_next(p)) {

			pm_labeled_edge_t* edge = ((pm_labeled_edge_t*)slist_data(p));
			pm_state_t* current = edge->state;

			if(slist_append(queue, current) == -1) {
				return -1;
			}
			pm_state_t* state = r->fail;

			pm_state_t* failState;

			while((failState = pm_goto_get(state, edge->label)) == NULL) {

				state = state->fail;

			}

			current->fail = failState;
			printf("Settings f(%d) = %d\n", current->id, current->fail->id);
			if(slist_append_list(current->output, current->fail->output) == -1) {
				return -1;
			}

		}
	}

	free(queue);
	return 0;
}

/****************************************************/
/****************************************************/

int pm_goto_set(pm_state_t *from_state,
	unsigned char symbol,
	pm_state_t *to_state) {

		if(from_state == NULL || to_state == NULL) {
			return -1;
		}

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

		if(state == NULL) {
			return NULL;
		}

		slist_node_t* p;

		for(p = state->slist_head(_transitions); p != NULL; p = slist_next(p)) {

			pm_labeled_edge_t* edge = ((pm_labeled_edge_t*)slist_data(p));

			if(edge->label == symbol) {
				return edge->state;
			}
		}

		return NULL;
	}


/****************************************************/
/****************************************************/


slist_t* pm_fsm_search(pm_state_t *state,
	unsigned char *c,
	size_t n) {

		slist_t* matched_list = (slist_t*)malloc(sizeof(slist_t));
		if(matched_list == NULL) {
			perror("Failed to allocate memory\n");
			exit(-1);
		}
		slist_init(matched_list);

		if(state == NULL || c == NULL) {
			return matched_list; //returning empty list
		}

		int j;

		pm_state_t* lastState;
		pm_state_t* matched_state;


		for(j = 0; j < n; j++) {

			while((matched_state = pm_goto_get(state, c[j])) == NULL) {

				lastState = state;
				state = state->fail;

				//if its the same state, increase j++, to avoid infinite loop.
				if(state->id == lastState->id) {
					j++;

					if(j == n) {
						return matched_list;
					}
				}
			}


			state = matched_state;

			if(state->slist_size(output) > 0) {

				slist_node_t* p;
				for(p = state->slist_head(output); p != NULL; p = slist_next(p)) {

					pm_match_t* match = (pm_match_t*)malloc(sizeof(pm_match_t));
					if(match == NULL) {
						perror("Failed to allocate memory\n");
						exit(-1);
					}
					memset(match, 0, sizeof(pm_match_t));

					char* pattern = (char*)slist_data(p);

					match->pattern = pattern;
					match->start_pos = (int)(j - (strlen(pattern) - 1));
					match->end_pos = j;
					match->fstate = state;

					printf("Pattern: %s, start at: %d, ends at: %d, last state = %d\n",
						match->pattern, match->start_pos, match->end_pos, match->fstate->id);

					slist_append(matched_list, match);

				}
			}
		}

		return matched_list;
}

/****************************************************/
/****************************************************/

void pm_destroy(pm_t* pm) {

	if(pm == NULL) {
		return;
	}

	pm_destroy_state(pm->zerostate);
	free(pm);

}

//Private Method
void pm_destroy_state(pm_state_t* state) {

	slist_node_t* p = state->slist_head(_transitions);

	while(p != NULL) {

		pm_state_t* nextState = ((pm_labeled_edge_t*)slist_data(p))->state;

		pm_destroy_state(nextState);
		p = slist_next(p);

	}

	slist_destroy(state->output, SLIST_LEAVE_DATA);
	slist_destroy(state->_transitions, SLIST_FREE_DATA);
	free(state);
}

/****************************************************/
/****************************************************/
