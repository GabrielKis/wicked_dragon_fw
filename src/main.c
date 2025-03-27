/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <app_version.h>

/* LED device tree specification */
#define LED0_NODE DT_ALIAS(led0)

#if !DT_NODE_HAS_STATUS(LED0_NODE, okay)
#error "LED0 GPIO node is not ready"
#endif

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

int main(void)
{
    int ret;

    printk("Zephyr Example Application %s\n", APP_VERSION_STRING);

    if (!device_is_ready(led.port)) {
        printk("Error: LED device %s is not ready\n", led.port->name);
        return 1;
    }

    ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        printk("Error %d: failed to configure LED pin\n", ret);
        return 1;
    }

    while (1) {
        /* Toggle LED state */
        gpio_pin_toggle_dt(&led);
        k_sleep(K_MSEC(50));
        gpio_pin_toggle_dt(&led);
        k_sleep(K_MSEC(1000));
    }

    return 0;
}
