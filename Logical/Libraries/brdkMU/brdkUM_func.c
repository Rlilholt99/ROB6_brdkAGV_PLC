#include <brdkEM_func.h>

void setCommand(signed long ctrlCmd, brdk_em_command_typ* emCmd, unsigned char value) {
	switch(ctrlCmd) {
		case 1: 				emCmd->reset = value; break;
		case 2: case 5: case 7: emCmd->start = value; break;
		case 3: 				emCmd->stop = value; break;
		case 4:					emCmd->hold = value; break;
		case 6:					emCmd->suspend = value; break;
		case 8:					emCmd->abort = value; break;
		case 9:					emCmd->clear = value; break;
		default: break;
	}
}

signed long cmdToState(signed long cmd) {
	signed long state = 0;
	switch(cmd) {
		case 1:	state = STATE_RESETTING; break;
		case 2:	state = STATE_STARTING; break;
		case 3:	state = STATE_STOPPING; break;
		case 4:	state = STATE_HOLDING; break;
		case 5:	state = STATE_UNHOLDING; break;
		case 6:	state = STATE_SUSPENDING; break;
		case 7:	state = STATE_UNSUSPENDING; break;
		case 8:	state = STATE_ABORTING; break;
		case 9:	state = STATE_CLEARING; break;
		default: break;
	}
	return state;
}

void handleProductOverflow(brdk_um_pml_a_pc_typ* arr) {
	unsigned short i;
	for(i=0; i<= BRDK_UM_MAX_PRODUCTS; i++)if(arr[i].Count < 0) arr[i].Count = 0;
}
