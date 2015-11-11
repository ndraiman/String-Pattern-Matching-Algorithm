#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "slist.h"
#include "pattern_matching.h"

int init_state(pm_t*, pm_state_t*, pm_int_t);

int pm_init(pm_t *pm) {

	pm->zerostate = (pm_state_t*)malloc(sizeof(pm_state_t));
	if(pm->zerostate == NULL) {
		return -1;
	}

	return init_state(pm, pm->zerostate, 0);
	// memset(pm->zerostate, 0, sizeof(pm_state_t));

	//Init zerostate variables
	// pm->zerostate->id = 0;
	// pm->zerostate->depth = 0;
	// pm->zerostate->fail = pm->zerostate;
	// pm->zerostate->output = (slist_t*)malloc(sizeof(slist_t));
	// pm->zerostate->_transitions = (slist_t*)malloc(sizeof(slist_t));
	// if(pm->zerostate->output == NULL || pm->zerostate->_transitions == NULL) {
	// 	return -1;
	// }
	// memset(pm->zerostate->output, 0, sizeof(slist_t));
	// memset(pm->zerostate->_transitions, 0, sizeof(slist_t));
	// slist_init(pm->zerostate->output);
	// slist_init(pm->zerostate->_transitions);

	// return 0;
}

int init_state(pm_t* pm, pm_state_t* state, pm_int_t depth) {

	memset(state, 0, sizeof(pm_state_t));

	state->depth = depth;
	state->id = pm->newstate;
	state->fail = pm->zerostate;
	//TODO should fail default to zerostate or NULL?

	state->output = (slist_t*)malloc(sizeof(slist_t));
	state->_transitions = (slist_t*)malloc(sizeof(slist_t));
	if(state->output == NULL || state->_transitions == NULL) {
		return -1;
	}
	memset(state->output, 0, sizeof(slist_t));
	memset(state->_transitions, 0, sizeof(slist_t));
	slist_init(state->output);
	slist_init(state->_transitions);

	return 0;
}

/****************************************************/
/****************************************************/

int pm_addstring(pm_t *pm, unsigned char *c, size_t n) {

	int i;
	pm_state_t* currentState = pm->zerostate;
	pm_state_t* nextState;

	for(i=0; i < n; i++) {

		if((nextState = pm_goto_get(currentState, c[i])) != NULL) {
			currentState = nextState;

		} else {

			pm_state_t* newState = (pm_state_t*)malloc(sizeof(pm_state_t));
			if(newState == NULL) {
				return -1;
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

	slist_append(currentState->output, c);

	return 0;
}

/****************************************************/
/****************************************************/

int pm_makeFSM(pm_t *pm) {

	slist_t* queue = (slist_t*)malloc(sizeof(slist_t));
	if(queue == NULL) {
		return -1;
	}

	slist_node_t* p;
	for(p = pm->zerostate->slist_head(_transitions); p != NULL; p = slist_next(p)) {
		pm_state_t* state = ((pm_labeled_edge_t*)slist_data(p))->state;
		slist_append(queue, state);
		state->fail = pm->zerostate;
	}

	while(slist_size(queue) != 0) {

		pm_state_t* r = slist_pop_first(queue);

		


	}


}

/****************************************************/
/****************************************************/

int pm_goto_set(pm_state_t *from_state,
	unsigned char symbol,
	pm_state_t *to_state) {

		printf("%d -> %c -> %d\n", from_state->id, symbol, to_state->id);

		pm_labeled_edge_t* edge = (pm_labeled_edge_t*)malloc(sizeof(pm_labeled_edge_t));
		if(edge == NULL) {
			return -1;
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

		slist_node_t* p;

		for(p = state->slist_head(_transitions); p != NULL; p = slist_next(p)) {

			if(((pm_labeled_edge_t*)slist_data(p))->label == symbol) {
				return ((pm_labeled_edge_t*)slist_data(p))->state;
			}
		}

		return NULL;
	}


/****************************************************/
/****************************************************/


slist_t* pm_fsm_search(pm_state_t *pm,
	unsigned char *c,
	size_t n) {


}

/****************************************************/
/****************************************************/

void pm_destroy(pm_t* pm) {


}
