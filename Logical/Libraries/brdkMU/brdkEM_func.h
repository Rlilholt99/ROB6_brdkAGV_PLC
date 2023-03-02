#ifndef BRDK_EM_FUNCS
#define BRDK_EM_FUNCS 1
     
#include <brdkMU_func.h>
#include <stdarg.h>

void brdkEMInitialize(brdkEM_typ* this, brdkEM_typ* parent);
brdk_em_color_typ flashColor(brdkEM_typ* this, brdk_em_color_typ color);
void lightTree(brdkEM_typ* this, brdk_em_color_typ color1, brdk_em_color_typ color2);
unsigned short RegisterChild(brdkEM_typ* this, brdkEM_typ* parent);
unsigned short UnregisterChild(brdkEM_typ* this, brdkEM_typ* parent);
void StateToA(char* pStateInfo, brdk_em_states_typ state);
void checkChildren(brdkEM_typ* this, brdk_em_states_typ STATE);
int parentStateOK(brdkEM_typ* this);
void observeChildrensState(brdkEM_typ* this, int numArgs, ...);
void observeParentState(brdkEM_typ* this, brdkEM_typ* parent, int numArgs, ...);
void checkCommands(brdkEM_typ* this, brdkEM_typ* parent, int numArgs, ...);
void setSubStateText(brdkEM_typ* this, unsigned long pStr);
void setChildrenMode(brdkEM_typ* this);
void findParent(brdkEM_typ* this, brdkEM_typ* parent, brdk_em_states_typ cmd, int numArgs, ...);
     
#endif /* !BRDK_EM_FUNCS */


