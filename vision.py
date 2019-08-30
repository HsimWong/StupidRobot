import sensor
import image
import time
import math
import pyb
BAUDRATE = 9600
HEIGHT = 25
GRAYSCALE_THRESHOLD = [(13, 75, 24, 59, -8, 127)]
BLACK_LINE_THRESHOLD = [(0, 10, -128, 127, -128, 127)]
ROIS = [(0, 100, 320, 20, 1.0)]
black_ROIS = [(100, 0, 120, 240, 1.0)]
#weight_sum = sum([r[4] for r in ROIS])
clock = time.clock()
uart = pyb.UART(3, BAUDRATE, timeout = 10, timeout_char = 10)
uart.init(BAUDRATE, timeout = 10, bits=8, parity=None, stop=1)
THETA = 3.1416 / 6
#red_led = pyb.LED(1)
#green_led = pyb.LED(2)
#blue_led = pyb.LED(3)
#ir_leds = pyb.LED(4)
def get_dist(pix_pos):
    ang = (pix_pos - 120) / (27.8/180)
    dist = HEIGHT / math.tan(THETA - ang)
    return dist
def init():
    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QVGA)
    sensor.skip_frames(1)
    sensor.set_auto_gain(False)
    sensor.set_auto_whitebal(False)
def wait():
    clock.tick()
    while not uart.any():
        clock.tick()

    else:

        tmp = uart.readline()
def follow_red(img):
    blobs = img.find_blobs(GRAYSCALE_THRESHOLD, roi = ROIS[0][0:4], merge = True)
    if blobs:
        most_pixels = 0
        largest_blob = None
        for blob in blobs:
            if blob.pixels() > (largest_blob.pixels() if not largest_blob == None else 0):
                largest_blob = blob
        if not largest_blob == None:
            img.draw_cross(largest_blob.cx(),
                          largest_blob.cy())
            return int((largest_blob.cx() / 320) * 127 + 128)
    return 0xff
def fetch_black(img):
    blobs = img.find_blobs(BLACK_LINE_THRESHOLD, roi = black_ROIS[0][0:4], merge = True)
    if blobs:
        most_pixels = 0
        largest_blob = None
        for blob in blobs:
            if blob.pixels() > (largest_blob.pixels() if not largest_blob == None else 0):
                largest_blob = blob
        if not largest_blob == None:
            img.draw_rectangle(largest_blob.arect())
            ret = (240 - largest_blob.cy())
            return math.floor((ret / 240) * 128)
    return 0x7f
def send(red_center, black_center):
    uart.writechar(red_center)
    uart.writechar(black_center)
# def loop():
#     img = sensor.snapshot()
#     red_center = follow_red(img)
#     black_center = fetch_black(img)

#     send(red_center, black_center)
def fetch_vision_result():
    img = sensor.snapshot()
    red_center = follow_red(img)
    black_center = fetch_black(img)
    return red_center, black_center

if __name__ == '__main__':
    main()
