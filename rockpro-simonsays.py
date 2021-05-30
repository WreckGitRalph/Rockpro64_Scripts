#!/usr/bin/python3

import io
import time
import psutil
import os

net_traffic_prev = 0

os.system("stty -F /dev/ttyS2 ospeed 115200")

while True:
    temp_file1 = open("/sys/class/thermal/thermal_zone0/temp", "r")
    temp_file2 = open("/sys/class/thermal/thermal_zone1/temp", "r")

    temp1 = temp_file1.readline()
    temp2 = temp_file2.readline()

    temp_file1.close()
    temp_file2.close()

    if temp1 > temp2:
        hightemp = int(temp1)
    else:
        hightemp = int(temp2)

    if hightemp < 40000:
        pwm = 0
    else:
        pwm = int(( (hightemp-40000) / 40000 ) * 255)

    net_counters = psutil.net_io_counters()
    net_traffic_curr = (net_counters.bytes_sent + net_counters.bytes_recv)
    net_traffic = str(net_traffic_curr - net_traffic_prev)
    net_traffic_prev = net_traffic_curr

    uart_out = open("/dev/ttyS2", "w")
    uart_out.writelines("\nSimonSays SET FAN "+str(pwm)+"\n")
    uart_out.writelines("\nSimonSays SET NET LED "+str(net_traffic)+"\n")
    uart_out.close()

    time.sleep(2)
