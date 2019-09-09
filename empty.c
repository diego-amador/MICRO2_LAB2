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


/* Board Header file */
#include "Board.h"

/* Defines for LCD Instructions */
#define DISPLAY_ON 0x0F // Display ON, Cursor On, Cursor Blink On, 40us
#define DISPLAY_OFF 0x0B // Display OFF, Cursor OFF, Cursor Blink OFF, 40 us
#define CLEAR_DISPLAY 0x01 // Clear Display 1.53 ms
// 8-bit mode
#define FUNCTION_SET_8bit 0x38 // Function Set: 8-bit mode, 2-lines, 5x8 Grid, 40us
#define FUNCTION_SET_4bit 0x20 // Function Set: 4-bit, 2-Lines, 5x8 grid, 40us
#define EMS 0x06 // Cursor Increment, No Shift, 40 us
#define EMS_SHIFT 0x07 // Cursor Incremenr, Shift, 40 us
#define C_2LINE 0x40// Cursor to Second Line, 40 us
#define SET_CURSOR 0b10000000 // Display OFF, Cursor ON, Blink OFF, 40 us
#define HOME 0b00000010
#define LOW (0)
#define HIGH (1)

/* Laboratory Tasks */
void blink();
void PushButton();
void LCD();
void ComplementaryTask();

/* Toggles Enable in LCD */
void enable();
/* Initializes LCD */
void LCD_init();
void LCD_init_4bit();
void write_char(uint8_t chars[]);
void send_command(uint8_t hex);
void write(uint8_t byte);

void send_command_4bit(uint8_t hex);
void write_4bit(uint8_t byte);

int buttonUpCallbackFxn(uint8_t index, uint8_t size);
int buttonDownCallbackFxn(uint8_t index, uint8_t size);

void printToLCD(uint8_t text[], int toggle);

/*  */
void PinConfig();
void PinConfig_4bit();

/*
 *  ======== mainThread ========
 */
void *lab2Thread(void *arg0)
{
    /* Call driver init functions */
    GPIO_init();
//    blink();
//    PushButton();
    LCD();
//    ComplementaryTask();


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

        while (GPIO_read(Board_GPIO_BUTTON0) == 1)
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
    PinConfig_4bit();
    LCD_init_4bit();

    write_char("ICOM 4217");

    usleep(50);
    send_command_4bit(HOME);
    usleep(50);
    send_command_4bit(SET_CURSOR | C_2LINE);
    usleep(50);

    write_char("Dame la A");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////   Lab#2: Complementary Task ///////////////////////////////////////////////////////////////////////////////////////////////////

void ComplementaryTask()
{

    const int size = 18;
    uint8_t arr[size][16] =
    {
     "Maria", "Marrero",
     "Yamil", "Gonzalez",
     "Diego", "Amador",
     "Christian", "Santiago",
     "ICOM", "4217",
     "CC3220S", "SimpleLink",
     "UPRM", "CAAM",
     "Mayaguez", "Puerto Rico",
     "Rio Piedras", "Bayamon"
    };

    uint8_t i = 0;


    PinConfig();
    LCD_init();

    // Configures Board GPIOs as Input
    GPIO_setConfig(Board_GPIO_BUTTON0, GPIO_CFG_INPUT | GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);
    GPIO_setConfig(Board_GPIO_BUTTON1, GPIO_CFG_INPUT | GPIO_CFG_IN_PD | GPIO_CFG_IN_INT_FALLING);

    write_char(arr[i]);
    usleep(50);
    send_command(HOME);
    usleep(50);
    send_command(SET_CURSOR | C_2LINE);
    usleep(50);

    write_char(arr[i+1]);

    while(1){

        usleep(50);
        send_command(HOME);
        usleep(50);


        if (GPIO_read(Board_GPIO_BUTTON1) == 1)
        {
            LCD_init();

            int oldpos = i;
            i = buttonUpCallbackFxn(i, size);

            write_char(arr[i]);
            usleep(50);
            send_command(HOME);
            usleep(50);
            send_command(SET_CURSOR | C_2LINE);
            usleep(50);

            if (i == size-1)
            {
                write_char(arr[0]);
            }else
            {
                write_char(arr[i+1]);
            }

        }
        if (GPIO_read(Board_GPIO_BUTTON0) == 0)
        {
            LCD_init();

            int oldpos = i;

            i = buttonDownCallbackFxn(i, size);

            write_char(arr[i]);

            send_command(HOME);
            usleep(50);
            send_command(SET_CURSOR | C_2LINE);
            usleep(50);

            write_char(arr[oldpos]);

//            i = buttonDownCallbackFxn(i
        }
    }





}

int buttonUpCallbackFxn(uint8_t index, uint8_t size)
{

    if (index==(size-1))
    {
        return 0;
    }
    else
    {
        return index+1;
    }
}


int buttonDownCallbackFxn(uint8_t index, uint8_t size)
{

    if (index<=0){
        return size-1;
    }
    else
    {
        return index-1;
    }

}


void PinConfig_4bit()
{
    /* Configures the pins as output */
    /* Data Pins */
    GPIO_setConfig(GPIO_P07_D4, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(GPIO_P61_D5, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(GPIO_P08_D6, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(GPIO_P18_D7, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    /* Control Pins */
    GPIO_setConfig(GPIO_P64_E, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(GPIO_P63_RS, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

}

void PinConfig()
{
    /* Configures the pins as output */
    /* Data Pins */
    GPIO_setConfig(GPIO_P15_D0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(GPIO_P50_D1, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(GPIO_P21_D2, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(GPIO_P06_D3, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(GPIO_P07_D4, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(GPIO_P61_D5, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(GPIO_P08_D6, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(GPIO_P18_D7, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    /* Control Pins */
    GPIO_setConfig(GPIO_P64_E, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(GPIO_P63_RS, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////   Complementary Task ///////////////////////////////////////////////////////////////////////////////////////////////////



void LCD_init()
{

    sleep(0.020);
    GPIO_write(GPIO_P64_E, LOW); // E = 0
    usleep(50);


    send_command(FUNCTION_SET_8bit); // Sets 8-bit mode, 2 lines, 5x8 matrix
    usleep(50); // delay at least 40 us
    send_command(FUNCTION_SET_8bit); // Sets 8-bit mode, 2 lines, 5x8 matrix
    usleep(50); // delay at least 4 us
    send_command(FUNCTION_SET_8bit); // Sets 8-bit mode, 2 lines, 5x8 matrix
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

void LCD_init_4bit()
{

    sleep(0.020);
    GPIO_write(GPIO_P64_E, LOW); // E = 0
    usleep(50);


    send_command_4bit(0x02); // Sets 8-bit mode, 2 lines, 5x8 matrix
    usleep(50); // delay at least 40 us
    send_command_4bit(FUNCTION_SET_4bit); // Sets 8-bit mode, 2 lines, 5x8 matrix
    usleep(50); // delay at least 4 us
    send_command_4bit(FUNCTION_SET_4bit); // Sets 8-bit mode, 2 lines, 5x8 matrix
    usleep(50); // delay at least 4 us

    send_command_4bit(DISPLAY_OFF); // Display OFF, Cursor OFF, Blink OFF
    usleep(50); // wait 40 us
    send_command_4bit(CLEAR_DISPLAY);
    usleep(50); // wait 40 us
    send_command_4bit(EMS); // Clear Display
    usleep(50); // wait 1.53 ms
    send_command_4bit(DISPLAY_ON); // Entry Mode Set with No Shift
    usleep(50);
}

void enable()
{
    usleep(50);
    GPIO_write(GPIO_P64_E, HIGH);
    usleep(50);
    GPIO_write(GPIO_P64_E, LOW);
    usleep(50);
}


void write_char(uint8_t chars[])
{
    volatile int i = 0;
    GPIO_write(GPIO_P63_RS, HIGH); // RS = 1

    while(chars[i] != 0) // while byte[i] != \0
    {
        write_4bit(chars[i]);
        i++;
        sleep(0.0001);
    }
}

void send_command(uint8_t hex)
{
    GPIO_write(GPIO_P63_RS, LOW); // RS = 0

    GPIO_write(GPIO_P18_D7, (1 & (hex >> 7)));
    GPIO_write(GPIO_P08_D6, (1 & (hex >> 6)));
    GPIO_write(GPIO_P61_D5, (1 & (hex >> 5)));
    GPIO_write(GPIO_P07_D4, (1 & (hex >> 4)));
    GPIO_write(GPIO_P06_D3, (1 & (hex >> 3)));
    GPIO_write(GPIO_P21_D2, (1 & (hex >> 2)));
    GPIO_write(GPIO_P50_D1, (1 & (hex >> 1)));
    GPIO_write(GPIO_P15_D0, (1 & (hex >> 0)));
    usleep(40); // delay at least 40 ns

    enable();

    usleep(40); // delay at least 25 ns
}

void send_command_4bit(uint8_t hex)
{
    GPIO_write(GPIO_P63_RS, LOW); // RS = 0

        GPIO_write(GPIO_P18_D7, (1 & (hex >> 7)));
        GPIO_write(GPIO_P08_D6, (1 & (hex >> 6)));
        GPIO_write(GPIO_P61_D5, (1 & (hex >> 5)));
        GPIO_write(GPIO_P07_D4, (1 & (hex >> 4)));

        usleep(50);

        GPIO_write(GPIO_P18_D7, (1 & (hex >> 3)));
        GPIO_write(GPIO_P08_D6, (1 & (hex >> 2)));
        GPIO_write(GPIO_P61_D5, (1 & (hex >> 1)));
        GPIO_write(GPIO_P07_D4, (1 & (hex >> 0)));


    //    GPIO_write(GPIO_P06_D3, (1 & (hex >> 3)));
    //    GPIO_write(GPIO_P21_D2, (1 & (hex >> 2)));
    //    GPIO_write(GPIO_P50_D1, (1 & (hex >> 1)));
    //    GPIO_write(GPIO_P15_D0, (1 & (hex >> 0)));
        usleep(40); // delay at least 40 ns

        enable();

        usleep(40); // delay at least 25 ns
}

void write(uint8_t byte)
{
    usleep(10000);  // delay at least 25 ns

    GPIO_write(GPIO_P18_D7, (1 & (byte >> 7)));
    GPIO_write(GPIO_P08_D6, (1 & (byte >> 6)));
    GPIO_write(GPIO_P61_D5, (1 & (byte >> 5)));
    GPIO_write(GPIO_P07_D4, (1 & (byte >> 4)));
    GPIO_write(GPIO_P06_D3, (1 & (byte >> 3)));
    GPIO_write(GPIO_P21_D2, (1 & (byte >> 2)));
    GPIO_write(GPIO_P50_D1, (1 & (byte >> 1)));
    GPIO_write(GPIO_P15_D0, (1 & (byte >> 0)));

    usleep(40); // delay at least 40 ns

    enable();

    usleep(40); // delay at least 25 ns
}

void write_4bit(uint8_t byte)
{
    usleep(10000);  // delay at least 25 ns

    GPIO_write(GPIO_P18_D7, (1 & (byte >> 7)));
    GPIO_write(GPIO_P08_D6, (1 & (byte >> 6)));
    GPIO_write(GPIO_P61_D5, (1 & (byte >> 5)));
    GPIO_write(GPIO_P07_D4, (1 & (byte >> 4)));

    usleep(50);

    GPIO_write(GPIO_P18_D7, (1 & (byte >> 3)));
    GPIO_write(GPIO_P08_D6, (1 & (byte >> 2)));
    GPIO_write(GPIO_P61_D5, (1 & (byte >> 1)));
    GPIO_write(GPIO_P07_D4, (1 & (byte >> 0)));

    usleep(40); // delay at least 40 ns

    enable();

    usleep(40); // delay at least 25 ns
}










