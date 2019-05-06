/******************************************************************************
 * EECS 397
 *
 * File name: spi.cpp
 *
 *Description:
 *   Specifies all SPI command sending here
 *
 *Notes:
 *
 * Author(s): Chude (Frank), Qian
 *
 * Created: 2/26/2019
 * Last Modified: 3/01/2019
 *
 *Changes: Started Constructing this file...
 *
 ******************************************************************************/

/*****************************************************************************
 * !Include stuffs from other files:
 ******************************************************************************/
#include "io_pins.h"
#include "spi.h"
#include "mbed.h"

/*****************************************************************************
 * !Private Constants: Only accessible to code below this point in this file
 ******************************************************************************/

/*****************************************************************************
 * !Private Data: Only accessible to code below this point in this file
 ******************************************************************************/
extern Serial pc;
SPI wag_spi(SPI_MOSI, SPI_MISO, SPI_SCLK); //Initializes SPI pins

/*****************************************************************************
******************************************************************************
* !Private Function Prototypes: These functions may only be called by code
*   below this point in this file
******************************************************************************
******************************************************************************/

/*****************************************************************************
******************************************************************************
* !Public Function Definitions: Function header comments for thses function
*   are in the header file
******************************************************************************
******************************************************************************/
void spi_send(struct spi_cfg spi_config, int spi_data){
    static int previous_id = SPI_NO_ID;
    spi_config.spi_ncs = 1;
    if(previous_id != spi_config.spi_id){
        previous_id = spi_config.spi_id;
        wag_spi.frequency(spi_config.spi_freq);
        wag_spi.format(spi_config.spi_no_bits, 0);
    }
    spi_config.spi_ncs = 0;
    wag_spi.write(spi_data);
    spi_config.spi_ncs = 1;
}