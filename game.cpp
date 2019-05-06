/******************************************************************************
 * EECS 397
 *
 * File name: game.cpp
 *
 *Description:
 *   Hosts the game module
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
int miss_count = 0;
int hit_count = 0;
int local_station_id = -1;
int round_no = 1;

//Off set the array read
int gnoller_offset = 0;
int whacker_offset = 0;

/*****************************************************************************
 * !Private Data: Only accessible to code below this point in this file
 ******************************************************************************/
extern Serial pc;

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

/*****************************************************************************/
void get_ID(int global_id){
    local_station_id = global_id;
}

/*****************************************************************************/
void set_offset(){
    if (local_station_id ==-1){
        pc.printf("{FATAL ERROR}: Game CPP local station id check failed\r\n");
    }
    if (local_station_id == STATION_B){
        gnoller_offset = 8;
        whacker_offset = 0;
    }
    if(local_station_id == STATION_A){
        gnoller_offset = 0;
        whacker_offset = 8;
    }
}

/*****************************************************************************/
void gnoller(float * sensor_measure, int * position_array) {
    miss_count = 0;
    hit_count = 0;
    pc.printf("====================[GNOLLER GAME] Recieve Game Start Command, reset internal counter!====================\r\n");
    for (int volley = 0; volley<MAX_VOLLEY_COUNT; volley++){
        pc.printf("**********[GNOLlER GAME] This is Volley Number %i **********\r\n",volley);
        //!Generate a position
        int point_position = rand() % 8;
        pc.printf("[GNOLlER GAME] Generated Random Number is: %i. Stepper Command Position is: %i\r\n",point_position,stp_read_location());
        //!Move to the position
        stp_move_specifically(position_array[point_position]);
        //!Turn on laser
        laser_on();
        wait_ms(LASER_POINT_DELAY);
        //!Laser on confirm and LED lit up:
        scanChannels(sensor_measure);
        pc.printf("[GNOLLER GAME]Currently requesting to see sensor number: %i \r\n",point_position+gnoller_offset);
        if (sensor_measure[point_position+gnoller_offset]>PTT_CATCH_THRESHOLD){
            pc.printf("[GNOLlER GAME] Sucessfully registered lazer point!\r\n");
            dsp_lit_targetboard(point_position); //! Lit Target Board
            wait_ms(TARGET_BOARD_DELAY);
        }else
        {
            pc.printf("{FATAL ERROR}: Laser is on, but sensor no readout!\r\n");
            pc.printf("{FATAL ERROR}: Current command position: %i, Current Stepper Position %i, Current Sensor Readout %f.\r\n",point_position, stp_read_location(),sensor_measure[point_position+gnoller_offset]);
            while(1);
        }
        //!Tick Tock and loop check sensor value till timeout
        Timer ticktock;
        ticktock.start();
        pc.printf("[GNOLLER GAME] Timer Started....\r\n");
        float time_elapsed = -1;
        while (time_elapsed<VOLLEY_TIMEOUT){
            scanChannels(sensor_measure);
            time_elapsed = ticktock.read();
            if(sensor_measure[point_position+whacker_offset]>PTT_CATCH_THRESHOLD){
                hit_count ++;
                pc.printf("[GNOLLER GAME] HIT DETECTED! Congrats!\r\n");
                break;
            } else
            {
                for (int local_pointer = 0; local_pointer < 8; local_pointer ++){
                    if(sensor_measure[local_pointer+whacker_offset]>PTT_CATCH_THRESHOLD && local_pointer != point_position){
                        pc.printf("[GNOLLER GAME] WRONG SENSOR HIT!!! MISS Count ++\r\n");
                        miss_count++;
                        break;
                    }
                }
            }
        }
        if (time_elapsed>VOLLEY_TIMEOUT){
            pc.printf("[GNOLLER GAME] MISSed due to TIMEOUT! MISS COUNT ++\r\n");
            miss_count++;
        }else
        {
            pc.printf("[GNOLLER GAME] This WHACK took %f seconds.\r\n", time_elapsed);
        }
        //! end and send score
        laser_off();
        dsp_turnoff_targetboard();        
        scanChannels(sensor_measure);
        bool wait_whacker_off = true;
        do{
            for (int local_pointer = 0; local_pointer < 8; local_pointer ++){
                if(sensor_measure[local_pointer+whacker_offset]<PTT_CATCH_THRESHOLD ){
                    wait_whacker_off = false;
                    break;
                }
            }
        }while (wait_whacker_off);
        pc.printf("[GNOLLER GAME] System clear....\r\n");
        wait(1);
        scoreUpdater();
        pc.printf("**********[GNOLLER GAME] Volley %i ENDED**********\r\n",volley);
        pc.printf("\r\n");
        pc.printf("\r\n");
        pc.printf("\r\n");        
    }
    miss_count = -1;
    hit_count = -1;
    pc.printf("====================[GNOLLER GAME] Volley %i ENDED====================\r\n");
}

/*****************************************************************************/
void whacker(float * sensor_measure, int * position_array){
    pc.printf("====================[Whacker GAME] Recieve Game Start Command====================\r\n");    
    for(int volley = 0; volley < MAX_VOLLEY_COUNT; volley++){
        pc.printf("**********[Whacker GAME] Current Volley is: %i**********\r\n", volley);     
        int desired_position = -1;
        bool sensor_catched =  false;
        //! Wait for gnoll laser and check my sesnor and start timer guard
        pc.printf("[Whacker Game] Timer Started......\r\n");
        Timer ticktock;
        ticktock.start();        
        while(!sensor_catched && (ticktock.read()<WHACKER_TIMEOUT)){
            //pc.printf("[WHACKER GAME] Waiting for GNOLL!...\r\n");
            scanChannels(sensor_measure);
            for (int i = 0; i<8; i++){
                if(sensor_measure[i+whacker_offset]>PTT_CATCH_THRESHOLD){
                    desired_position = i;
                    sensor_catched = true;
                    pc.printf("[WHACKER GAME] Sucessfully catched gnoll point at position %i\r\n", desired_position);
                    break;
                }
            }
        }
        
        // Whacker timeout handler
        if(ticktock.read()>WHACKER_TIMEOUT){
            pc.printf("{Fatal ERROR!}: [WHACKER GAME] NO GNOLL HIT DETECTED!....\r\n");
            while(1);
        }else
        {
            pc.printf("[WHACKER GAME] This WHACK took %f seconds.\r\n", ticktock.read());
        }
        
        // Desired position Guard
        if(desired_position<0 || desired_position >7){
            pc.printf("{Fatal ERROR!}: [WHACKER GAME] Commanding an impossible position! %i\r\n",desired_position);
            while(1);
        }
        
        //! Move to position to hit gnoll
        stp_move_specifically(position_array[desired_position]);
        
        //! Laser on check laser success:
        laser_on();
        wait_ms(LASER_POINT_DELAY);        
        scanChannels(sensor_measure);
        pc.printf("[GNOLLER GAME]Currently requesting to see sensor number: %i \r\n",desired_position+gnoller_offset);        
        if (sensor_measure[desired_position+gnoller_offset]>PTT_CATCH_THRESHOLD){
            pc.printf("[WHACKER GAME] Sucessfully registered lazer point!\r\n");
            dsp_lit_targetboard(desired_position); //! Lit Target Board
            wait_ms(TARGET_BOARD_DELAY);
        }else
        {
            //Laser alignment fail checker:
            pc.printf("{FATAL ERROR}: [WHACKER GAME] Laser is on, but sensor no readout!\r\n");
            pc.printf("{FATAL ERROR}: [WHACKER GAME] Current command position: %i, Current Stepper Position %i, Current Sensor Readout %f.\r\n",desired_position, stp_read_location(),sensor_measure[desired_position+gnoller_offset]);
            while(1);
        }

        //! GNOLLER LAZER SHUTOFF CHECK WITH TIMEOUT:
        pc.printf("[WHACKER GAME] Start LAZER TURNOFF COUNTER...\r\n");
        ticktock.reset();
        ticktock.start();
        do {
            scanChannels(sensor_measure);
            //pc.printf("[WHACKER Game] Waiting the sensor to turn off...\r\n");
        }while(sensor_measure[desired_position+whacker_offset]>PTT_CATCH_THRESHOLD && ticktock.read()<WHACKER_TIMEOUT);
        pc.printf("[WHACKER Game] Gnoll turned off!...\r\n");
        // GNOLLER LAZER STICKY HANDLER
        if(ticktock.read()>WHACKER_TIMEOUT){
            pc.printf("{FATAL ERROR}: [WHACKER GAME] Sticky Laser for GNOLLER!\r\n");
        }
        // Reset for next one:
        laser_off();
        dsp_turnoff_targetboard();
    pc.printf("**********[WHACKER GAME] Volley %i ENDED**********\r\n",volley);
    pc.printf("\r\n");
    pc.printf("\r\n");
    pc.printf("\r\n");
    }
    pc.printf("====================[WHACKER Game] Whacker Round Complete...Handover......====================\r\n");
}

/*****************************************************************************/
void scoreUpdater(){
    if(hit_count == -1 || miss_count == -1){
        pc.printf("{FATAL ERROR}: Gnoller Failed to update the score!\r\n");
    }
    char display_array[4] = {0};
    int display_value = hit_count*100+miss_count;
    pc.printf("[SCORE UPDATER] Requested Miss Count is: %02i. Requested Hit Count is: %02i. Output shoud be: %04i\r\n",miss_count,hit_count,display_value);
    if (display_value>1500 || display_value%100>15 || display_value<0){
        pc.printf("{FATAL ERROR}: Hit count and miss count Out of bound!\r\n");
        while(1);
    }
    bin2bcd_array(display_value,display_array);
    dsp_bcd(display_array);
}