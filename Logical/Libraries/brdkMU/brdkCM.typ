
TYPE
	brdk_cm_setup_typ : 	STRUCT  (*Equipment module setup structure.*)
		name : STRING[BRDK_MU_NAME_STRING_SIZE]; (*Name of the module.*)
		nameIdx : DINT := -1; (*Index to be added to name. If -1 no index will be set.*)
		parent : brdk_mu_setup_module_typ; (*Parameters to setup a parent module.*)
		sibling : ARRAY[0..BRDK_MU_MAX_SIBLINGS]OF brdk_mu_setup_module_typ; (*Enables the logging of the last state and sub state.*)
		status : brdk_cm_setup_status_typ; (*Enables the logging of the last state and sub state.*)
	END_STRUCT;
	brdk_cm_setup_status_typ : 	STRUCT  (*Enable or disable status functionality.*)
		state : BOOL := TRUE; (*State string.*)
		last : BOOL := TRUE; (*Last log entries.*)
	END_STRUCT;
	brdk_cm_status_typ : 	STRUCT  (*Control module status type.*)
		ready : BOOL := FALSE; (*Control module is ready for use.*)
		state : STRING[BRDK_MU_MAX_DESCRIPTION_STRING]; (*Can be set in the task to descripe what the module is doing in this state.*)
		time : UDINT; (*[ms] Actual time spend in the current state.*)
		last : ARRAY[0..BRDK_MU_MAX_LAST_SIZE]OF STRING[25]; (*The last state the module was in.*)
		cycleTime : UDINT; (*[µsec] Cycle time of the modules's task.*)
		parent : brdk_mu_status_module_typ; (*Information about the parent.*)
		sibling : ARRAY[0..BRDK_MU_MAX_SIBLINGS]OF brdk_mu_status_module_typ; (*List of available siblings.*)
		sleeping : BOOL; (*Indicates that the module is sleeping meaning that the module is not being executed because a new instance is being transferred.*)
		level : UDINT; (*Level in the hierarchy. Parent is level 0.*)
		name : STRING[BRDK_MU_NAME_STRING_SIZE]; (*Name of the module.*)
	END_STRUCT;
	brdk_cm_internal_mu_list_typ : 	STRUCT  (*Holds index's in the MU list.*)
		idx : DINT := -1; (*Own index in the mu list.*)
		parentIdx : DINT := -1; (*Parent index in the mu list.*)
		siblingIdx : ARRAY[0..BRDK_MU_MAX_SIBLINGS]OF DINT := [4(-1)]; (*Siblings index in the mu list.*)
	END_STRUCT;
	brdk_cm_internal_typ : 	STRUCT  (*Control module internal variables.*)
		initSubstate : UINT := 0; (*Internal temp sub state to be used in INIT PROGRAM.*)
		muList : brdk_cm_internal_mu_list_typ; (*Holds index's in the MU list.*)
		old : brdk_cm_state_old_typ; (*Holds the old states.*)
	END_STRUCT;
	brdk_cm_state_old_typ : 	STRUCT  (*Holds the old states.*)
		state : UINT; (*Old CM state.*)
		substate : UINT; (*Old CM substate.*)
	END_STRUCT;
END_TYPE
