/******************************************************************************
 * EECS 397
 *
 * File name: display.h
 *
 *Description:
 *   Header file for the Wagnoll machine display functioning group
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

#ifndef DISPLAY_H
#define DISPLAY_H

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
 * *Name: configure_as1107 ()
 *
 * Description: configures as 1107 for
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
void configure_as1107();

/*****************************************************************************
 * *Name: initiate_as1107_zeros ()
 *
 * Description: initiate as1107 with all zerios
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
void initiate_as1107_zeros();

/*****************************************************************************
 * *Name: initiate_as1107_all (bool mode)
 *
 * Description: This will light all segments on the seven segment display 
 * (and decimal points) at full brightness and will also light eight of the 
 * target board discrete LED's, A or B group depending on which station you 
 * are connected to.
 *
 * Inputs:
 *    Parameters:
 *          bool mode     Ture for test mode on, false for test mode off.

 *    Globals:
 *
 * Outputs:
 *    Parameters:
 *    Globals:
 *    Returns: void
 *
 * Notes: not required in lab, but very handy to have for testing circuit.
 *
 * Changes: 2/20/2019 Started construction...
 *
 ******************************************************************************/
void initiate_as1107_all(bool);

/*****************************************************************************
 * *Name: bin2bcd_array (int userInput, char * bcd)
 *
 * Description: convert obtained user input to bcd array for interpretation
 *
 * Inputs:
 *    Parameters:
 *          int userInput     User Message Input
 *          char* bcd       Pointer to the BCD array
 *    Globals:
 *
 * Outputs:
 *    Parameters:
 *    Globals: char bcdlocal updates it with proper values
 *    Returns: void
 *
 * Notes: this function has two approaches: VERSION1 and VERSION2 using Debug
 *          VERSION1: Uses modulus divide method
 *          VERSION2: Uses sprintf method
 *
 * Changes: 2/20/2019 Started construction...
 *
 ******************************************************************************/
void bin2bcd_array(int, char*);

/*****************************************************************************
 * *Name: dsp_bcd (char * bcd)
 *
 * Description: convert obtained user input to bcd array for transmission
 *
 * Inputs:
 *    Parameters:
 *          char* bcd       Pointer to the BCD array
 *    Globals:
 *
 * Outputs:
 *    Parameters:
 *    Globals:
 *    Returns: void
 *
 *
 * Changes: 2/20/2019 Started construction...
 *
 ******************************************************************************/
void dsp_bcd(char*);

/*****************************************************************************
 * *Name: dsp_lit_targetboard (int position)
 *
 * Description: Light up the target board specific led utilizing given int command
 *
 * Inputs:
 *    Parameters:
 *          int position        the posiition index of what targetboard led
 *    Globals:
 *
 * Outputs:
 *    Parameters:
 *    Globals:
 *    Returns: void
 *
 *
 * Changes: 4/23/2019 Started construction...
 *
 ******************************************************************************/
void dsp_lit_targetboard(int);

/*****************************************************************************
 * *Name: dsp_turnoff_targetboard ()
 *
 * Description: Resets Target Board LEDs.
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
 * Changes: 4/25/2019 Started construction...
 *
 ******************************************************************************/
void dsp_turnoff_targetboard();

#endif /*DISPLAY_H*/
