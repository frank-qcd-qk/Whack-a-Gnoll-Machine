/******************************************************************************
 * EECS 397
 *
 * File name: test.cpp
 *
 *Description:
 *   Centralization of all test functions
 *
 *Notes:
 *
 * Author(s): Chude (Frank), Qian
 *
 * Created: 4/06/2019
 * Last Modified: 4/07/2019
 *
 *Changes: Started constuction of test cpp
 *
 ******************************************************************************/

//TODO:

/*****************************************************************************
 * !Include stuffs from other files:
 ******************************************************************************/
#include "display.h"
#include "io_pins.h"
#include "mbed.h"
#include "spi.h"
#include "stepper.h"
#include "laser.h"
#include "phototransistor.h"
#include "test.h"

/*****************************************************************************
 * !Private Constants: Only accessible to code below this point in this file
 ******************************************************************************/
//* Global Test Mode Selector:
//This is set on purpose so each test has a begin, run, end seperation. Can be
//helpful for future expansion.
//? Step pre
#define NOT_INIT -1

//? Step 1
#define START_DISPLAYTEST 0
#define DISPLAYTEST_MAIN 1
#define END_DISPLAYTEST 2

//? Step 2
#define START_TARGETBOARDTEST 3
#define TARGETBOARDTEST_MAIN 4
#define END_TARGETBOARDTEST 5

//? Step 3
#define START_LASER_TEST 6
#define LASERTEST_MAIN 7
#define END_LASERTEST 8

//? Step 4
#define START_PHOTOTRANSISTORTEST 9
#define PHOTOTRANSISTORTEST_MAIN 10
#define END_PHOTOTRANSISTORTEST 11

//? Step 5
#define START_PUSHBUTTONTEST 12
#define PUSHBUTTONTEST_MAIN 13
#define END_PUSHBUTTONTEST 14

//? Step 6
#define START_STEPPERTEST 15
#define STEPPERTEST_MAIN 16
#define END_STEPPERTEST 17

//? Step 7
#define START_CALIBRATIONTEST 18
#define CALIBRATIONTEST_MAIN 19
#define END_CALIBRATIONTEST 20

//? Step 8
#define START_RELIABILITYTEST 21
#define RELIABILITYTEST_MAIN 22
#define END_RELIABILITYTEST 23

#define catcher 24

int test_step = -1; //So that in case system mess up, it can be easily traced.

/*****************************************************************************
 * !Private Data: Only accessible to code below this point in this file
 ******************************************************************************/
extern Serial pc;
extern spi_cfg as1107;
extern spi_cfg drv8806;

extern DigitalIn jog_ccw;
extern DigitalIn jog_cw;
extern DigitalIn cal;
extern DigitalIn win;
extern DigitalIn main_start;
extern DigitalIn station_ss;
#define STATION_A 1
#define STATION_B 0
extern DigitalIn home_sensor;

#define ASCII_ESC 27

//* Interrupt setup:
InterruptIn buttonEvent(USER_BUTTON);
Timeout button_timeout;
volatile bool button_pressed_state = false;
volatile bool button_enable = true;

//* Local TEST_mux_result storage:
float TEST_mux_result[16];
int TEST_beacon_position[8];
bool TEST_cal_status = false;
/*****************************************************************************
******************************************************************************
* !Private Function Prototypes: These functions may only be called by code
*   below this point in this file
******************************************************************************
******************************************************************************/

/*****************************************************************************
 * *Name: button_timer_cb ()
 *
 * Description: serves as the timer for button debounce
 *
 * Inputs:
 *    Parameters:
 *    Globals:
 *
 * Outputs:
 *    Parameters:
 *    Globals:
 *    Returns: void
 *
 *
 * Changes: 4/07/2019 move hosting location...
 *
 ******************************************************************************/
//? Button re-enable timer:
void button_timer_cb(){
    button_enable = true;
}

/*****************************************************************************
 * *Name: button_rise_cb ()
 *
 * Description: serves as the interrupt monitor for test state
 *
 * Inputs:
 *    Parameters:
 *    Globals:
 *
 * Outputs:
 *    Parameters:
 *    Globals:
 *    Returns: void
 *
 *
 * Changes: 4/07/2019 move hosting location...
 *
 ******************************************************************************/
//? Button rising edge callback:
void button_rise_cb(){
    if(button_enable){
        button_enable = false;
        button_pressed_state = true;
        button_timeout.attach(&button_timer_cb,0.3);
    }
    if(button_pressed_state){
        test_step++;
    }
}

/*****************************************************************************
******************************************************************************
* !Public Function Definitions: Function header comments for thses function
*   are in the header file
******************************************************************************
******************************************************************************/
void TEST_targetboard_once(){
    pc.printf("[TEST_targetboard_once] Target Board Test On....\r\n");
    int current_led_address = 0;
        for(int i = 1; i <=128; i=i*2){
            current_led_address = 0x0500+i;
            //pc.printf("Current LED address %d", current_led_address);
            spi_send(as1107,current_led_address);
            wait_ms(100);
            spi_send(as1107,0x0500);
            wait_ms(100);
        }
    spi_send(as1107,0x0500);
    pc.printf("[TEST_targetboard_once] Target Board Test Cycled Throught....\r\n");
}

void TEST_laser_once(){
    pc.printf("[TEST_laser_once] Laser test initiated...\r\n");
    laser_on();
    wait(0.5);
    laser_off();
    wait(0.5);
    pc.printf("[TEST_laser_once] Laser test gracefully ENDED...\r\n");
}

void TEST_phototransistor_once(float * userInput){
    //*Clear Screen 
    pc.printf("%c[%d;%df",ASCII_ESC,1,0);
    //*Setup
    pc.printf("[TEST_phototransistor_once] Phototransistor test initiated...");
    pc.printf("%c[%d;%df",ASCII_ESC,2,0);
    pc.printf("CH1     CH2     CH3     CH4     CH5     CH6     CH7     CH8\r\n");
    pc.printf("%c[%d;%df",ASCII_ESC,4,0);
    pc.printf("CH9    CH10    CH11    CH12    CH13    CH14    CH15    CH16\r\n");
    scanChannels(userInput);
    for (int i  = 0; i<16; i++){
        if(i<8){
            pc.printf("%c[%d;%df",ASCII_ESC,3,i*8);
            pc.printf("%3.2fV ",userInput[i]*3.3f);
        }else if(i>7 && i<16){
            pc.printf("%c[%d;%df",ASCII_ESC,5,(i-8)*8);
            pc.printf("%3.2fV ",userInput[i]*3.3f);
        }
    }
    wait_ms(100); // For debug only
}

void TEST_allButton_once(){
    //Display zero if the button or switch is not pressed and one if it is.
    int print_pointer=0;
    //*Clear Screen
    pc.printf("%c[%d;%df",ASCII_ESC,1,0);
    //*Setup
    pc.printf("[TEST_allButton_once] Button test initiated...");
    pc.printf("%c[%d;%df",ASCII_ESC,2,0);
    pc.printf("Jog Left    Jog Right   Cal         Win         Main\r\n");
    pc.printf("%c[%d;%df",ASCII_ESC,3,print_pointer*15);
    pc.printf("%i",!jog_cw.read());
    print_pointer++;
    pc.printf("%c[%d;%df",ASCII_ESC,3,print_pointer*15);
    pc.printf("%i",!jog_ccw.read());
    print_pointer++;
    pc.printf("%c[%d;%df",ASCII_ESC,3,print_pointer*15);
    pc.printf("%i",!cal.read());
    print_pointer++;   
    pc.printf("%c[%d;%df",ASCII_ESC,3,print_pointer*15);
    pc.printf("%i",!win.read());
    print_pointer++;    
    pc.printf("%c[%d;%df",ASCII_ESC,3,print_pointer*15);
    pc.printf("%i",!main_start.read());
    print_pointer++;
    pc.printf("%c[%d;%df",ASCII_ESC,4,0);
    pc.printf("HomeSensor   Station\r\n");
    print_pointer=0;
    pc.printf("%c[%d;%df",ASCII_ESC,5,print_pointer*15);
    pc.printf("%i",!home_sensor.read());
    print_pointer++;
    pc.printf("%c[%d;%df",ASCII_ESC,5,print_pointer*15);
    if(station_ss==STATION_B){
        pc.printf("B");
    }else
    {
        pc.printf("A");
    }
}

void TEST_phototransistor_byStation_once(float * userInput){
    pc.printf("%c[%d;%df",ASCII_ESC,2,0);
    pc.printf("CH1     CH2     CH3     CH4     CH5     CH6     CH7     CH8\r\n");    
    scanChannels(userInput);
    if(station_ss==STATION_B){
        //Station B
        for (int i  = 8; i<16; i++){
            pc.printf("%c[%d;%df",ASCII_ESC,3,(i-8)*8);
            pc.printf("%3.2fV \r\n",userInput[i]*3.3f);
        }
    }else{
        //Station A
        for (int i  = 0; i<8; i++){
            pc.printf("%c[%d;%df",ASCII_ESC,3,(i)*8);
            pc.printf("%3.2fV \r\n",userInput[i]*3.3f);
        }        
    }
}

void TEST_stepper(){

    //hen you will repetitively scan 8 of the phototransistors and 
    //display each PT's output value on the console.
    pc.printf("%c[2J", ASCII_ESC);
    pc.printf("%c[2H", ASCII_ESC);

    TEST_phototransistor_byStation_once(TEST_mux_result);
    stp_manual_op();
 
}

void TEST_calibration(int * beacons,float * userINPUT){
    for (int i=0; i<8; i++){
        scanChannels(userINPUT);
        //for each sensor, identify the position that mostly brightly illuminates 
        //the sensor and store that position as the location of that sensor in your
        // position array
        if (station_ss==STATION_A){
            while(1>0){
                scanChannels(userINPUT);
                stp_step(STP_LEFT);
                if(userINPUT[7-i]>=0.93){
                    beacons[i] = stp_read_location();
                    pc.printf("[TEST_calibration]Location recorded for %i at %i \r\n", i, stp_read_location());
                    wait_ms(100);
                    for (int timer =0; timer <50; timer++){
                        stp_step(STP_LEFT);
                        wait_ms(2);
                    }
                    break;
                }
            }

        }else
        {
            while(1>0){
                scanChannels(userINPUT);
                stp_step(STP_LEFT);
                if(userINPUT[15-i]>=0.93){
                    beacons[i] = stp_read_location();
                    pc.printf("Location recorded for %i at %i", i, stp_read_location());
                    for (int timer =0; timer <50; timer++){
                        stp_step(STP_LEFT);
                        wait_ms(2);
                    }
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
    TEST_cal_status = true;
    laser_off();
    pc.printf("%c[2J", ASCII_ESC);
    pc.printf("%c[2H", ASCII_ESC);
    pc.printf("%c[%d;%df",ASCII_ESC,1,0);
    pc.printf("[TEST_calibration] Calibration Complete!");
    pc.printf("%c[%d;%df",ASCII_ESC,2,0);
    pc.printf("LOC1     LOC2     LOC3     LOC4     LOC5     LOC6     LOC7     LOC8\r\n");    
    pc.printf("%c[%d;%df",ASCII_ESC,4,0);
    pc.printf("Diff1     Diff2     Diff3     Diff4     Diff5     Diff6     Diff7\r\n");       
    for (int another_counter = 0; another_counter<8; another_counter++){
        pc.printf("%c[%d;%df",ASCII_ESC,3,another_counter*12);
        pc.printf("%i ",beacons[another_counter]);
        if(another_counter<7){
            pc.printf("%c[%d;%df",ASCII_ESC,5,another_counter*14);
            pc.printf("%i",beacons[another_counter+1]-beacons[another_counter]);
        }
    }
}

void TEST_reliability(int *beacons, float* userInput){
    for(int total_run = 0; total_run<5;total_run++){
        pc.printf("[TEST_reliability] Run %i:.....\r\n",total_run);
        //! Move to closer one:
        while(stp_read_location()<=beacons[0]){
            stp_step(STP_LEFT);
            wait_ms(1);
        }
        wait_ms(100);        
        scanChannels(TEST_mux_result);
        if (station_ss ==STATION_A){
            pc.printf("[TEST_reliability]Closest: %3.2fV \r\n", TEST_mux_result[7]*3.3f);
        }else
        {
            pc.printf("[TEST_reliability]Closest: %3.2fV \r\n", TEST_mux_result[15]*3.3f);
        }
        while(stp_read_location()<=beacons[7]){
            stp_step(STP_LEFT);
            wait_ms(1);
        }
        wait_ms(100);
        scanChannels(TEST_mux_result);
        if (station_ss ==STATION_A){
            pc.printf("[TEST_reliability]Further: %3.2fV \r\n", TEST_mux_result[0]*3.3f);
        }else
        {
            pc.printf("[TEST_reliability]Further: %3.2fV \r\n", TEST_mux_result[8]*3.3f);
        }
        while(stp_read_location()<=(beacons[7]+50)){
            stp_step(STP_LEFT);
            wait_ms(1);
        }
        while(stp_read_location()>=1){
            stp_step(STP_RIGHT);
            wait_ms(1);
        }
    }
}

void TEST_start(){
    //! Boot Up Test Sequence:
    pc.printf("%c[2J", ASCII_ESC);
    pc.printf("%c[2H", ASCII_ESC);
    pc.printf("==============================NEW RUN==============================\r\n");

    //The test framework you write will first display a welcome message on the 
    //console and 0000 on the seven segment display and then will do nothing 
    //until the user button is pressed and released.
    pc.printf("Welcome to system self test, use blue press button to proceed.\r\n");
    initiate_as1107_zeros();
    //Once the user button is pressed and released it will begin executing the 
    //first test, the Display test.
    buttonEvent.rise(&button_rise_cb);
    while(1>0) {
        switch (test_step) {
            case NOT_INIT:
                pc.printf("[STATE SWITCH] SYSTEM not INIT, press blue button...\r\n");
                break;
            
            case START_DISPLAYTEST:
                 // The display test will first display a message on the console
                 // indicating the display test is being run
                pc.printf("[STATE SWITCH]Display Test ON!\r\n");
                test_step++;
                break;

            case DISPLAYTEST_MAIN:
                // and then put the display controller in display test mode
                initiate_as1107_all(true);
                break;

            case END_DISPLAYTEST:
                //Then it should turn off display test mode,
                initiate_as1107_all(false);
                //returning to just displaying 0000 on the display,
                initiate_as1107_zeros();
                test_step++;
                //*Clear Screen
                pc.printf("%c[2J", ASCII_ESC);
                pc.printf("%c[2H", ASCII_ESC);                 
                break;

            case START_TARGETBOARDTEST:
                //Display a message on the console indicating Target Board LED 
                //Test is being run.
                pc.printf("[STATE SWITCH]Target Board Test ON!\r\n");
                test_step++;
                break;

            case TARGETBOARDTEST_MAIN:
                //arget Board LED Test
                TEST_targetboard_once();
                break;

            case END_TARGETBOARDTEST:
                test_step++;
                //*Clear Screen
                pc.printf("%c[2J", ASCII_ESC);
                pc.printf("%c[2H", ASCII_ESC);                   
                break;

            case START_LASER_TEST:
                //Display a message on the console indicating the Laser test is 
                //being run.
                pc.printf("[STATE SWITCH]Laser Board Test ON!\r\n");
                test_step++;
                break;

            case LASERTEST_MAIN:
                //Laser test
                TEST_laser_once();
                break;

            case END_LASERTEST:
                test_step++;
                //*Clear Screen
                pc.printf("%c[2J", ASCII_ESC);
                pc.printf("%c[2H", ASCII_ESC);                   
                break;

            case START_PHOTOTRANSISTORTEST:
                //Display a message on the console indicating the 
                //Phototransistor test is being run.                
                pc.printf("[STATE SWITCH]Phototransistor Test ON!\r\n");
                pc.printf("%c[2J", ASCII_ESC);
                pc.printf("%c[2H", ASCII_ESC);                
                test_step++;
                break;

            case PHOTOTRANSISTORTEST_MAIN:
                TEST_phototransistor_once(TEST_mux_result);
                break;
            
            case END_PHOTOTRANSISTORTEST:
                pc.printf("%c[2J", ASCII_ESC);
                pc.printf("%c[2H", ASCII_ESC);
                test_step++;
                break;

            case START_PUSHBUTTONTEST:
                pc.printf("[STATE SWITCH]PUSHBUTTON Test ON!\r\n");
                pc.printf("%c[2J", ASCII_ESC);
                pc.printf("%c[2H", ASCII_ESC);
                test_step++;
                break;
            
            case PUSHBUTTONTEST_MAIN:
                TEST_allButton_once();
                break;

            case END_PUSHBUTTONTEST:
                pc.printf("%c[2J", ASCII_ESC);
                pc.printf("%c[2H", ASCII_ESC);
                test_step++;
                break;
            
            case START_STEPPERTEST:
                //Display a message on the Console indicating the Stepper test is being run. 
                pc.printf("[STATE SWITCH]Stepper Test on!\r\n");
                stp_find_home();
                //you will first turn on the laser,
                laser_on();
                test_step++;
                break;
            
            case STEPPERTEST_MAIN:
                TEST_stepper();
                break;

            case END_STEPPERTEST:
                pc.printf("%c[2J", ASCII_ESC);
                pc.printf("%c[2H", ASCII_ESC);
                laser_off();
                test_step++;
                break;

            case START_CALIBRATIONTEST:
                //Display a message on the console that the calibration test is being run.
                pc.printf("[STATE SWITCH]Calibration Test on!\r\n");
                //irst use your find home function to move the carriage to home position 
                //and set the current position to 0.
                stp_find_home();
                //Then you should start scanning the appropriate phototransistors 
                //while moving the laser away from the home position
                laser_on();
                test_step++;
                break;

            case CALIBRATIONTEST_MAIN:
                TEST_calibration(TEST_beacon_position,TEST_mux_result);
                test_step++;
                break;

            case END_CALIBRATIONTEST:
                break;

            case START_RELIABILITYTEST:
                pc.printf("%c[2J", ASCII_ESC);
                pc.printf("%c[2H", ASCII_ESC);            
                if (!TEST_cal_status){
                    pc.printf("[STATE SWITCH]ERROR! Calibration Test not finished! Restart Calibration");
                    test_step = 18;
                }else{
                    //Move to the home position. Turn on the laser.                    
                    stp_find_home();
                    pc.printf("[STATE SWITCH]Reliability Test on!\r\n");
                    laser_on();
                }
                test_step++;
                break;

            case RELIABILITYTEST_MAIN:
                TEST_reliability(TEST_beacon_position,TEST_mux_result);
                test_step++;
                break;

            case END_RELIABILITYTEST:
                break;

            case catcher:
                test_step = 0;
                break;

            default:
                pc.printf("ERROR00: TEST COMMAND NOT FOUND!\r\n");
                break;
        }
    }
}