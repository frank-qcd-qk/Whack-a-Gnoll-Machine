/******************************************************************************
 * EECS 397
 *
 * Assignment Name: Lab6_WaG
 *
 * Author: Frank (Chude), Qian
 *
 * Purpose: The beginning of a long journey for WaG machine
 *
 * Last Modified: 3/01/2019
 *
 *******************************************************************************/
#include "display.h"
#include "game.h"
#include "io_pins.h"
#include "laser.h"
#include "phototransistor.h"
#include "spi.h"
#include "stepper.h"
#include "mbed.h"
#include "utility.h"
#include "test.h"

//* Serial Define
#define ASCII_ESC 27
#define BAUD_RATE 460800
//* Useful constructors:
Serial pc(USBTX, USBRX, BAUD_RATE);  // Serial Initialization

//* Game related declaration:
DigitalIn win(UI_SPARE,PullUp);
DigitalIn main_start(UI_START_BUTTON,PullUp);
DigitalIn station_ss(UI_STATION_SELECT,PullUp);
DigitalIn myButton(USER_BUTTON);
extern DigitalIn cal;
extern DigitalIn win;


//*Global Storage
int STATION_ID = -1;
float PTT_Readout[16];
int laser_position[8]={-1};
bool preFlight_check = false;

int main(void)
{
    pc.printf("%c[2J", ASCII_ESC);
    pc.printf("%c[2H", ASCII_ESC);
    pc.printf("===============Welcome to WAG Machine===============\r\n");
    //!Obtain station ID.
    STATION_ID = station_ss.read();
    pc.printf("[Main] Current Station ID is %i\r\n",STATION_ID);
    srand(time(NULL)); //* Random Seed generator
    //!Starts system Initialization.
    while(main_start.read()==1){
        
        if(cal.read() == 0){
            while(cal.read()==0);
            preFlight_check = preFlight(STATION_ID,laser_position,PTT_Readout);
            if (!preFlight_check){
                pc.printf("{Fatal Error}: Calibration Failed!\r\n");
                while(1);
            }else{
                pc.printf("[Main] Pre-Flight Configuration Complete! \r\n");
                break;
            }         
        }
        if(myButton.read()==1){
            pc.printf("USER BUTTON PRESSED!...\r\n");
            while(myButton.read()==1){pc.printf("USER BUTTON PRESSED!...\r\n");};
            TEST_start();
        }   
    }
    pc.printf("[Main] Pre-Game sequence complete, game handler take over...\r\n");
    wait(1);
    
    //! Main Game
    while(1>0){
        if(STATION_ID == STATION_A){
            //! Station Flip loop
            while(1>0){
                //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                pc.printf("[Main] I am station %i, and serving as GNOLLER soon.... \r\n", STATION_ID);                
                //? Wait for button hit
                while(main_start.read() == 1);
                pc.printf("[Main] I am station %i, and serving as GNOLLER now.... \r\n", STATION_ID);                
                //? Final Calibration check
                if (!preFlight_check){
                    pc.printf("{FATAL ABORT!}: preFlight_check not passed!....\r\n");
                }else
                {
                    pc.printf("[MAIN] GAME STARTS NOW..... Game_Gnoller Take Controll....\r\n");
                }
                //? Hand Over
                gnoller(PTT_Readout,laser_position);
                pc.printf("[Main] Gnoller complete...Take over and move on...\r\n");                
                stp_find_home();              
                Calibrate_Target(STATION_ID,laser_position,PTT_Readout);
                
                //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                pc.printf("[Main] I am station %i, and serving as whacker soon.... \r\n", STATION_ID);                
                //? Wait for button hit                
                while(main_start.read() == 1);
                pc.printf("[Main] I am station %i, and serving as whacker now.... \r\n", STATION_ID);                
                //? Final Calibration check
                if (!preFlight_check){
                    pc.printf("{FATAL ABORT!}: preFlight_check not passed!....\r\n");
                }else
                {
                    pc.printf("[MAIN] GAME STARTS NOW..... Game_Whacker Take Controll....\r\n");
                }                
                //? Hand Over                
                whacker(PTT_Readout,laser_position);
                pc.printf("[Main] Whacker complete...Take over and move on...\r\n");     
                stp_find_home();                              
                Calibrate_Target(STATION_ID,laser_position,PTT_Readout);
                                           
            }
        }
        else if (STATION_ID == STATION_B)
        {
            //! Station Flip loop
            while(1>0){
                //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                pc.printf("[Main] I am station %i, and serving as whacker soon.... \r\n", STATION_ID);                
                //? Wait for button hit
                while(main_start.read() == 1);
                pc.printf("[Main] I am station %i, and serving as whacker now.... \r\n", STATION_ID);
                //? Final Calibration check
                if (!preFlight_check){
                    pc.printf("{FATAL ABORT!}: preFlight_check not passed!....\r\n");
                }else
                {
                    pc.printf("[MAIN] GAME STARTS NOW..... Game_Whacker Take Controll....\r\n");
                }
                //? Hand Over                                
                whacker(PTT_Readout,laser_position);
                pc.printf("[Main] Whacker complete...Take over and move on...\r\n");      
                stp_find_home();          
                Calibrate_Target(STATION_ID,laser_position,PTT_Readout);

                //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                pc.printf("[Main] I am station %i, and serving as GNOLLER soon.... \r\n", STATION_ID);                
                //? Wait for button hit
                while(main_start.read() == 1);
                pc.printf("[Main] I am station %i, and serving as GNOLLER now.... \r\n", STATION_ID);
                //? Final Calibration check
                if (!preFlight_check){
                    pc.printf("{FATAL ABORT!}: preFlight_check not passed!....\r\n");
                }else
                {
                    pc.printf("[MAIN] GAME STARTS NOW..... Game_Gnoller Take Controll....\r\n");
                }
                //? Hand Over                                
                gnoller(PTT_Readout,laser_position);
                pc.printf("[Main] Gnoller complete...Take over and move on...\r\n");  
                stp_find_home();              
                Calibrate_Target(STATION_ID,laser_position,PTT_Readout);
                
            }

        }
        else{
            pc.printf("{Fatal Error}: Game Logic Out of bound!\r\n");
        }
    }
}