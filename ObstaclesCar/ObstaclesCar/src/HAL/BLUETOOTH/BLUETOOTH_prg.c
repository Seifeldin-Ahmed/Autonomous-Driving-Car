#include "BLUETOOTH_int.h"

void BLUETOOTH_Init(u32 Copy_BaudRate, UART_ENUM_CharSize Copy_CharSize, UART_ENUM_Parity Copy_Parity, UART_ENUM_StopBits Copy_StopBits)
{
	UART_Init(Copy_BaudRate, Copy_CharSize, Copy_Parity, Copy_StopBits);
}

u8 BLUETOOTH_Receive()
{
	return UART_Receive();
}

void BLUETOOTH_TransmitChar(char *Add_Char)
{
	UART_TransmitChar(Add_Char);
}

void BLUETOOTH_TransmitString(char *Add_String)
{
	UART_TransmitString(Add_String);
}