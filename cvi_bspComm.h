/*******************************************************************************
 *   Revised:        All copyrights reserved to redmorningcn.
 *   Revision:       v1.0
 *   Writer:	     redmorningcn.
 *
 *   Notes:
 *     				E-mail: redmorningcn@qq.com
 *
 *******************************************************************************/
#ifndef	CVI_BSPCOMM_H
#define	CVI_BSPCOMM_H

/********************************************************************************************/
/* Include files																			*/
/********************************************************************************************/
#include <utility.h>
#include <ansi_c.h>
#include <cvirte.h>
#include <userint.h>
#include <formatio.h>

#include "csnr_package_deal.h" 
#include "cvi_com1.h"   


/********************************************************************************************/
// Constants
/********************************************************************************************/
#define		COMM_DATA_BUF_SZIE		256
#define		MASTE_ADDR_HOST			0xCA

/********************************************************************************************/
// struct
/********************************************************************************************/
//���տ��ƽṹ��
struct stcRecvCtrl
{
	unsigned char		DestAddr;			//���յ�ַ		slave  =0xA1\A2
	unsigned char		SourceAddr; 		//Դ��ַ		master	 =0x80
	unsigned char		FramNum;			//֡���
	unsigned char		Len;				//������Ч���ݳ���
	unsigned char		FrameCode;			//֡������
	unsigned char		recvflg;			//���յ���Ч����
	unsigned char 		printflg;			//���ݴ�ӡ
	unsigned char		Tmp[1];
	unsigned int		DataCode;			//���տ�����
} ;

//���ӿ��ƽṹ��
struct stcConnCtrl
{
	unsigned char		ConnFlg;			//���ӿ���, 1���������ӣ�0������������
	unsigned char		RecvEndFlg; 		//���ձ�ʾ��1�����ݽ�����ɣ�0�������ݽ��ա�
	unsigned char		TimeOut;			//��ʱʱ�䣬��λ1s
	unsigned char		ErrFlg; 			//�����ʾ������������0�����Ӵ���1
	unsigned char		DestAddr;			//���յ�ַ	slave  = 0xCA
	unsigned char		sourceaddr; 		//Դ��ַ	master = 0x80
	unsigned char		SendFramNum;		//֡���
	unsigned char		SendFlg;			//���ͱ�ʾ�� �����ݷ��ͣ�1�������ݷ��ͣ�0
	unsigned char		FrameCode;			//֡������
	int					sendlen;			//�������ݳ���
} ;

//pc���ڿ���
typedef struct _stcThreadCommCtrl_
{
	unsigned char		wr[COMM_DATA_BUF_SZIE];			//������
	unsigned char		rd[COMM_DATA_BUF_SZIE];			//������
	struct 	stcRecvCtrl	RxCtrl;							//���տ���
	struct 	stcConnCtrl	ConnCtrl;						//���Ϳ���
	stcUartContrl		*sCom;							//���̶߳�Ӧ�Ĵ���
	int					ThreadID;						//����ID
	int					ThreadExit;						//�����ͷű�ʶ

} stcThreadCommCtrl;


/********************************************************************************************/
/* Globals																					*/
/********************************************************************************************/
//----------------------------------------------------------------------------
// ��    �ܣ� ��ʼ���շ��̡߳�Ϊʵ��com1��ͨѶ�����뿪���ú��������������߳�
// ��    ����
// ��    �ߣ� redmornigcn 20170927
//----------------------------------------------------------------------------
int	cvi_InitBspComm(stcThreadCommCtrl * T_Ctrl);

//----------------------------------------------------------------------------
// ��    �ܣ� �ر��շ��̡߳��˳��ļ�����ʱ�����˳��̣߳��ͷ���Դ��
// ��    ����
// ��    �ߣ� redmornigcn 20170927
//----------------------------------------------------------------------------
int	cvi_CloseBspComm(stcThreadCommCtrl * T_Ctrl);


/********************************************************************************************/
/* Prototypes																				*/
/********************************************************************************************/

#endif
