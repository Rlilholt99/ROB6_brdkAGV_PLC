#include <brdkMU_func.h>

signed long brdkCMChangeParent(unsigned long pCM, unsigned long pParentName, signed long parentIndex) {
	signed long tmpIdx = 0;
	brdk_mu_setup_module_typ module;
	brdkCM_typ* this;
	if(pCM != 0 && pParentName != 0 && emListReady) {
		this = (brdkCM_typ*)pCM;
		brdkStrCpy((unsigned long)module.name,pParentName);
		module.index = parentIndex;
		tmpIdx = getMUIndex((unsigned long)this, &module);
		if(tmpIdx != -1) {
			this->internal.muList.parentIdx = tmpIdx;
			brdkStrCpy((unsigned long)this->setup.parent.name,(unsigned long)module.name);
			this->setup.parent.index = module.index;
			return this->internal.muList.parentIdx;
		}
	}
	return -1; 
}
