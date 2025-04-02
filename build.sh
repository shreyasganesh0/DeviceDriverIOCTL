#!/bin/bash

sudo rmmod char_driver

make clean

make

make app

sudo insmod char_driver.ko majorno=500 numdevices=4

sudo chmod 777 /dev/my*

./devuserapp Assignment5_tests/input0.txt > test_op/output0.txt
./devuserapp Assignment5_tests/input1.txt > test_op/output1.txt
./devuserapp Assignment5_tests/input2.txt > test_op/output2.txt
./devuserapp Assignment5_tests/input3.txt > test_op/output3.txt
./devuserapp Assignment5_tests/input4.txt > test_op/output4.txt

diff Assignment5_tests/output0.txt test_op/output0.txt
diff Assignment5_tests/output1.txt test_op/output1.txt
diff Assignment5_tests/output2.txt test_op/output2.txt
diff Assignment5_tests/output3.txt test_op/output3.txt
diff Assignment5_tests/output4.txt test_op/output4.txt

echo "Finished the build";

