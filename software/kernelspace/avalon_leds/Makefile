KDIR ?= $(KERNEL_PATH)
TARGET = avalon_leds
HOST = 192.168.1.108

default:
	$(MAKE) -C $(KDIR) ARCH=arm M=$(CURDIR)

clean:
	$(MAKE) -C $(KDIR) ARCH=arm M=$(CURDIR) clean

help:
	$(MAKE) -C $(KDIR) ARCH=arm M=$(CURDIR) help
install:
	scp $(TARGET).ko root@${HOST}:/root/$(TARGET)