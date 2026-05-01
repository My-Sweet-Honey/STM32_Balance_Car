#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

void Serial_Init(void);
uint8_t Serial_GetRxFlag(void);
uint8_t Serial_GetRxData(void);
void BT_control(void);
void USART1_IRQHandler(void);

#endif
