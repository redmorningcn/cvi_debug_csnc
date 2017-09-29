
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
//uir文件名
#define		UIR_MAIN_NAME			"csnc_debug.uir"

/********************************************************************************************/
/* Globals										    */
/********************************************************************************************/
//面板变量
stcSysCtrl	sCtrl;		


/********************************************************************************************/
/* Application entry point.																	*/
/********************************************************************************************/
int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)				/* Initialize CVI libraries */
		return -1;	/* out of memory */
	LoadCsncDebugPanel();
	/* Schedule two thread functions */				//多线程，创建新线程函数
	cvi_InitBspComm(&sCtrl.PC);

	RunUserInterface ();

	cvi_CloseBspComm(&sCtrl.PC);					//线程退出 							
	/*	Discard the loaded panels from memory */
	DiscardPanel (gCsncDebugPanelHandle);

	return 0;
}


