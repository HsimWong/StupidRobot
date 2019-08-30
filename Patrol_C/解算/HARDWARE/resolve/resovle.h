#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"

#define RBxref -154 //�Һ��Źؽ�x����
#define RByref -87.5 //�Źؽ�y����
#define RBzref -20 //�Źؽ�z����
#define RBL1 58	//��һ�ؽڳ���
#define RBL2 44 //�ڶ��ؽڳ���
#define RBL3 79.5 //�����ؽڳ���
#define RBL1S 3364 //L1��ƽ��
#define RBL2S 1936 //L2��ƽ��
#define RBL3S 6320.25 //L3��ƽ��

#define RFxref 154 //��ǰ���Źؽ�x����
#define RFyref -87.5 //�Źؽ�y����
#define RFzref -20 //�Źؽ�z����
#define RFL1 58	//��һ�ؽڳ���
#define RFL2 44 //�ڶ��ؽڳ���
#define RFL3 79.5 //�����ؽڳ���
#define RFL1S 3364 //L1��ƽ��
#define RFL2S 1936 //L2��ƽ��
#define RL3S 6320.25 //L3��ƽ��

#define LBxref -154 //�Źؽ�x����
#define LByref 87.5 //�Źؽ�y����
#define LBzref -20 //�Źؽ�z����
#define LBL1 58	//��һ�ؽڳ���
#define LBL2 44 //�ڶ��ؽڳ���
#define LBL3 79.5 //�����ؽڳ���
#define LBL1S 3364 //L1��ƽ��
#define LBL2S 1936 //L2��ƽ��
#define LBL3S 6320.25 //L3��ƽ��

#define LFxref 154 //�Źؽ�x����
#define LFyref 87.5 //�Źؽ�y����
#define LFzref -20 //�Źؽ�z����
#define LFL1 58	//��һ�ؽڳ���
#define LFL2 73 //�ڶ��ؽڳ���
#define LFL3 73	 //�����ؽڳ���
#define LFL1S 3364 //L1��ƽ��
#define LFL2S 5329 //L2��ƽ��
#define LFL3S 5329 //L3��ƽ��

//#define S 150.00 //�ڷ�
#define H 40.00 //�粽�߶�
#define T 1.0 //����
#define pi 3.14 //��
//#define a 308.00 //��
#define b 175.00 //��
#define m 0 //����
#define g 0 //����ϵ��
#define J 0 //ת������
//#define h -190 //�߶� ����
#define LFw -87.5 //y���� ��ǰ 
#define LBw -87.5 //y���� ��� 
#define RFw 87.5 //y���� 
#define RBw 87.5 //y����

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


///////////////////����///////////////////

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


///////////////////////��ʱ��TIM3/////////////////////////////
void TIM3_Int_Init(u16 arr,u16 psc);

