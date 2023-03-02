#include <brdkMU_func.h>

enum mu_state_typ
{
	MU_UNVALID,
	MU_ENABLED,
	MU_SLEEPING,
	MU_UPDATING
};

typedef struct brdk_mu_pointers_typ
{	enum mu_state_typ state;
	unsigned char type;	/* 0 = em, 1 = cm */
	unsigned long ptr;
} brdk_mu_pointers_typ;

static brdk_mu_pointers_typ muList[BRDK_MU_MAX_MODULES];	
unsigned char emListReady = false;

/* addes a module mu list */
signed long addToMUList(unsigned long this, unsigned char type) {
	signed long i;
	for(i=0; i< BRDK_MU_MAX_MODULES; i++) {
		if(muList[i].ptr == this) return i;	/* module already inside list */
		else if(muList[i].ptr == 0) { 				/* find an empty slot in the list */
			muList[i].ptr = this;
			muList[i].state = MU_ENABLED;
			muList[i].type = type;	
			return i;
		}
	}
	return -1;
}

/* returns the index in the em list of the wanted module */
signed long getMUIndex(unsigned long this, brdk_mu_setup_module_typ* module) {
	signed long i;
	brdkEM_typ* em;
	brdkCM_typ* cm;
	for(i=0; i < BRDK_MU_MAX_MODULES; i++) {
		if(muList[i].ptr != this) {
			if(muList[i].ptr != 0){
				if(muList[i].state != MU_UNVALID) {
					switch(muList[i].type) {
						case 0:
							em = (brdkEM_typ*)muList[i].ptr;
							if(!brdkStrCmp((unsigned long)em->setup.name,(unsigned long)module->name) && em->setup.nameIdx == module->index) return i;
							break;	

						case 1:
							cm = (brdkCM_typ*)muList[i].ptr;
							if(!brdkStrCmp((unsigned long)cm->setup.name,(unsigned long)module->name) && cm->setup.nameIdx == module->index) return i;
							break;

					}		
				}
			}
			else return -1; /* no further modules */
		}
	}
	return -1;
}

/* returns the pointer of the em */
brdkEM_typ* getEm(signed long idx) {
	if(idx != -1) {
		/* TODO - make check of unit type */
		if(muList[idx].state != MU_UNVALID && muList[idx].ptr != 0) return (brdkEM_typ*)muList[idx].ptr;
	}
	return NULL;
}

/* returns the pointer of the em */
brdkCM_typ* getCm(signed long idx) {
	if(idx != -1) {
		/* TODO - make check of unit type */
		if(muList[idx].state != MU_UNVALID && muList[idx].ptr != 0) return (brdkCM_typ*)muList[idx].ptr;
	}
	return NULL;
}

signed long sleepMU(signed long idx, unsigned long this) {
	if(idx != -1) {
		muList[idx].state = MU_UNVALID;
		switch(muList[idx].type) {
			case 0:
				TMP_alloc(sizeof(brdkEM_typ),(void*)&muList[idx].ptr);
				brdkStrMemCpy(muList[idx].ptr,this, sizeof(brdkEM_typ));
				break;	

			case 1:
				TMP_alloc(sizeof(brdkCM_typ),(void*)&muList[idx].ptr);
				brdkStrMemCpy(muList[idx].ptr,this, sizeof(brdkCM_typ));
				break;

		}
		muList[idx].state = MU_SLEEPING;
		return idx;
	}
	return -1;
}

signed long wakeMU(signed long idx, unsigned long this) {
	if(idx != -1) {
		if(muList[idx].state == MU_SLEEPING) {
			muList[idx].state = MU_UNVALID;
			switch(muList[idx].type) {
				case 0:
					brdkStrMemCpy(this,muList[idx].ptr, sizeof(brdkEM_typ));	/* UPDATE THIS TO NAME BASE INSTEAD OF MEMORY */	
					TMP_free(sizeof(brdkEM_typ),(void*)muList[idx].ptr);
					break;	
	
				case 1:
					brdkStrMemCpy(this,muList[idx].ptr, sizeof(brdkCM_typ));	/* UPDATE THIS TO NAME BASE INSTEAD OF MEMORY */	
					TMP_free(sizeof(brdkCM_typ),(void*)muList[idx].ptr);
					break;
	
			}
			muList[idx].state = MU_ENABLED;			
		}
		muList[idx].state = MU_UNVALID;
		muList[idx].ptr = this;
		muList[idx].state = MU_ENABLED;
		return idx;
	}
	return -1;
}

/* returns if a module is sleeping or not */
unsigned char moduleSleeping(signed long idx) {
	return muList[idx].state == MU_SLEEPING;
}

/* updates the siblings state and sub state info for the em function block */
void updateSiblingInfo(signed long (*idxArr)[], brdk_mu_status_module_typ* status) {
	int i;
	brdkEM_typ* sibling;
	for(i=0; i<= BRDK_MU_MAX_SIBLINGS; i++) {
		sibling = getEm((*idxArr)[i]);
		if(sibling != NULL) {
			status[i].state = sibling->state;
			status[i].substate = sibling->substate;
			status[i].sleeping = sibling->status.sleeping;
			status[i].mode = sibling->mode;
			status[i].sc = sibling->cmd.sc;
		}
	}
}

/* gets the cycle time of the task the em structure is called inside */
unsigned long getCycleTime() {
	RTInfo_typ RTInfo_0;
	RTInfo_0.enable = 1;
	RTInfo(&RTInfo_0);
	return RTInfo_0.cycle_time;
}

