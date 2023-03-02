#include <brdkEM_func.h>

unsigned long brdkEMModeToA(signed long mode, unsigned long pEM) {
	brdkEM_typ* this;
	unsigned long pStr;
	unsigned char i;
	switch(mode) {
		case MODE_UNDEFINED: 	pStr = (unsigned long)&"MODE_UNDEFINED"; 	break;	/* 0 */
		case MODE_PRODUCING: 	pStr = (unsigned long)&"MODE_PRODUCING"; 	break;	/* 1 */
		case MODE_MAINTENANCE: 	pStr = (unsigned long)&"MODE_MAINTENANCE"; 	break;	/* 2 */
		case MODE_MANUAL: 		pStr = (unsigned long)&"MODE_MANUAL"; 		break;	/* 3 */
		case MODE_CLEAN: 		pStr = (unsigned long)&"MODE_CLEAN"; 		break;	/* 16 */
		case MODE_JOG: 			pStr = (unsigned long)&"MODE_JOG"; 			break;	/* 17 */
		case MODE_CIP: 			pStr = (unsigned long)&"MODE_CIP"; 			break;	/* 18 */
		case MODE_CALIBRATING: 	pStr = (unsigned long)&"MODE_CALIBRATING"; 	break;	/* 19 */
		default:
			pStr = (unsigned long)&"UNKNOWN_USER_MODE";
			if(pEM != 0) {
				this = (brdkEM_typ*)pEM;
				for(i=0; i < BRDK_EM_MAX_CUSTOM_MODES; i++) {
					if(mode == this->setup.customModeNames[i].mode) {
						pStr = (unsigned long)&this->setup.customModeNames[i].modeName;
						break;
					}
				}
			}
			break;
	}
	return pStr;
}
