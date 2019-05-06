/******************************************************************************
 * EECS 397
 *
 * File name: phototransistor.cpp
 *
 *Description:
 *   interaction with phototransistor input
 *
 *Notes:
 *
 * Author(s): Chude (Frank), Qian
 *
 * Created: 2/20/2019
 * Last Modified: 3/29/2019
 *
 *Changes: begin construct the functional region
 *
 ******************************************************************************/

/*****************************************************************************
 * !Include stuffs from other files:
 ******************************************************************************/
#include "phototransistor.h"
#include "io_pins.h"
#include "mbed.h"

//! Debug Selector:

/*****************************************************************************
 * !Private Constants: Only accessible to code below this point in this file
 ******************************************************************************/


/*****************************************************************************
 * !Private Data: Only accessible to code below this point in this file
 ******************************************************************************/
extern Serial pc;
BusOut mux_select(ANA_MUX_S0,ANA_MUX_S1,ANA_MUX_S2,ANA_MUX_S3);
AnalogIn signal(ANA_MUX_OUT);
/*****************************************************************************
******************************************************************************
* !Private Function Prototypes: These functions may only be called by code
*   below this point in this file
******************************************************************************
******************************************************************************/
/*****************************************************************************
******************************************************************************
* !Public Function Definitions: Function header comments for thses function
*   are in the header file
******************************************************************************
******************************************************************************/
void scanChannels(float * userInput){
    for(int i = 0; i<16; i++){
        mux_select = i; // channel selection
        wait_us(100); // wait 100usec
        userInput[i]=signal.read();
    }
}