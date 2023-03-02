#include <brdkUM_func.h>

void brdkUM(struct brdkUM* this) {

	if(this->pEM != 0) {
		brdkEM_typ* em = (brdkEM_typ*)this->pEM;

		/* COMMAND TAGS */
		if(this->mode == BRDK_UM_MODE_ALL_ENABLED) {

			switch(this->internal.modeState) {

				case 0:	/* wait for a mode change request */
					if(this->Command.UnitModeChangeRequest) {
						this->internal.oldMode = em->mode;	/* store the state before the command request */
						/*if()*/
					}
					break;

			}

			switch(this->internal.commandState) {

				case 0:	/* wait for a command change request */
					if(this->Command.CmdChangeRequest) {
						this->internal.oldState = em->state;	/* store the state before the command request */
						setCommand(this->Command.CntrlCmd, &em->cmd, true);
						this->internal.commandState = 10;	
					}
					break;

				case 10:	/* check if a state has been changed due to a request */
					if(this->Command.CmdChangeRequest) {
						if(this->internal.oldState != em->state) {
							this->internal.commandState = 20;
						}
					}
					else {
						setCommand(this->Command.CntrlCmd, &em->cmd, false);
						this->internal.commandState = 0;
					}
					break;

				case 20:	/* wait for the state to have changed */
					if(!this->Command.CmdChangeRequest) {
						this->internal.commandState = 0;
					}
					break;

			}
			
		}
		else this->internal.commandState = 0;	/* reset command mode */
	
		/* STATUS AND ADMIN TAGS */
		if(this->mode == BRDK_UM_MODE_ALL_ENABLED || this->mode == BRDK_UM_MODE_STATUS_ONLY) {
	
			/* STATUS TAGS */
			this->Status.UnitModeCurrent 	= em->mode;
			this->Status.MachSpeed 			= this->Command.MachSpeed;
			switch(this->internal.commandState) {

				case 10: case 20: 
					this->Status.StateRequested	= em->status.unvalidatedCmd ? 0 : cmdToState(this->Command.CntrlCmd);
					this->Status.StateChangeInProcess = this->Command.CntrlCmd != 0 && !em->status.unvalidatedCmd;
					break;

				default:
					this->Status.StateRequested	= 0;
					this->Status.StateChangeInProcess = this->Command.CmdChangeRequest;
					break;

			}
			this->Status.StateCurrent 		= em->state;
			
			/* ADMIN TAGS */
			RTC_gettime(&this->internal.rtc);
			this->Admin.PLCDateTime[0] = (signed long)this->internal.rtc.year;		/* Array element 0 = Year */
			this->Admin.PLCDateTime[1] = (signed long)this->internal.rtc.month;		/* Array element 1 = Month */
			this->Admin.PLCDateTime[2] = (signed long)this->internal.rtc.day;		/* Array element 2 = Day */
			this->Admin.PLCDateTime[3] = (signed long)this->internal.rtc.hour;		/* Array element 3 = Hour (24hr format) */
			this->Admin.PLCDateTime[4] = (signed long)this->internal.rtc.minute;	/* Array element 4 = Min */
			this->Admin.PLCDateTime[5] = (signed long)this->internal.rtc.second;	/* Array element 5 = Sec */
			this->Admin.PLCDateTime[6] = (signed long)this->internal.rtc.millisec;	/* Array element 6 = USec (1/1,000,000 sec) */
			handleProductOverflow((brdk_um_pml_a_pc_typ*)&this->Admin.ProdDefectiveCount);
			handleProductOverflow((brdk_um_pml_a_pc_typ*)&this->Admin.ProdProcessedCount);

		}
	}
}
