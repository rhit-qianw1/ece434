#!/usr/bin/env python3

import smbus
import Adafruit_BBIO.GPIO as GPIO
import time

# get i2c bus
bus = smbus.SMBus(2)

# assign addresses. My another sensor does not work with the same wiring. So I use the same addresses. 
address1 = 0x4a
address2 = 0x4a

# read values
value1 = -1
value2 = -1

# set low and high limits
temp_low = 10
temp_high = 30

bus.write_byte_data(address1, 1, 0x60)
bus.write_byte_data(address1, 2, temp_low)
bus.write_byte_data(address1, 3, temp_high)

bus.write_byte_data(address2, 1, 0x60)
bus.write_byte_data(address2, 2, temp_low)
bus.write_byte_data(address2, 3, temp_high)

# setup GPIO
GPIO.setup("P9_11", GPIO.IN)
GPIO.setup("P9_13", GPIO.IN)
GPIO.cleanup()

while True:
    # when P8_11 is triggered
    if GPIO.input("P9_11") == 0:
        value1 = bus.read_i2c_block_data(address1, 0, 2)
        print("The first sensor temperature is: ", value1[0] * 9/5 + 32)

    # when P8_13 is triggered
    if GPIO.input("P9_13") == 0:
        value2 = bus.read_i2c_block_data(address2, 0, 2)
        print("The second sensor temperature is: ", value2[0] * 9/5 + 32)

    time.sleep(1)

