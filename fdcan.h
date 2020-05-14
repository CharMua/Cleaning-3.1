#ifndef _FDCAN_H
#define _FDCAN_H

#include "global.h"
#include "sys.h"


#define FDCAN1_RX0_INT_ENABLE 1

#define CAN_RX_BUF  80

u8 FDCAN1_Mode_Init(u16 presc, u8 ntsjw, u16 ntsg1, u8 ntsg2, u32 mode);
u8 FDCAN1_Send_Msg(u8 *msg, u32 len);
u8 MYFDCAN1_Send(u8 *msg, u32 len, u32 id);
u8 FDCAN1_Receive_Msg(u8 *buf);
void connection_handler(void);
void init_motor(void);
void uart_init(u32 bound);
void Speed_To_Can(uint32_t *speed);
#endif
