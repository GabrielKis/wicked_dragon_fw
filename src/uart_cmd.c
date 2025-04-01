/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/sys/ring_buffer.h>

#define UART_THREAD_STACK_SIZE 1024
#define UART_THREAD_PRIORITY 7

#define RING_BUF_SIZE 64

// Thread initialization
K_THREAD_STACK_DEFINE(uart_thread_stack, UART_THREAD_STACK_SIZE);
struct k_thread uart_thread_data;
k_tid_t uart_thread_id;

// UART and ring buffer initialization
RING_BUF_DECLARE(uart_ringbuf, RING_BUF_SIZE);
static const struct device *uart_dev = DEVICE_DT_GET(DT_NODELABEL(uart0));

// -----------------------------------
// Static functions
// -----------------------------------
static void process_uart_data(uint8_t data)
{
    if (data == '1') {
        printk("LED ON\n");
    } else if (data == '0') {
        printk("LED OFF\n");
    }
}

static void uart_cb(const struct device *dev, void *user_data)
{
    uint8_t received_byte;
    if (!uart_irq_update(dev)) {
        return;
    }

    while (uart_irq_rx_ready(dev)) {
        uart_fifo_read(dev, &received_byte, 1);
        ring_buf_put(&uart_ringbuf, &received_byte, 1);
    }
}

static void uart_thread_entry(void *p1, void *p2, void *p3)
{
    printk("UART Thread Started\n");

    if (!device_is_ready(uart_dev)) {
        printk("UART device not ready\n");
        return;
    }

    uart_irq_callback_user_data_set(uart_dev, uart_cb, NULL);
    uart_irq_rx_enable(uart_dev);

    uint8_t data;
    while (1) {
        while (ring_buf_get(&uart_ringbuf, &data, 1) > 0) {
            process_uart_data(data);
        }
        k_msleep(10);
    }
}

// -----------------------------------
// Public functions
// -----------------------------------
void uart_thread_start(void)
{
    uart_thread_id = k_thread_create(&uart_thread_data, uart_thread_stack,
                                     K_THREAD_STACK_SIZEOF(uart_thread_stack),
                                     uart_thread_entry, NULL, NULL, NULL,
                                     UART_THREAD_PRIORITY, 0, K_NO_WAIT);
    printk("UART thread created\n");
}