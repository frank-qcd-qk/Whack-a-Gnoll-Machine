/******************************************************************************
 * EECS 397
 *
 * File name: utility.h
 *
 *Description:
 *   file used to store utility before game start
 *
 *Notes:
 *
 * Author(s): Chude (Frank), Qian
 *
 * Created: 4/22/2019
 * Last Modified: 4/07/2019
 *
 *Changes: Started Constructing this file...
 ******************************************************************************/
#ifndef UTILITY_H
#define UTILITY_H

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
 * *Name: Calibrate_Target ( int, int *, float *)
 *
 * Description: calibrate the target position
 *
 * Inputs:
 *    Parameters: 
 *          int Station_ID          use to identify what station we are
 *          int* position_array     use to know what are the positions
 *          float* Sensor_readout   use to know what the sensor array (16) readout are    
 *    Globals:
 *
 * Outputs:
 *    Parameters:
 *    Globals:
 *    Returns: void
 *
 * Notes:
 *
 * Changes: 4/22/2019 Started construction...
 *
 ******************************************************************************/
void Calibrate_Target(int, int*, float*);

/*****************************************************************************
 * *Name: preFlight ( int, int *, float *)
 *
 * Description: Performs preFlight check
 *
 * Inputs:
 *    Parameters: 
 *          int Station_ID          use to identify what station we are
 *          int* position_array     use to know what are the positions
 *          float* Sensor_readout   use to know what the sensor array (16) readout are    * 
 *    Globals:
 *
 * Outputs:
 *    Parameters:
 *    Globals:
 *    Returns: void
 *
 * Notes:
 *
 * Changes: 4/22/2019 Started construction...
 *
 ******************************************************************************/
bool preFlight(int, int*, float*);


#endif /*UTILITY_H*/
