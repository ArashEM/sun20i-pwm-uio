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
#include <linux/uio_driver.h>

static const struct of_device_id sun20i_of_device_ids[] = {
    { .compatible = "allwinner, sun20i-pwm-uio"},
    { /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, sun20i_of_device_ids);

static int sun20i_pwm_probe(struct platform_device *pdev)
{
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
MODULE_DESCRIPTION("Allwinner sun20i PWM driver");
MODULE_LICENSE("GPL v2");
