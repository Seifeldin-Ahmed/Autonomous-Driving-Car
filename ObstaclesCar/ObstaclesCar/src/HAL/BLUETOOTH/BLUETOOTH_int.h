#ifndef MCAL_BLUETOOTH_BLUETOOTH_INT_H_
#define MCAL_BLUETOOTH_BLUETOOTH_INT_H_

#include "../../LIB/std_types.h"
#include "../../MCAL/UART/UART_int.h"

// ---------------------------------------- Functions ---------------------------------------- //

void BLUETOOTH_Init(u32 Copy_BaudRate, UART_ENUM_CharSize Copy_CharSize, UART_ENUM_Parity Copy_Parity, UART_ENUM_StopBits Copy_StopBits);

u8 BLUETOOTH_Receive();

void BLUETOOTH_TransmitChar(char *Add_Char);

void BLUETOOTH_TransmitString(char *Add_String);

#endif
