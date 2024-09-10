obj-m := sun20i-pwm-uio.o

all:
	make -C ${KBUILDDIR} M=$(PWD) modules

clean:
	make -C ${KBUILDDIR} M=$(PWD) clean
