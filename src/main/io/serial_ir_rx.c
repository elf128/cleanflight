/*
 * serial_ir_rx.c
 *
 *  Created on: Feb 25, 2016
 *      Author: elf128
 */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>

#include <platform.h>

#include "io/serial_ir_rx.h"
#include "drivers/serial.h"
#include "drivers/system.h"
#include "io/serial.h"
#include "config/config.h"

#if defined(USE_IR_TIMING)

#define IR_BAUDRATE 4800
#define IR_FRAME_MINIMUM_SEPARATION 4000
#define IR_GATE_MINIMUM_SEPARATION 5000000

irrxData_t* config; // This is pointer to section of Master Config (To avoid massive including)
static bool irNewPackage;
static uint32_t receivedData;
static uint32_t curentData;

static void onIrData( uint16_t data )
{
	// got some data from serial port
	uint32_t irTime, irInterval;
    static uint32_t irTimeLast = 0;
    static uint8_t byteCounter;

    irTime = micros();
    irInterval = irTime - irTimeLast;
    irTimeLast = irTime;

    if (irInterval > IR_FRAME_MINIMUM_SEPARATION)
    {
    	// New package for sure. Clearing garbage in case previous frame was corrupted
        byteCounter  = 0;
        curentData = 0;
    }

    if (irInterval > IR_GATE_MINIMUM_SEPARATION)
    {
    	// silence this long mean, we have to consider this gate even if it's the same gate.
    	// This handle case when track has only one gate.
    	receivedData = 0;
    }

    if (byteCounter < 4)
    {
    	curentData <<= 8;
    	curentData |= (uint8_t)data;
        byteCounter++;
        if (byteCounter == 4) {
        	if ( receivedData != curentData )
        	{
        		// During gate passing we'll constantly receive GateID
        		// We don't want to react
        		irNewPackage = true;
        		receivedData = curentData;
        	}
        } else {
        	irNewPackage = false;
        }
    }
}

void initSerialIR( irrxData_t* p_config )
{
	// init all serial stuff and remember where config is.
	config = p_config;

    serialPortConfig_t *portConfig = findSerialPortConfig(FUNCTION_IR_TIMING);
    if (!portConfig) {
        return;
    }

    serialPort_t *irPort = openSerialPort(portConfig->identifier, FUNCTION_IR_TIMING, onIrData, IR_BAUDRATE, MODE_RX, SERIAL_NOT_INVERTED);

}

void enableTiming( void )
{
	//enable timing update upon gate visibility
}

void disableTiming( void )
{
	//disable timing update
}

void addGate( uint8_t idx, uint32_t ID )
{
	// register additional gate
}

void removeGate( uint8_t idx )
{
	// remove gate, shift all gates with higher index up.
}

uint8_t getGateCount( void )
{
	//return number of currently registered gates.
	// ( timing table is valid only within those numbers.)
	//
}

uint32_t getGateTime( uint8_t gateIdx )
{
    // return last time from begining of the lap til this gate.
	// Zero gate gives you lap time
}

uint32_t getBestTime( void )
{
    // return best lap time since enable of timing
	// ( timing starts from
}

void irrxTick(void)
{
	// Read sereal port and check for known gates
	// Update timing information in case gate found.
}

#endif
