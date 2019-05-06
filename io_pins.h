/******************************************************************************
 * EECS 397
 *
 *File name: io_pins.h
 *
 *Description:
 *   Header file for the io pins configuration for Whack a Gnoll machine.
 *
 *Notes:
 *
 * Author(s): Chude (Frank), Qian
 *
 * Created: 2/20/2019
 * Last Modified: 2/20/2019
 *
 *Changes:
 * Started the file...
 *
 ******************************************************************************/

#ifndef IO_PINS_H
#define IO_PINS_H

/*****************************************************************************
 * !Include stuffs from other files:
 ******************************************************************************/
#include "io_pins.h"
#include "mbed.h"

/*****************************************************************************
 * Public Constants for pins configuration: Accessible by any code that includes
 * this header file
 ******************************************************************************/

//! Laser Function Group:
#define LZR_ENABLE PB_5

//! Stepper Function Group:
#define STP_HOME_SENSOR PE_11
#define STP_DRV8806_NSS PA_15

//! Display Function Group:
#define DSP_AS1107_NSS PC_7

//! Analog Function Group:
#define ANA_MUX_S0 PC_6
#define ANA_MUX_S1 PB_15
#define ANA_MUX_S2 PB_13
#define ANA_MUX_S3 PB_12
#define ANA_MUX_OUT PB_1

//! UI Function Group:
#define UI_CAL_BUTTON PG_9
#define UI_START_BUTTON PG_14
#define UI_STATION_SELECT PF_15
#define UI_JOG_LEFT_BUTTON PE_13
#define UI_JOG_RIGHT_BUTTON PF_14
#define UI_SPARE PB_3

//! SPI Function Group:
#define SPI_SCLK PA_5
#define SPI_MOSI PA_7
#define SPI_MISO PA_6

//! Expansion (future) Function Group:
#define EXP_NSS PA_4

#endif /*IO_PINS_H */