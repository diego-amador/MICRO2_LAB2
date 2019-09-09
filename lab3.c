/*
 * lab3.c
 *
 *  Created on: Sep 9, 2019
 *      Author: csant
 */

/* For usleep() */
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>



void *lab3Thread(void *arg0)
{
    GPIO_init();

    return (NULL);
}

