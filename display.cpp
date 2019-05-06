/******************************************************************************
 * EECS 397
 *
 * File name: display.cpp
 *
 *Description:
 *   Sends Command to Display module
 *
 *Notes:
 *
 * Author(s): Chude (Frank), Qian
 *
 * Created: 2/20/2019
 * Last Modified: 3/29/2019
 *
 *Changes: Updated and added with target board display test
 *Fixes: isolated SPI ID to SPI file
 *
 ******************************************************************************/

//todo: 1. check if can set a private variable to record if intialization has been performed

/*****************************************************************************
 * !Include stuffs from other files:
 ******************************************************************************/
#include "display.h"
#include "io_pins.h"
#include "mbed.h"
#include "spi.h"

//! Debug Selector:
#define VERSION1 //Use Mod method, faster

/*****************************************************************************
 * !Private Constants: Only accessible to code below this point in this file
 ******************************************************************************/
//* Seven Segment Display Driver OP Mode Define
#define DSP_TST_ON 0x0f01   // Display test on
#define DSP_TST_OFF 0x0f00  // Display test off
#define DSP_DECODE_MASK 0x090f  // B-hex decode on digits 0-3, no decode on others
#define DSP_BRIGHTNESS 0x0a0f  // Set Display to Max brightness
#define DSP_SCAN_LIMIT 0x0b04  // Display Digits 0-4
#define DSP_NORM_OP 0x0c01     // normal operation, reset feature register

//* AS1107 SPI configure
#define AS1107_SPI_NO_BITS 16
#define AS1107_SPI_MODE 0
#define AS1107_SPI_FREQ 1000000

//* Digit Configuration
#define Digit1 0x0100
#define Digit2 0x0200
#define Digit3 0x0300
#define Digit4 0x0400
#define Digit5 0x0500

//* Config send flag
bool is_configured_dsp = false;

/*****************************************************************************
 * !Private Data: Only accessible to code below this point in this file
 ******************************************************************************/
extern Serial pc;
struct spi_cfg as1107 {
    SPI_AS1107_ID, DSP_AS1107_NSS, AS1107_SPI_MODE, AS1107_SPI_FREQ,
        AS1107_SPI_NO_BITS,
};
/*****************************************************************************
******************************************************************************
* !Private Function Prototypes: These functions may only be called by code
*   below this point in this file
******************************************************************************
******************************************************************************/
DigitalOut dsp_nss(DSP_AS1107_NSS);//This sets up the chip select for the AS1107

/*****************************************************************************
******************************************************************************
* !Public Function Definitions: Function header comments for thses function
*   are in the header file
******************************************************************************
******************************************************************************/
void configure_as1107() {
    spi_send(as1107, DSP_DECODE_MASK);  // Set Decode
    spi_send(as1107, DSP_BRIGHTNESS);   // Set brightness
    spi_send(as1107, DSP_SCAN_LIMIT);   // set scan digit
    spi_send(as1107, DSP_NORM_OP);      // Operation set to normal
    pc.printf("[Display] DP Module Configuration Complete! \r\n");
    is_configured_dsp = true;
}

/*****************************************************************************/
void initiate_as1107_zeros() {
    if (!is_configured_dsp){
        configure_as1107();
    }
    spi_send(as1107, Digit1 + 0);  // Digit 1 Write 0;
    spi_send(as1107, Digit2 + 0);  // Digit 2 Write 0;
    spi_send(as1107, Digit3 + 0);  // Digit 3 Write 0;
    spi_send(as1107, Digit4 + 0);  // Digit 4 Write 0;
    spi_send(as1107, Digit5 + 0);  // Digit 4 Write 0;
    pc.printf("[Display] Program Init: all digit should read as 0... \r\n");
}

/*****************************************************************************/
void initiate_as1107_all(bool mode) {
    if (!is_configured_dsp){
        configure_as1107();
    }
    
    if (mode) {
        spi_send(as1107, DSP_TST_ON);  // Turn all segment on!
        pc.printf("[Display] DP Test ON: ALL digit should light up... \r\n");
        wait(2.0);
    } else {
        spi_send(as1107, DSP_TST_OFF);  // Turn all segment on!
        pc.printf("[Display] DP Test OFF: Resume to previous state... \r\n");
        wait(2.0);
    }
}

/*****************************************************************************/
void bin2bcd_array(int userInput, char* bcdLocal) {
    int cpy_userInput = userInput;
#ifdef VERSION1
    int pointer = 0;
    while (cpy_userInput != 0) {
        bcdLocal[pointer] = cpy_userInput % 10;
        cpy_userInput /= 10;
        pointer++;
    }
#endif
#ifdef VERSION2
    char tmp_str[5] = {0};
    //! The trick for fixing digit messup: use %04d to auto fill zeros in the
    //! front of number
    sprintf(tmp_str, "%04d", cpy_userInput);
    for (int counter = 0; tmp_str[counter] != 0x00; counter++) {
        bcdLocal[3 - counter] = tmp_str[counter] - 0x30;
    }
#endif
}

/*****************************************************************************/
void dsp_bcd(char* bcd_array) {
    int instruction = 0;
    for (int pointer = 0; pointer < 4; pointer++) {
        instruction = 16 * 16 * (pointer + 1) + bcd_array[pointer];
        spi_send(as1107, instruction);
    }
}

/*****************************************************************************/
void dsp_lit_targetboard(int position){
    //! Fault Handling mechanism
    if (position>7 || position<0){
        pc.printf("{FATAL ERROR} [DISPLAY]: Commanded an impossible LED position! \r\n");
        while(1);
    }
    //Convert utilizing the character of 2^n
    int command = (int)pow(2.0,(double)position)+Digit5;
    pc.printf("[DISPLAY] Current Position Requested: %i, Current Command Set is: %i \r\n", position, command);
    spi_send(as1107,command);
}


/*****************************************************************************/
void dsp_turnoff_targetboard(){
    spi_send(as1107, Digit5);
    pc.printf("[DISPLAY] Target Board turned off\r\n");
}