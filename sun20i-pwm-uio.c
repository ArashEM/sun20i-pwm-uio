/**
 * @file sun20i-pwm-uio.c
 * @author Arash Golgol (arash.golgol@gmail.com)
 * @brief UIO driver for T133 PWM peripheral
 * @version 1.0
 * @date 2024-09-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <linux/module.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/printk.h>
#include <linux/uio_driver.h>

static irqreturn_t pwm_irqhander(int irq, struct uio_info *dev_info)
{
    return IRQ_HANDLED;
}

static const struct of_device_id sun20i_of_device_ids[] = {
    { .compatible = "allwinner,sun20i-pwm-uio" },
    { /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, sun20i_of_device_ids);

static int sun20i_pwm_probe(struct platform_device *pdev)
{
    struct uio_info *info;
    struct resource *res;
    struct device *dev = &pdev->dev;
    int irq;
    int ret;

    info = devm_kzalloc(dev, sizeof(*info), GFP_KERNEL);
    if(!info)
        return -ENOMEM;

    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if(unlikely(res == NULL)) {
        dev_err(dev, "Invalid memory resource!\n");
        return -EINVAL;
    }

    irq = platform_get_irq(pdev, 0);
    if(irq < 0) {
        dev_err(dev, "Invalid IRQ number\n");
        return -EINVAL;
    }

    /* size must be multiples of page size*/
    resource_size_t size =
        ((resource_size(res) >> PAGE_SHIFT) + 1) << PAGE_SHIFT;

    info->mem[0].addr = res->start & PAGE_MASK;
    info->mem[0].offs = res->start & ~PAGE_MASK;
    info->mem[0].size = size;
    info->mem[0].memtype = UIO_MEM_PHYS;

    info->name = "sun20i-pwm";
    info->version = "1.0.0";

    info->irq = irq;
    info->irq_flags = 0;
    info->handler = &pwm_irqhander;

    ret = devm_uio_register_device(dev, info);
    if(ret) {
        dev_err(dev, "Unable to register UIO device\n");
        return ret;
    }

    platform_set_drvdata(pdev, info);
    
    return 0;
}

static int sun20i_pwm_remove(struct platform_device *pdev)
{
    return 0;
}

static struct platform_driver sun20i_pwm_driver = {
    .probe = sun20i_pwm_probe,
    .remove = sun20i_pwm_remove,
    .driver = {
        .of_match_table = sun20i_of_device_ids,
        .name = "sun20i-pwm-uio",
    }
};
module_platform_driver(sun20i_pwm_driver);

MODULE_ALIAS("platform:sun20i-pwm-uio");
MODULE_AUTHOR("Arash Golgol arash.golgol@gmail.com");
MODULE_DESCRIPTION("Allwinner sun20i PWM UIO driver");
MODULE_LICENSE("GPL v2");
