/*
 *  etml0400.c
 *  RSP display driver init
 *
 *  Copyright (c) 2019 Markku Nivala, Innokas medical
 *
 */

#include <common.h>
#include <asm/gpio.h>
#include "etml0400.h"

void set_command (unsigned char y)
{
    unsigned char i;
    gpio_direction_output(47, 0);
    gpio_direction_output(44, 0);
    ndelay(10);
    gpio_direction_output(45, 0);
    ndelay(70);
    gpio_direction_output(44, 1);
    ndelay(80);
    for (i = 0; i < 8; i++)
    {
        gpio_direction_output(44, 0);
        ndelay(10);
        if (y & 0x80)
        {
            gpio_direction_output(45, 1);
            ndelay(70);
        }
        else
        {
            gpio_direction_output(45, 0);
            ndelay(70);
        }
        gpio_direction_output(44, 1);
        ndelay(80);
        y = y << 1;
    }
    gpio_direction_output(47, 1);

}

void set_data(unsigned char w)
{
    unsigned char i;
    gpio_direction_output(47, 0);
    gpio_direction_output(44, 0);
    ndelay(10);
    gpio_direction_output(45, 1);
    ndelay(70);
    gpio_direction_output(44, 1);
    ndelay(80);
    for (i = 0; i < 8; i++)
    {
        gpio_direction_output(44, 0);
        ndelay(10);
        if (w & 0x80)
        {
            gpio_direction_output(45, 1);
            ndelay(70);
        }
        else
        {
            gpio_direction_output(45, 0);
            ndelay(70);
        }
        gpio_direction_output(44, 1);
        ndelay(80);
        w = w << 1;
    }
    gpio_direction_output(47, 1);
}


u16 read_data(unsigned char w)
{
    int i;
    u16 value = 0;
    ndelay(40);
    gpio_direction_output(47, 0);
    gpio_direction_output(44, 0);
    ndelay(10);

    gpio_direction_output(45, 0);
    ndelay(70);
    gpio_direction_output(44, 1);
    ndelay(80);
    for (i = 0; i < 8; i++)
    {
        gpio_direction_output(44, 0);
        ndelay(10);
        if (w & 0x80)
        {
            gpio_direction_output(45, 1);
            ndelay(70);
        }
        else
        {
            gpio_direction_output(45, 0);
            ndelay(70);
        }
        gpio_direction_output(44, 1);
        ndelay(80);
        w = w << 1;
    }

    gpio_direction_output(45, 0);
    for (i = 7; i >= 0; i--)
    {
        gpio_direction_output(44, 0);
        ndelay(10);
        value |= gpio_direction_input(46) << i;
        ndelay(70);
        gpio_direction_output(44, 1);
        ndelay(80);
    }

    gpio_direction_output(47, 1);
    return value;
}

int etml0400_init()
{
    int ret;
    ret = gpio_request(68, "display");
    if (ret)
    {
        printf("Could not get reset handle");
    }
    gpio_direction_output(68, 1);
    mdelay(5);
    gpio_direction_output(68, 0);
    mdelay(10);
    gpio_direction_output(68, 1);
    mdelay(200);

    ret = gpio_request(47, "display");
    if (ret)
    {
        printf("Could not get cs handle");
    }
    ret = gpio_request(46, "display");
    if (ret)
    {
        printf("Could not get read handle");
    }
    ret = gpio_request(45, "display");
    if (ret)
    {
        printf("Could not get write handle");
    }
    ret = gpio_request(44, "display");
    if (ret)
    {
        printf("Could not get clk handle");
    }

    set_command(0xFF);
    set_data(0xFF);
    set_data(0x98);
    set_data(0x06);
    set_data(0x04);
    set_data(0x01);       //Change to Page 1

    set_command(0x08);
    set_data(0x18);

    set_command(0x20);
    set_data(0x01);  // RGB interface selection 01=SYNC mode£¬00=DE mode


    set_command(0x21);
    set_data(0x01);     //DE = 1 Active

    set_command(0x30);
    set_data(0x02);     //480 X 800

    set_command(0x31);
    set_data(0x02);     //Column Inversion

    set_command(0x40);
    set_data(0x16);     //2.5VCI/-2VCL  15

    set_command(0x41);
    set_data(0x55);     //6/-6 44  55

    set_command(0x42);
    set_data(0x02);     //DDVDH+VCI-VCL/DDVDL-DDVDH

    set_command(0x43);
    set_data(0x09);     //VGH_CLAMP 0FF ;

    set_command(0x44);
    set_data(0x09);     //VGL_CLAMP ON ;

    set_command(0x50);
    set_data(0x78);     //50

    set_command(0x51);
    set_data(0x78);     //50

    set_command(0x52);
    set_data(0x00);     //Flicker

    set_command(0x53);
    set_data(0x80);     //Flicker6D

    set_command(0x57);
    set_data(0x50);     //

    set_command(0x60);
    set_data(0x07);     //SDTI

    set_command(0x61);
    set_data(0x00);     //CRTI

    set_command(0x62);
    set_data(0x08);     //EQTI

    set_command(0x63);
    set_data(0x00);     //PCTI

    //++++++++++++++++++ Gamma Setting ++++++++++++++++++//

    set_command(0xA0);
    set_data(0x00);     //Gamma 0

    set_command(0xA1);
    set_data(0x0B);     //Gamma 4

    set_command(0xA2);
    set_data(0x1A);     //Gamma 8

    set_command(0xA3);
    set_data(0x11);     //Gamma 16

    set_command(0xA4);
    set_data(0x0A);     //Gamma 24

    set_command(0xA5);
    set_data(0x1B);     //Gamma 52

    set_command(0xA6);
    set_data(0x08);     //Gamma 80

    set_command(0xA7);
    set_data(0x06);     //Gamma 108

    set_command(0xA8);
    set_data(0x03);     //Gamma 147

    set_command(0xA9);
    set_data(0x0C);     //Gamma 175

    set_command(0xAA);
    set_data(0x03);     //Gamma 203

    set_command(0xAB);
    set_data(0x0B);     //Gamma 231

    set_command(0xAC);
    set_data(0x0F);     //Gamma 239

    set_command(0xAD);
    set_data(0x2F);     //Gamma 247

    set_command(0xAE);
    set_data(0x27);     //Gamma 251

    set_data(0xAF);
    set_data(0x00);     //Gamma 255

    ///==============Nagitive
    set_command(0xC0);
    set_data(0x00);     //Gamma 0

    set_command(0xC1);
    set_data(0x04);     //Gamma 4

    set_command(0xC2);
    set_data(0x0B);     //Gamma 8

    set_command(0xC3);
    set_data(0x0E);     //Gamma 16

    set_command(0xC4);
    set_data(0x08);     //Gamma 24

    set_command(0xC5);
    set_data(0x14);     //Gamma 52

    set_command(0xC6);
    set_data(0x09);     //Gamma 80

    set_command(0xC7);
    set_data(0x07);     //Gamma 108

    set_command(0xC8);
    set_data(0x06);     //Gamma 147

    set_command(0xC9);
    set_data(0x09);     //Gamma 175

    set_command(0xCA);
    set_data(0x08);     //Gamma 203

    set_command(0xCB);
    set_data(0x04);     //Gamma 231

    set_command(0xCC);
    set_data(0x0B);     //Gamma 239

    set_command(0xCD);
    set_data(0x29);     //Gamma 247

    set_command(0xCE);
    set_data(0x27);     //Gamma 251

    set_command(0xCF);
    set_data(0x00);     //Gamma 255





    //+++++++++++++++++++++++++++++++++++++++++++++++++++//

    //****************************************************************************//
    //****************************** Page 6 Command ******************************//
    //****************************************************************************//
    set_command(0xFF);
    set_data(0xFF);
    set_data(0x98);
    set_data(0x06);
    set_data(0x04);
    set_data(0x06);        //Change to Page 6

    set_command(0x00);
    set_data(0x21);        //

    set_command(0x01);
    set_data(0x09);        //

    set_command(0x02);
    set_data(0x00);        //

    set_command(0x03);
    set_data(0x00);        //

    set_command(0x04);
    set_data(0x01);        //

    set_command(0x05);
    set_data(0x01);        //

    set_command(0x06);
    set_data(0x80);        //

    set_command(0x07);
    set_data(0x05);        //

    set_command(0x08);
    set_data(0x02);        //

    set_command(0x09);
    set_data(0x80);        //

    set_command(0x0A);
    set_data(0x00);        //

    set_command(0x0B);
    set_data(0x00);        //

    set_command(0x0C);
    set_data(0x0A);        //

    set_command(0x0D);
    set_data(0x0A);        //

    set_command(0x0E);
    set_data(0x00);        //

    set_command(0x0F);
    set_data(0x00);        //

    set_command(0x10);
    set_data(0xE0);        //

    set_command(0x11);
    set_data(0xE4);        //

    set_command(0x12);
    set_data(0x04);        //

    set_command(0x13);
    set_data(0x00);        //

    set_command(0x14);
    set_data(0x00);        //

    set_command(0x15);
    set_data(0xC0);        //

    set_command(0x16);
    set_data(0x08);        //

    set_command(0x17);
    set_data(0x00);        //

    set_command(0x18);
    set_data(0x00);        //

    set_command(0x19);
    set_data(0x00);        //

    set_command(0x1A);
    set_data(0x00);        //

    set_command(0x1B);
    set_data(0x00);        //

    set_command(0x1C);
    set_data(0x00);        //

    set_command(0x1D);
    set_data(0x00);        //


    set_command(0x20);
    set_data(0x01);        //

    set_command(0x21);
    set_data(0x23);        //

    set_command(0x22);
    set_data(0x45);        //

    set_command(0x23);
    set_data(0x67);        //

    set_command(0x24);
    set_data(0x01);        //

    set_command(0x25);
    set_data(0x23);        //

    set_command(0x26);
    set_data(0x45);        //

    set_command(0x27);
    set_data(0x67);        //

    set_command(0x30);
    set_data(0x01);        //

    set_command(0x31);
    set_data(0x11);        //GOUT1

    set_command(0x32);
    set_data(0x00);        //GOUT2

    set_command(0x33);
    set_data(0xEE);        //GOUT3

    set_command(0x34);
    set_data(0xFF);        //GOUT4

    set_command(0x35);
    set_data(0xCB);        //GOUT5   GCK4

    set_command(0x36);
    set_data(0xDA);        //GOUT6   GCK3

    set_command(0x37);
    set_data(0xAD);        //GOUT7   GCK2

    set_command(0x38);
    set_data(0xBC);        //GOUT8   GCK1

    set_command(0x39);
    set_data(0x76);        //GOUT9   STV1

    set_command(0x3A);
    set_data(0x67);        //GOUT10  STV2

    set_command(0x3B);
    set_data(0x22);        //GOUT11

    set_command(0x3C);
    set_data(0x22);        //GOUT12

    set_command(0x3D);
    set_data(0x22);        //GOUT13

    set_command(0x3E);
    set_data(0x22);        //GOUT14

    set_command(0x3F);
    set_data(0x22);        //

    set_command(0x40);
    set_data(0x22);        //

    set_command(0x52);
    set_data(0x10);        //

    set_command(0x53);
    set_data(0x10);        //


    //****************************************************************************//
    //****************************** Page 7 Command ******************************//
    //****************************************************************************//
    set_command(0xFF);
    set_data(0xFF);
    set_data(0x98);
    set_data(0x06);
    set_data(0x04);
    set_data(0x07);        //Change to Page 7

    set_command(0x17);
    set_data(0x22);     //VGL_RE

    set_command(0x02);
    set_data(0x77);     //

    set_command(0xE1);
    set_data(0x79);     //

    //****************************************************************************//

    set_command(0xFF);
    set_data(0xFF);
    set_data(0x98);
    set_data(0x06);
    set_data(0x04);
    set_data(0x00);      //Change to Page 0

    set_command(0x36);      //Display Control       Scan direction
    set_data(0x03);

    set_command(0x11);
    set_data(0x00);
    ndelay(120);

    set_command(0x29);      // Display On
    set_data(0x00);

    set_command(0x35);
    set_data(0x00);     //
    ndelay(120);

    return 0;
}

