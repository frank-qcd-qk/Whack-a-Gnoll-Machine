/******************************************************************************
 * EECS 397
 *
 * File name: stepper.h
 *
 *Description:
 *   Header file for controlling the stepper motor
 *
 *Notes:
 *
 * Author(s): Chude (Frank), Qian
 *
 * Created: 2/28/2019
 * Last Modified: 4/07/2019
 *
 *Changes: removed other header files...
 *
 ******************************************************************************/
#ifndef STEPPER_H
#define STEPPER_H

/*****************************************************************************
 * !Public Constants: Accessible by any code that includes this header file
 ******************************************************************************/
// Its all magic here, of directions....Just pick one and guess!
#define STP_CW 0 
#define STP_CCW 1
#define STP_LEFT STP_CW
#define STP_RIGHT STP_CCW

/*****************************************************************************
******************************************************************************
* !Public Function Prototypes: These functions may be called by any code
*   that includes this header file
******************************************************************************
******************************************************************************/

/*****************************************************************************
 * *Name: stp_init ()
 *
 * Description: Initiate stepper motor with SPI only
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
 * Notes:
 *
 * Changes: 2/20/2019 Started construction...
 *
 ******************************************************************************/
void stp_init();

/*****************************************************************************
 * *Name: stp_step (int direction)
 *
 * Description: Control the stepper motor
 *
 * Inputs:
 *    Parameters:
 *          int op_mode         the direction the stepper should step to 
 *    Globals:
 *
 * Outputs:
 *    Parameters:
 *    Globals:
 *    Returns: void
 *
 * Notes:
 *
 * Changes: 2/28/2019 Started construction...
 *
 ******************************************************************************/
void stp_step(int);

/*****************************************************************************
 * *Name: stp_find_home ()
 *
 * Description: Finds home for the laser, and resets local stepper postion recorder
 *
 * Inputs:
 *    Parameters:
 *    Globals:
 *
 * Outputs:
 *    Parameters:
 *    Globals:
 *          is_configured           records if the system have find home before
 *          stepper position        records the position of the stepper 
 *    Returns: void
 *
 * Notes:
 *
 * Changes: 4/07/2019 Started construction...
 *
 ******************************************************************************/
void stp_find_home();

/*****************************************************************************
 * *Name: stp_manual_op ()
 *
 * Description: manually drives stepper from jog button
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
 * Notes:
 *
 * Changes: 4/07/2019 Started construction...
 *
 ******************************************************************************/
void stp_manual_op();

/*****************************************************************************
 * *Name: stp_read_location ()
 *
 * Description: stepper position access wrapper
 *
 * Inputs:
 *    Parameters:
 *    Globals:
 *
 * Outputs:
 *    Parameters: the current stepper position
 *    Globals:
 *    Returns: laser's position
 *
 * Notes: used for protecting the stepper posiiton
 *
 * Changes: 4/07/2019 Started construction...
 *
 ******************************************************************************/
int stp_read_location();

/*****************************************************************************
 * *Name: step_move_specifically (int)
 *
 * Description: specifically command a stepper to go to specific position
 *
 * Inputs:
 *    Parameters:
 *          int position        the position that the stepper should be
 *    Globals:
 *
 * Outputs:
 *    Parameters:
 *    Globals:
 *    Returns: laser's position
 *
 * Notes:
 *
 * Changes: 4/07/2019 Started construction...
 *
 ******************************************************************************/
void stp_move_specifically(int);

#endif /*STEPPER_H*/
