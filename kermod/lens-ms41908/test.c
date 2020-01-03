#include <stdio.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <signal.h>
#include <sys/select.h>
#include <pthread.h>
#include <sys/mman.h> 

#define LENS_START		0x1 //��ʼ������ƴ���
#define LENS_MOTORCTRL			0x3 //	�������
#define LENS_REGSET			0x4	//�Ĵ���ֵ����
#define LENS_REGGET			0x5	//�Ĵ���ֵ��ȡ

#define LOW 0
#define HIGH 1
#define IRISINSPIREPIN	7,5 /*41908��Ȧ��������*/
#define VDFZINSPIREPIN  7,6/*41908VD_FZ��������*/
#define RESETPIN        7,4/*41908оƬ��λ����*/
#define SPICSPIN        1,5/*41908Ƭѡ����*/

#define GPIOCONFIG_IOCSET		0x0
#define GPIOCONFIG_IOCGET		0x1
#define GPIOCONFIG_IOCDIR		0x2

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

/*
*	���ݵ�GPIO ��Ϣ
*	gio_id��IO���
*	value��IO ��״ֵ̬
*/
struct gpio_info
{
	unsigned short	 gio_id;
	char value;
};

void input_handler(int sig)
{
	static struct timeval now,before;
	gettimeofday(&now, NULL);//��ȡ��ǰʱ�䣬��/ ΢��������
	
	unsigned int nDiff = now.tv_sec*1000+ now.tv_usec/1000 - before.tv_sec*1000-before.tv_usec/1000;
	
	if(nDiff <39 || nDiff > 42)
	printf("before = %ld -----now = %ld ms-------nDiff = %d\n",before.tv_sec*1000+ before.tv_usec/1000,now.tv_sec*1000+ now.tv_usec/1000,nDiff); 
	
	before = now;
}

/*****************************************************************
  Function: 	HAL_LENS_GpioSet
  Description:	gpio״̬���ú���
  Input:		cValue:���õ�ֵ cGroup��gpio������ cId��gpio���ź� 
  Output:		��
  Return:		�����ɹ����򷵻�0�����򷵻�<0			
******************************************************************/
int HAL_LENS_GpioSet(char cValue,char cGroup,char cId)
{	
	int nGpioFd = -1;
	int nRet;
	struct gpio_info gpio;
	
	gpio.gio_id = (cGroup<<8)|(cId);
	gpio.value = cValue;	

	nGpioFd = open("/dev/gpioconfig", O_RDWR, 0);
	if(nGpioFd < 0)
	{
		perror("#open dev");
		return -1;
	}
	
	nRet = ioctl(nGpioFd, GPIOCONFIG_IOCSET, &gpio);
	if(nRet < 0)
	{
		perror("HAL_LENS_GpioSet #ioctl");
	}

	close(nGpioFd);
	
	return 0;
	
}

/*****************************************************************
  Function: 	HAL_LENS_DelayMs
  Description:	ms��ʱ����
  Input:		ms ��ʱʱ��  
  Output:		��
  Return:				
******************************************************************/

static void HAL_LENS_DelayMs(int ms) 
{ 
	usleep(ms*1000);
}

/*****************************************************************
  Function: 	HAL_LENS_IrisInspire
  Description:	 MS41908��Ȧ�����ź�
  Input:		��
  Output:		��
  Return:		�����ɹ����򷵻�0�����򷵻�<0			
******************************************************************/
static int HAL_LENS_IrisInspire(void)
{
	HAL_LENS_GpioSet(LOW,IRISINSPIREPIN);
	HAL_LENS_DelayMs(1);
	HAL_LENS_GpioSet(HIGH,IRISINSPIREPIN);
	HAL_LENS_DelayMs(1);
	HAL_LENS_GpioSet(LOW,IRISINSPIREPIN);
	HAL_LENS_DelayMs(20);
 
	return 0;
}
#ifdef 1
int main()
{
		
	LENS_CTRL_ST stCtrl;
	
	int fd = 0,nRet;
	
	fd = open("/dev/LensDrv", O_RDWR, 0);
	if(fd < 0)
	{
		perror("#open dev");
		return ;
	}
	
	while(1)
	{
		stCtrl.usRegAddr = 0x00;
		nRet = ioctl(fd, LENS_REGGET, &stCtrl);
		if(nRet < 0)
		{
			perror("LENS_REGGET #ioctl");
			break;
		}
		printf("LENS_REGGET,usRegAddr:%d,usRegValue:%d\n",stCtrl.usRegAddr,stCtrl.usRegValue);
		sleep(1);

		stCtrl.usRegAddr = 0x00;
		if (stCtrl.usRegValue)
			stCtrl.usRegValue = 0;
		else
			stCtrl.usRegValue = 300;

		nRet = ioctl(fd, LENS_REGSET, &stCtrl);
		if(nRet < 0)
		{
			perror("LENS_REGSET #ioctl");
			break;
		}
		HAL_LENS_IrisInspire();
		printf("LENS_REGSET,usRegAddr:%d,usRegValue:%d\n",stCtrl.usRegAddr,stCtrl.usRegValue);
		sleep(1);
	}
	

	close(fd);

	return 0;
}
#else
int main()
{
		
	int oflags;
	
	int fd = 0;
	
	fd = open("/dev/LensDrv", O_RDWR, 0);
	if(fd < 0)
	{
		perror("#open dev");
		return ;
	}

	
	//�����ź���������,��SIGIO�ź�ͬinput_handler������������,һ������SIGIO�ź�,�ͻ�ִ��input_handler 
	signal(SIGIO, input_handler);	
	//STDIN_FILENO�Ǵ򿪵��豸�ļ�������,F_SETOWN�������������Ǹ�ʲô��,getpid()�Ǹ�ϵͳ���ã�
	
	//�����Ƿ��ص�ǰ���̵Ľ��̺�,���������Ĺ�����STDIN_FILENO��������豸�ļ���ӵ����Ϊ��ǰ���̡�
	fcntl(fd, F_SETOWN, getpid());    

	//�õ����ļ���������״̬
	oflags = fcntl(fd, F_GETFL);
	//�����ļ���������״̬Ϊoflags | FASYNC����,һ���ļ������������óɾ���FASYNC���Ե�״̬��
	//Ҳ���ǽ��豸�ļ��л����첽����ģʽ����ʱϵͳ�ͻ��Զ��������������fasync������
	fcntl(fd, F_SETFL, oflags | FASYNC);  
	
	
	while(1)
	{
		usleep(1000);
	}
	

	close(fd);

	return 0;
}
#endif
