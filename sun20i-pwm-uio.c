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
#include <linux/reset.h>
#include <linux/clk.h>
#include <linux/uio_driver.h>

struct sun20i_pwm {
    struct clk *bus_clk;
    struct reset_control *rstc;
    void __iomem *base;
};

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
    struct sun20i_pwm *pwm;
    struct device *dev = &pdev->dev;
    int irq;
    int ret;

    info = devm_kzalloc(dev, sizeof(*info), GFP_KERNEL);
    if(!info)
        return -ENOMEM;

    pwm = devm_kzalloc(dev, sizeof(*pwm), GFP_KERNEL);
    if(!pwm)
        return -ENOMEM;

    info->priv = pwm;

    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if(unlikely(res == NULL)) {
        dev_err(dev, "Invalid memory resource!\n");
        return -EINVAL;
    }

    pwm->base = devm_ioremap_resource(dev, res);
    if(IS_ERR(pwm->base))
        return PTR_ERR(pwm->base);

    pwm->bus_clk = devm_clk_get(dev, "bus");
    if(IS_ERR(pwm->bus_clk))
        return dev_err_probe(dev, PTR_ERR(pwm->bus_clk), 
                            "Unable to get bus clock\n");

    pwm->rstc = devm_reset_control_get_shared(dev, NULL);
    if(IS_ERR(pwm->rstc))
        return dev_err_probe(dev, PTR_ERR(pwm->rstc), 
                            "Unable to get reset\n");

    ret = reset_control_deassert(pwm->rstc);
    if(ret) {
        dev_err(dev, "Unable to deassert reset: %pe\n", ERR_PTR(ret));
        return ret;
    }

    ret = clk_prepare_enable(pwm->bus_clk);
    if(ret) {
        dev_err(dev, "Unable to prepare bus clock: %pe\n", ERR_PTR(ret));
        goto clk_err;
    }

    irq = platform_get_irq(pdev, 0);
    if(irq < 0) {
        dev_err(dev, "Invalid IRQ number\n");
        ret = -ENAVAIL;
        goto irq_err;
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
        goto irq_err;
    }

    platform_set_drvdata(pdev, info);

    return 0;

irq_err:
    clk_disable_unprepare(pwm->bus_clk);
clk_err:
    reset_control_assert(pwm->rstc);
    return ret;
}

static int sun20i_pwm_remove(struct platform_device *pdev)
{
    struct uio_info *info = platform_get_drvdata(pdev);
    struct sun20i_pwm *pwm = info->priv;

    clk_disable_unprepare(pwm->bus_clk);
    reset_control_assert(pwm->rstc);

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
