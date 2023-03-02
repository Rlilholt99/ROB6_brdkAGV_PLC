#ifndef BRDK_UM_FUNCS
#define BRDK_UM_FUNCS 1

#include <brdkMU_func.h>
     
void setCommand(signed long ctrlCmd, brdk_em_command_typ* emCmd, unsigned char value);
signed long cmdToState(signed long cmd);
void handleProductOverflow(brdk_um_pml_a_pc_typ* arr);

#endif /* !BRDK_UM_FUNCS */


