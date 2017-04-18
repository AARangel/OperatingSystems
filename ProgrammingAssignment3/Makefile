obj-m+=inputDriver.o
obj-m+=inputDriver.o
DeviceDriver2 := inputDriver.o inputDriver.o
 
all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	$(CC) ddtest.c -o test
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
	rm test

