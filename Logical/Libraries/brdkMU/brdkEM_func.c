#include <brdkEM_func.h>

static unsigned char semCreator = false;
SemCreate_typ SemCreateMUList;
char tmpStr[BRDK_MU_MAX_DESCRIPTION_STRING];

/* initialize the equipment module */
void brdkEMInitialize(brdkEM_typ* this, brdkEM_typ* parent) {
	int i;
	brdk_mu_setup_module_typ module;

	switch(this->internal.initSubstate) {
		
		case 0:
			if(this->setup.name[0] == 0) ST_name(0,(char*)&this->setup.name,(unsigned char)0); 														/* set module name to task name if not set*/
			brdkStrCpy((unsigned long)this->status.name,(unsigned long)this->setup.name);
			if(this->setup.status.stateSubstateMode) {
				brdkStrCpy((unsigned long)this->status.state,(unsigned long)brdkEMStateToA(this->state));
				brdkStrCpy((unsigned long)this->status.mode,brdkEMModeToA(this->mode, this));
			}
			if(this->setup.nameIdx > -1) brdkStrAppendUdintToA(this->setup.nameIdx,(unsigned long)&this->status.name, BRDK_STR_CONVERT_DECIMAL,-1);	/* add index to task name */
			this->internal.muList.parentIdx = -1;
			for(i=0; i<= BRDK_EM_MAX_CHILDREN; i++) this->internal.muList.childrenIdx[i] = -1;
			for(i=0; i<= BRDK_MU_MAX_SIBLINGS; i++) this->internal.muList.siblingIdx[i] = -1;
			if(!semCreator) {
				setSubStateText(this,(unsigned long)"creating global semaphore");
				semCreator = true;
				this->internal.initSubstate = 10;
			}
			else this->internal.initSubstate = 20;
			break;

		case 10:
			SemCreateMUList.enable = true;
			SemCreateMUList.initCount = 0;
			SemCreateMUList.maxCount = 1;
			SemCreate(&SemCreateMUList);
			if(SemCreateMUList.status != ERR_OK) setSubStateText(this,(unsigned long)"creating global semaphore");
			else {
				SemRelease(SemCreateMUList.ident);
				this->internal.muList.idx = addToMUList((unsigned long)this,0);
				if(this->internal.muList.idx != -1) {
					emListReady = true;
					this->internal.initSubstate = !this->setup.parent.name[0] ? 40 : 30;	/* check if the module has a parent name assigned */
				}
				else {
					setSubStateText(this,(unsigned long)"em list full. Expand BRDK_EM_MAX_EMS.");
					this->internal.initSubstate = 200;
				}
			}
			break;

		case 20:	/* add module to em list */
			if(!emListReady) setSubStateText(this,(unsigned long)"waiting for em list to be ready");
			else {
				brdkStrCpy((unsigned long)module.name,(unsigned long)this->setup.name);
				module.index = this->setup.nameIdx;
				this->internal.muList.idx = getMUIndex((unsigned long)this, &module);
				if(this->internal.muList.idx != -1) {
					if(moduleSleeping(this->internal.muList.idx)) {
						wakeMU(this->internal.muList.idx,(unsigned long)this);
						if(this->setup.status.last) {
							brdkStrMemMove((unsigned long)this->status.last[1],(unsigned long)this->status.last[0], sizeof(this->status.last[0])*BRDK_MU_MAX_LAST_SIZE);
							brdkStrCpy((unsigned long)this->status.last[0],(unsigned long)"module awake");
						}
						this->internal.initSubstate = 65000;
					}
					else {	/* module name already exists in the list */
						this->status.substate[0] = 0;	/* reset sub state text */
						if(this->setup.status.stateSubstateMode) {
							brdkStrCpy((unsigned long)tmpStr,(unsigned long)"module name ");
							brdkStrCat((unsigned long)tmpStr,(unsigned long)module.name);
							brdkStrCat((unsigned long)tmpStr,(unsigned long)" already used. Rename module.");
							setSubStateText(this,(unsigned long)tmpStr);
						}
						this->internal.muList.idx = -1;
						this->internal.initSubstate = 100;
					}
				}
				else {
					if(SemAcquire(SemCreateMUList.ident) != ERR_OK) setSubStateText(this,(unsigned long)"waiting global semaphore to be free");
					else {
						this->internal.muList.idx = addToMUList((unsigned long)this,0);
						SemRelease(SemCreateMUList.ident);
						setSubStateText(this,(unsigned long)"module added em list");
						if(this->internal.muList.idx != -1) this->internal.initSubstate = !this->setup.parent.name[0] ? 40 : 30;	/* check if the module has a parent name assigned */	
						else {
							setSubStateText(this,(unsigned long)"em list full. Expand BRDK_EM_MAX_EMS.");
							this->internal.initSubstate = 200;
						}
					}
				}
			}
			break;

		case 30:	/* find parent */
			this->internal.muList.parentIdx = getMUIndex((unsigned long)this, &this->setup.parent);
			if(this->internal.muList.parentIdx == -1) setSubStateText(this,(unsigned long)"parent not found");
			else this->internal.initSubstate = 40;
			break;


		case 40:	/* find siblings */
			for(i=0; i<= BRDK_MU_MAX_SIBLINGS; i++) {
				if(this->setup.sibling[i].name[0] != 0) {
					this->internal.muList.siblingIdx[i] = getMUIndex((unsigned long)this, &this->setup.sibling[i]);
					if(this->internal.muList.siblingIdx[i] == -1) {
						if(this->setup.status.stateSubstateMode) {
							brdkStrCpy((unsigned long)tmpStr,(unsigned long)"sibling: ");
							brdkStrCat((unsigned long)tmpStr,(unsigned long)this->setup.sibling[i].name);
							brdkStrCat((unsigned long)tmpStr,(unsigned long)" not found");
							setSubStateText(this,(unsigned long)tmpStr);
						}
						break;
					}
				}
			}
			if(i>=BRDK_MU_MAX_SIBLINGS) {
				this->status.substate[0] = 0;	/* reset status text */
				this->internal.initSubstate = this->internal.SemCreate_0.ident == 0 ? 50 : 60;
			}
			break;

		case 50:
			this->internal.SemCreate_0.enable = true;
			this->internal.SemCreate_0.initCount = 0;
			this->internal.SemCreate_0.maxCount = 1;
			SemCreate(&this->internal.SemCreate_0);
			switch(this->internal.SemCreate_0.status) {
		
				case 0:	/* ERR_OK */
					SemRelease(this->internal.SemCreate_0.ident);		/* release the semaphore so it's ready */
					this->internal.initSubstate = 60;
					break;

				case 65535:	/* ERR_FUB_BUSY */
					break;

				default:
					setSubStateText(this,(unsigned long)"error creating the semaphore");
					break;

			}
			break;

		case 60:
			switch(this->setup.activation) {
				case BRDK_EM_ACTIVATE:
					this->internal.deactivateRequest = false;
					this->internal.initSubstate = 70;
					break;
			
				case BRDK_EM_ACTIVATE_BY_PARENT:
					if(parent != NULL) {
						if(!parent->internal.deactivateRequest) {
							this->internal.initSubstate = 70;
							this->internal.deactivateRequest = false;
						}
					}
					break;
			
				default: break;
			
			}
			break;
		
		case 70:
			setSubStateText(this,(unsigned long)"register child to parent");
			if(RegisterChild(this, parent) == 0) {
				this->internal.initSubstate = 65000;				/* add child to parent's children list */
				if(this->substate == 0) this->substate = 65000;		/* if substate is still 0 it's auto set to SC */
			}
			break;

		case 100: /* error module name already used. Check if anme is being changed */
			break;

	}
}

/* traverse and find the real interrupter */
unsigned long findInterrupter(unsigned long pEM) {
	if(((brdkEM_typ*)pEM)->internal.pInterrupter != pEM) return findInterrupter(((brdkEM_typ*)pEM)->internal.pInterrupter);
	return pEM;
}

/* flash functionality */
brdk_em_color_typ flashColor(brdkEM_typ* this, brdk_em_color_typ color) {
	if(this->internal.flashTime >= BRDK_EM_FLASH_TIME*2) this->internal.flashTime = 0;
	else this->internal.flashTime += this->status.cycleTime/1000;
	return this->internal.flashTime >= BRDK_EM_FLASH_TIME ? color : WHITE;
} 

/* light tree functionality */
void lightTree(brdkEM_typ* this, brdk_em_color_typ color1, brdk_em_color_typ color2) {
	this->status.lightTree.red 		= color1 == RED || color2 == RED;
	this->status.lightTree.yellow 	= color1 == ORANGE || color2 == ORANGE;
	this->status.lightTree.blue 	= color1 == BLUE || color2 == BLUE;
	this->status.lightTree.green 	= color1 == GREEN || color2 == GREEN;
}

/* set default sub state text if not set in program */
void setSubStateText(brdkEM_typ* this, unsigned long pStr) {
	const int MAX_COUNTER = 99999;
	if(this->status.substate[0] == 0 && this->setup.status.stateSubstateMode) {
		if(pStr != 0) {
			brdkStrCpy((unsigned long)this->status.substate,pStr);
			this->internal.statusCnt.internalCounter = this->internal.statusCnt.internalCounter > MAX_COUNTER ? 0 : this->internal.statusCnt.internalCounter+1;		/* increase the status counter to indicate that a change was made */
		}
		else {
			if(this->description[0] == 0) {
				if(this->substate == 0) {
					brdkStrCpy((unsigned long)this->status.substate,(unsigned long)"0: waiting for state change (SC)");
					this->internal.statusCnt.internalCounter = this->internal.statusCnt.internalCounter > MAX_COUNTER ? 0 : this->internal.statusCnt.internalCounter+1;		/* increase the status counter to indicate that a change was made */
				}
				else if(this->internal.cmdSC) {
					switch(this->state) {
						case STATE_DEACTIVATED: brdkStrCpy((unsigned long)this->status.substate, (unsigned long)"SC: waiting for activate command"); break;
						case STATE_STOPPED: case STATE_COMPLETE: brdkStrCpy((unsigned long)this->status.substate, (unsigned long)"SC: waiting for reset command"); break;
						case STATE_IDLE: case STATE_SUSPENDED: case STATE_HELD:	brdkStrCpy((unsigned long)this->status.substate, (unsigned long)"SC: waiting for start command"); break;
						case STATE_ABORTED: brdkStrCpy((unsigned long)this->status.substate, (unsigned long)"SC: waiting for clear command"); break;
						default: brdkStrCpy((unsigned long)this->status.substate, (unsigned long)"SC: waiting for command"); break;
					}
					this->internal.statusCnt.internalCounter = this->internal.statusCnt.internalCounter > MAX_COUNTER ? 0 : this->internal.statusCnt.internalCounter+1;		/* increase the status counter to indicate that a change was made */
				}
			} 
			else {
				brdkStrUdintToA(this->substate,(unsigned long)&this->status.substate,BRDK_STR_CONVERT_DECIMAL);
				brdkStrCat((unsigned long)this->status.substate, (unsigned long)": ");
				brdkStrCat((unsigned long)this->status.substate, (unsigned long)this->description);
				this->internal.statusCnt.internalCounter = this->internal.statusCnt.internalCounter > MAX_COUNTER ? 0 : this->internal.statusCnt.internalCounter+1;		/* increase the status counter to indicate that a change was made */
			}
		}
	}
}

/* if the parent mode is changed we also changes it for alle the children */
void setChildrenMode(brdkEM_typ* this) {
	int i;
	for(i=0; i <= BRDK_EM_MAX_CHILDREN; i++) {
		brdkEM_typ* child = getEm(this->internal.muList.childrenIdx[i]);
		if(child != NULL) {
			if(this->setup.reactTo == BRDK_EM_REACT_TO_NONE) child->mode = child->mode;
			else child->mode = this->mode;
		}
		else break;
	}
}

/* CHECKS IF ALL THE CHILDREN IS IN STATE */
void checkChildren(brdkEM_typ* this, brdk_em_states_typ STATE) {
	brdkEM_typ* parent;
	brdkEM_typ* child;
	if(this->internal.cmdSC) {
		int i = 0, j;
		while(i < this->status.childCount) {
			child = getEm(this->internal.muList.childrenIdx[i]);
			if(child != NULL) {
				j = i;
				if(child->state == STATE_ACTIVATING || child->state == STATE_DEACTIVATED || child->state == STATE_DEACTIVATING) i++;
				else {
					switch(child->setup.reactTo) {

						case BRDK_EM_REACT_TO_ALL:
							if(child->state == STATE) i++;
							break;

						case BRDK_EM_REACT_TO_ABORT_AND_STOP:
							if(STATE == STATE_ABORTED || STATE == STATE_STOPPED) {
								if(child->state == STATE) i++;
							}
							else i++;
							break;

						case BRDK_EM_REACT_TO_ABORT_ONLY:
							if(STATE == STATE_ABORTED) {
								if(child->state == STATE) i++;
							}
							else i++;
							break;

						case BRDK_EM_REACT_TO_STOP_ONLY:
							if(STATE == STATE_STOPPED) {
								if(child->state == STATE) i++;
							}
							else i++;
							break;

						case BRDK_EM_REACT_TO_NONE: case BRDK_EM_LISTEN_TO_ABORT_AND_STOP: case BRDK_EM_LISTEN_TO_ABORT_ONLY: case BRDK_EM_LISTEN_TO_STOP_ONLY:
							i++;
							break;

					}
				}
				if(j == i) {	/* index not increased */
					if(child->internal.waitingForIdx != -1) {	/* check to see if a child is also waiting for someone */
						if(this->internal.waitingForIdx != child->internal.waitingForIdx) { /* check if the index has changed - otherwise there is nothing to update in the name */
							if(this->setup.status.waitingFor) brdkStrCpy((unsigned long)this->status.waitingFor.name,(unsigned long)child->status.waitingFor.name);
							this->internal.waitingForIdx = child->internal.waitingForIdx;
						}
						if(this->internal.statusCnt.externalCounter != child->internal.statusCnt.externalCounter) {	/* check if there is a change in the status text */
							if(this->setup.status.waitingFor) brdkStrCpy((unsigned long)this->status.waitingFor.substate,(unsigned long)child->status.waitingFor.substate);
							this->internal.statusCnt.externalCounter = child->internal.statusCnt.externalCounter;
						}
					}
					else {
						if(this->internal.waitingForIdx != child->internal.muList.idx) { /* check if the index has changed - otherwise there is nothing to update in the name */	
							if(this->setup.status.stateSubstateMode) {
								brdkStrCpy((unsigned long)this->status.waitingFor.name,(unsigned long)child->setup.name);
								brdkStrCpy((unsigned long)tmpStr, (unsigned long)"SC: waiting for ");
								brdkStrCat((unsigned long)tmpStr, (unsigned long)this->status.waitingFor.name);
								setSubStateText(this,(unsigned long)tmpStr);
							}
							this->internal.waitingForIdx = child->internal.muList.idx;
						}
						if(this->internal.statusCnt.externalCounter != child->internal.statusCnt.internalCounter) {	/* check if there is a change in the status text */
							if(this->setup.status.waitingFor) brdkStrCpy((unsigned long)this->status.waitingFor.substate,(unsigned long)child->status.substate);
							this->internal.statusCnt.externalCounter = child->internal.statusCnt.internalCounter;
						}
					}
					break;
				}
			}
			else break;
		}
		if(parentStateOK(this)) {
			if(i==this->status.childCount) {
				if(this->state == STATE_RESETTING || this->state == STATE_CLEARING) {	/* clear interrupted by data */
					brdkStrMemSet((unsigned long)&this->status.interruptedBy,0,sizeof(this->status.interruptedBy));
					this->internal.pInterrupter = 0;
				}
				if(this->internal.waitingForIdx != -1) {
					if(this->setup.status.waitingFor) brdkStrMemSet((unsigned long)&this->status.waitingFor,0,sizeof(this->status.waitingFor)); /* clear waiting for data */
					this->internal.waitingForIdx = -1;				/* reset waiting for index */
					this->internal.statusCnt.externalCounter = -1;	/* reset external status counter */
				}
				this->state = STATE;
			}
		}
		else {
			if(!this->status.waitingFor.name[0]) {
				parent = getEm(this->internal.muList.parentIdx);
				if(parent != NULL) {
					if(this->internal.waitingForIdx != parent->internal.muList.idx) { /* check if the index has changed - otherwise there is nothing to update */
						this->internal.waitingForIdx = parent->internal.muList.idx;
						if(parent->state == STATE_DEACTIVATED || parent->state == STATE_DEACTIVATING) setSubStateText(this,(unsigned long)"SC: parent is deactivated");
						else setSubStateText(this,(unsigned long)"SC: waiting for parent");
						if(this->setup.status.waitingFor) brdkStrCpy((unsigned long)this->status.waitingFor.name,(unsigned long)parent->setup.name);
					}
					if(this->internal.statusCnt.externalCounter != parent->internal.statusCnt.internalCounter) {	/* check if there is a change in the status text */
						if(this->setup.status.waitingFor) brdkStrCpy((unsigned long)this->status.waitingFor.substate,(unsigned long)parent->status.substate);
						this->internal.statusCnt.externalCounter = parent->internal.statusCnt.internalCounter;
					}					
				}
			}
		}
	}
}

/* make sure eventual parent is also in same state before state change */
int parentStateOK(brdkEM_typ* this) {
	int status = false;
	brdkEM_typ* parent = getEm(this->internal.muList.parentIdx);
	if(parent != NULL) {
		switch(this->setup.reactTo) {
			case BRDK_EM_REACT_TO_ALL: 				status = (parent->state == this->state || this->internal.findParent); break;
			case BRDK_EM_REACT_TO_ABORT_AND_STOP: 	status = (this->state == STATE_ABORTING || this->state == STATE_STOPPING) ? (parent->state == this->state) : true; break;
			case BRDK_EM_REACT_TO_ABORT_ONLY:		status = (this->state == STATE_ABORTING) ? (parent->state == this->state) : true; break;
			case BRDK_EM_REACT_TO_STOP_ONLY:		status = (this->state == STATE_STOPPING) ? (parent->state == this->state) : true; break;
			case BRDK_EM_REACT_TO_NONE: case BRDK_EM_LISTEN_TO_ABORT_AND_STOP: case BRDK_EM_LISTEN_TO_ABORT_ONLY: case BRDK_EM_LISTEN_TO_STOP_ONLY:	status = true;
		}
	}
	else status = true;
	return status;
}

/* automatically take the module to it's parents state */
void findParent(brdkEM_typ* this, brdkEM_typ* parent, brdk_em_states_typ cmd, int numArgs, ...) {
	if(parent != NULL) {
		va_list list;
		int i;
		if(this->state == parent->state) this->internal.findParent = false;
		else {
			va_start(list, numArgs);		
			for(i=0; i < numArgs; i++) { 
				brdk_em_states_typ checkState = va_arg(list, brdk_em_states_typ);
				if(parent->state == checkState) {
					switch(cmd) {
						case STATE_RESETTING: this->cmd.reset = true; break;
						case STATE_STARTING: case STATE_UNHOLDING: case STATE_UNSUSPENDING: this->cmd.start = true; break;
						case STATE_EXECUTE: 
							switch(checkState) {
								case STATE_HOLDING: case STATE_HELD: this->cmd.hold = true; break;
								case STATE_SUSPENDING: case STATE_SUSPENDED: this->cmd.suspend = true; break;
								case STATE_COMPLETING: case STATE_COMPLETE: this->cmd.complete = true; break;
								default: break;
							}
							break;
						default: break;
					}
				}
			}
		}
	}
}

/* observes if one of the children is in one of the states */
void observeChildrensState(brdkEM_typ* this, int numArgs, ...) {
	int i,j;
	brdkEM_typ* child;
	va_list list;																					/* define argument list variable */
	for(i=0; i <= BRDK_EM_MAX_CHILDREN; i++) {														/* check all children */
		child = getEm(this->internal.muList.childrenIdx[i]);
		if(child != NULL) {											/* */
			brdk_em_setup_react_typ reactTo = child->setup.reactTo;
			if(reactTo == BRDK_EM_REACT_TO_NONE || reactTo == BRDK_EM_REACT_TO_NONE || reactTo == BRDK_EM_LISTEN_TO_ABORT_AND_STOP || reactTo == BRDK_EM_LISTEN_TO_ABORT_ONLY || reactTo == BRDK_EM_LISTEN_TO_STOP_ONLY) this->state = this->state;
			else {
				va_start(list, numArgs);		
				for(j=0; j < numArgs; j++) { 
					brdk_em_states_typ child_state = va_arg(list, brdk_em_states_typ);
					if(child->state == child_state) {
						switch(child_state) {
							
							case STATE_ABORTING:
								if(reactTo == BRDK_EM_REACT_TO_ALL || reactTo == BRDK_EM_REACT_TO_ABORT_AND_STOP || reactTo == BRDK_EM_REACT_TO_ABORT_ONLY) {
									this->internal.pInterrupter = findInterrupter((unsigned long)child); /* save who went in aborted or aborting */
									this->state = STATE_ABORTING;
								}
								break;
								
							case STATE_STOPPING:
								if((this->internal.cmdSC || this->setup.skipStopSC) && (reactTo == BRDK_EM_REACT_TO_ALL || reactTo == BRDK_EM_REACT_TO_ABORT_AND_STOP || reactTo == BRDK_EM_REACT_TO_STOP_ONLY)) {
									this->internal.pInterrupter = findInterrupter((unsigned long)child); /* save who went in stopped or stopping */
									this->state = STATE_STOPPING;
								}
								break;
								
							case STATE_SUSPENDING:
								if(this->internal.cmdSC && reactTo == BRDK_EM_REACT_TO_ALL) {
									this->internal.pInterrupter = findInterrupter((unsigned long)child); /* save who went in suspending or suspended */
									this->state = STATE_SUSPENDING;
								}
								break;
								
							case STATE_HOLDING:
								if(this->internal.cmdSC && reactTo == BRDK_EM_REACT_TO_ALL) {
									this->internal.pInterrupter = findInterrupter((unsigned long)child); /* save who went in holding or held */
									this->state = STATE_HOLDING;
								}
								break;
								
							default:
								break;
						
						}
						va_end(list);
						break;
					}
				}
			}
		}
		else {
			va_end(list);
			break;
		}
	}
}

/* observes the parent state by the given states */
void observeParentState(brdkEM_typ* this, brdkEM_typ* parent, int numArgs, ...) {
	int i;
	brdk_em_setup_react_typ reactTo =  this->setup.reactTo;
	if(parent == NULL || reactTo == BRDK_EM_REACT_TO_NONE) this->state = this->state;
	else {
		va_list list;																									/* define argument list variable */
		va_start(list, numArgs);																						/* init list */										
		for(i=0; i < numArgs; i++) if(parent->state==va_arg(list, brdk_em_states_typ)) {
			switch(parent->state) {

				case STATE_ABORTING:
					if(reactTo == BRDK_EM_REACT_TO_ALL || reactTo == BRDK_EM_REACT_TO_ABORT_AND_STOP || reactTo == BRDK_EM_REACT_TO_ABORT_ONLY || reactTo == BRDK_EM_LISTEN_TO_ABORT_AND_STOP || reactTo == BRDK_EM_LISTEN_TO_ABORT_ONLY) {
						this->state = parent->state; 	/* get next state item and check if it's equal to parent state. and then set chils state*/
						this->internal.pInterrupter = parent->internal.pInterrupter;
					}
					break;

				case STATE_STOPPING:
					if((this->internal.cmdSC || this->setup.skipStopSC) && (reactTo == BRDK_EM_REACT_TO_ALL || reactTo == BRDK_EM_REACT_TO_ABORT_AND_STOP || reactTo == BRDK_EM_REACT_TO_STOP_ONLY || reactTo == BRDK_EM_LISTEN_TO_STOP_ONLY)) {
						this->state = parent->state; 	/* get next state item and check if it's equal to parent state. and then set chils state*/
						this->internal.pInterrupter = parent->internal.pInterrupter;
					}
					break;

				case STATE_CLEARING:
					if(this->internal.cmdSC && (reactTo == BRDK_EM_REACT_TO_ALL || reactTo == BRDK_EM_REACT_TO_ABORT_AND_STOP || reactTo == BRDK_EM_REACT_TO_ABORT_ONLY)) this->state = parent->state; 	/* get next state item and check if it's equal to parent state. and then set chils state*/
					break;

				default:
					if(this->internal.cmdSC && reactTo == BRDK_EM_REACT_TO_ALL) this->state = parent->state; 	/* get next state item and check if it's equal to parent state. and then set chils state*/
					break;
			
			}	
		}
		va_end(list);
	}																													/* cleanup the system stack */
}

/* check for commands -> STATE_STOPPING means check for command stop... */
void checkCommands(brdkEM_typ* this, brdkEM_typ* parent, int numArgs, ...) {
	brdkEM_typ* child;
	int i,j;
	if(this->internal.cmdSC || this->cmd.abort || (this->cmd.stop && this->setup.skipStopSC)) {
		va_list list;																											/* define argument list variable */
		va_start(list, numArgs);																								/* init list */										
		for(i=0; i < numArgs; i++) { 
			switch(va_arg(list, brdk_em_states_typ)) {
				
				case STATE_STOPPING:
					if(this->cmd.stop) {
						i = numArgs;
						this->internal.pInterrupter = (unsigned long)this; /* module interrupted itself */
						this->state = STATE_STOPPING;
					}
					break;	
			
				case STATE_RESETTING:
					if(this->cmd.reset) {
						i = numArgs;
						this->state = STATE_RESETTING;
					}
					break;
					
				case STATE_CLEARING:
					if(this->cmd.clear) {
						this->state = STATE_CLEARING;
					}
					break;
					
				case STATE_STARTING:	
					if(this->cmd.start) {
						i = numArgs;
						this->state = STATE_STARTING;
					}
					break;
					
				case STATE_HOLDING:	
					if(this->cmd.hold) {
						i = numArgs;
						this->internal.pInterrupter = (unsigned long)this; /* module interrupted itself */
						this->state = STATE_HOLDING;
					}
					break;
					
				case STATE_UNHOLDING:	
					if(this->cmd.start) {
						i = numArgs;
						this->state = STATE_UNHOLDING;
					}
					break;
					
				case STATE_SUSPENDING:	
					if(this->cmd.suspend) {
						i = numArgs;
						this->internal.pInterrupter = (unsigned long)this; /* module interrupted itself */
						this->state = STATE_SUSPENDING;
					}
					break;
					
				case STATE_UNSUSPENDING:	
					if(this->cmd.start) {
						i = numArgs;
						this->state = STATE_UNSUSPENDING;
					}
					break;
					
				case STATE_COMPLETING:	
					if(this->cmd.complete) {
						i = numArgs;
						this->state = STATE_COMPLETING;
					}
					break;
					
				case STATE_ABORTING:
					if(this->cmd.abort) {
						i = numArgs;
						this->internal.pInterrupter = (unsigned long)this; /* module interrupted itself */
						this->state = STATE_ABORTING;
					}
					break;

				case STATE_ACTIVATING:
					/* wait for initialization complete */
					if(this->internal.initSubstate == 65000) {
						switch(this->setup.activation) {

							case BRDK_EM_ACTIVATE:
								this->state = STATE_ACTIVATING;
								break;

							case BRDK_EM_ACTIVATE_BY_PARENT:
								if(parent != NULL) {
									switch(parent->state) {
										case STATE_DEACTIVATING: case STATE_DEACTIVATED: setSubStateText(this,(unsigned long)"waiting for parent's activation"); break;
										default: 
											if(!parent->internal.deactivateRequest) this->state = STATE_ACTIVATING; 
											break;
									}
								}
								break;

							default:
								setSubStateText(this, (unsigned long)"waiting for activation");
								break;

						}
					}
					break;
					
				case STATE_DEACTIVATING:
					switch(this->setup.activation) {
				
						case BRDK_EM_DEACTIVATE:
							this->internal.deactivateRequest = true;
							for(j=0; j <= BRDK_EM_MAX_CHILDREN; j++) {		/* check all children */
								child = getEm(this->internal.muList.childrenIdx[j]);
								if(child != NULL) {
									if(child->state != STATE_DEACTIVATED && child->state && STATE_DEACTIVATING) break;
								}
							}
							if(j > BRDK_EM_MAX_CHILDREN) {
								i = numArgs;
								this->state = STATE_DEACTIVATING;
							}
							break;
			
						case BRDK_EM_ACTIVATE_BY_PARENT:
							if(parent != NULL) {
								switch(parent->state) {
									case STATE_DEACTIVATING: case STATE_DEACTIVATED: this->state = STATE_DEACTIVATING; break;
									
									case STATE_ABORTED: case STATE_STOPPED:
										/* check if parent has deactivation command and then follow */
										if(parent->internal.deactivateRequest) {
											this->internal.deactivateRequest = true;
											for(j=0; j <= BRDK_EM_MAX_CHILDREN; j++) {		/* check all children */
												child = getEm(this->internal.muList.childrenIdx[j]);
												if(child != NULL) {
													if(child->state != STATE_DEACTIVATED && child->state && STATE_DEACTIVATING) break;
												}
											}
											if(j > BRDK_EM_MAX_CHILDREN) {
												i = numArgs;
												this->state = STATE_DEACTIVATING;
											}
										}
										break;
									
									default: break;
								}
							}
							break;

						default:
							break;

					}
					break;
			
				default:
					break;
					
			}
		}									
		va_end(list);
		/* check if an unvalid command has been given */
		this->status.unvalidatedCmd = this->state != this->internal.old.state ? false : this->cmd.abort || this->cmd.clear || this->cmd.stop || this->cmd.reset || this->cmd.start || this->cmd.hold || this->cmd.suspend || this->cmd.complete;
	}	
}

/* Add's a child to the parents child list */
unsigned short RegisterChild(brdkEM_typ* this, brdkEM_typ* parent) {
	int i=0, j=0;
	if(parent != NULL) {
		for(i=0; i < BRDK_EM_MAX_CHILDREN; i++) {
			if(parent->internal.muList.childrenIdx[i] == this->internal.muList.idx) break;	/* child already exists in array. index is returned */
			else if(parent->internal.muList.childrenIdx[i] == -1) {							/* finds an empty spot in the array */
				j = SemAcquire(parent->internal.SemCreate_0.ident);							/* checks if the semaphore is free */
				if(j == ERR_OK) {
					parent->internal.muList.childrenIdx[i] = this->internal.muList.idx;		/* puts the index in the array */
					parent->status.childCount++;
					SemRelease(parent->internal.SemCreate_0.ident);							/* releases the semaphore again */
					i = ERR_OK;
				}
				else {
					i = j;	/* 33322: Semaphore with the specified ident does not exist   33323: Semaphore is being used */
					if(this->setup.status.stateSubstateMode) {
						brdkStrCpy((unsigned long)tmpStr,(unsigned long)"semaphore error: ");
						brdkStrAppendUdintToA(j,(unsigned long)tmpStr,10,-1);
						setSubStateText(this,(unsigned long)tmpStr);
					}
				}
				break;
			}
		}
		if(i == BRDK_EM_MAX_CHILDREN) {  /* error: child array too small. Solution: make child array bigger */
			setSubStateText(this,(unsigned long)"parent child array full. Increase BRDK_EM_MAX_CHILDREN.");
		}
	}
	return i;
}

/* Unregister a child */
unsigned short UnregisterChild(brdkEM_typ* this, brdkEM_typ* parent) {
	int i=65000,j;
	if(parent != NULL) {
		for(i=0; i < BRDK_EM_MAX_CHILDREN; i++) {
			if(parent->internal.muList.childrenIdx[i] == this->internal.muList.idx) {	/* child found */
				j = SemAcquire(parent->internal.SemCreate_0.ident);	/* checks if the semaphore is free */
				if(j == ERR_OK) {
					parent->internal.muList.childrenIdx[i] = -1;			/* pointer is removed from the array */
					parent->status.childCount--;
					for(j=i; j < BRDK_EM_MAX_CHILDREN; j++) parent->internal.muList.childrenIdx[j] = parent->internal.muList.childrenIdx[j+1];
					SemRelease(parent->internal.SemCreate_0.ident);		/* releases the semaphore again */
					i = ERR_OK;
					this->internal.muList.parentIdx = -1;
				}
				else {
					i = j;	/* 33322: Semaphore with the specified ident does not exist   33323: Semaphore is being used */ 
					if(this->setup.status.stateSubstateMode) {
						brdkStrCpy((unsigned long)tmpStr,(unsigned long)"semaphore error: ");
						brdkStrAppendUdintToA(j,(unsigned long)tmpStr,10,-1);
						setSubStateText(this,(unsigned long)tmpStr);
					}
				}
				break;
			}
		}
		if(i == BRDK_EM_MAX_CHILDREN) {  /* error: child not found in array */
			setSubStateText(this,(unsigned long)"child not found in parent array.");
		}
	}
	return i;
}
