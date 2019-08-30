#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"

#define RBxref -154 //右后髋关节x坐标
#define RByref -87.5 //髋关节y坐标
#define RBzref -20 //髋关节z坐标
#define RBL1 58	//第一关节长度
#define RBL2 44 //第二关节长度
#define RBL3 79.5 //第三关节长度
#define RBL1S 3364 //L1的平方
#define RBL2S 1936 //L2的平方
#define RBL3S 6320.25 //L3的平方

#define RFxref 154 //右前腿髋关节x坐标
#define RFyref -87.5 //髋关节y坐标
#define RFzref -20 //髋关节z坐标
#define RFL1 58	//第一关节长度
#define RFL2 44 //第二关节长度
#define RFL3 79.5 //第三关节长度
#define RFL1S 3364 //L1的平方
#define RFL2S 1936 //L2的平方
#define RL3S 6320.25 //L3的平方

#define LBxref -154 //髋关节x坐标
#define LByref 87.5 //髋关节y坐标
#define LBzref -20 //髋关节z坐标
#define LBL1 58	//第一关节长度
#define LBL2 44 //第二关节长度
#define LBL3 79.5 //第三关节长度
#define LBL1S 3364 //L1的平方
#define LBL2S 1936 //L2的平方
#define LBL3S 6320.25 //L3的平方

#define LFxref 154 //髋关节x坐标
#define LFyref 87.5 //髋关节y坐标
#define LFzref -20 //髋关节z坐标
#define LFL1 58	//第一关节长度
#define LFL2 73 //第二关节长度
#define LFL3 73	 //第三关节长度
#define LFL1S 3364 //L1的平方
#define LFL2S 5329 //L2的平方
#define LFL3S 5329 //L3的平方

//#define S 150.00 //摆幅
#define H 40.00 //跨步高度
#define T 1.0 //周期
#define pi 3.14 //π
//#define a 308.00 //长
#define b 175.00 //宽
#define m 0 //质量
#define g 0 //重力系数
#define J 0 //转动惯量
//#define h -190 //高度 负数
#define LFw -87.5 //y坐标 左前 
#define LBw -87.5 //y坐标 左后 
#define RFw 87.5 //y坐标 
#define RBw 87.5 //y坐标

void RFfaicaulc(float x, float y, float z);
float RFfai1cailc(float x, float y, float z);
float RFfai2cailc(float x, float y, float z);
float RFfai3cailc(float x, float y, float z);
void RFcaulc(float x, float y, float z);

void RBfaicaulc(float x, float y, float z);
float RBfai1cailc(float x, float y, float z);
float RBfai2cailc(float x, float y, float z);
float RBfai3cailc(float x, float y, float z);
void RBcaulc(float x, float y, float z);

void LBfaicaulc(float x, float y, float z);
float LBfai1cailc(float x, float y, float z);
float LBfai2cailc(float x, float y, float z);
float LBfai3cailc(float x, float y, float z);
void LBcalc(float x, float y, float z);

void LFfaicaulc(float x, float y, float z);
float LFfai1cailc(float x, float y, float z);
float LFfai2cailc(float x, float y, float z);
float LFfai3cailc(float x, float y, float z);
void LFcalc(float x, float y, float z);


///////////////////解算///////////////////

float LFangcalc(float t);
float LFx_supportcaulc(float t);
float LFxsupportcaulc(float t);
float LFysupportcaulc(float t);
float LFzsupportcaulc(float t);
float LFx_swingcaulc(float t);
float LFxswingcaulc(float t);
float LFyswingcaulc(float t);
float LFzswing1caulc(float t);
float LFzswing2caulc(float t);
void LFwaycalc(float t);

float LBangcalc(float t);
float LBx_supportcaulc(float t);
float LBxsupportcaulc(float t);
float LBysupportcaulc(float t);
float LBzsupportcaulc(float t);
float LBx_swingcaulc(float t);
float LBxswingcaulc(float t);
float LByswingcaulc(float t);
float LBzswing1caulc(float t);
float LBzswing2caulc(float t);
void LBwaycalc(float t);

float RFangcalc(float t);
float RFx_supportcaulc(float t);
float RFxsupportcaulc(float t);
float RFysupportcaulc(float t);
float RFzsupportcaulc(float t);
float RFx_swingcaulc(float t);
float RFxswingcaulc(float t);
float RFyswingcaulc(float t);
float RFzswing1caulc(float t);
float RFzswing2caulc(float t);
void RFwaycalc(float t);

float RBangcalc(float t);
float RBx_supportcaulc(float t);
float RBxsupportcaulc(float t);
float RBysupportcaulc(float t);
float RBzsupportcaulc(float t);
float RBx_swingcaulc(float t);
float RBxswingcaulc(float t);
float RByswingcaulc(float t);
float RBzswing1caulc(float t);
float RBzswing2caulc(float t);
void RBwaycalc(float t);


void LFwaycalc1(float t);
void RFwaycalc1(float t);


///////////////////////定时器TIM3/////////////////////////////
void TIM3_Int_Init(u16 arr,u16 psc);

