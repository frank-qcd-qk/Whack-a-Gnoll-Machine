/******************************************************************************
 * EECS 397
 *
 * File name: game.h
 *
 *Description:
 *   Header file for the Wagnoll machine main game code
 *
 *Notes:
 *
 * Author(s): Chude (Frank), Qian
 *
 * Created: 4/20/2019
 * Last Modified: 4/22/2019
 *
 *Changes: removed include other h files
 *
 ******************************************************************************/

#ifndef GAME_H
#define GAME_H

/*****************************************************************************
 * !Public Constants: Accessible by any code that includes this header file
 ******************************************************************************/
//Station code for easier recognition
#define STATION_A 1
#define STATION_B 0

//Acter define
#define GNOLLER 1
#define WHACKER 0

//Game Count Define:
#define MAX_VOLLEY_COUNT 15


//Magic numbers (ms)
#define VOLLEY_TIMEOUT 20 //!(IN seconds)
#define WHACKER_TIMEOUT 20 //!(IN seconds)
#define LASER_POINT_DELAY 10
#define TARGET_BOARD_DELAY 10

#define PTT_CATCH_THRESHOLD 0.1


/*****************************************************************************
******************************************************************************
* !Public Function Prototypes: These functions may be called by any code
*   that includes this header file
******************************************************************************
******************************************************************************/

/*****************************************************************************
 * *Name: get_ID (int)
 *
 * Description: Gets the Station ID and update the local station storage
 *
 * Inputs:
 *    Parameters:
 *              int global_id      Station ID readout
 *    Globals:
 *
 * Outputs:
 *    Parameters:
 *    Globals:
 *          local_station_id        station ID recorder locally
 *    Returns: void
 *
 * Notes: not required in lab, but very handy to have for testing circuit.
 *
 * Changes: 4/20/2019 Started construction...
 *
 ******************************************************************************/

void get_ID(int);

/*****************************************************************************
 * *Name: set_offset ()
 *
 * Description: Used to set the sensor array offset
 *
 * Inputs:
 *    Parameters:
 *    Globals:
 *          local_station_id        The station ID stored privately
 *
 * Outputs:
 *    Parameters:
 *    Globals:
 *          gnoller_offset          Help offset the sensor array readout
 *          whacker_offset          Help offset the sensor array readout
 *      
 *    Returns: void
 *
 * Notes: not required in lab, but very handy to have for testing circuit.
 *
 * Changes: 4/20/2019 Started construction...
 *
 ******************************************************************************/
void set_offset();

/*****************************************************************************
 * *Name: gnoller (float * sensor_measure, Int * position_array)
 *
 * Description: Gnoller Game Main
 *
 * Inputs:
 *    Parameters:
 *          sensor_measure          The phototransistor array (16) readout
 *          position_array          The position array (8) stored value
 *    Globals:
 *
 * Outputs:
 *    Parameters:
 *    Globals:
 *    Returns: void
 *
 * Notes:
 *
 * Changes: 4/20/2019 Started construction...
 *
 ******************************************************************************/
void gnoller(float *, int *);

/*****************************************************************************
 * *Name: whacker (float * sensor_measure, Int * position_array)
 *
 * Description: Whacker Game Main
 *
 * Inputs:
 *    Parameters:
 *          sensor_measure          The phototransistor array (16) readout
 *          position_array          The position array (8) stored value* 
 *    Globals:
 *
 * Outputs:
 *    Parameters:
 *    Globals:
 *    Returns: void
 *
 * Notes:
 *
 * Changes: 4/20/2019 Started construction...
 *
 ******************************************************************************/
void whacker(float *, int*);

/*****************************************************************************
 * *Name: scoreUpdater ()
 *
 * Description: called to update the score on the seven seg display
 *
 * Inputs:
 *    Parameters:
 *    Globals:
 *          hit_count        the recorder for the total hit count
 *          hit_count        the recorder for the total miss count
 * Outputs:
 *    Parameters:
 *    Globals:
 *    Returns: void
 *
 * Notes:
 *
 * Changes: 4/20/2019 Started construction...
 *
 ******************************************************************************/
void scoreUpdater();

#endif /*GAME_H*/
