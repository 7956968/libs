#ifndef __LENS_DRV__
#define __LENS_DRV__

#include "lensProc.h"

#define LENS_START		0x1 //��ʼ������ƴ���
#define LENS_MOTORCTRL			0x3 //	�������
#define LENS_REGSET			0x4	//�Ĵ���ֵ����
#define LENS_REGGET			0x5	//�Ĵ���ֵ��ȡ
#define LENS_PI_LED_ON		0x6	//PI LED �򿪣�������PI
#define LENS_PI_LED_OFF		0x7	//PI LED �ر�
#define LENS_PRINTF			0X10 //��ӡ����

#define MEMDEV_SIZE 4096
typedef enum
{
	LENS_ZOOMTELE  = 1,
	LENS_ZOOMWIDE = 2,
	LENS_FOCUSNEAR = 3,
	LENS_FOCUSFAR  = 4,
	LENS_CTRLBUTT,
} LENS_CTRL_E;

typedef enum 
{
	LENS_MOTORRUN = 1,
	LENS_MOTORSTOP = 2,
	LENS_DISABLE = 3,
	LENS_IDLE = 4,
	LENS_MOTORBUTT,
}LENS_STATE_E;

typedef struct lensCtrl
{
	LENS_CTRL_E eZoomCtrlType;
	LENS_CTRL_E eFocusCtrlType;
	unsigned int unZoomStep;
	unsigned int unFocusStep;
	unsigned short usRegAddr;
	unsigned short usRegValue;
}LENS_CTRL_ST;

struct mem_dev                                     
 {                                                        
   char *data;                      
   unsigned long size;       
 };
int LENS_GetInfo(PROCLENSINFO_OBJ *pLensInfo);

#endif
