#ifndef BRDK_MU_FUNCS
#define BRDK_MU_FUNCS 1

#include <brdkMU.h>
#include <bur/plctypes.h>

#ifdef __cplusplus
	extern "C"
	{
#endif

#ifdef __cplusplus
	};
#endif

#define false 0
#define true 1
#define NULL 0

extern SemCreate_typ SemCreateMUList;
extern unsigned char emListReady;

signed long addToMUList(unsigned long this, unsigned char type);
signed long getMUIndex(unsigned long this, brdk_mu_setup_module_typ* module);
brdkEM_typ* getEm(signed long idx);
signed long sleepMU(signed long idx, unsigned long this);
signed long wakeMU(signed long idx, unsigned long this);
unsigned long getCycleTime();
void updateSiblingInfo(signed long (*idxArr)[], brdk_mu_status_module_typ* status);
unsigned char moduleSleeping(signed long idx);

#endif /* !BRDK_MU_FUNCS */
