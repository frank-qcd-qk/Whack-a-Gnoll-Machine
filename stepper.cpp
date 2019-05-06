

/******************************************************************************
 * EECS 397
 *
 * File name: stepper.cpp
 *
 *Description:
 *   Sends Command to stepper module
 *
 *Notes:
 *
 * Author(s): Chude (Frank), Qian
 *
 * Created: 2/28/2019
 * Last Modified: 3/29/2019
 *
 *Changes: Started Constructing this file...
 *Fixes: isolated SPI ID to SPI file
 *
 ******************************************************************************/

/*****************************************************************************
 * !Include stuffs from other files:
 ******************************************************************************/
#include "stepper.h"
#include "io_pins.h"
#include "mbed.h"
#include "spi.h"

/*****************************************************************************
 * !Private Constants: Only accessible to code below this point in this file
 ******************************************************************************/
//* DRV8806 setup
#define DRV8806_SPI_NO_BITS 8
#define DRV8806_SPI_MODE 0
#define DRV8806_SPI_FREQ 1000000
#define STEPPER_DELAY 1600  // In microseconds
/*****************************************************************************
 * !Private Data: Only accessible to code below this point in this file
 ******************************************************************************/
extern Serial pc;
DigitalOut stp_nss(
    STP_DRV8806_NSS);  // This sets up the chip select for the AS1107
struct spi_cfg drv8806 {
    SPI_DRV8806_ID, STP_DRV8806_NSS, DRV8806_SPI_MODE, DRV8806_SPI_FREQ,
        DRV8806_SPI_NO_BITS,
};

//*Button interface setup
DigitalIn jog_cw(UI_JOG_LEFT_BUTTON, PullUp);
DigitalIn jog_ccw(UI_JOG_RIGHT_BUTTON, PullUp);
DigitalIn cal(UI_CAL_BUTTON,PullUp);
DigitalIn home_sensor(STP_HOME_SENSOR,PullUp);

//*Stepper value define
#define MAX_STEPPER_DISTANCE 400
#define STEPPER_DISTANCE_UNINIT -1

/*****************************************************************************
******************************************************************************
* !Private Function Prototypes: These functions may only be called by code
*   below this point in this file
******************************************************************************
******************************************************************************/
int stepper_position;
bool is_configured_stp = false;
/*****************************************************************************
******************************************************************************
* !Public Function Definitions: Function header comments for thses function
*   are in the header file
******************************************************************************
******************************************************************************/

/*****************************************************************************/
void stp_init() { 
    spi_send(drv8806, 0x00); 
    stepper_position = STEPPER_DISTANCE_UNINIT;
}

void stp_step(int op_mode) {
    static int step_table[4] = {0x03, 0x06, 0x0c, 0x09};
    static int current_pointer = 0;
    if (op_mode == STP_CW) {
        //* Overshoot handling
        if (stepper_position<=5000){
            if(stepper_position != STEPPER_DISTANCE_UNINIT){
                stepper_position++;
                //pc.printf("[Stepper] Current Position is: %i",stepper_position);
            }
        }else
        {
            pc.printf("[STEPPER]ERROR! EXCEED MAXIMUM POSITION! STOP...\r\n");
            while(1>0);
        }
        //*turn table handling
        if (current_pointer == 3){
            current_pointer = 0;
        }else{
            current_pointer++;
        }
        spi_send(drv8806, step_table[current_pointer]);
        wait_us(STEPPER_DELAY);
    } else if (op_mode == STP_CCW) {
        //* Overshoot handling, sensor protection
        if ((stepper_position>=0 || home_sensor.read() != 0) || (stepper_position == STEPPER_DISTANCE_UNINIT)){
            if(stepper_position != STEPPER_DISTANCE_UNINIT){
                stepper_position--;
                //pc.printf("[Stepper] Current Position is: %i",stepper_position);
            }
        }else
        {
            pc.printf("[STEPPER]ERROR! EXCEED HOME POSITION! STOP...\r\n");
            while(1>0);
        }
        //* turn table handling
        if (current_pointer == 0){
            current_pointer = 3;
        }else{
            current_pointer--;
        }
        spi_send(drv8806, step_table[current_pointer]);
        wait_us(STEPPER_DELAY);
    }

}

void stp_find_home(){
    pc.printf("[STEPPER] WARNING: FINDING HOME... DO NOT INTERRUPT!\r\n");
    stp_init();
    //! If the home sensor happens to be actuated when the test begins, 
    //! move the stepper away from the sensor (left)  100 steps.  If the home 
    //! sensor is still actuated, something is wrong
    if (home_sensor.read() == 0) {
        for(int i = 0; i < 100; i++)
            stp_step(STP_LEFT);
        if (home_sensor == 0) {
            //print a fault message to the console and freeze
            pc.printf("[STEPPER]ERROR! STICKY HOME_SENSOR ERROR!\r\n");
            while(1);
        }
    }
    // If the sensor is no longer actuated, then move the stepper back toward
    // the home sensor until the sensor is first actuated.
    while (home_sensor.read() != 0) {
        stp_step(STP_RIGHT);
    }
    stepper_position = 0;
    pc.printf("[STEPPER] HOME FOUND!\r\n");
    wait(1); //for requirement need!
    is_configured_stp = true;
}

void stp_manual_op(){
    //* Dumb Proof
    if (jog_ccw.read() == 0 && jog_cw.read() == 0) {
        pc.printf("[STEPPER]STOP PRESSING TWO BUTTONS!!!!!\r\n");
    } else if (jog_cw.read() == 0) {
        stp_step(STP_LEFT);
        pc.printf("[STEPPER] Commanded left!");
    } else if ((jog_ccw.read() == 0) &&(home_sensor.read()!=0) ) { //Low level lock
        stp_step(STP_RIGHT);
        pc.printf("[STEPPER] Commanded right!");
    } else{
        pc.printf("[STEPPER]NO command");
    }    
}

int stp_read_location(){
    return(stepper_position);
}

void stp_move_specifically(int position){
    if (!is_configured_stp){
        pc.printf("[STEPPER] position configuration status failed! Resetting....\r\n");
        stp_find_home();
    }
    if (position < stepper_position){
        while(position<stepper_position){
            stp_step(STP_RIGHT);
            //pc.printf("[STEPPER] Requested Position: %i, current position %i.\r\n", position,stepper_position);
        }
    } else if (position > stepper_position)
    {
        while(position>stepper_position){
            stp_step(STP_LEFT);
            //pc.printf("[STEPPER] Requested Position: %i, current position %i.\r\n", stepper_position, position);
        }
    } else
    {
        pc.printf("[STEPPER] Right on position!\r\n");
    }
    
}