/******************************************************************************
 * EECS 397
 *
 * File name: phototransistor.h
 *
 *Description:
 *   Header file for the Wagnoll machine phototransistor functioning group
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
#ifndef PHOTOTRANSISTOR_H
#define PHOTOTRANSISTOR_H
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
 * *Name: scanChannels (float *)
 *
 * Description: scans the mux and record value into channel
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
void scanChannels(float *);

#endif /*PHOTOTRANSISTOR_H*/
