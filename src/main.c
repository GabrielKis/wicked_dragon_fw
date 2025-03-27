/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <app_version.h>

int main(void)
{
	printk("Zephyr Example Application %s\n", APP_VERSION_STRING);
	printk("Use the sensor to change LED blinking period\n");
	uint32_t count=0;

	while (1) {
		printk("count: %d\n", count++);
		k_sleep(K_MSEC(1000));
	}

	return 0;
}
