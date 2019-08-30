import sensor
from math import *
UPSTARIS = 0                         # or 1



RB = []
RBfai1 = None
RBfai2 = None
RBfai3 = None
RBfai  = None
RF = []
RFfai1 = None
RFfai2 = None
RFfai3 = None
RFfai = None

S = None
a = None
u = None
h = None
b_RF = [] 
RFang = None
RFx_support = None
RFx_swing = None 

LFfai1 = None
LFfai2 = None
LFfai3 = None 
LFfai = None
FOOT[3]

FLAG = 0


def RFx_supportcalc(t):
	return a / 2 - S / 6 + s * t / T - S / T 

def RFx_swingcaulc(t):
	return (- S * t / T + a / 2 + S / 3)

def RFxswingcaulc(t):
	return RFx_swing * cos(RFang) - sin(RFang) * RFw

def RFyswingcaulc(t):
	return RFx_swing * sin(RFang) + cos(RFang) * RFw

def RFzswing1caulc(t):
	return h + H * sin(2 * pi * t / T)

def RFzswing2caulc(t):
	return h + H * cos(3 * pi * t / T - pi)

def RFxsupportcaulc(t):
	return RFx_support * cos(RFang) - sin(RFang) * RFw

def RFysupportcaulc(t):
	return RFx_support * sin(RFang) + cos(RFang) * RFw

def RFzsupportcaulc(t):
	return h

def LFx_supportcaulc(t):
	return  S/6+a/2-S*t/T+S/T

def LFxsupportcaulc(t):
	return LFx_support*cos(LFang) - sin(LFang)*LFw;

def LFysupportcaulc(t):
	return LFx_support*sin(LFang) + cos(LFang)*LFw;

def LFzsupportcaulc(t):
	return h;

def LFx_swingcaulc(t):
	return S*t/T+a/2-S/3

def LFxswingcaulc(t):
	return LFx_swing*cos(LFang) - sin(LFang)*LFw;

def LFyswingcaulc(t):
	return LFx_swing*sin(LFang) + cos(LFang)*LFw;

def LFzswing1caulc(t):
	return h+H*sin(2*pi*t/T);

def LFzswing2caulc(t):
	return h+H*cos(3*pi*t/T - pi)

def RFwaycalc(t):
	RFx_support = RFxsupportcalc(t)
	RFx_swing = RFx_swingcaulc(t)
	if t < (T / 2):
		b_RF[0] = RFxswingcaulc(t)
		b_RF[1] = RFyswingcaulc(t)
		b_RF[2] = RFzswing1caulc(t) if \
			t < (T / 3) else RFzswing2caulc(t)
	else:
		b_RF[0] = RFxsupportcaulc(t)
		b_RF[1] = RFysupportcaulc(t)
		b_RF[2] = RFzsupportcaulc(t)

def LFwaycalc(t):
	LFx_support = LFxsupportcalc(t)
	LFx_swing = LFx_swingcaulc(t)
	if t < (T / 2):
		b_LF[0] = LFxswingcaulc(t)
		b_LF[1] = LFyswingcaulc(t)
		b_LF[2] = LFzswing1caulc(t) if \
			t < (T / 3) else LFzswing2caulc(t)
	else:
		b_LF[0] = LFxsupportcaulc(t)
		b_LF[1] = LFysupportcaulc(t)
		b_LF[2] = LFzsupportcaulc(t)		

def LFfai1cailc(x, y, z)
	return atan(-1 * (y - 87.5) / (z + 20))

def LFfai2cailc(x, y, z)
	return atan((x - LFxref) / (sqrt((y - LFyref) * (y - LFyref) + \
		(z - LFzref) * (z - LFzref)) - LFL1)) + LFfai

def LFfai3cailc(x, y, z)
	return ((LFL2 + LFL3) / LFL3) * LFfai

def LFfaicaulc(x, y, z):
	LFfai1 = LFfai1cailc(x,y,z)
	numerator = (LFL2S +(x - LFxref)*(x - LFxref) +\
				(y - LFyref - LFL1*sin(LFfai1))**2 + \
				(z - LFzref + LFL1*cos(LFfai1))**2 - LFL3*LFL3)
	denominator = 2*LFL2*sqrt((x - LFxref)**2 + \
				(y - LFyref - LFL1*sin(LFfai1))**2 + \
				(z - LFzref + LFL1*cos(LFfai1))**2)
	return acos(numerator / denominator)
	
def  LFcalc(x, y, z):
	FOOT[0] = LFfai1cailc(x,y,z)
	LFfaicaulc(x,y,z)
	FOOT[1] = LFfai2cailc(x,y,z)
	FOOT[2] = LFfai3cailc(x,y,z)
	

# 0xff $(int)(LF2[])(24) $(int)(LF1[])(24) $(int)(LF0[])(24) $RF1 $RF2 $RF0 $RB2 $RB1 $RB0 $LB(2, 1, 0) upstairs(judge)
def send():
	send_array = [0xff,].extend(LF2)