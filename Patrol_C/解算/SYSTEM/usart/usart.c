#include "sys.h"
#include "usart.h"	  
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/8/18
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
////////////////////////////////////////////////////////////////////////////////// 	  
u8 U2TxBuffer[258];
u8 U2TxCounter=0;
u8 U2count=0; 
u8 TxBuffer[258];
u8 TxCounter=0;
u8 count=0; 
u8 U1TxBuffer[258];
u8 U1TxCounter=0;
u8 U1count=0; 
int recieve_2to1ed[16];
extern int recieve_2to1[16];
int recieve_3to1ed[16];
extern int recieve_3to1[16];
int p,j;
u16 USART_RX_STA3;
extern float LF[3];
extern float RB[3];
extern float RF[3];
extern float LB[3];

extern int angle;
extern int distance;

extern float SL;
extern float SR;

extern int FSM;

extern int upstairs;




//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*ʹ��microLib�ķ���*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	  
  
void uart1_init(u32 bound){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 

}

void USART1_IRQHandler(void)                	//����1�жϷ������/////////////////////////////////////////////////////////
{
	 unsigned char data;
  if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
  {   
    /* Write one byte to the transmit data register */
    USART_SendData(USART1, U1TxBuffer[U1TxCounter++]);                    

    /* Clear the USART1 transmit interrupt */
    USART_ClearITPendingBit(USART1, USART_IT_TXE); 

    if(U1TxCounter == U1count)
    {
      /* Disable the USART1 Transmit interrupt */
      USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
    }    
  }else if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
		data = USART_ReceiveData(USART1);	//��ȡ���յ�������		  		 
		
		if (data == 0x7f)
		{						
		}
		if (data == 0xff)
		{						
		}
		else if (data > 0x80)
		{
			angle = data;
		}
		else if (data > 0x00)
		{
			distance = data;
		}
		
		stepadjust(angle);
		USART_SendData(USART1,0x01) ;
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
		FSM++;
		
  /* Clear the USART1 transmit interrupt */
		USART_ClearITPendingBit(USART1,USART_IT_ORE);
  }
}

#endif	

void USART2_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;        

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //USART2 TX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //??????;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); //??C;
			
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //USART2 RX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //????;
	GPIO_Init(GPIOA, &GPIO_InitStructure); //??C;

	USART_InitStructure.USART_BaudRate = 9600; //???;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //???8?;
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //???1?;
	USART_InitStructure.USART_Parity = USART_Parity_No ; //????;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	//?????;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//????;
	USART_Init(USART2, &USART_InitStructure);//??????;
}


void Initial_USART2(u32 baudrate)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* ʹ�� USART2 ģ���ʱ��  ʹ�� USART2��Ӧ�����Ŷ˿�PA��ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE );

  	 /* ����USART2 �ķ�������
	 ����PA9 Ϊ�������  ˢ��Ƶ��50MHz
	  */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);    
  	/* 
	  ����USART2 �Ľ�������
	  ����PA10Ϊ�������� 
	 */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
	/* 
	  USART2������:
	  1.������Ϊ���ó���ָ�������� baudrate;
	  2. 8λ����			  USART_WordLength_8b;
	  3.һ��ֹͣλ			  USART_StopBits_1;
	  4. ����żЧ��			  USART_Parity_No ;
	  5.��ʹ��Ӳ��������	  USART_HardwareFlowControl_None;
	  6.ʹ�ܷ��ͺͽ��չ���	  USART_Mode_Rx | USART_Mode_Tx;
	 */
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//Ӧ�����õ�USART2
	USART_Init(USART2, &USART_InitStructure); 
	USART_ITConfig(USART2, USART_IT_TXE, DISABLE);        
  USART_ClearFlag(USART2,USART_FLAG_TC);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	//ʹ�ܽ����ж�
	//����USART2
  	USART_Cmd(USART2, ENABLE);
	
}
void U2NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
		/* Enable the USART1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

volatile unsigned char RC_Flag;
//------------------------------------------------------
void USART2_IRQHandler(void)
{
  unsigned char data;
  if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
  {   
    /* Write one byte to the transmit data register */
    USART_SendData(USART2, U2TxBuffer[U2TxCounter++]);                    

    /* Clear the USART1 transmit interrupt */
    USART_ClearITPendingBit(USART2, USART_IT_TXE); 

    if(U2TxCounter == U2count)
    {
      /* Disable the USART1 Transmit interrupt */
      USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
    }    
  }else if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  {
		data = USART_ReceiveData(USART2);	//��ȡ���յ�������
		
		if (USART_RX_STA!=0)
		{
			recieve_2to1ed[USART_RX_STA-1] = data;
			USART_RX_STA++;
		}
			
		if(data == 0xff)//���տ�ʼ
		{
			USART_RX_STA = 1;
		}	
			
			
			if(data == 0x00)//�������
			{
				if(USART_RX_STA == 8)
				{
					for(p=0; p<6; p++) //������ȷ��ʼת¼
					{
						recieve_2to1[p] = recieve_2to1ed[p];
					}
				}
					else USART_RX_STA=0;	//��������� 
			}
				
			
		  		 
		
		
  /* Clear the USART1 transmit interrupt */
  USART_ClearITPendingBit(USART2, USART_IT_RXNE);
  }
}

void UART2_Put_Char(char DataToSend)
{
	//��Ҫ���͵��ֽ�д��UART1�ķ��ͻ�����
	//USART_SendData(USART1, (unsigned char) DataToSend);
	//�ȴ��������
  	//while (!(USART1->SR & USART_FLAG_TXE));

	  U2TxBuffer[count++] = DataToSend;  
    USART_ITConfig(USART2, USART_IT_TXE, ENABLE);  
}

void UART2_Put_String(char *Str)
{
	//�ж�Strָ��������Ƿ���Ч.
	while(*Str){
	//�Ƿ��ǻس��ַ� �����,������Ӧ�Ļس� 0x0d 0x0a
	if(*Str=='\r')UART2_Put_Char(0x0d);
		else if(*Str=='\n')UART2_Put_Char(0x0a);
			else UART2_Put_Char(*Str);
	Str++;
	}
}




void control2elec()
{
		USART_SendData(USART2,0xff) ;
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
		for(j=2; j>=0; j--)	
		{
			if(LF[j] == 0x00)
			{
				LF[j] = 0x01;
			}
			if(LF[j] == 0xff)
			{
				LF[j] = 0xfe;
			}
			USART_SendData(USART2,LF[j]) ;
//			USART_SendData(USART2,0x01) ;
			while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
		}	
		
				if(RF[1] == 0x00)
				{
					RF[1] = 0x01;
				}
				if(RF[1] == 0xff)
				{
					RF[1] = 0xfe;
				}
				USART_SendData(USART2,RF[1]) ;
			while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
				
				if(RF[2] == 0x00)
				{
					RF[2] = 0x01;
				}
				if(RF[2] == 0xff)
				{
					RF[2] = 0xfe;
				}
				USART_SendData(USART2,RF[2]) ;
			while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
				
				if(RF[0] == 0x00)
				{
					RF[0] = 0x01;
				}
				if(RF[0] == 0xff)
				{
					RF[0] = 0xfe;
				}
				USART_SendData(USART2,RF[0]) ;
			while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);

			for(j=2; j>=0; j--)	
			{
				if(RB[j] == 0x00)
				{
					RB[j] = 0x01;
				}
				if(RB[j] == 0xff)
				{
					RB[j] = 0xfe;
				}
				USART_SendData(USART2,RB[j]) ;
			while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
		}
			for(j=2; j>=0; j--)	
			{
				if(LB[j] == 0x00)
				{
					LB[j] = 0x01;
				}
				if(LB[j] == 0xff)
				{
					LB[j] = 0xfe;
				}
			USART_SendData(USART2,LB[j]) ;
			while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
		}
				if (upstairs == 0)
		{
			USART_SendData(USART2,0x01) ;
			while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
		}	
		else if (upstairs == 1)
		{
	
			USART_SendData(USART2,0xfe) ;
			while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
		
		}	
		USART_SendData(USART2,0x00) ;
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
}

void stepadjust(float angle)
{
	float D;
	if (angle>=63+128)
	{		
		D = (angle - 63 - 128) * K;
		if (D>180)
		{
			D = 180;
		}
		SL = 180;
		SR = 180 - D;
	}
	if (angle<63+128)
	{
		D = (63 - angle + 128) * K;
		if (D>180)
		{
			D = 180;
		}
		SR = 180;
		SL = 180 - D;
	}

}



