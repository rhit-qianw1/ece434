# My another sensor does not work.
# There is only one showing up on the i2c detect.
# The wiring are the same

#!/bin/bash

TMP101_1=0x4a
TMP101_2=0x4a


TMP101_1_value=$(i2cget -y 2 ${TMP101_1} 00)
TMP101_1_value_f=$(($TMP101_1_value * 9/5 + 32))
TMP101_2_value=$(i2cget -y 2 ${TMP101_2} 00)
TMP101_2_value_f=$(($TMP101_2_value * 9/5 + 32))

echo "The first sensor value is: $TMP101_1_value_f"
echo "The second sensor value is: $TMP101_2_value_f"
