
TYPE
	brdk_um_mode_typ : 
		( (*Unit Machine running modes.*)
		BRDK_UM_MODE_MANUAL := 0, (*Commands from the remote system is set in the structure but everthing has to be set from outside the function block.*)
		BRDK_UM_MODE_STATUS_ONLY := 1, (*Possible status and admin tags are written automatically. All commands from the remote system has to be handled from outside the function block.*)
		BRDK_UM_MODE_ALL_ENABLED := 2 (*Possible command, status and admin tags are written automatically.*)
		) := BRDK_UM_MODE_MANUAL;
	brdk_um_pml_c_typ : 	STRUCT  (*Command tags are used to control the operation of the unit machine.  Command tags include unit state commands which control the state transitions in the base state model.  The command tags also include parameters and process variables which control how the machine operates.  Command tags generally originate from the machine user or a remote system.  The originator of the command in this report is defined as the “requestor” or “information sender”.  The unit machine in this report is known as the “execution system”.*)
		UnitMode : DINT := 0; (*This value is predefined by the user / OEM, and are the desired unit modes of the machine.  The UnitMode tag is a numerical representation of the commanded mode.  There can be any number of unit modes, and for each unit mode there is an accompanying state model. Example unit modes are Production, Maintenance, Manual, Clean Out, Dry Run, Setup, etc.*)
		UnitModeChangeRequest : BOOL := FALSE; (*When a unit mode request takes place a numerical value must be present in the Command.UnitMode tag to change the unit mode.  Local processing and conditioning of the requested mode change is necessary in order to accept, reject, or condition the timing of the change request.*)
		MachSpeed : REAL := 0; (*This defines the set point for the current speed of the machine in primary packages per minute.  Keeping speed in a primary package unit of measure (UOM) allows for easier control integration.  The primary package UOM is the normalized rate for the machine, normalized to a value chosen on the line.  The following example is for a bottle line running at balance line speed of 1000 packages/minute.  The UOM chosen is equivalent to be the actual count of the Filler, or Labeler.*)
		CntrlCmd : DINT := 0; (*The tag holds the value of the command that provides the state command to drive a state change in the Base State Model, this tag is typically manipulated locally.  Local processing of this tag, local machine conditions and local commands drive changes between machine states.  This tag can be set by a local or remote source.  All values in the table below are reserved.  *)
		CmdChangeRequest : BOOL := FALSE; (*This CmdChangeRequest bit will command the machine to proceed to change the state to the target state. The tag can be used to condition when a change of state can occur.  The target state will be one of the states in the base state model.*)
	END_STRUCT;
	brdk_um_pml_c_ctrl_cmd_typ : 
		( (*Control commands as enumerations.*)
		CTRL_CMD_UNDEFINED := 0,
		CTRL_CMD_RESET := 1, (*Reset --> Go to STATE_RESETTING - Can only be set in STATE_STOPPED.*)
		CTRL_CMD_START := 2, (*Start --> Go to STATE_STARTING - Can only be set in STATE_IDLE, STATE_SUSPENDED, STATE_HELD.*)
		CTRL_CMD_STOP := 3, (*Stop --> Go to STATE_STOPPING - Can be set in all states.*)
		CTRL_CMD_HOLD := 4, (*Hold --> Go to STATE_HOLDING - Can only be set in STATE_EXECUTE.*)
		CTRL_CMD_UNHOLD := 5, (*Unhold --> Go to STATE_EXECUTE - Can only be set in STATE_HOLDING.*)
		CTRL_CMD_SUSPEND := 6, (*Suspend --> Go to STATE_SUSPENDING - Can only be set in STATE_EXECUTE.*)
		CTRL_CMD_UNSUSPEND := 7, (*Unsuspend --> Go to STATE_EXECUTE - Can only be set in STATE_SUSPENDED.*)
		CTRL_CMD_ABORT := 8, (*Abort --> Go to STATE_ABORTING - Can be set in all states.*)
		CTRL_CMD_CLEAR := 9 (*Clear --> Go to STATE_STOPPING - Can only be set in STATE_ABORTED.*)
		);
	brdk_um_pml_s_typ : 	STRUCT  (*Status tags are used to describe the operation of the unit machine.  Status tags include state commands which describe the state transitions in the base state model.  The status tags also include parameters and process variables which describe how the machine operates.  Status tags generally originate from the unit machine user and can be used on a remote system.  The originator of the status tags in this report is defined as the “Execution system”.  *)
		UnitModeCurrent : DINT := 0; (*This value is predefined by the user / OEM of the available unit modes of the machine allowing a possible different set of states for the base State Model and could provide completely different functionality in the same machinery such as Cleanout, Production, Etc. *)
		StateCurrent : DINT := 0; (*The StateCurrent status tag specifies the current state in the current unit mode of the unit machine.  The numerical values are in the table below are reserved.*)
		MachSpeed : REAL := 0; (*This describes the set point for the current speed of the machine in primary packages per minute.  Keeping speed in a primary package unit of measure (UOM) allows for easier control integration.  The primary package UOM is the normalized rate for the machine, normalized to a value chosen on the line.  The following example is for a bottle line running at balance line speed of 1000 packages/minute.  The UOM chosen is equivalent to be the actual count of the Filler, or Labeler.*)
		CurMachSpeed : REAL := 0; (*This the actual value of the machine speed. Keeping units in primary package unit of measure (UOM), allows for easier control integration.  The primary package UOM is the normalized rate for the machine, normalized to a value chosen on the line.  Pack Counts are parameters stored in the Administration tags or downloaded parameters stored in Command tags parameters.*)
		EquipmentInterlock : brdk_um_pml_s_ei_typ; (*Equipment interlock.*)
		StateRequested : DINT := 0; (*This value is used for state transition checking to ensure that a target state can be transitioned to.  The target state, StateRequested, is a numerical value corresponding to a state in the base state model.*)
		StateChangeInProcess : BOOL := FALSE; (*This bit indicates that a change in state is in progress following a state change request command.*)
	END_STRUCT;
	brdk_um_pml_s_ei_typ : 	STRUCT  (*Equipment interlock.*)
		Blocked : BOOL := FALSE; (*This bit, when set to 1, indicates that a downstream system is not able to accept product.  In this condition, the equipment is capable of producing product but is in a suspended state due to a downstream system.  This tag is necessary for external equipment monitoring so that the reason for the machine being in a suspended state can be identified.*)
		Starved : BOOL := FALSE; (*This bit, when set to 1, indicates that an upstream system is not able to supply product.  In this condition, the equipment is capable of producing product but is in a suspended state due to an upstream system.  This tag is necessary for external equipment monitoring so that the reason for the machine being in a suspended state can be identified.*)
	END_STRUCT;
	brdk_um_pml_a_typ : 	STRUCT  (*Administration tags are used to describe the quality and alarm information of the unit machine.  Administration tags include alarm parameters which describe the conditions within the base state model typically for production data acquisition (PDA) systems. The administration tags also include parameters which can describe how well the machine operates, or specific information on the product quality produced by the machine.  Administration tags generally originate from the unit machine and can be used on the HMI or a remote system.*)
		StopReason : brdk_um_pml_a_sr_typ; (*Stop reason.*)
		ProdProcessedCount : ARRAY[0..BRDK_UM_MAX_PRODUCTS]OF brdk_um_pml_a_pc_typ; (*This tag represents the number of products processed by the production machine.  An example of tag usage would be the number of products that were made, including all good and defective products. This tag can be used locally or remotely if needed.  The extent of the array is typically limited to the number of products needed to be counted.  The number of products processed minus the defective count is the number of products made by the machine. The array index of # = 0 can be reserved for the count of the number of units from the primary production stream.*)
		ProdDefectiveCount : ARRAY[0..BRDK_UM_MAX_PRODUCTS]OF brdk_um_pml_a_pc_typ; (*This tag represents the product that is marked as defective in the production machine, to be used if applicable.  An example of tag usage would the number of products rejected or products that are termed defective. This tag can be used locally or remotely if needed.  The extent of the array is typically limited to the number of products needed to be counted as defective. When this tag is used with Admin.ProdProcessedCount[#] the number of good products / well formed cycles made by the machine can be calculated.  The array index of # = 0 can be reserved for the total cumulative rejected units from the primary production stream.*)
		PLCDateTime : ARRAY[0..6]OF DINT; (*The data elements associated with time and date shall be in the format of year:month:day and hour:min:sec:usec per ISO 8601.  The data type shall be an array of 32 bit Integers.*)
	END_STRUCT;
	brdk_um_pml_a_sr_typ : 	STRUCT  (*Stop reason.*)
		ID : DINT := 0; (*Stop Reason ID number.  Unique value assigned to each Stop Reason.*)
	END_STRUCT;
	brdk_um_pml_a_pc_typ : 	STRUCT  (*Product structure.*)
		Count : DINT := 0; (*The count value is used as a variable for displaying information about the amount of product.  The value is indexed upon the machine processing a unit of the product defined by the ID and NAME.  This could be displayed on HMI screens or higher level PDA systems.  The counter rolls over to 0 at 2,147,483,647*)
	END_STRUCT;
	brdk_um_pml_internal_typ : 	STRUCT  (*Internal variables.*)
		modeState : UINT; (*Mode change state.*)
		commandState : UINT; (*Command change state.*)
		oldMode : DINT; (*Mode before command request.*)
		oldState : brdk_em_states_typ; (*State before command request.*)
		rtc : RTCtime_typ; (*Holds the current date and time.*)
	END_STRUCT;
END_TYPE
