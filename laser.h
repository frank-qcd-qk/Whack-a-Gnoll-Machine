/******************************************************************************
 * EECS 397
 *
 * File name: laser.h
 *
 *Description:
 *   Header file for the Wagnoll machine laser functioning group
 *
 *Notes:
 *
 * Author(s): Chude (Frank), Qian
 *
 * Created: 2/20/2019
 * Last Modified: 4/07/2019
 *
 *Changes: removed include other h files
 *
 ******************************************************************************/
#ifndef LASER_H
#define LASER_H

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
 * *Name: laser_init ()
 *
 * Description: Initializes laser
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
void laser_init();


/*****************************************************************************
 * *Name: laser_on ()
 *
 * Description: Turn on laser
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
void laser_on();


/*****************************************************************************
 * *Name: laser_off ()
 *
 * Description: Turn off laser
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
void laser_off();

#endif // !LASER_H
