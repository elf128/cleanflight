/*
 * serial_ir_rx.h
 *
 *  Created on: Feb 25, 2016
 *      Author: vadamenko
 */

#ifndef SRC_MAIN_IO_SERIAL_IR_RX_H_
#define SRC_MAIN_IO_SERIAL_IR_RX_H_

#define MAX_GATES 8

extern uint32_t timingTable[ MAX_GATES ];

typedef struct irrxData_t {
	uint32_t GateID[ MAX_GATES ];
	uint8_t registeredGates;
} irrxData_t;

void initSerialIR( irrxData_t* config );
void enableTiming( void );
void disableTiming( void );

void addGate( uint8_t idx, uint32_t ID );
void removeGate( uint8_t idx );

uint8_t getGateCount( void );
uint32_t getGateTime( uint8_t gateIdx );
uint32_t getBestTime( void );

void irrxTick(void);

#endif /* SRC_MAIN_IO_SERIAL_IR_RX_H_ */
