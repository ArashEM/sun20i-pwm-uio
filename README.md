# Introduction 
This repository includes `uio` driver for `T113-S3 PWM` peripheral.  

# Directories
- `driver`: kernel space driver  
   It's basically platform device which is registered into `uio` framework. All register handling is done in user space.
- `lib`: user space library  
   Shared library for interfacing with `PWM` peripheral. 
- `app`: user space application  
   User space application which implement simple use case of library APIs.
- `udev`: Simple udev rule to create `/dev/uio0` device node with proper access. This way you don't need `root` access to use `uio` device.

# Links
1. [UIO howto](https://www.kernel.org/doc/html/v4.14/driver-api/uio-howto.html)
2. [UIO generic driver for platform device](https://elixir.bootlin.com/linux/v6.10.9/source/drivers/uio/uio_pdrv_genirq.c)