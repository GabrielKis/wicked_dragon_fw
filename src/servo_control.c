/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>

/* PWM device for servo control */
#define PERIOD_US 20000          /* 20ms (50Hz) - standard servo period */

#define SERVO_THREAD_STACK_SIZE 1024
#define SERVO_THREAD_PRIORITY 7

#if !DT_NODE_HAS_STATUS(DT_ALIAS(led0), okay)
#error "LED0 GPIO node is not ready"
#endif

#if !DT_NODE_HAS_STATUS(DT_ALIAS(pwm0), okay)
#error "LED0 GPIO node is not ready"
#endif

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct pwm_dt_spec servo = PWM_DT_SPEC_GET(DT_ALIAS(pwm0));

// Thread initialization
K_THREAD_STACK_DEFINE(servo_thread_stack, SERVO_THREAD_STACK_SIZE);
struct k_thread servo_thread_data;
k_tid_t servo_thread_id;

// -----------------------------------
// Static functions
// -----------------------------------
static void servo_thread_entry(void *p1, void *p2, void *p3)
{
    int ret;
    printk("Servo Thread Started\n");

    if (!device_is_ready(servo.dev)) {
        printk("Error: PWM device %s is not ready\n", servo.dev->name);
        return;
    }

    if (!device_is_ready(led.port)) {
        printk("Error: LED device %s is not ready\n", led.port->name);
        return;
    }

    /* Set initial position (middle) */
    ret = pwm_set(servo.dev, 0, PWM_USEC(PERIOD_US), PWM_USEC(PERIOD_US / 2), 0);
    if (ret < 0) {
        printk("Error %d: failed to set pulse width\n", ret);
        return;
    }

    ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        printk("Error %d: failed to configure LED pin\n", ret);
        return;
    }

    while (1) {
        /* Toggle LED state */
        gpio_pin_toggle_dt(&led);
        k_sleep(K_MSEC(50));
        gpio_pin_toggle_dt(&led);
        k_sleep(K_MSEC(1000));
    }

    return;
}

// -----------------------------------
// Public functions
// -----------------------------------
void servo_thread_start(void)
{
    servo_thread_id = k_thread_create(&servo_thread_data, servo_thread_stack,
                                    K_THREAD_STACK_SIZEOF(servo_thread_stack),
                                    servo_thread_entry, NULL, NULL, NULL,
                                    SERVO_THREAD_PRIORITY, 0, K_NO_WAIT);
    printk("SERVO thread created\n");
}
