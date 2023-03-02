#include <brdkEM_func.h>

unsigned long brdkEMStateToA(brdk_em_states_typ state) {
	unsigned long pStr;
	switch(state) {
		case STATE_DEACTIVATED: 	pStr = (unsigned long)&"STATE_DEACTIVATED"; 	break;	/* 0 */
		case STATE_CLEARING: 		pStr = (unsigned long)&"STATE_CLEARING"; 		break;	/* 1 */
		case STATE_STOPPED: 		pStr = (unsigned long)&"STATE_STOPPED"; 		break;	/* 2 */
		case STATE_STARTING: 		pStr = (unsigned long)&"STATE_STARTING"; 		break;	/* 3 */
		case STATE_IDLE: 			pStr = (unsigned long)&"STATE_IDLE"; 			break;	/* 4 */
		case STATE_SUSPENDED: 		pStr = (unsigned long)&"STATE_SUSPENDED"; 		break;	/* 5 */
		case STATE_EXECUTE: 		pStr = (unsigned long)&"STATE_EXECUTE"; 		break;	/* 6 */
		case STATE_STOPPING: 		pStr = (unsigned long)&"STATE_STOPPING"; 		break;	/* 7 */
		case STATE_ABORTING: 		pStr = (unsigned long)&"STATE_ABORTING"; 		break;	/* 8 */
		case STATE_ABORTED: 		pStr = (unsigned long)&"STATE_ABORTED"; 		break;	/* 9 */
		case STATE_HOLDING: 		pStr = (unsigned long)&"STATE_HOLDING"; 		break;	/* 10 */
		case STATE_HELD: 			pStr = (unsigned long)&"STATE_HELD"; 			break;	/* 11 */
		case STATE_UNHOLDING: 		pStr = (unsigned long)&"STATE_UNHOLDING"; 		break;	/* 12 */
		case STATE_SUSPENDING: 		pStr = (unsigned long)&"STATE_SUSPENDING"; 		break;	/* 13 */
		case STATE_UNSUSPENDING: 	pStr = (unsigned long)&"STATE_UNSUSPENDING"; 	break;	/* 14 */
		case STATE_RESETTING: 		pStr = (unsigned long)&"STATE_RESETTING"; 		break;	/* 15 */
		case STATE_COMPLETING: 		pStr = (unsigned long)&"STATE_COMPLETING"; 		break;	/* 16 */
		case STATE_COMPLETE: 		pStr = (unsigned long)&"STATE_COMPLETE"; 		break;	/* 17 */
		case STATE_DEACTIVATING: 	pStr = (unsigned long)&"STATE_DEACTIVATING"; 	break;	/* 18 */
		case STATE_ACTIVATING: 		pStr = (unsigned long)&"STATE_ACTIVATING"; 		break;	/* 19 */
	}
	return pStr;
}
