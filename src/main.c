/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <app_version.h>
#include "uart_cmd.h"
#include "servo_control.h"

int main(void)
{
    printk("Zephyr Example Application %s\n", APP_VERSION_STRING);

    servo_thread_start();
    uart_thread_start();

    while (1) {
        k_msleep(1000);  // Delay to avoid CPU usage
    }

    return 0;
}
