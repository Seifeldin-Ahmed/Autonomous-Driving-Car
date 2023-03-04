#undef F_CPU
#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

// MCAL
#include "MCAL/G_INT/G_INT_int.h"
#include "MCAL/EXT_INT/EXT_INT_int.h"
#include "MCAL/WATCHDOG/WATCHDOG_int.h"

// HAL
#include "HAL/PORT/PORT_int.h"
#include "HAL/WHEELS/WHEELS_int.h"
#include "HAL/BLUETOOTH/BLUETOOTH_int.h"
#include "HAL/ULTRASONIC/ULTRASONIC_int.h"

void ISR_ModeSelect();
void BluetoothMode_SuperLoop();
void UltrasonicMode_SuperLoop();

int main(int argc, char const *argv[])
{
  WATCHDOG_Stop(); // 1) Good Practice At Startup, To Prevent MCU From Entering Reset Loop
  G_INT_Enable();  // 2) Enable Global Interrupt

  PORT_Init();      // 3) Initialize Port's Pins (Input, Output)
  WHEELS_Init(255); // 4) Initialize Wheels (Motors' Pins, PWM)

  EXT_INT_Enable(EXT_INT_INT0, EXT_INT_TRIGGER_ANY_CHANGE, ISR_ModeSelect); // 5) Enable External Interrupt 0: To ReLaunch The MCU In Different Mode

  if (PORT_GetPinValue(DIO_PORTD, DIO_PIN2))
  {
    ULTRASONIC_Init(EXT_INT_INT1, TIMER_TIMER1); // Initialize Ultrasonic Sensor (Pins, Interrupt, Timer)
    UltrasonicMode_SuperLoop();                  // SuperLoop
  }
  else
  {
    BLUETOOTH_Init(9600, UART_CHAR_SIZE_8_BIT, UART_PARITY_NONE, UART_STOP_BITS_ONE); // Initialize Bluetooth Module (UART)
    BluetoothMode_SuperLoop();                                                        // SuperLoop
  }

  return 0;
}

void BluetoothMode_SuperLoop()
{
  u8 BluetoothCommand;
  while (TRUE)
  {
    BluetoothCommand = BLUETOOTH_Receive();
    switch (BluetoothCommand)
    {
    case 'F':
      WHEELS_Move(WHEELS_DIRECTION_FORWARD);
      break;

    case 'B':
      WHEELS_Move(WHEELS_DIRECTION_BACKWARD);
      break;

    case 'L':
      WHEELS_Move(WHEELS_DIRECTION_LEFT);
      break;

    case 'R':
      WHEELS_Move(WHEELS_DIRECTION_RIGHT);
      break;

    case 'S':
      WHEELS_Stop();
      break;

    case 'D':
      WHEELS_Stop();
      break;

    default:
      if (BluetoothCommand == 'q')
        WHEELS_SetSpeed(255);
      else if ((BluetoothCommand >= '0') && (BluetoothCommand <= '9'))
        WHEELS_SetSpeed((BluetoothCommand - 48) * 25);
      else
        WHEELS_Stop();
    }
  }
}

void UltrasonicMode_SuperLoop()
{
  while (TRUE)
  {
    if (ULTRASONIC_GetDistanceCM() > 30)
    {
      WHEELS_Move(WHEELS_DIRECTION_FORWARD);
    }
    else
    {
      // Move A Little Backward
      WHEELS_Move(WHEELS_DIRECTION_BACKWARD);
      _delay_ms(500);
      WHEELS_Stop();

      // Measure Distance At Right
      WHEELS_Move(WHEELS_DIRECTION_RIGHT);
      _delay_ms(250);
      WHEELS_Stop();
      float RightDistance = ULTRASONIC_GetDistanceCM();

      // Measure Distance At Left
      WHEELS_Move(WHEELS_DIRECTION_LEFT);
      _delay_ms(500);
      WHEELS_Stop();
      float LeftDistance = ULTRASONIC_GetDistanceCM();

      _delay_ms(500);
      if (RightDistance > LeftDistance)
      {
        WHEELS_Move(WHEELS_DIRECTION_RIGHT);
        _delay_ms(500);
        WHEELS_Stop();
      }
    }
    _delay_ms(500);
  }
}

void ISR_ModeSelect()
{
  WATCHDOG_Start(WATCHDOG_TIMEOUT_1000_ms);
}