# Introduction 
This is `uio` Linux driver for `T113-S3` PWM sub system.  

# Build and deploy
1. build
    ```bash
    export KBUILDDIR=<KERNEL BUILD DIR>
    export ARCH=arm
    export CROSS_COMPILE=arm-none-eabi-
    make
    ```
2. Copy `sun20i-pwm-uio.ko` to proper directory in target rootfs (e.g. `/lib/modules/6.6.47/kernel/drivers/pwm/`)
3. run `depmod -a` in target to update `module.alias` and `module.dep`

# DTS
In order to instantiate this driver, add following lines to `sun8i-t113s-mangopi-mq-r-t113.dts` file.  
We need `pinctrl` to configure _PWM_ pins for this kernel module.
```
&pio {
    /omit-if-no-ref/
	pwm2_pe8_pin: pwm2-pe8-pin {
        pins = "PE8";
        function = "pwm2";
    };

	/omit-if-no-ref/
	pwm3_pe9_pin: pwm3-pe9-pin {
        pins = "PE9";
        function = "pwm3";
    };

	/omit-if-no-ref/
	pwm4_pe10_pin: pwm4-pe10-pin {
        pins = "PE10";
        function = "pwm4";
    };

	/omit-if-no-ref/
	pwm5_pe13_pin: pwm5-pe13-pin {
        pins = "PE13";
        function = "pwm5";
    };
};

&soc {
    pwm_uio: pwm_uio@2000c00 {
        compatible = "allwinner,sun20i-pwm-uio";
        reg = <0x2000c00 0x400>;
        interrupts = <GIC_SPI 18 IRQ_TYPE_LEVEL_HIGH>;
        clocks = <&ccu CLK_BUS_PWM>, <&dcxo>;
        clock-names = "bus", "mod";
        resets = <&ccu RST_BUS_PWM>;
        pinctrl-0 = <&pwm2_pe8_pin>, 
                    <&pwm3_pe9_pin>,
                    <&pwm4_pe10_pin>,
                    <&pwm5_pe13_pin>;
        pinctrl-names = "default";
    };
};
```