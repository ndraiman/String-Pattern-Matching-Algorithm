#include "pattern_matching.h"



int pm_init(pm_t *pm) {

}

int pm_addstring(pm_t *pm, unsigned char *c, size_t n) {

}


int pm_makeFSM(pm_t *pm) {

}

int pm_goto_set(pm_state_t *from_state,
	unsigned char symbol,
	pm_state_t *to_state) {


	}


void pm_state_t* pm_goto_get(pm_state_t *state,
  unsigned char symbol) {


	}


slist_t* pm_fsm_search(pm_state_t *pm,
	unsigned char *c,
	size_t n) {


}


void pm_destroy(pm_t *) {


}
