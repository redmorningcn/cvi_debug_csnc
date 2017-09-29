
#include <utility.h>
#include <ansi_c.h>
#include <cvirte.h>
#include <userint.h>

#include "csnc_debug.h"   

#include "cvi_com1.h"  
#include "cvi_csnc_debug.h"  
#include "cvi_config.h"   

/********************************************************************************************/
/* Constants										    */
/********************************************************************************************/
//uir�ļ���
#define		UIR_MAIN_NAME			"csnc_debug.uir"

/********************************************************************************************/
/* Globals										    */
/********************************************************************************************/
//������
stcSysCtrl	sCtrl;		


/********************************************************************************************/
/* Application entry point.																	*/
/********************************************************************************************/
int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)				/* Initialize CVI libraries */
		return -1;	/* out of memory */
	LoadCsncDebugPanel();
	/* Schedule two thread functions */				//���̣߳��������̺߳���
	cvi_InitBspComm(&sCtrl.PC);

	RunUserInterface ();

	cvi_CloseBspComm(&sCtrl.PC);					//�߳��˳� 							
	/*	Discard the loaded panels from memory */
	DiscardPanel (gCsncDebugPanelHandle);

	return 0;
}


