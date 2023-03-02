
FUNCTION_BLOCK brdkEM (*Represents an equipment module.*) (*$GROUP=User*)
	VAR_INPUT
		description : STRING[BRDK_MU_MAX_DESCRIPTION_STRING]; (*Description of what the substate is doing or waiting for.*)
		mode : DINT := MODE_PRODUCING; (*Control modes. Based on ISA-TR88.00.02 control modes. Modes can only be changed in STATE_STOPPED.*)
		cmd : brdk_em_command_typ; (*Control commands. Based on ISA-TR88.00.02 commands.*)
		setup : brdk_em_setup_typ; (*Setup of module.*)
		substate : UINT := 0; (*Equipment module substate can be used in the task. When a state is done set cmd.sc = true and the fb will take the unit to it's next state.*)
	END_VAR
	VAR_OUTPUT
		state : brdk_em_states_typ := STATE_DEACTIVATED; (*Equipment module state. State changes are controlled in the fb and is not allowed outside this fb. Based on ISA-TR88.00.02 states.*)
		status : brdk_em_status_typ; (*Module status.*)
	END_VAR
	VAR
		internal : brdk_em_internal_typ; (*Internal variables.*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION brdkEMSleep : DINT (*Put an equipment module to sleep. $RETURN=index;Index in the list.*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		pEM : UDINT; (*Pointer to the equipment module.*)
	END_VAR
END_FUNCTION

FUNCTION brdkEMModeToA : UDINT (*Converts an equipment module mode int to a string representation. $RETURN=pointer;String pointer to mode string.*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		mode : DINT := 0; (*Equipment module mode.*)
		pEM : UDINT := 0; (*Pointer to equipment module.*)
	END_VAR
END_FUNCTION

FUNCTION brdkEMStateToA : UDINT (*Converts an equipment module state enum to a string representation. $RETURN=pointer;String pointer to state string.*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		state : brdk_em_states_typ; (*Equipment module state.*)
	END_VAR
END_FUNCTION

FUNCTION_BLOCK brdkCM (*Represents a control module. The module does not follow the ISA-TR88.00.02 standard.*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		description : STRING[BRDK_MU_MAX_DESCRIPTION_STRING]; (*Description of what the state / substate is doing or waiting for.*)
		state : UINT := 0; (*Control module state must be used in the task.*)
		substate : UINT := 0; (*Control module substate must be used in the task.*)
		setup : brdk_cm_setup_typ; (*Setup of module.*)
	END_VAR
	VAR_OUTPUT
		status : brdk_cm_status_typ; (*Module status.*)
	END_VAR
	VAR
		internal : brdk_cm_internal_typ; (*Internal variables.*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION brdkCMSleep : DINT (*Put a control module to sleep. $RETURN=index;Index in the list.*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		pCM : UDINT; (*Pointer to the control module.*)
	END_VAR
END_FUNCTION

FUNCTION brdkCMChangeParent : DINT (*Changes the parent of a control module. $RETURN=index;Index in the new parent.*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		pCM : UDINT; (*Pointer to the control module.*)
		pParentName : UDINT; (*Pointert to the logical name of the parent module.*)
		parentIndex : DINT; (*Index to be added to the module name. If -1 no index will be set.*)
	END_VAR
END_FUNCTION

FUNCTION_BLOCK brdkUM (*Unit Machine / Information Receiver module. It makes communication with a remote system easy. This fb makes it possible to remote control an equipment module.*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		mode : brdk_um_mode_typ := BRDK_UM_MODE_MANUAL; (*Communication mode.*)
		pEM : UDINT; (*Pointer to the main equipment module (machine unit).*)
		Status : brdk_um_pml_s_typ; (*Status tags.*)
		Admin : brdk_um_pml_a_typ; (*Administration tags.*)
	END_VAR
	VAR_OUTPUT
		Command : brdk_um_pml_c_typ; (*Command tags.*)
	END_VAR
	VAR
		internal : brdk_um_pml_internal_typ; (*Internal variables.*)
	END_VAR
END_FUNCTION_BLOCK
