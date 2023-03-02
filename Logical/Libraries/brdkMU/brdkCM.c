#include <brdkCM_func.h>

void brdkCM(struct brdkCM* this) {
	int i;
	brdkEM_typ* parent = getEm(this->internal.muList.parentIdx);
	brdk_mu_setup_module_typ module;

	/* initialize control module */
	switch(this->internal.initSubstate) {

		case 0:	/* this substate is only called in void _INIT / PROGRAM _INIT */
			if(this->setup.status.state) brdkStrCpy((unsigned long)this->status.state,(unsigned long)"initializing - looking for module pointers");
			if(this->setup.name[0] == 0) ST_name(0,(char*)&this->setup.name,(unsigned char)0); 														/* set module name to task name if not set*/
			brdkStrCpy((unsigned long)this->status.name,(unsigned long)this->setup.name);
			if(this->setup.nameIdx > -1) brdkStrAppendUdintToA(this->setup.nameIdx,(unsigned long)&this->status.name, BRDK_STR_CONVERT_DECIMAL,-1);	/* add index to task name */
			this->internal.muList.parentIdx = -1;
			for(i=0; i<= BRDK_MU_MAX_SIBLINGS; i++) this->internal.muList.siblingIdx[i] = -1;
			this->internal.initSubstate = 10;
			break;

		case 10:
			if(!emListReady) {
				if(this->setup.status.state) brdkStrCpy((unsigned long)this->status.state,(unsigned long)"waiting for mu list to be ready");
			}
			else {
				brdkStrCpy((unsigned long)module.name,(unsigned long)this->setup.name);
				module.index = this->setup.nameIdx;
				this->internal.muList.idx = getMUIndex((unsigned long)this, &module);
				if(this->internal.muList.idx != -1) {
					wakeMU(this->internal.muList.idx,(unsigned long)this);
					if(this->setup.status.last) {
						brdkStrMemMove((unsigned long)this->status.last[1],(unsigned long)this->status.last[0], sizeof(this->status.last[0])*BRDK_MU_MAX_LAST_SIZE);
						brdkStrCpy((unsigned long)this->status.last[0],(unsigned long)"module awake");
					}
					this->internal.initSubstate = 65001;
				}
				else {
					if(SemAcquire(SemCreateMUList.ident) != ERR_OK) {
						if(this->setup.status.state) brdkStrCpy((unsigned long)this->status.state,(unsigned long)"waiting global semaphore to be free");
					}
					else {
						this->internal.muList.idx = addToMUList((unsigned long)this,0);
						SemRelease(SemCreateMUList.ident);
						if(this->setup.status.state) brdkStrCpy((unsigned long)this->status.state,(unsigned long)"module added em list");
						if(this->internal.muList.idx != -1) this->internal.initSubstate = !this->setup.parent.name[0] ? 40 : 30;	/* check if the module has a parent name assigned */	
						else {
							if(this->setup.status.state) brdkStrCpy((unsigned long)this->status.state,(unsigned long)"em list full. Expand BRDK_EM_MAX_EMS.");
							this->internal.initSubstate = 200;
						}
					}
				}
			}
	
		case 30:	/* find parent */
			this->internal.muList.parentIdx = getMUIndex((unsigned long)this, &this->setup.parent);
			if(this->internal.muList.parentIdx == -1) {
				if(this->setup.status.state) brdkStrCpy((unsigned long)this->status.state,(unsigned long)"parent not found");
			}
			else this->internal.initSubstate = 40;
			break;


		case 40:	/* find siblings */
			for(i=0; i<= BRDK_MU_MAX_SIBLINGS; i++) {
				if(this->setup.sibling[i].name[0] != 0) {
					this->internal.muList.siblingIdx[i] = getMUIndex((unsigned long)this, &this->setup.sibling[i]);
					if(this->internal.muList.siblingIdx[i] == -1) {
						if(this->setup.status.state) {
							brdkStrCpy((unsigned long)this->status.state,(unsigned long)"sibling: ");
							brdkStrCat((unsigned long)this->status.state,(unsigned long)this->setup.sibling[i].name);
							brdkStrCat((unsigned long)this->status.state,(unsigned long)" not found");
						}
						break;
					}
				}
			}
			if(i>=BRDK_MU_MAX_SIBLINGS) {
				this->status.state[0] = 0;	/* reset status text */
				this->internal.initSubstate = 65000;
			}
			break;

		case 65000:	/* module ready */
			/* check if status is not set */
			if(this->status.state[0] == 0 && this->setup.status.state) {
				brdkStrUdintToA(this->state,(unsigned long)this->status.state,BRDK_STR_CONVERT_DECIMAL);
				if(this->substate != 0) {
					brdkStrCat((unsigned long)this->status.state,(unsigned long)": ");
					brdkStrAppendUdintToA(this->substate,(unsigned long)this->status.state,BRDK_STR_CONVERT_DECIMAL,-1);
				}
				if(this->description[0] != 0) {
					brdkStrCat((unsigned long)this->status.state,(unsigned long)": ");
					brdkStrCat((unsigned long)this->status.state,(unsigned long)this->description);
				}
			}
		
			/* state change */
			if(this->state != this->internal.old.state || this->substate != this->internal.old.substate) {	
				/* save last state for log */
				if(this->setup.status.last) {
					brdkStrMemMove((unsigned long)this->status.last[1],(unsigned long)this->status.last[0], sizeof(this->status.last[0])*BRDK_MU_MAX_LAST_SIZE);
					brdkStrUdintToA(this->internal.old.state,(unsigned long)this->status.last[0],BRDK_STR_CONVERT_DECIMAL);
					brdkStrCat((unsigned long)this->status.last[0],(unsigned long)": ");
					if(this->internal.old.substate || this->substate != 0) {	/* if the substate != 0 then we also want to log the oldsubstate even if it's 0 */
						brdkStrAppendUdintToA(this->internal.old.substate,(unsigned long)this->status.last[0],BRDK_STR_CONVERT_DECIMAL,-1);	
						brdkStrCat((unsigned long)this->status.last[0], (unsigned long)": ");
					}
					brdkStrAppendUdintToA(this->status.time, (unsigned long)this->status.last[0],BRDK_STR_CONVERT_DECIMAL,-1);
					brdkStrCat((unsigned long)this->status.last[0], (unsigned long)"ms");
				}
				/* reset */
				if(this->state != this->internal.old.state) this->substate = 0;	/* if the state has been changed the substate will be reset */
				this->status.state[0] = this->description[0] = 0;
				this->internal.old.state = this->state;
				this->internal.old.substate = this->substate;
				this->status.time = 0;
			}
			else this->status.time += this->status.cycleTime/1000;
	
			/* update parent info */
			if(parent != NULL) {
				this->status.parent.state = parent->state;
				this->status.parent.substate = parent->substate;
				this->status.parent.sleeping = parent->status.sleeping;
				this->status.parent.sc = parent->cmd.sc;
			}
	
			/* update siblings info */
			updateSiblingInfo(&this->internal.muList.siblingIdx, &this->status.sibling[0]);
	
			this->status.level = parent != NULL ? parent->status.level+1 : 0;	/* sets the hierarchy level */	
			
			break;

	}
	
	if(!this->status.cycleTime) this->status.cycleTime = getCycleTime();	/* get the modules cycle time */
	this->status.ready = this->internal.initSubstate == 65000;
	
}
