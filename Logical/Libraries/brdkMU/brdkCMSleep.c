#include <brdkMU_func.h>

signed long brdkCMSleep(unsigned long pCM) {
	brdkCM_typ* this;
	if(pCM != 0) {
		this = (brdkCM_typ*)pCM;
		this->status.sleeping = true;
		if(this->setup.status.last) {
			brdkStrMemMove((unsigned long)this->status.last[1],(unsigned long)this->status.last[0], sizeof(this->status.last[0])*BRDK_MU_MAX_LAST_SIZE);
			brdkStrCpy((unsigned long)this->status.last[0],(unsigned long)"module put to sleep");
		}
		return sleepMU(this->internal.muList.idx, pCM);
	}
	return -1;
}
