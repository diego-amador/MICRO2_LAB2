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
#define DISPLAY_ON 0x0F // Display ON, Cursor On, Cursor Blink On, 40us
#define DISPLAY_OFF 0x0B // Display OFF, Cursor OFF, Cursor Blink OFF, 40 us
#define CLEAR_DISPLAY 0x01 // Clear Display 1.53 ms
#define FUNCTION_SET 0x38 // Function Set: 8-bit mode, 2-lines, 5x8 Grid, 40us
#define EMS 0x06 // Cursor Increment, No Shift, 40 us
#define EMS_SHIFT 0x07 // Cursor Incremenr, Shift, 40 us
#define C_2LINE 0x40// Cursor to Second Line, 40 us
#define SET_CURSOR 0b10000000 // Display OFF, Cursor ON, Blink OFF, 40 us
#define HOME 0b00000010
#define LOW (0)
#define HIGH (1)


void blink();
void PushButton();
void LCD();
void enable();
void LCD_init();
void write_char(uint8_t chars[]);
void send_command(uint8_t hex);
void write(uint8_t byte);
void PinConfig();

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    /* Call driver init functions */
    GPIO_init();
    GPIO_setConfig(Board_GPIO_BUTTON1, GPIO_CFG_IN_PD | GPIO_CFG_IN_INT_RISING);

    while(1)
    {
        volatile int pressed;
        pressed = GPIO_read(Board_GPIO_BUTTON1);
        printf("%d\n", pressed);
    }
//    blink();
//    PushButton();
//    LCD();


    return 0;

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////   Lab#2: Experiment 1 ///////////////////////////////////////////////////////////////////////////////////////////////////

void blink()
{
    /* 1 second delay */
    uint32_t time = 1;

    /* Configure the LED pin */
    GPIO_setConfig(Board_GPIO_LED0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH);

    /* Turn on user LED */
    GPIO_write(Board_GPIO_LED0, HIGH);

    while (1) {
        sleep(time);
        GPIO_write(Board_GPIO_LED0, LOW);
        sleep(time);
        GPIO_write(Board_GPIO_LED0, HIGH);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////   Lab#2: Experiment 2 ///////////////////////////////////////////////////////////////////////////////////////////////////

void PushButton()
{
    while(1)
    {

        while (GPIO_read(Board_GPIO_BUTTON0) == 0)
        {

            GPIO_write(Board_GPIO_LED0, HIGH);

        }

        GPIO_write(Board_GPIO_LED0, LOW);

    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////   Lab#2: Experiment 3 ///////////////////////////////////////////////////////////////////////////////////////////////////

void LCD()
{
    PinConfig();
    LCD_init();

    write_char("FUCK THIS");

    usleep(50);
    send_command(HOME);
    usleep(50);
    send_command(SET_CURSOR | C_2LINE);
    usleep(50);

    write_char("Christian Diego");
}



void PinConfig()
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

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////   Complementary Task ///////////////////////////////////////////////////////////////////////////////////////////////////



void LCD_init()
{

    sleep(0.020);
    GPIO_write(GPIO_P02_E, LOW); // E = 0
    usleep(50);


    send_command(FUNCTION_SET); // Sets 8-bit mode, 2 lines, 5x8 matrix
    usleep(50); // delay at least 40 us
    send_command(FUNCTION_SET); // Sets 8-bit mode, 2 lines, 5x8 matrix
    usleep(50); // delay at least 4 us
    send_command(FUNCTION_SET); // Sets 8-bit mode, 2 lines, 5x8 matrix
    usleep(50); // delay at least 4 us

    send_command(DISPLAY_OFF); // Display OFF, Cursor OFF, Blink OFF
    usleep(50); // wait 40 us
    send_command(CLEAR_DISPLAY);
    usleep(50); // wait 40 us
    send_command(EMS); // Clear Display
    usleep(50); // wait 1.53 ms
    send_command(DISPLAY_ON); // Entry Mode Set with No Shift
    usleep(50);
}

void enable()
{
    usleep(50);
    GPIO_write(GPIO_P02_E, HIGH);
    usleep(50);
    GPIO_write(GPIO_P02_E, LOW);
    usleep(50);
}


void write_char(uint8_t chars[])
{
    volatile int i = 0;
    GPIO_write(GPIO_P62_RS, HIGH); // RS = 1

    while(chars[i] != 0) // while byte[i] != \0
    {
        write(chars[i]);
        i++;
        sleep(0.005);
    }
}

void send_command(uint8_t hex)
{
    GPIO_write(GPIO_P62_RS, LOW); // RS = 0

    GPIO_write(GPIO_P15_D7, (1 & (hex >> 7)));
    GPIO_write(GPIO_P55_D6, (1 & (hex >> 6)));
    GPIO_write(GPIO_P21_D5, (1 & (hex >> 5)));
    GPIO_write(GPIO_P06_D4, (1 & (hex >> 4)));
    GPIO_write(GPIO_P07_D3, (1 & (hex >> 3)));
    GPIO_write(GPIO_P45_D2, (1 & (hex >> 2)));
    GPIO_write(GPIO_P08_D1, (1 & (hex >> 1)));
    GPIO_write(GPIO_P18_D0, (1 & (hex >> 0)));
    usleep(40); // delay at least 40 ns

    enable();

    usleep(40); // delay at least 25 ns
}

void write(uint8_t byte)
{
    usleep(10000);  // delay at least 25 ns

    GPIO_write(GPIO_P15_D7, (1 & (byte >> 7)));
    GPIO_write(GPIO_P55_D6, (1 & (byte >> 6)));
    GPIO_write(GPIO_P21_D5, (1 & (byte >> 5)));
    GPIO_write(GPIO_P06_D4, (1 & (byte >> 4)));
    GPIO_write(GPIO_P07_D3, (1 & (byte >> 3)));
    GPIO_write(GPIO_P45_D2, (1 & (byte >> 2)));
    GPIO_write(GPIO_P08_D1, (1 & (byte >> 1)));
    GPIO_write(GPIO_P18_D0, (1 & (byte >> 0)));
    usleep(40); // delay at least 40 ns

    enable();

    usleep(40); // delay at least 25 ns
}










