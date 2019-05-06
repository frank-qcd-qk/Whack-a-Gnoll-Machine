/******************************************************************************
 * EECS 397
 *
 * File name: laser.cpp
 *
 *Description:
 *   Sends Command to laser module
 *
 *Notes:
 *
 * Author(s): Chude (Frank), Qian
 *
 * Created: 2/20/2019
 * Last Modified: 3/29/2019
 *
 *Changes: started with constructing basic laser commands
 *
 ******************************************************************************/

/*****************************************************************************
 * !Include stuffs from other files:
 ******************************************************************************/
#include "laser.h"
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
DigitalOut laserOut(LZR_ENABLE);//This sets up the chip select for the AS1107

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
void laser_on(){
    pc.printf("[Laser] Laser on...\r\n");
    laserOut.write(1);
}

void laser_off(){
    pc.printf("[Laser] Laser off...\r\n");
    laserOut.write(0);
}

void laser_init(){
    laserOut.write(0);
    pc.printf("[Laser] Laser ready and stand by...\r\n");
    //Potentially for future uses.
}

