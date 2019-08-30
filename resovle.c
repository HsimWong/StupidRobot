#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "resovle.h"
#include "math.h"

extern float S,a;

extern int u;
extern int h;

extern float RB[3];
float RBfai1, RBfai2, RBfai3, RBfai;

float RBfai1cailc(float x, float y, float z)
{
	return atan(-1*(y-RByref)/(z-RBzref));
}

void RBfaicaulc(float x, float y, float z)
{
	
	float numerator, denominator, fraction;
	RBfai1 = RBfai1cailc(x,y,z);
	numerator = (RBL2S +(x - RBxref)*(x - RBxref) + (y - RByref - RBL1*sin(RBfai1)) * 
							(y - RByref - RBL1*sin(RBfai1)) + (z - RBzref + RBL1*cos(RBfai1)) * 
							(z - RBzref + RBL1*cos(RBfai1)) - RBL3*RBL3);
	denominator = 2*RBL2*sqrt((x - RBxref)*(x - RBxref) + 
							(y - RByref - RBL1*sin(RBfai1)) * (y - RByref - RBL1*sin(RBfai1)) + 
							(z - RBzref + RBL1*cos(RBfai1)) * (z - RBzref + RBL1*cos(RBfai1)));
	fraction = numerator/denominator;
	RBfai = acos(fraction);
}

float RBfai2cailc(float x, float y, float z)
{
	return -atan((x - RBxref)/(sqrt((y - RByref)*(y - RByref)+(z - RBzref)*(z - RBzref))-RBL1))+RBfai;
}

float RBfai3cailc(float x, float y, float z)
{
	return ((RBL2+RBL3)/RBL3)*RBfai;
}

void RBcaulc(float x, float y, float z)
{
	RBfaicaulc(x,y,z);
	RB[0] = RBfai1cailc(x,y,z);
	RB[1] = RBfai2cailc(x,y,z);
	RB[2] = RBfai3cailc(x,y,z);
}


extern float RF[3];
float RFfai1, RFfai2, RFfai3, RFfai;

float RFfai1cailc(float x, float y, float z)
{
	return atan(-1*(y-RFyref)/(z-RFzref));
}

void RFfaicaulc(float x, float y, float z)
{
	
	float numerator, denominator, fraction;
	RFfai1 = RFfai1cailc(x,y,z);
	numerator = (RFL2S +(x - RFxref)*(x - RFxref) + (y - RFyref - RFL1*sin(RFfai1)) * 
							(y - RFyref - RFL1*sin(RFfai1)) + (z - RFzref + RFL1*cos(RFfai1)) * 
							(z - RFzref + RFL1*cos(RFfai1)) - RFL3*RFL3);
	denominator = 2*RFL2*sqrt((x - RFxref)*(x - RFxref) + 
							(y - RFyref - RFL1*sin(RFfai1)) * (y - RFyref - RFL1*sin(RFfai1)) + 
							(z - RFzref + RFL1*cos(RFfai1)) * (z - RFzref + RFL1*cos(RFfai1)));
	fraction = numerator/denominator;
	RFfai = acos(fraction);
}

float RFfai2cailc(float x, float y, float z)
{
	return atan((x - RFxref)/(sqrt((y - RFyref)*(y - RFyref)+(z - RFzref)*(z - RFzref))-RFL1))+RFfai;
}

float RFfai3cailc(float x, float y, float z)
{
	return (RFL2+RFL3)/RFL3*RFfai;
}

void RFcaulc(float x, float y, float z)
{
	RFfaicaulc(x,y,z);
	RF[0] = RFfai1cailc(x,y,z);
	RF[1] = RFfai2cailc(x,y,z);
	RF[2] = RFfai3cailc(x,y,z);
}

float LBfai1, LBfai2, LBfai3, LBfai;
extern float LB[3];

float LBfai1cailc(float x, float y, float z)
{
	return atan(-1*(y-LByref)/(z-LBzref));
}

void LBfaicaulc(float x, float y, float z)
{
	
	float numerator, denominator, fraction;
	LBfai1 = LBfai1cailc(x,y,z);
	numerator = (LBL2S +(x - LBxref)*(x - LBxref) + (y - LByref - LBL1*sin(LBfai1)) * 
							(y - LByref - LBL1*sin(LBfai1)) + (z - LBzref + LBL1*cos(LBfai1)) * 
							(z - LBzref + LBL1*cos(LBfai1)) - LBL3*LBL3);
	denominator = 2*LBL2*sqrt((x - LBxref)*(x - LBxref) + 
							(y - LByref - LBL1*sin(LBfai1)) * (y - LByref - LBL1*sin(LBfai1)) + 
							(z - LBzref + LBL1*cos(LBfai1)) * (z - LBzref + LBL1*cos(LBfai1)));
	fraction = numerator/denominator;
	LBfai = acos(fraction);
}

float LBfai2cailc(float x, float y, float z)
{
	return -atan((x - LBxref)/(sqrt((y - LByref)*(y - LByref)+(z - LBzref)*(z - LBzref))-LBL1))+LBfai;/////////////////////////
}

float LBfai3cailc(float x, float y, float z)
{
	return (LBL2+LBL3)/LBL3*LBfai;
}

void LBcalc(float x, float y, float z)
{
	
	
	LBfaicaulc(x,y,z);
	LB[0] = LBfai1cailc(x,y,z);
	LB[1] = LBfai2cailc(x,y,z);
	LB[2] = LBfai3cailc(x,y,z);
	
	
}	

float LFfai1, LFfai2, LFfai3, LFfai;
extern float FOOT[3];

float LFfai1cailc(float x, float y, float z)
{
	return atan(-1*(y-87.5)/(z+20));
}

void LFfaicaulc(float x, float y, float z)
{
	
	float numerator, denominator, fraction;
	LFfai1 = LFfai1cailc(x,y,z);
	numerator = (LFL2S +(x - LFxref)*(x - LFxref) + (y - LFyref - LFL1*sin(LFfai1)) * 
							(y - LFyref - LFL1*sin(LFfai1)) + (z - LFzref + LFL1*cos(LFfai1)) * 
							(z - LFzref + LFL1*cos(LFfai1)) - LFL3*LFL3);
	denominator = 2*LFL2*sqrt((x - LFxref)*(x - LFxref) + 
							(y - LFyref - LFL1*sin(LFfai1)) * (y - LFyref - LFL1*sin(LFfai1)) + 
							(z - LFzref + LFL1*cos(LFfai1)) * (z - LFzref + LFL1*cos(LFfai1)));
	fraction = numerator/denominator;
	LFfai = acos(fraction);
}

float LFfai2cailc(float x, float y, float z)
{
	return atan((x - LFxref)/(sqrt((y - LFyref)*(y - LFyref)+(z - LFzref)*(z - LFzref))-LFL1))+LFfai;
}

float LFfai3cailc(float x, float y, float z)
{
	return ((LFL2+LFL3)/LFL3)*LFfai;
}

void LFcalc(float x, float y, float z)
{
	
	
	FOOT[0] = LFfai1cailc(x,y,z); //修改：LF与LFfai对调
	LFfaicaulc(x,y,z);
	FOOT[1] = LFfai2cailc(x,y,z);
	FOOT[2] = LFfai3cailc(x,y,z);
	
	
}	

	
	





extern float b_LF[3]; //角度
float LFang, LFx_support, LFx_swing; //基本足端轨迹x
	
float LFangcalc(float t) // 角度
{
	LFang = m*g*b*S*(t*t/12 - t*t*t/(6*T))/(J*sqrt(a*a+b*b));
}
float LFx_supportcaulc(float t) //基本轨迹x
{
	LFx_support = S/6+a/2-S*t/T+S/T;//a/2-S/6+S*t/T-S/T;
}
float LFxsupportcaulc(float t) //最终轨迹x
{
	return LFx_support*cos(LFang) - sin(LFang)*LFw;
}

float LFysupportcaulc(float t) //最终轨迹y
{
	return LFx_support*sin(LFang) + cos(LFang)*LFw;
}
float LFzsupportcaulc(float t) //最终轨迹z
{
	return h;
}

float LFx_swingcaulc(float t) //基本轨迹x
{
	LFx_swing = S*t/T+a/2-S/3; //-S*t/T+a/2+S/3;
}
float LFxswingcaulc(float t) //最终轨迹x
{
	return LFx_swing*cos(LFang) - sin(LFang)*LFw;
}
float LFyswingcaulc(float t) //最终轨迹y
{
	return LFx_swing*sin(LFang) + cos(LFang)*LFw;
}
float LFzswing1caulc(float t) //最终轨迹z 1
{
	return h+H*sin(2*pi*t/T);
}
float LFzswing2caulc(float t) //最终轨迹z 2
{
	return h+H*cos(3*pi*t/T - pi);
}

void LFwaycalc(float t)
{
	LFx_supportcaulc(t);
	LFx_swingcaulc(t);
if (t<T/2){
	b_LF[0] = -1*LFxswingcaulc(t);
	b_LF[1] = LFyswingcaulc(t);
	if (t<T/3){
		b_LF[2] = LFzswing1caulc(t);	
	}else{
		b_LF[2] = LFzswing2caulc(t);	
	}	
}else{
	b_LF[0] = -1*LFxsupportcaulc(t);
	b_LF[1] = LFysupportcaulc(t);
	b_LF[2] = LFzsupportcaulc(t);	
}
}







extern float b_LB[3]; 
float LBang, LBx_support, LBx_swing; 
	
float LBangcalc(float t) 
{
	LBang = m*g*b*S*(t*t/12 - t*t*t/(6*T))/(J*sqrt(a*a+b*b));
}
float LBx_supportcaulc(float t) 
{	
	LBx_support = a/2-S/6+S*t/T-S/T;//S/6+a/2-S*t/T+S/T
}
float LBxsupportcaulc(float t) 
{
	return LBx_support*cos(LBang) - sin(LBang)*LBw;
}

float LBysupportcaulc(float t) 
{
	return LBx_support*sin(LBang) + cos(LBang)*LBw;
}
float LBzsupportcaulc(float t) 
{
	return h;
}

float LBx_swingcaulc(float t) 
{
	LBx_swing = -S*t/T+a/2+S/3;//S*t/T+a/2-S/3;
}
float LBxswingcaulc(float t) 
{
	return LBx_swing*cos(LBang) - sin(LBang)*LBw;
}
float LByswingcaulc(float t) 
{
	return LBx_swing*sin(LBang) + cos(LBang)*LBw;
}
float LBzswing1caulc(float t) 
{
	return h+H*sin(2*pi*t/T);
}
float LBzswing2caulc(float t) 
{
	return h+H*cos(3*pi*t/T - pi);
}

void LBwaycalc(float t)
{
	LBx_supportcaulc(t);
	LBx_swingcaulc(t);
if (t<T/2){
	b_LB[0] = -1*LBxswingcaulc(t);
	b_LB[1] = -1*LByswingcaulc(t);
	if (t<T/3){
		b_LB[2] = LBzswing1caulc(t);	
	}else{
		b_LB[2] = LBzswing2caulc(t);	
	}	
}else{
	b_LB[0] = -1*LBxsupportcaulc(t);
	b_LB[1] = -1*LBysupportcaulc(t);
	b_LB[2] = LBzsupportcaulc(t);	
}
}







extern float b_RF[3]; 
float RFang, RFx_support, RFx_swing; ///////////////////////////////////////////////////////////////
	
float RFangcalc(float t) 
{
	RFang = m*g*b*S*(t*t/12 - t*t*t/(6*T))/(J*sqrt(a*a+b*b));
}
float RFx_supportcaulc(float t) 
{
	RFx_support = a/2-S/6+S*t/T-S/T;
}
float RFxsupportcaulc(float t) 
{
	return RFx_support*cos(RFang) - sin(RFang)*RFw;
}

float RFysupportcaulc(float t) 
{
	return RFx_support*sin(RFang) + cos(RFang)*RFw;
}
float RFzsupportcaulc(float t) 
{
	return h;
}

float RFx_swingcaulc(float t) 
{
	RFx_swing = -S*t/T+a/2+S/3;
}
float RFxswingcaulc(float t) 
{
	return RFx_swing*cos(RFang) - sin(RFang)*RFw;
}
float RFyswingcaulc(float t) 
{
	return RFx_swing*sin(RFang) + cos(RFang)*RFw;
}
float RFzswing1caulc(float t) 
{
	return h+H*sin(2*pi*t/T);
}
float RFzswing2caulc(float t) 
{
	return h+H*cos(3*pi*t/T - pi);
}

void RFwaycalc(float t)
{
	RFx_supportcaulc(t);
	RFx_swingcaulc(t);
if (t<T/2){
	b_RF[0] = RFxswingcaulc(t);
	b_RF[1] = RFyswingcaulc(t);
	if (t<T/3){
		b_RF[2] = RFzswing1caulc(t);	
	}else{
		b_RF[2] = RFzswing2caulc(t);	
	}	
}else{
	b_RF[0] = RFxsupportcaulc(t);
	b_RF[1] = RFysupportcaulc(t);
	b_RF[2] = RFzsupportcaulc(t);	
}
}


extern float b_RB[3]; 
float RBang, RBx_support, RBx_swing; 
	
float RBangcalc(float t) 
{
	RBang = m*g*b*S*(t*t/12 - t*t*t/(6*T))/(J*sqrt(a*a+b*b));
}
float RBx_supportcaulc(float t) 
{
	RBx_support = S/6+a/2-S*t/T+S/T;	//a/2-s/6
}
float RBxsupportcaulc(float t) 
{
	return RBx_support*cos(RBang) - sin(RBang)*RBw;
}

float RBysupportcaulc(float t) 
{
	return RBx_support*sin(RBang) + cos(RBang)*RBw;
}
float RBzsupportcaulc(float t) 
{
	return h;
}

float RBx_swingcaulc(float t) 
{
	RBx_swing = S*t/T+a/2-S/3;
}
float RBxswingcaulc(float t) 
{
	return RBx_swing*cos(RBang) - sin(RBang)*RBw;
}
float RByswingcaulc(float t) 
{
	return RBx_swing*sin(RBang) + cos(RBang)*RBw;
}
float RBzswing1caulc(float t) 
{
	return h+H*sin(2*pi*t/T);
}
float RBzswing2caulc(float t) 
{
	return h+H*cos(3*pi*t/T - pi);
}

void RBwaycalc(float t)
{
	RBx_supportcaulc(t);
	RBx_swingcaulc(t);
if (t<T/2){
	b_RB[0] = RBxswingcaulc(t);
	b_RB[1] = -1*RByswingcaulc(t);
	if (t<T/3){
		b_RB[2] = RBzswing1caulc(t);	
	}else{
		b_RB[2] = RBzswing2caulc(t);	
	}	
}else{
	b_RB[0] = RBxsupportcaulc(t);
	b_RB[1] = -1*RBysupportcaulc(t);
	b_RB[2] = RBzsupportcaulc(t);	
}
}


void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
							 
}
//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		u++;
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update); 
//		if(u%2)	
//			{
				USART_SendData(USART1,0x01) ;
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);

//			}			
	}
}

void LFwaycalc1(float t)
{
	
	if (t < 0.1)
	{
		b_LF[0] = 154 + S/6;
		b_LF[1] = 87.5;
		b_LF[2] = -200;			
	}
	else if (t < 1)
	{
		b_LF[0] = 154 - S/3;
		b_LF[1] = 87.5;
		b_LF[2] = -200;				
	}
	else if (t <1.6666)
	{
		b_LF[0] = 154;
		b_LF[1] = 87.5;
		b_LF[2] = -200 + H;			
	}
	else 
	{
		b_LF[0] = 154 + S/6;
		b_LF[1] = 87.5;
		b_LF[2] = -200;			
	}
	
	
//if (t<T/2){
//	b_LF[0] = -1*LFxswingcaulc(t);
//	b_LF[1] = LFyswingcaulc(t);
//	if (t<T/3){
//		b_LF[2] = LFzswing1caulc(t);	
//	}else{
//		b_LF[2] = LFzswing2caulc(t);	
//	}	
//}else{
//	b_LF[0] = -1*LFxsupportcaulc(t);
//	b_LF[1] = LFysupportcaulc(t);
//	b_LF[2] = LFzsupportcaulc(t);	
}

void RFwaycalc1(float t)
{
	
	if (t < 0.1)
	{
		b_RF[0] = 154 - S/3;
		b_RF[1] = 87.5;
		b_RF[2] = -200;			
	}
	else if (t < 1)
	{
		b_RF[0] = 154 + S/6;
		b_RF[1] = 87.5;
		b_RF[2] = -200;				
	}
	else if (t <1.3333)
	{
		b_RF[0] = 154;
		b_RF[1] = 87.5;
		b_RF[2] = -200 + H;			
	}
	else 
	{
		b_RF[0] = 154 - S/3;
		b_RF[1] = 87.5;
		b_RF[2] = -200;			
	}
	
	
//if (t<T/2){
//	b_LF[0] = -1*LFxswingcaulc(t);
//	b_LF[1] = LFyswingcaulc(t);
//	if (t<T/3){
//		b_LF[2] = LFzswing1caulc(t);	
//	}else{
//		b_LF[2] = LFzswing2caulc(t);	
//	}	
//}else{
//	b_LF[0] = -1*LFxsupportcaulc(t);
//	b_LF[1] = LFysupportcaulc(t);
//	b_LF[2] = LFzsupportcaulc(t);	
}
	
