#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "trilist.h"
#include "resovle.h"
#include "string.h"


#define Period 1*72000000

 
/************************************************
���ߣ�
��;�����ư壬����1.���㣬2.�˹켣��3.��̬ά��
���ݸ�ʽ������Ƕ�1������Ƕ�2��������������Ƕ�12��mpu��̬�Ƕ�x��mpu��̬�Ƕ�y��mpu��̬�Ƕ�z��mpu��̬���ٶ�x��mpu��̬���ٶ�y��mpu��̬���ٶ�z
************************************************/

int preangle[12];
int nowangle[12];
int preanglex,preangley,preanglez;
int nowanglex,nowangley,nowanglez;
int preaccx,preaccy,preaccz;
int nowaccx,nowaccy,nowaccz;
int k,i,u;
float current_time;
float S;
float a = 308;
float Fa = 308;
float Ba = 308;
float SL = 180;
float SR = 180;
int angle = 0;
int distance = 0;
int upstairs = 0;



float RB[3];
float RF[3] = {0,0,0};
float FOOT[3];
float LB[3];
float LF[3];

float arr_arcsin[271];
float arr_arccos[271];
float arr_arctan[271];

int recieve_2to1[16];

int recieve_3to1[16];

int h = -190;

float b_LB[3]; 
float b_LF[3]; 
float b_RB[3]; 
float b_RF[3];

int FSM = 0;


void delay(double count)
{
	for(i=0;i<count;i++);

}


 int main(void)
 {	
	char str[100];
	int i;
 	u16 t;  
	u16 len;	
	u16 times=0;
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart1_init(9600);	 //���ڳ�ʼ��Ϊ115200
	tri_list_init();
	Initial_USART2(9600);
	TIM3_Int_Init(7199,7999); //Ƶ��=72000000/20000*7200=0.5hz
	 delay_init();
	 //��ʱ����ʼ��
//	 USART_SendData(USART1,0x01) ;
//	 while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
	 
 	while(1)
	{
//		control2elec();
		
			//current_time = TIM3->CNT;
//			current_time = TIM3->CNT/10000;
		
		
		
		
/////////////////////////////////////////////////////////////////////////////////////////		
			
			upstairs = 0;
		
			control2elec();
		
			current_time = TIM_GetCounter(TIM3); 
			current_time = current_time/3600;
		
		
			S = SL;
//			a = Fa;
			
			h = -190;
			
	  	RFwaycalc(current_time);
			LFcalc(1 * b_RF[0],1 * b_RF[1], b_RF[2]);
			LF[0] = (FOOT[0]/3.14)*180;								//x��200��z��171.61
			LF[1] = (FOOT[1]/3.14)*180;
			LF[2] = (FOOT[2]/3.14)*180;
			i++;
		
			
			h = -190;
			S = SR;
//			a = Ba;
			LFwaycalc(current_time);
			LFcalc(-1 * b_LF[0] + 10 + 0,-1 * b_LF[1], b_LF[2]);
			RB[0] = (FOOT[0]/3.14)*180;								//x��200��z��171.61
			RB[1] = (FOOT[1]/3.14)*180;
			RB[2] = (FOOT[2]/3.14)*180;
			i++;
			
			
			
			if(current_time >1)
			{
				current_time = current_time - 1;
			}
			else
			{
				current_time = current_time + 1;
			}
			
			
			h = -190;
			S = SR;
//			a = Fa;
			RFwaycalc(current_time);
			LFcalc(1 * b_RF[0],1 * b_RF[1], b_RF[2]);
			RF[0] = (FOOT[0]/3.14)*180;								//x��200��z��171.61
			RF[1] = (FOOT[1]/3.14)*180;
			RF[2] = (FOOT[2]/3.14)*180;
			i++;
		
			h = -190;
			S = SL;
//			a = Ba;
			LFwaycalc(current_time);
			LFcalc(-1 * b_LF[0] + 10,-1 * b_LF[1], b_LF[2]);
			LB[0] = (FOOT[0]/3.14)*180;								//x��200��z��171.61
			LB[1] = (FOOT[1]/3.14)*180;
			LB[2] = (FOOT[2]/3.14)*180;
			i++;	
			
//			if(FSM % 2)	
//			{
//				USART_SendData(USART1,0x01) ;
//				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
//				FSM = 0;

//			}		

				
		
/////////////////////////////////////////////////////////////////////////////////////////
			
//			control2elec();
//		
//			current_time = TIM_GetCounter(TIM3); 
//			current_time = current_time/3600;
//		
//		
//			S = SL;
//			
//	  	RFwaycalc1(current_time);
//			LFcalc(1 * b_RF[0],1 * b_RF[1], b_RF[2]);
//			LF[0] = (FOOT[0]/3.14)*180;								//x��200��z��171.61
//			LF[1] = (FOOT[1]/3.14)*180;
//			LF[2] = (FOOT[2]/3.14)*180;
//			i++;
//		
//			S = SR;
//			LFwaycalc1(current_time);
//			LFcalc(1 * b_LF[0],1 * b_LF[1], b_LF[2]);
//			RB[0] = (FOOT[0]/3.14)*180;								//x��200��z��171.61
//			RB[1] = (FOOT[1]/3.14)*180;
//			RB[2] = (FOOT[2]/3.14)*180;
//			i++;
//			
//			
//			
//			if(current_time >1)
//			{
//				current_time = current_time - 1;
//			}
//			else
//			{
//				current_time = current_time + 1;
//			}
//			
//			S = SR;
//			
//			RFwaycalc1(current_time);
//			LFcalc(1 * b_RF[0],1 * b_RF[1], b_RF[2]);
//			RF[0] = (FOOT[0]/3.14)*180;								//x��200��z��171.61
//			RF[1] = (FOOT[1]/3.14)*180;
//			RF[2] = (FOOT[2]/3.14)*180;
//			i++;
//		
//			
//			S = SL;
//			
//			LFwaycalc1(current_time);
//			LFcalc(1 * b_LF[0],1 * b_LF[1], b_LF[2]);
//			LB[0] = (FOOT[0]/3.14)*180;								//x��200��z��171.61
//			LB[1] = (FOOT[1]/3.14)*180;
//			LB[2] = (FOOT[2]/3.14)*180;
//			i++;
		



/////////////////////////////////////////////////////////////////////////////////







//////////////////////////////////////////////////////////////////////////////////
		
//			current_time = TIM_GetCounter(TIM3); 
//			current_time = current_time/3600;
//		
//		
//	  	RFwaycalc(current_time);
//			LFcalc(1 * b_RF[0],1 * b_RF[1], b_RF[2]);
//			LF[0] = 0;								//x��200��z��171.61
//			LF[1] = 0;
//			LF[2] = 0;
//			i++;
//		
//			RB[0] = 0;								//x��200��z��171.61
//			RB[1] = 0;
//			RB[2] = 0;
//			LB[0] = -90;								//x��200��z��171.61
//			LB[1] = 0;
//			LB[2] = 0;
//			RF[0] = -90;								//x��200��z��171.61
//			RF[1] = 0;
//			RF[2] = 0;
//			i++;
//			control2elec();
//			delay(4000000);
			
//			LF[0] = 0;								//x��200��z��171.61
//			LF[1] = 0;
//			LF[2] = 0;
//			i++;

//		
//			LFwaycalc(current_time);
//			LFcalc(-1 * b_LF[0],-1 * b_LF[1], b_LF[2]);
//			RB[0] = 0;								//x��200��z��171.61
//			RB[1] = 0;
//			RB[2] = 0;
//			LB[0] = 0;								//x��200��z��171.61
//			LB[1] = 0;
//			LB[2] = 0;
//			RF[0] = 0;								//x��200��z��171.61
//			RF[1] = 0;
//			RF[2] = 0;


//			control2elec();
//			delay(4000000);
///////////////////////////////////////////////////////////////

//			LBwaycalc(current_time);
//			LBcalc(1 * b_LB[0],1 * b_LB[1], b_LB[2]);
//			LB[0] = (LB[0]/3.14)*180;								//x��200��z��171.61
//			LB[1] = (LB[1]/3.14)*180;
//			LB[2] = (LB[2]/3.14)*180;
//			i++;
//		
//			RFwaycalc(current_time);
//			RFcaulc(1 * b_RF[0],-1 * b_RF[1], b_RF[2]);
//			RF[0] = (RF[0]/3.14)*180;								//x��200��z��171.61
//			RF[1] = (RF[1]/3.14)*180;
//			RF[2] = (RF[2]/3.14)*180;
//			i++;
//			
//			RBwaycalc(current_time);
//			RBcaulc(-1 * b_RB[0],1 * b_RB[1], b_RB[2]);
//			RB[0] = (RB[0]/3.14)*180;								//x��200��z��171.61
//			RB[1] = (RB[1]/3.14)*180;
//			RB[2] = (RB[2]/3.14)*180;
//			i++;
		
//			LFwaycalc(0.1);
//			LFcalc(-1 * b_LF[0],-1 * b_LF[1], b_LF[2]);
//			LF[0] = (LF[0]/3.14)*180;								//x��200��z��171.61
//			LF[1] = (LF[1]/3.14)*180;
//			LF[2] = (LF[2]/3.14)*180;
//			i++;

		
		
		
		
		
		
//		USART_SendData(USART2,0xff) ;
//		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
//		for(k=0; k<3; k++)	
//		{
//			USART_SendData(USART2,0x1f) ;
//			while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
//		}
//		for(k=0; k<3; k++)	
//		{			
//			USART_SendData(USART2,0x2f) ;
//			while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
//		}
//		for(k=0; k<3; k++)	
//		{
//			USART_SendData(USART2,0x3f) ;
//			while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
//		}
//		USART_SendData(USART2,i) ;
//		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
		
		
//		LFcalc(136.25,204.14,-136.64);//ƫ��ȣ�1 ||LF[0]=45 | LF[0]=45
//																		//   					LF[1]=30 | LF[1]=30
//																		//						LF[2]=60 | LF[2]=61.273
//		LFcalc(141,187.13,-119.63);//ƫ��ȣ�-3   ||LF[0]=45 | LF[0]=45
//																		//   					LF[1]=60 | LF[1]=60
//																		//						LF[2]=110| LF[2]=107.106232
//		LFcalc(164.91,196.89,-129.39);//ƫ��ȣ�3  ||LF[0]=45 | LF[0]=45
																		//   					LF[1]=60 | LF[1]=60
																		//						LF[2]=30 | LF[2]=30
//		LFcalc(231.86,192.75,-125.25);//ƫ��ȣ�0 ||LF[0]=45 | LF[0]=45
																		//   					LF[1]=60 | LF[1]=60
																		//						LF[2]=30 | LF[2]=30
//		LFcalc(127.28,87.5,-177.20);//ƫ��ȣ�0 ||LF[0]=45 | LF[0]=45
//		
//			LF[0] = (FOOT[0]/3.14)*180;								//x��200��z��171.61
//			LF[1] = (FOOT[1]/3.14)*180;
//			LF[2] = (FOOT[2]/3.14)*180;
//			i++;
																		// 					  LF[1]=30 | LF[1]=30
																		//					  LF[2]=90 | LF[2]=94
//		LFcalc(198,184.73,-117.23);//ƫ��ȣ�0 ||LF[0]=45 | LF[0]=45
																		// 					  LF[1]=90 | LF[1]=90
																		//					  LF[2]=80 | LF[2]=83.2561
																		
																		
																		
//			LFcalc(164.91,196.89,-129.39);
//																		
//			LF[0] = (LF[0]/3.14)*180;
//			LF[1] = (LF[1]/3.14)*180;
//			LF[2] = (LF[2]/3.14)*180;
//			i++;
		
//		sprintf(str,"Angle:%.4f,%.4f,%.4f times:%d\r\n",RF[0],RF[1],RF[2],i);
//		UART2_Put_String(str);		
//		delay_ms(10);//�ȴ��������
		
		
		
		
		
//		if(USART_RX_STA&0x8000)
//		{					   
//			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
//			printf("\r\n�����͵���ϢΪ:\r\n\r\n");
//			for(t=0;t<len;t++)
//			{
//				USART_SendData(USART1, USART_RX_BUF[t]);//�򴮿�1��������
//				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
//			}
//			printf("\r\n\r\n");//���뻻��
//			USART_RX_STA=0;
//		}else
//		{
//			times++;
//			if(times%5000==0)
//			{
//				printf("\r\nս��STM32������ ����ʵ��\r\n");
//				printf("����ԭ��@ALIENTEK\r\n\r\n");
//			}
//			if(times%200==0)printf("����������,�Իس�������\n");  
//			if(times%30==0)LED0=!LED0;//��˸LED,��ʾϵͳ��������.
//			delay_ms(10);   
//		}
		
	}	 
 }

