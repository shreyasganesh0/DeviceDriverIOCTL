# Assignment 5

# Usage

- To run will build, install the driver and run the tests putting outputs in test_op directory
```
./build.sh
```

- Build driver using
```
make
```
- Install driver using
```
sudo insmod char_driver.ko [majorno=x] [numdevices=x] [size=x]
```
- Must provide permissions to created device files
```
sudo chmod 777 /dev/my*
```
- Make the test app
```
make app
```
- Run test app
```
devuserapp input.txt > output.txt
```
