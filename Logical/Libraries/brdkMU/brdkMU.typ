
TYPE
	brdk_mu_setup_module_typ : 	STRUCT  (*Parameters to setup a connection to a module.*)
		name : STRING[BRDK_MU_NAME_STRING_SIZE]; (*Logical name of the module.*)
		index : DINT := -1; (*Index to be added to the module name. If -1 no index will be set.*)
	END_STRUCT;
	brdk_mu_status_module_typ : 	STRUCT  (*Status of a module.*)
		state : brdk_em_states_typ := STATE_DEACTIVATED; (*Module states. Based on ISA-TR88.00.02 state model.*)
		substate : UINT := 0; (*Equipment module substate can be used in the task. When a state is done set cmd.sc = true and the fb will take the unit to it's next state.*)
		sleeping : BOOL := FALSE; (*Indicates that the module is sleeping meaning that the module is not being executed because a new instance is being transferred.*)
		mode : DINT; (*Mode of the module.*)
		sc : BOOL; (*The State Complete indication within the Acting state procedure will cause a state transition to occur.*)
	END_STRUCT;
END_TYPE
