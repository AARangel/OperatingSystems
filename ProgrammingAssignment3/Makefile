obj-m+=inputDriver.o
obj-m+=outputDriver.o
obj-m+=DeviceDriver2.0
DeviceDriver2-objs := inputDriver.o outputDriver.o
 
all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	$(CC) ddtest.c -o test
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
	rm test

