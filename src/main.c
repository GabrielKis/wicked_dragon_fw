/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/pwm.h>

#include <app_version.h>
#include "uart_cmd.h"

/* LED device tree specification */
#define LED0_NODE DT_ALIAS(led0)

/* PWM device for servo control */
#define MIN_PULSE_WIDTH_US 1000  /* 1ms - typically -90 degrees */
#define MAX_PULSE_WIDTH_US 2000  /* 2ms - typically +90 degrees */
#define PERIOD_US 20000          /* 20ms (50Hz) - standard servo period */

#if !DT_NODE_HAS_STATUS(LED0_NODE, okay)
#error "LED0 GPIO node is not ready"
#endif

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct pwm_dt_spec servo = PWM_DT_SPEC_GET(DT_ALIAS(pwm0));

int main(void)
{
    int ret;
    printk("Zephyr Servo Control Example\n");

    if (!device_is_ready(servo.dev)) {
        printk("Error: PWM device %s is not ready\n", servo.dev->name);
        return 1;
    }

    if (!device_is_ready(led.port)) {
        printk("Error: LED device %s is not ready\n", led.port->name);
        return 1;
    }


    /* Set initial position (middle) */
    ret = pwm_set(servo.dev, 0, PWM_USEC(PERIOD_US), PWM_USEC(PERIOD_US / 2), 0);
    if (ret < 0) {
        printk("Error %d: failed to set pulse width\n", ret);
        return 1;
    }

    printk("Zephyr Example Application %s\n", APP_VERSION_STRING);

    ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        printk("Error %d: failed to configure LED pin\n", ret);
        return 1;
    }

    uart_thread_start();

    while (1) {
        k_msleep(10);  // Small delay to prevent busy-waiting

        // /* Toggle LED state */
        // gpio_pin_toggle_dt(&led);
        // k_sleep(K_MSEC(50));
        // gpio_pin_toggle_dt(&led);
        // k_sleep(K_MSEC(1000));
    }

    return 0;
}
