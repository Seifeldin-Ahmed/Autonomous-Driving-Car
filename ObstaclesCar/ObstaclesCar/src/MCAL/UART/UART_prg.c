#include <avr/interrupt.h>
#include "UART_int.h"

#define F_OSC 1000000UL

char *UART_TransmitData;
u8 *UART_ReceivedData;
u8 UART_ReceivedDataSize;

void UART_Init(u32 Copy_BaudRate, UART_ENUM_CharSize Copy_CharSize, UART_ENUM_Parity Copy_Parity, UART_ENUM_StopBits Copy_StopBits)
{
	// Set UBRR = F_OSC / 16 / BAUD_RATE - 1
	u16 Local_UBRR = F_OSC / 16 / Copy_BaudRate - 1;
	UBRR0 = Local_UBRR;

	// Set Char Size
	UCSR0C = (Copy_CharSize << UCSZ00); // Set:   UCSZ1:0

	// Set Parity Mode
	UCSR0C |= (Copy_Parity << UPM00); // Set:   UPM1:0

	// Set Stop Bits
	UCSR0C |= (Copy_StopBits << USBS0);

	// Enable Receiver & Transmitter
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
}

u8 UART_Receive()
{
	// Wait For Data To Be Received
	while ((UCSR0A & (1 << RXC0)) == 0)
		;

	// Return Data From Buffer
	return UDR0;
}

void UART_TransmitChar(char *Add_Char)
{
	// Wait For Data Register To Be Empty (Ready For New Data To Be Put Into)
	while ((UCSR0A & (1 << UDRE0)) == 0)
		;

	// Copy Data Into Buffer
	UDR0 = *Add_Char;
}

void UART_TransmitString(char *Add_String)
{
	while (*Add_String != '\0')
		UART_TransmitChar(Add_String++);
}

void UART_TransmitAsync(char *Add_TransmitData)
{
	// Remember The Address Of Data To Be Transmitted (For Interrupt To Use)
	UART_TransmitData = Add_TransmitData;

	// Enable Transmit Complete & Data Register Empty Interrupts
	UCSR0B |= ((1 << TXCIE0) | (1 << UDRIE0));
}

void UART_ReceiveAsync(u8 *Add_ReceivedData, u16 Copy_Size)
{
	// Remember The Address Of Where Received Data To Be Stored (For Interrupt To Use)
	UART_ReceivedData = Add_ReceivedData;
	UART_ReceivedDataSize = Copy_Size;

	// Enable Receive Complete Interrupts
	UCSR0B |= (1 << RXCIE0);
}

// --------------------------------------------------------- ISR --------------------------------------------------------- //

// Receive Complete Interrupt Request
ISR(USART_RX_vect)
{
	// Copy Received Data From UDR
	*UART_ReceivedData++ = UDR0;

	if (--UART_ReceivedDataSize == 0)
		// Disable Receive Complete Interrupt
		UCSR0B &= ~(1 << RXCIE0);
}

// Data Register Empty Interrupt Request
ISR(USART_UDRE_vect)
{
	if (*UART_TransmitData != '\0')
		UDR0 = *UART_TransmitData++;
}

// Transmit Complete Interrupt Request
ISR(USART_TX_vect)
{
	// Disable Transmit Complete & Data Register Empty Interrupts
	UCSR0B &= ~((1 << TXCIE0) | (1 << UDRIE0));
}
