from overall import *
import pyb
from resolve import *

k = 0
i = 0
u = 0
current_time = 0
a = 308
Fa = 308
Ba = 308

RB = []
RF = [0,0,0]
LB = []
LF = []

uart1 = pyb.UART(3, BAUDRATE, timeout = 10, timeout_char = 10)
uart1.init(BAUDRATE, timeout = 10, bits=8, parity=None, stop=1)
timer = pyb.Timer(4)              # create a timer object using timer 4
timer.init(freq = 9000, period = 7200)


# 0xff $(int)(LF2[])(24) $(int)(LF1[])(24) $(int)(LF0[])(24) $RF1 $RF2 $RF0 $RB2 $RB1 $RB0 $LB(2, 1, 0) upstairs(judge)
def send():
	uart1.writechar(0xff)
	for j in [2, 1, 0]:
		LF[j] = 0x01 if LF[j] == 0 else LF[j]
		LF[j] = 0xfe if LF[j] == 0xff else LF[j]
		uart1.writechar(LF[j])
	for j in [1, 2, 0]:
		RF[j] = 0x01 if RF[j] == 0 else RF[j]
		RF[j] = 0xfe if RF[j] == 0xff else RF[j]
		uart1.writechar(RF[j])
	for j in [2,1,0]:
		RB[j] = 0x01 if RB[j] == 0 else RB[j]
		RB[j] = 0xfe if RB[j] == 0xff else RB[j]
		uart1.writechar(RB[j])
	for j in [2,1,0]:
		LB[j] = 0x01 if LB[j] == 0 else LB[j]
		LB[j] = 0xfe if LB[j] == 0xff else LB[j]
		uart1.writechar(LB[j])
	sent_upstairs = 0x01 if upstairs == 0 else upstairs
	sent_upstairs = 0xfe if upstairs == 0xff else upstairs
	uart1.writechar(sent_upstairs)
	uart1.writechar(0x00)



def loop():
	upstairs = 0
	send()
	current_time = timer.counter() / 3600
	S = SL
	h = -190
	RFwaycalc(current_time);
	LFwaycalc(1 * b_RF[0],1 * b_RF[1], b_RF[2])
	LF[0] = (FOOT[0]/3.14)*180
	LF[1] = (FOOT[1]/3.14)*180
	LF[2] = (FOOT[2]/3.14)*180
	i += 1
	h = -190
	S = SR
	LFwaycalc(current_time);
	LFcalc(-1 * b_LF[0] + 10 + 0,-1 * b_LF[1], b_LF[2]);
	RB[0] = (FOOT[0]/3.14)*180;								
	RB[1] = (FOOT[1]/3.14)*180;
	RB[2] = (FOOT[2]/3.14)*180;
	i+= 1

	current_time += -1 if current_time > 1 else 1

	h = -190
	S = SR
	RFwaycalc(current_time)
	LFcalc(1 * b_RF[0],1 * b_RF[1], b_RF[2]);
	RF[0] = (FOOT[0]/3.14)*180
	RF[1] = (FOOT[1]/3.14)*180;
	RF[2] = (FOOT[2]/3.14)*180;
	i += 1

	h = -190
	S = SL
	LFwaycalc(current_time)
	LFcalc(-1 * b_LF[0] + 10,-1 * b_LF[1], b_LF[2]);
	RF[0] = (FOOT[0]/3.14)*180
	RF[1] = (FOOT[1]/3.14)*180;
	RF[2] = (FOOT[2]/3.14)*180;
	i += 1







def main():
	while True:
		loop()

if __name__ == '__main__':
	main()