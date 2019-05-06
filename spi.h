/******************************************************************************
 * EECS 397
 *
 * File name: spi.h
 *
 *Description:
 *   Header file for spi driver group
 *
 *Notes:
 *
 * Author(s): Chude (Frank), Qian
 *
 * Created: 2/26/2019
 * Last Modified: 4/07/2019
 *
 *Changes: Started Constructing this file...
 *Fixes: Added SPI ID 2019-03-29
 *Fixes: removed including other header file...
 ******************************************************************************/
#ifndef SPI_H
#define SPI_H

/*****************************************************************************
 * !Public Constants: Accessible by any code that includes this header file
 ******************************************************************************/
#define SPI_NO_ID 0
#define SPI_AS1107_ID 1
#define SPI_DRV8806_ID 2

struct spi_cfg {
    int spi_id;          // ID should be unique for each slave type
    DigitalOut spi_ncs;  // The pin driving the slave's chip select
    int spi_mode;        // SPI mode needed by this slave
    int spi_freq;        // SPI frequency needed by this slave
    int spi_no_bits;     // No. of data bits to be transfered each send to the
                         // slave
};

/*****************************************************************************
******************************************************************************
* !Public Function Prototypes: These functions may be called by any code
*   that includes this header file
******************************************************************************
******************************************************************************/

/*****************************************************************************
 * *Name: spi_send ()
 *
 * Description: send SPI command
 *
 * Inputs:
 *    Parameters: spi_cfg type configuration;; data as an integer
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
void spi_send(struct spi_cfg spi_config, int data);


#endif /*SPI_H*/
