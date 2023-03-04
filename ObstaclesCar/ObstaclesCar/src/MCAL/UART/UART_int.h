#ifndef MCAL_UART_UART_INT_H_
#define MCAL_UART_UART_INT_H_

#include "../../LIB/std_types.h"

// ---------------------------------------- Enums ---------------------------------------- //

typedef enum
{
	UART_CHAR_SIZE_5_BIT,
	UART_CHAR_SIZE_6_BIT,
	UART_CHAR_SIZE_7_BIT,
	UART_CHAR_SIZE_8_BIT
} UART_ENUM_CharSize;

typedef enum
{
	UART_PARITY_NONE,
	UART_PARITY_EVEN,
	UART_PARITY_ODD
} UART_ENUM_Parity;

typedef enum
{
	UART_STOP_BITS_ONE,
	UART_STOP_BITS_TWO
} UART_ENUM_StopBits;

// ---------------------------------------- Functions ---------------------------------------- //

void UART_Init(u32 Copy_BaudRate, UART_ENUM_CharSize Copy_CharSize, UART_ENUM_Parity Copy_Parity, UART_ENUM_StopBits Copy_StopBits);

u8 UART_Receive();

void UART_TransmitChar(char *Add_Char);

void UART_TransmitString(char *Add_String);

void UART_TransmitAsync(char *Add_TransmitData);

void UART_ReceiveAsync(u8 *Add_ReceivedData, u16 Copy_Size);

#endif
