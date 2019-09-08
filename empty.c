/*
 * Copyright (c) 2015-2019, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== empty.c ========
 */

/* For usleep() */
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/devices/cc32xx/driverlib/gpio.h>
#include <ti/devices/cc32xx/inc/hw_types.h>
#include <ti/devices/cc32xx/inc/hw_memmap.h>
// #include <ti/drivers/I2C.h>
// #include <ti/drivers/SPI.h>
// #include <ti/drivers/UART.h>
// #include <ti/drivers/Watchdog.h>

/* Board Header file */
#include "Board.h"

/* Defines for LCD Instructions */
//#define DCB_ON 0b00001111 // Display ON, Cursor On, Cursor Blink On, 40us
//#define DCB_OFF 0b00001000 // Display OFF, Cursor OFF, Cursor Blink OFF, 40 us
//#define CLEAR_DISPLAY 0b00000001 // Clear Display 1.53 ms
//#define FUNCTION_SET 0b00111000 // Function Set: 8-bit mode, 2-lines, 5x8 Grid, 40us
//#define EMS_NOSHIFT 0b00000110 // Cursor Increment, No Shift, 40 us
//#define EMS_SHIFT 0b00000111 // Cursor Incremenr, Shift, 40 us
//#define C_2LINE 0b11000000 // Cursor to Second Line, 40 us
//#define SET_CURSOR 0b00010000 // Display OFF, Cursor ON, Blink OFF, 40 us
#define DISPLAY_ON 0x0E // Display ON, Cursor On, Cursor Blink On, 40us
#define CURSOR_ON 0x0A
#define CURSOR_OFF 0x00
#define DISPLAY_OFF 0x08 // Display OFF, Cursor OFF, Cursor Blink OFF, 40 us
#define CLEAR_DISPLAY 0x01 // Clear Display 1.53 ms
#define FUNCTION_SET 0x38 // Function Set: 8-bit mode, 2-lines, 5x8 Grid, 40us
#define EMS_NOSHIFT 0x06 // Cursor Increment, No Shift, 40 us
#define EMS_SHIFT 0x07 // Cursor Incremenr, Shift, 40 us
#define C_2LINE 0b11000000 // Cursor to Second Line, 40 us
#define SET_CURSOR 0b00010000 // Display OFF, Cursor ON, Blink OFF, 40 us
#define LOW (0)
#define HIGH (1)


/*
 * Write Operation Timing
 *
 * RS, RW, E = 0
 * RS, RW, E = 1
 * delay 25 ns
 * write data / send command
 * delay 40 ns
 * RS, RW, E = 0
 * delay 25 ns
 * delay 10 ns
*/


void blink();
void enable();
void LCD_init();
void write_char(unsigned char *byte);
void send_command(uint8_t hex);
void init_pins();

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    /* Call driver init functions */
    GPIO_init();
    init_pins();

    LCD_init();

    write_char("A");

//    write_char("Hello World!");

    /* Initialization of LCD 16x2 Display */
    return 0;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////   Lab#2: Experiment 1 ///////////////////////////////////////////////////////////////////////////////////////////////////

void blink()
{
    /* 1 second delay */
    uint32_t time = 1;

    /* Configure the LED pin */
    GPIO_setConfig(Board_GPIO_LED0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    /* Turn on user LED */
    GPIO_write(Board_GPIO_LED0, Board_GPIO_LED_ON);

    while (1) {
        sleep(time);
        GPIO_write(Board_GPIO_LED0, Board_GPIO_LED_OFF);
        sleep(time);
        GPIO_write(Board_GPIO_LED0, Board_GPIO_LED_ON);
    }
}

void init_pins()
{
    /* Configures the pins as output */
    /* Data Pins */
    GPIO_setConfig(GPIO_P18_D0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(GPIO_P08_D1, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(GPIO_P45_D2, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(GPIO_P07_D3, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(GPIO_P06_D4, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(GPIO_P21_D5, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(GPIO_P55_D6, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(GPIO_P15_D7, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    /* Control Pins */
    GPIO_setConfig(GPIO_P02_E, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(GPIO_P62_RS, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(GPIO_P01_RW, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    /* Writes a LOW to all pins */
    GPIO_write(GPIO_P18_D0, LOW);
    GPIO_write(GPIO_P08_D1, LOW);
    GPIO_write(GPIO_P45_D2, LOW);
    GPIO_write(GPIO_P07_D3, LOW);
    GPIO_write(GPIO_P06_D4, LOW);
    GPIO_write(GPIO_P21_D5, LOW);
    GPIO_write(GPIO_P55_D6, LOW);
    GPIO_write(GPIO_P15_D7, LOW);

    GPIO_write(GPIO_P02_E, LOW);
    GPIO_write(GPIO_P62_RS, LOW);
    GPIO_write(GPIO_P01_RW, LOW);

}

void enable()
{
    GPIO_toggle(GPIO_P02_E);
    usleep(10000);
    GPIO_toggle(GPIO_P02_E);
}

void check_busy()
{
    uint8_t busy_flag;

    GPIO_setConfig(GPIO_P15_D7, GPIO_CFG_INPUT | GPIO_CFG_IN_PD);

    GPIO_write(GPIO_P01_RW, HIGH);
    GPIO_write(GPIO_P62_RS, LOW);
    GPIO_write(GPIO_P02_E, HIGH);

    usleep(1);

    busy_flag = GPIO_read(GPIO_P15_D7);

    printf("%d\n", busy_flag);

    while(busy_flag){

    }


    GPIO_write(GPIO_P02_E, LOW);

    usleep(1);

    GPIO_write(GPIO_P01_RW, LOW); // RW = 0 , to enable write)

    usleep(40);

    GPIO_setConfig(GPIO_P15_D7, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
}

void LCD_init()
{


    GPIO_write(GPIO_P01_RW, 0); // Default set to logic-0 (low) since we are only going to write


    usleep(40000); //delay at least 40 ms
    GPIO_write(GPIO_P62_RS, 0); // RS = 0

    GPIO_write(GPIO_P02_E, 0); // E = 0
    usleep(1000);
    GPIO_write(GPIO_P02_E, 1);
    usleep(1000);


    send_command(FUNCTION_SET); // Sets 8-bit mode, 2 lines, 5x8 matrix
    usleep(42000); // delay at least 40 us
    send_command(FUNCTION_SET); // Sets 8-bit mode, 2 lines, 5x8 matrix
    usleep(120); // delay at least 4 us
    send_command(FUNCTION_SET); // Sets 8-bit mode, 2 lines, 5x8 matrix
    usleep(40); // delay at least 4 us

    send_command(DCB_OFF); // Display OFF, Cursor OFF, Blink OFF
    usleep(40); // wait 40 us
    send_command(DCB_ON);
    usleep(40); // wait 40 us
    send_command(CLEAR_DISPLAY); // Clear Display
    usleep(2000); // wait 1.53 ms
    send_command(DCB_ON); // Display ON, Cursor ON, Blink ON
    usleep(40); // wait 40 us
    send_command(EMS_SHIFT); // Entry Mode Set with No Shift

    usleep(10000);

    enable();

    GPIO_write(GPIO_P62_RS, 1);
}

void write_char(unsigned char *byte)
{
    GPIO_write(GPIO_P62_RS, 1); // RS = 0
    sleep(0.000000060); // delay of at least 60 ns
    check_busy();

    while(*byte) // while byte[i] != \0
    {

        GPIO_write(GPIO_P15_D7, (*byte >> 7) & 0x01);
        GPIO_write(GPIO_P55_D6, (*byte >> 6) & 0x01);
        GPIO_write(GPIO_P21_D5, (*byte >> 5) & 0x01);
        GPIO_write(GPIO_P06_D4, (*byte >> 4) & 0x01);
        GPIO_write(GPIO_P07_D3, (*byte >> 3) & 0x01);
        GPIO_write(GPIO_P45_D2, (*byte >> 2) & 0x01);
        GPIO_write(GPIO_P08_D1, (*byte >> 1) & 0x01);
        GPIO_write(GPIO_P18_D0, (*byte >> 0) & 0x01);

        enable();

        byte++;
    }

}

void send_command(uint8_t hex)
{
    check_busy();

    usleep(10000);  // delay at least 25 ns

    GPIO_write(GPIO_P15_D7, (1 << 1) - 1 & (hex >> 7));
    printf("%d", (1 & (hex >> 7)));

    GPIO_write(GPIO_P55_D6, (1 & (hex >> 6)));
    printf("%d", (1 & (hex >> 6)));

    GPIO_write(GPIO_P21_D5, (1 & (hex >> 5)));
    printf("%d", 1 & (hex >> 5));

    GPIO_write(GPIO_P06_D4, (1 & (hex >> 4)));
    printf("%d", 1 & (hex >> 4));

    GPIO_write(GPIO_P07_D3, (1 & (hex >> 3)));
    printf("%d", 1 & (hex >> 3));

    GPIO_write(GPIO_P45_D2, (1 & (hex >> 2)));
    printf("%d", 1 & (hex >> 2));

    GPIO_write(GPIO_P08_D1, (1 & (hex >> 1)));
    printf("%d", 1 & (hex >> 1));

    GPIO_write(GPIO_P18_D0, (1 & (hex >> 0)));
    printf("%d\n", 1 & (hex >> 0));

    usleep(40); // delay at least 40 ns

    enable();

    usleep(40); // delay at least 25 ns
}










