/******************************************************************************
 * EECS 397
 *
 * File name: utility.cpp
 *
 *Description:
 *   Hosts pregame start and pregame calibration
 *
 *Notes:
 *
 * Author(s): Chude (Frank), Qian
 *
 * Created: 4/20/2019
 * Last Modified: 4/22/2019
 *
 *
 ******************************************************************************/

/*****************************************************************************
 * !Include stuffs from other files:
 ******************************************************************************/
#include "display.h"
#include "game.h"
#include "io_pins.h"
#include "laser.h"
#include "phototransistor.h"
#include "spi.h"
#include "stepper.h"
#include "mbed.h"

/*****************************************************************************
 * !Private Constants: Only accessible to code below this point in this file
 ******************************************************************************/

/*****************************************************************************
 * !Private Data: Only accessible to code below this point in this file
 ******************************************************************************/
extern Serial pc;
#define ASCII_ESC 27
#define PTT_CALIBRATION_CATCH 0.8

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

void Calibrate_Target(int station_id, int* position_array, float * sensor_read){
    laser_on();
    for (int i=0; i<8; i++){
        scanChannels(sensor_read);
        //for each sensor, identify the position that mostly brightly illuminates 
        //the sensor and store that position as the location of that sensor in your
        // position array
        if (station_id==STATION_A){
            while(1>0){
                scanChannels(sensor_read);
                //wait_ms(1);
                stp_step(STP_LEFT);
                if(sensor_read[7-i]>=PTT_CALIBRATION_CATCH){
                    int previous,current;
                    previous = sensor_read[7-i];
                    do{
                        stp_step(STP_LEFT);
                        scanChannels(sensor_read);
                        previous = current;
                        sensor_read[7-i] = current;
                    }while(current>previous);
                    position_array[7-i] = stp_read_location();
                    pc.printf("[Utility] Location recorded for %i at %i \r\n", 7-i, stp_read_location());
                    wait_ms(100);
                    // for (int timer =0; timer <50; timer++){
                    //     stp_step(STP_LEFT);
                    //     wait_ms(1);
                    // }
                    break;
                }
            }

        }else
        {
            while(1>0){
                scanChannels(sensor_read);
                wait_ms(1);                
                stp_step(STP_LEFT);
                if(sensor_read[15-i]>=PTT_CALIBRATION_CATCH){
                    int previous,current;
                    previous = sensor_read[15-i];
                    do{
                        stp_step(STP_LEFT);
                        scanChannels(sensor_read);
                        previous = current;
                        sensor_read[15-i] = current;
                    }while(current>previous);
                    position_array[7-i] = stp_read_location();
                    pc.printf("[Utility] Location recorded for %i at %i\r\n", 7-i, stp_read_location());
                    wait_ms(100);                    
//                    for (int timer =0; timer <50; timer++){
//                        stp_step(STP_LEFT);
//                        wait_ms(1);
//                    }
                    break;
                }
            } 
        }
        
    }
    //When you have successfully  located all eight sensors, turn off the laser, 
    //return to home position, set the calibration status to CALIBRATED, and 
    //print the location of each sensor to the console.  Also calculate and 
    //print the relative distance between the first and second sensor, the second 
    //and third, etc.  In other words, print the position of the second sensor
    // minus the position of the first sensor, and so on.      
    laser_off();
    //!Final Verification;
    for (int i = 0; i<8; i++){
        laser_on();
        stp_move_specifically(position_array[i]);
        scanChannels(sensor_read);
        if (station_id == STATION_A){
            if(sensor_read[i]>=PTT_CATCH_THRESHOLD){
                pc.printf("[Utility] Position %i verified, value readout is: %f\r\n",i,sensor_read[i]);
                laser_off();
            }else
            {
                pc.printf("{FATAL ERROR!}: Sensor Repeatability Test Failed....Sensor Read: %f\r\n",sensor_read[i]);
                while(1);
            }
            
        }else
        {
            if(sensor_read[8+i]>=PTT_CATCH_THRESHOLD){
                pc.printf("[Utility] Position %i verified, value readout is: %f\r\n",i,sensor_read[8+i]);
                laser_off();
            }else
            {
                pc.printf("{FATAL ERROR!}: Sensor Repeatability Test Failed....Sensor Read: %f\r\n",sensor_read[8+i]);
                while(1);
            }
        }
        
    }    
}

bool preFlight(int station_id, int* position_array, float * sensor_read){
    pc.printf("[Utility] Starts Pre-start checklist....\r\n");    
    initiate_as1107_all(true);
    get_ID(station_id);
    set_offset();
    //? Sometimes the call for cancel display test does not get executed, double calling for safety.
    initiate_as1107_all(false);
    wait_ms(10);
    initiate_as1107_all(false);
    wait_ms(10);
    initiate_as1107_zeros();
    pc.printf("[Utility] ID Set, Offset set, initiated all zeros!...\r\n");
    pc.printf("[Utility] Starts Calibration...\r\n");
    pc.printf("[Utility] Homing Sensor First....\r\n");
    //!Finds home first!
    stp_find_home();
    //!Determine the sensor positions!
    pc.printf("[Utility] Start Laser Position Calibration....\r\n");
    Calibrate_Target(station_id, position_array, sensor_read);
    //!check Array Loading
    for (int i = 0; i<16; i++){
        if (position_array[i]<0){
            pc.printf("[Utility] Sensor Calibration Failed...Array Empty\r\n");
            return false;
        }
    }

    //!Reset
    stp_move_specifically(10);
    return true;
}