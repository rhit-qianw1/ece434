#!/usr/bin/env python3

import smbus

# get i2c bus
bus = smbus.SMBus(2)

# assign addresses. My another sensor does not work with the same wiring. So I use the same addresses. 
address1 = 0x4a
address2 = 0x4a

# read values
value1 = bus.read_i2c_block_data(address1, 0x00, 2)
value2 = bus.read_i2c_block_data(address2, 0x00, 2)

# convert value to Fahrenheit and print
print("The first sensor value is:", value1[0] * 9/5 + 32)
print("The first sensor value is:", value2[0] * 9/5 + 32)
