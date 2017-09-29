/*******************************************************************************
 *   Filename:       ansytime.c
 *   Revised:        All copyrights reserved to redmorningcn.
 *   Revision:       v1.0
 *   Writer:	     redmorningcn.
 *   Date:			 20170927
 *   Description:
 *   Notes:
 *     				E-mail: redmorningcn@qq.com
 *
 *******************************************************************************/

/********************************************************************************************/
/* Include files																			*/
/********************************************************************************************/
#include "asynctmr.h"
#include <utility.h>
#include <ansi_c.h>
#include <cvirte.h>
#include <userint.h>

#include "cvi_bspComm.h"		 				//����ͨѶ
#include "cvi_config.h"		 					//����ͨѶ   

/********************************************************************************************/
/* Constants																				*/
/********************************************************************************************/
//uir�ļ���

/********************************************************************************************/
/* Globals																					*/
/********************************************************************************************/


/********************************************************************************************/
/* local Prototypes																			*/
/********************************************************************************************/


//----------------------------------------------------------------------------
// ��    �ܣ� ���ڵ��ա�������.ͨ����ʶλ���̣߳����Ƴ����շ�Э��
// ��    ����
// ��    �ߣ� redmornigcn 20170927
//----------------------------------------------------------------------------
static int CVICALLBACK Thread_CommRecvAndSendCtrl (void *functionData)
{
	char recbuf[COMM_DATA_BUF_SZIE];
	char sendbuf[COMM_DATA_BUF_SZIE];
	int	 reclen;
	stcCsnrProtocolPara sCsncPara;		//csns�Y��

	while (!sCtrl.PC.ThreadExit)								//�̲߳��˳�
	{
//////////////////////////////////////////////////////////////////����
		if(cvi_GetComRecEndFlg(sCtrl.PC.sCom))					//�������ݣ�����ʾ
		{
			cvi_ClearComRecEndFlg(sCtrl.PC.sCom);				//��ս��ձ�ʶ
			reclen = cvi_ReadCom(sCtrl.PC.sCom,recbuf,sizeof(recbuf));	
																//ȡ��������

			sCsncPara.rxtxbuf 	= recbuf;						//ԭʼ����
			sCsncPara.rxtxlen 	= reclen;						//ԭʼ���ݳ���
			sCsncPara.databuf	= sCtrl.PC.rd;					//����������

			char	flg = 0;
			flg = DataUnpack_CSNC(&sCsncPara);					//�������

		    if ( MASTE_ADDR_HOST != sCsncPara.destaddr || flg == 0 ) //�������󡢵�ַ�����˳���
		    {
				sCtrl.PC.RxCtrl.recvflg = 0;					//����ʧ��
	        }
	        else
	        {
				sCtrl.PC.RxCtrl.SourceAddr 	= sCsncPara.sourceaddr; 			//Դ��ַ
				sCtrl.PC.RxCtrl.DestAddr	= sCsncPara.destaddr;				//Ŀ���ַ
				sCtrl.PC.RxCtrl.FramNum		= sCsncPara.framnum;				//���յ���֡��
				sCtrl.PC.RxCtrl.FrameCode	= (char)sCsncPara.framcode & (0x0f);//ָ���� ������װ��δ����
				sCtrl.PC.RxCtrl.Len			= sCsncPara.datalen;				//����������

				sCtrl.PC.RxCtrl.recvflg 	= 1;								//���ճɹ�( ��Ӧ�ò�ӿ� )
				sCtrl.PC.RxCtrl.printflg 	= 1;								//���ݴ�ӡ��CVI��
	        }
		}

////////////////////////////////////////////////////////////////���ݷ���
		if(
					sCtrl.PC.sCom->sConfig.open == 1			//�����Ѵ�
				&&  sCtrl.PC.ConnCtrl.SendFlg 	!= 0			//���ݷ��������ݷ���
		  )
		{
			sCsncPara.databuf	= sCtrl.PC.wr;					//����������
			sCsncPara.datalen	= (unsigned char)sCtrl.PC.ConnCtrl.sendlen;	//��������������
			sCsncPara.rxtxbuf 	= (uint8 *)sendbuf;				//

			sCsncPara.sourceaddr= sCtrl.PC.ConnCtrl.sourceaddr;	//��ַ
			sCsncPara.destaddr  = sCtrl.PC.ConnCtrl.DestAddr;
			sCsncPara.framnum   = sCtrl.PC.ConnCtrl.SendFramNum;
			sCsncPara.framcode	= sCtrl.PC.ConnCtrl.FrameCode;

			DataPackage_CSNC(&sCsncPara);						//��csncЭ����

			cvi_SendCom(sCtrl.PC.sCom, sCsncPara.rxtxbuf, sCsncPara.rxtxlen);//ͨ�����ڷ�������

			sCtrl.PC.ConnCtrl.SendFlg 	= 0;					//�÷���λ��Ч.���η��ͽ���()
		}
	}

	return 0;
}

//----------------------------------------------------------------------------
// ��    �ܣ� ��ʼ���շ��̡߳�Ϊʵ��com1��ͨѶ�����뿪���ú��������������߳�
// ��    ����
// ��    �ߣ� redmornigcn 20170927
//----------------------------------------------------------------------------
int	cvi_InitBspComm(stcThreadCommCtrl * T_Ctrl)
{
	if(T_Ctrl->ThreadID == 0)				//��ʼ���շ������߳�
	{
		T_Ctrl->sCom = &gsCom1Contrl;		//����1��������
		T_Ctrl->ThreadExit = 0;				//�̲߳��ͷ�

		cvi_OpenCom(T_Ctrl->sCom);			//�򿪸ø��̶߳�Ӧ�Ĵ���
		
		CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE,
							   Thread_CommRecvAndSendCtrl,		//���̴߳�����
							   NULL,
							   &T_Ctrl->ThreadID);
	}
	
	return	T_Ctrl->ThreadID;				//�����߳�ID
}

//----------------------------------------------------------------------------
// ��    �ܣ� �ر��շ��߳�
// ��    ����
// ��    �ߣ� redmornigcn 20170927
//----------------------------------------------------------------------------
int	cvi_CloseBspComm(stcThreadCommCtrl * T_Ctrl)
{
	if( T_Ctrl->ThreadID )					//�رղ��ͷ��߳�
	{
		T_Ctrl->ThreadExit = 1;				//�߳��˳������߳��ͷű�ʶ
		/* Wait for the thread functions to finish executing */
		CmtWaitForThreadPoolFunctionCompletion (DEFAULT_THREAD_POOL_HANDLE,
												T_Ctrl->ThreadID,
												OPT_TP_PROCESS_EVENTS_WHILE_WAITING);
		/* Release thread functions */
		CmtReleaseThreadPoolFunctionID (DEFAULT_THREAD_POOL_HANDLE, T_Ctrl->ThreadID);
		
		cvi_CloseCom(T_Ctrl->sCom);			//�򿪸ø��̶߳�Ӧ�Ĵ���
	}

	return	T_Ctrl->ThreadID;
}
