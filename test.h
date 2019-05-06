/******************************************************************************
 * EECS 397
 *
 * File name: test.h
 *
 *Description:
 *   Header file for the Wagnoll machine test functioning group
 *
 *Notes:
 *
 * Author(s): Chude (Frank), Qian
 *
 * Created: 4/05/2019
 * Last Modified: 4/07/2019
 *
 *Changes: Started constructing file
 *
 ******************************************************************************/

#ifndef TEST_H
#define TEST_H

/*****************************************************************************
 * !Public Constants: Accessible by any code that includes this header file
 ******************************************************************************/

/*****************************************************************************
******************************************************************************
* !Public Function Prototypes: These functions may be called by any code
*   that includes this header file
******************************************************************************
******************************************************************************/

/*****************************************************************************
 * *Name: TEST_targetboard_once ()
 *
 * Description: Develop a Target Board LED test that lights LED1 on the Target
 * board for 100 mS, then turns off LED1 and turns on LED2 for 100 mS, and so
 * on cycling through all 8 discrete LEDs.
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
void TEST_targetboard_once();

/*****************************************************************************
 * *Name: TEST_laser_once ()
 *
 * Description: urn the laser on for 0.5 seconds and then off for 0.5 seconds
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
 * Changes: 3/28/2019 Started construction...
 *
 ******************************************************************************/
void TEST_laser_once();

/*****************************************************************************
 * *Name: TEST_phototransistor_once ()
 *
 * Description: Scan all 16 Phototransistors (1A first and ending with 8B) and
 * then print the values  to the serial console on a single line
 *
 * Inputs:
 *    Parameters: float16 array for storing mux value
 *    Globals:
 *
 * Outputs:
 *    Parameters:
 *    Globals:
 *    Returns: void
 *
 *
 * Changes: 3/28/2019 Started construction...
 *
 ******************************************************************************/
void TEST_phototransistor_once(float*);

/*****************************************************************************
 * *Name: TEST_allButton_once ()
 *
 * Description: The pushbutton test will verify the correct functioning of the
 * Jog Left, Jog Right, Cal, and Win Buttons on the WaG controller board, and
 * the separate start button.  It will also verify the correct operation of the
 * home sensor switch and the Station select jumper.
 *
 * Your test should repeatedly display the status of the five buttons, the
 * stepper home sensor and the Station select jumper.  Each of the status items
 * should have a label (e.g. Jog Left) and a value.  You should display 0 or 1
 * as the value for each switch except the Station select jumper.  Display zero
 * if the button or switch is not pressed and one if it is.
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
 * Changes: 4/07/2019 Started construction...
 *
 ******************************************************************************/
void TEST_allButton_once();

/*****************************************************************************
 * *Name: TEST_phototransistor_byStation_once (float *)
 *
 * Description: prints the mux scan result with respect to station
 * Inputs:
 *    Parameters: Test Mux Recorder
 *    Globals:
 *
 * Outputs:
 *    Parameters:
 *    Globals:
 *    Returns: void
 *
 *
 * Changes: 3/28/2019 Started construction...
 *
 ******************************************************************************/
void TEST_phototransistor_byStation_once(float *);

/*****************************************************************************
 * *Name: TEST_stepper ()
 *
 * Description: The stepper test will verify that you can move the stepper
 * using the jog buttons, but will also make use of the laser, phototransistors,
 * and home sensor. Display a message on the Console indicating the Stepper test
 * is being run. The test will first move the laser carriage to the home position
 * (position where the home sensor is actuated) and then will allow you to move
 * the carriage manually using the jog pushbuttons.  While doing this, it will
 * turn on the laser and display the values read from the phototransistors so
 * you can see when the laser is illuminating each PT
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
 * Changes: 4/07/2019 Started construction...
 *
 ******************************************************************************/
void TEST_stepper();

/*****************************************************************************
 * *Name: TEST_calibration (int *, float *)
 *
 * Description: The calibration test will locate the position of each
 * phototransistor and save the position information so the WaG game can point
 * the laser at any sensor as needed.
 * Inputs:
 *    Parameters:
 *          int* beacons         the position array
 *          float* userINPUT     the sensor readout
 *    Globals:
 *
 * Outputs:
 *    Parameters:
 *    Globals:
 *    Returns: void
 *
 *
 * Changes: 4/07/2019 Started construction...
 *
 ******************************************************************************/
void TEST_calibration(int *, float *);

/*****************************************************************************
 * *Name: TEST_reliability (int *, float *)
 *
 * Description: Move to the home position. Turn on the laser.  Move to the 
 * position of the closest sensor and print the value of the sensor output. 
 * (you should be moving based solely on the position values stored in the 
 * sensor position array)  Move to the furthest sensor.  print the value of that
 * sensor's output.  Move a little further away from home (but not so far as the
 * maximum position limit).  Now move back to the sensor  and print the output 
 *value again.  Finally move back to the sensor closest to home position and 
 *print it's value. Repeat this sequence 5 times.
 * Inputs:
 *    Parameters:
 *          int* beacons         the position array
 *          float* userINPUT     the sensor readout
 *    Globals:
 *
 * Outputs:
 *    Parameters:
 *    Globals:
 *    Returns: void
 *
 *
 * Changes: 4/07/2019 Started construction...
 *
 ******************************************************************************/
void TEST_reliability(int *, float *);

/*****************************************************************************
 * *Name: TEST_start ()
 *
 * Description: Starts test wrapper
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
 * Changes: 4/07/2019 Started construction...
 *
 ******************************************************************************/
void TEST_start();


#endif /*TEST_H*/
