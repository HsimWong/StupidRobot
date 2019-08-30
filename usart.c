#include "sys.h"
#include "usart.h"	  
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/8/18
//版本：V1.5
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
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
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*使用microLib的方法*/
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
 
#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  
  
void uart1_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1 

}

void USART1_IRQHandler(void)                	//串口1中断服务程序/////////////////////////////////////////////////////////
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
		data = USART_ReceiveData(USART1);	//读取接收到的数据		  		 
		
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

	/* 使能 USART2 模块的时钟  使能 USART2对应的引脚端口PA的时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE );

  	 /* 配置USART2 的发送引脚
	 配置PA9 为复用输出  刷新频率50MHz
	  */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);    
  	/* 
	  配置USART2 的接收引脚
	  配置PA10为浮地输入 
	 */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
	/* 
	  USART2的配置:
	  1.波特率为调用程序指定的输入 baudrate;
	  2. 8位数据			  USART_WordLength_8b;
	  3.一个停止位			  USART_StopBits_1;
	  4. 无奇偶效验			  USART_Parity_No ;
	  5.不使用硬件流控制	  USART_HardwareFlowControl_None;
	  6.使能发送和接收功能	  USART_Mode_Rx | USART_Mode_Tx;
	 */
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//应用配置到USART2
	USART_Init(USART2, &USART_InitStructure); 
	USART_ITConfig(USART2, USART_IT_TXE, DISABLE);        
  USART_ClearFlag(USART2,USART_FLAG_TC);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	//使能接收中断
	//启动USART2
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
		data = USART_ReceiveData(USART2);	//读取接收到的数据
		
		if (USART_RX_STA!=0)
		{
			recieve_2to1ed[USART_RX_STA-1] = data;
			USART_RX_STA++;
		}
			
		if(data == 0xff)//接收开始
		{
			USART_RX_STA = 1;
		}	
			
			
			if(data == 0x00)//接收完成
			{
				if(USART_RX_STA == 8)
				{
					for(p=0; p<6; p++) //接收正确开始转录
					{
						recieve_2to1[p] = recieve_2to1ed[p];
					}
				}
					else USART_RX_STA=0;	//接收完成了 
			}
				
			
		  		 
		
		
  /* Clear the USART1 transmit interrupt */
  USART_ClearITPendingBit(USART2, USART_IT_RXNE);
  }
}

void UART2_Put_Char(char DataToSend)
{
	//将要发送的字节写到UART1的发送缓冲区
	//USART_SendData(USART1, (unsigned char) DataToSend);
	//等待发送完成
  	//while (!(USART1->SR & USART_FLAG_TXE));

	  U2TxBuffer[count++] = DataToSend;  
    USART_ITConfig(USART2, USART_IT_TXE, ENABLE);  
}

void UART2_Put_String(char *Str)
{
	//判断Str指向的数据是否有效.
	while(*Str){
	//是否是回车字符 如果是,则发送相应的回车 0x0d 0x0a
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



