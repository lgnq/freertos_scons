/*
 * FreeRTOS V202011.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/* FreeRTOS.org includes. */
#include "FreeRTOS.h"

/* Demo application includes. */
#include "drv_led.h"

void led_init(void)
{
    GPIO_SetDir(1, 1<<18, 1);
    GPIO_SetDir(1, 1<<20, 1);
    GPIO_SetDir(1, 1<<21, 1);
    GPIO_SetDir(1, 1<<23, 1);
}

void led_on(char led)
{
    switch (led)
    {
    case 0: /* P1.18 = 1 */
        GPIO_SetValue(1, 1<<18);
        break;
    case 1: /* P1.20 = 1 */
        GPIO_SetValue(1, 1<<20);
        break;
    case 2: /* P1.21 = 1 */
        GPIO_SetValue(1, 1<<21);
        break;
    case 3: /* P1.23 = 1 */
        GPIO_SetValue(1, 1<<23);
        break;
    default:
        break;
    }
}

void led_off(char led)
{
    switch(led)
    {
    case 0: /* P1.18 = 0 */
        GPIO_ClearValue(1, 1<<18);
        break;
    case 1: /* P1.20 = 0 */
        GPIO_ClearValue(1, 1<<20);
        break;
    case 2: /* P1.21 = 0 */
        GPIO_ClearValue(1, 1<<21);
        break;
    case 3: /* P1.23 = 0 */
        GPIO_ClearValue(1, 1<<23);
        break;
    default:
        break;
    }
}