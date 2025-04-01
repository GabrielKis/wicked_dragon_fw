/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/pwm.h>
#include <app_version.h>

/* LED device tree specification */
#define LED0_NODE DT_ALIAS(led0)

/* PWM device for servo control */
#define PWM_SERVO_NODE DT_ALIAS(pwm_servo)
/* Standard servo values (in microseconds) */
#define MIN_PULSE_WIDTH_US 1000  /* 1ms - typically -90 degrees */
#define MAX_PULSE_WIDTH_US 2000  /* 2ms - typically +90 degrees */
#define PERIOD_US 20000          /* 20ms (50Hz) - standard servo period */

#if !DT_NODE_HAS_STATUS(LED0_NODE, okay)
#error "LED0 GPIO node is not ready"
#endif

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
const struct device *pwm_dev = DEVICE_DT_GET(DT_ALIAS(pwm_servo));

int main(void)
{
    int ret;
    printk("Zephyr Servo Control Example\n");

    if (!device_is_ready(pwm_dev)) {
        printk("Error: PWM device %s is not ready\n", pwm_dev->name);
        return 1;
    }

    /* Set initial position (middle) */
    ret = pwm_set(pwm_dev, 0, PWM_USEC(PERIOD_US), PWM_USEC(PERIOD_US / 2), 0);
    if (ret < 0) {
        printk("Error %d: failed to set pulse width\n", ret);
        return 1;
    }


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
