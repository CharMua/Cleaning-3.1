#include "fdcan.h"
#include "delay.h"

FDCAN_HandleTypeDef FDCAN1_Handler;
FDCAN_RxHeaderTypeDef FDCAN1_RxHeader;
FDCAN_TxHeaderTypeDef FDCAN1_TxHeader;

u8 FDCAN1_Mode_Init(u16 presc, u8 ntsjw, u16 ntsg1, u8 ntsg2, u32 mode)
{
    FDCAN_FilterTypeDef FDCAN1_RXFilter;
    FDCAN_FilterTypeDef FDCAN1_RXFilter1;
    FDCAN_FilterTypeDef FDCAN1_RXFilter2;
    FDCAN_FilterTypeDef FDCAN1_RXFilter3;
    FDCAN_FilterTypeDef FDCAN1_RXFilter4;

    //?????FDCAN1
    HAL_FDCAN_DeInit(&FDCAN1_Handler); //??????????????
    FDCAN1_Handler.Instance = FDCAN1;
    FDCAN1_Handler.Init.FrameFormat = FDCAN_FRAME_CLASSIC; //?????
    FDCAN1_Handler.Init.Mode = mode;                       //???????
    FDCAN1_Handler.Init.AutoRetransmission = DISABLE;      //??????????????????????????????
    FDCAN1_Handler.Init.TransmitPause = DISABLE;           //?????????
    FDCAN1_Handler.Init.ProtocolException = DISABLE;       //???Э????????
    FDCAN1_Handler.Init.NominalPrescaler = presc;          //??????
    FDCAN1_Handler.Init.NominalSyncJumpWidth = ntsjw;      //??????????????
    FDCAN1_Handler.Init.NominalTimeSeg1 = ntsg1;           //tsg1??Χ:2~256
    FDCAN1_Handler.Init.NominalTimeSeg2 = ntsg2;           //tsg2??Χ:2~128
    FDCAN1_Handler.Init.MessageRAMOffset = 0;              //???RAM??? 0
    FDCAN1_Handler.Init.StdFiltersNbr = 4;                 //??????ID???????? 4
    FDCAN1_Handler.Init.ExtFiltersNbr = 0;                 //??????ID???????? 0

    FDCAN1_Handler.Init.RxFifo0ElmtsNbr = 1;                  //????FIFO0????? 1
    FDCAN1_Handler.Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_8; //????FIFO0????С??8???
    FDCAN1_Handler.Init.RxBuffersNbr = 0;                     //????????? 0

    FDCAN1_Handler.Init.TxEventsNbr = 0;                           //?????????? 0
    FDCAN1_Handler.Init.TxBuffersNbr = 0;                          //????????? 0
    FDCAN1_Handler.Init.TxFifoQueueElmtsNbr = 1;                   //????FIFO????????? 1
    FDCAN1_Handler.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION; //????FIFO??????
    FDCAN1_Handler.Init.TxElmtSize = FDCAN_DATA_BYTES_8;           //?????С:8???
    if (HAL_FDCAN_Init(&FDCAN1_Handler) != HAL_OK)
        return 1; //?????FDCAN

    /*RX filter to TPDO12*/
    FDCAN1_RXFilter.IdType = FDCAN_STANDARD_ID;             //???ID
    FDCAN1_RXFilter.FilterIndex = 0;                        //?????????
    FDCAN1_RXFilter.FilterType = FDCAN_FILTER_MASK;         //?????????
    FDCAN1_RXFilter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0; //??????0??????FIFO0
    FDCAN1_RXFilter.FilterID1 = 0x281;                      //TPDO2 cobe_id
    FDCAN1_RXFilter.FilterID2 = 0x7ff;                      //???FDCAN?????????????????????32λ????
    if (HAL_FDCAN_ConfigFilter(&FDCAN1_Handler, &FDCAN1_RXFilter) != HAL_OK)
        return 2;

    FDCAN1_RXFilter1.IdType = FDCAN_STANDARD_ID;             //???ID
    FDCAN1_RXFilter1.FilterIndex = 1;                        //?????????
    FDCAN1_RXFilter1.FilterType = FDCAN_FILTER_MASK;         //?????????
    FDCAN1_RXFilter1.FilterConfig = FDCAN_FILTER_TO_RXFIFO0; //??????0??????FIFO0
    FDCAN1_RXFilter1.FilterID1 = 0x282;                      //TPDO2 cobe_id
    FDCAN1_RXFilter1.FilterID2 = 0x7ff;                      //???FDCAN?????????????????????32λ????
    if (HAL_FDCAN_ConfigFilter(&FDCAN1_Handler, &FDCAN1_RXFilter1) != HAL_OK)
        return 3;

    FDCAN1_RXFilter2.IdType = FDCAN_STANDARD_ID;             //???ID
    FDCAN1_RXFilter2.FilterIndex = 2;                        //?????????
    FDCAN1_RXFilter2.FilterType = FDCAN_FILTER_MASK;         //?????????
    FDCAN1_RXFilter2.FilterConfig = FDCAN_FILTER_TO_RXFIFO0; //??????0??????FIFO0
    FDCAN1_RXFilter2.FilterID1 = 0x181;                      //TPDO1 cobe_id
    FDCAN1_RXFilter2.FilterID2 = 0x7ff;                      //???FDCAN?????????????????????32λ????
    if (HAL_FDCAN_ConfigFilter(&FDCAN1_Handler, &FDCAN1_RXFilter2) != HAL_OK)
        return 4;

    FDCAN1_RXFilter4.IdType = FDCAN_STANDARD_ID;             //???ID
    FDCAN1_RXFilter4.FilterIndex = 3;                        //?????????
    FDCAN1_RXFilter4.FilterType = FDCAN_FILTER_MASK;         //?????????
    FDCAN1_RXFilter4.FilterConfig = FDCAN_FILTER_TO_RXFIFO0; //??????0??????FIFO0
    FDCAN1_RXFilter4.FilterID1 = 0x182;                      //TPDO1 cobe_id
    FDCAN1_RXFilter4.FilterID2 = 0x7ff;                      //???FDCAN?????????????????????32λ????
    if (HAL_FDCAN_ConfigFilter(&FDCAN1_Handler, &FDCAN1_RXFilter4) != HAL_OK)
        return 5;
    HAL_FDCAN_ConfigGlobalFilter(&FDCAN1_Handler, FDCAN_REJECT, FDCAN_REJECT, DISABLE, DISABLE);
    /*RX filter to TPDO12*/

    HAL_FDCAN_Start(&FDCAN1_Handler);
    HAL_FDCAN_ActivateNotification(&FDCAN1_Handler, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
    return 0;
}

void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef *hfdcan)
{
    GPIO_InitTypeDef GPIO_Initure;
    RCC_PeriphCLKInitTypeDef FDCAN_PeriphClk;

    __HAL_RCC_FDCAN_CLK_ENABLE(); //???FDCAN1???
    __HAL_RCC_GPIOA_CLK_ENABLE(); //????GPIOA???

    //FDCAN1?????????PLL1Q
    FDCAN_PeriphClk.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
    FDCAN_PeriphClk.FdcanClockSelection = RCC_FDCANCLKSOURCE_PLL;
    HAL_RCCEx_PeriphCLKConfig(&FDCAN_PeriphClk);

    GPIO_Initure.Pin = GPIO_PIN_8 | GPIO_PIN_9;  //PA11,12
    GPIO_Initure.Mode = GPIO_MODE_AF_PP;         //??????
    GPIO_Initure.Pull = GPIO_PULLUP;             //????
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_MEDIUM; //??????
    GPIO_Initure.Alternate = GPIO_AF9_FDCAN1;    //?????CAN1
    HAL_GPIO_Init(GPIOB, &GPIO_Initure);         //?????

#if FDCAN1_RX0_INT_ENABLE
    HAL_NVIC_SetPriority(FDCAN1_IT0_IRQn, 1, 2);
    HAL_NVIC_EnableIRQ(FDCAN1_IT0_IRQn);
#endif
}

void HAL_FDCAN_MspDeInit(FDCAN_HandleTypeDef *hfdcan)
{
    __HAL_RCC_FDCAN_FORCE_RESET();   //??λFDCAN1
    __HAL_RCC_FDCAN_RELEASE_RESET(); //????λ

#if FDCAN1_RX0_INT_ENABLE
    HAL_NVIC_DisableIRQ(FDCAN1_IT0_IRQn);
#endif
}

/*UART_HandleTypeDef UART1_Handler; //UART句柄

void uart_init(u32 bound)
{
    //UART 初始化设置
    UART1_Handler.Instance = USART1;                    //USART1
    UART1_Handler.Init.BaudRate = bound;                //波特率
    UART1_Handler.Init.WordLength = UART_WORDLENGTH_8B; //字长为8位数据格式
    UART1_Handler.Init.StopBits = UART_STOPBITS_1;      //一个停止位
    UART1_Handler.Init.Parity = UART_PARITY_NONE;       //无奇偶校验位
    UART1_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE; //无硬件流控
    UART1_Handler.Init.Mode = UART_MODE_TX_RX;          //收发模式
    HAL_UART_Init(&UART1_Handler);                      //HAL_UART_Init()会使能UART1
}

int fputc(int ch, FILE *f)
{
    u8 temp[1] = {ch};
    HAL_UART_Transmit(&UART1_Handler, temp, 1, 5);
    return ch;
}*/
u8 FDCAN1_Send_Msg(u8 *msg, u32 len)
{
    FDCAN1_TxHeader.Identifier = 0x601;             //32λID
    FDCAN1_TxHeader.IdType = FDCAN_STANDARD_ID;     //???ID
    FDCAN1_TxHeader.TxFrameType = FDCAN_DATA_FRAME; //?????
    FDCAN1_TxHeader.DataLength = len;               //???????
    FDCAN1_TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    FDCAN1_TxHeader.BitRateSwitch = FDCAN_BRS_OFF;           //????????л?
    FDCAN1_TxHeader.FDFormat = FDCAN_CLASSIC_CAN;            //?????CAN??
    FDCAN1_TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS; //????????
    FDCAN1_TxHeader.MessageMarker = 0;

    if (HAL_FDCAN_AddMessageToTxFifoQ(&FDCAN1_Handler, &FDCAN1_TxHeader, msg) != HAL_OK)
        return 1; //????
    return 0;
}

u8 MYFDCAN1_Send(u8 *msg, u32 len, u32 id)
{
    FDCAN1_TxHeader.Identifier = id;                //32λID
    FDCAN1_TxHeader.IdType = FDCAN_STANDARD_ID;     //???ID
    FDCAN1_TxHeader.TxFrameType = FDCAN_DATA_FRAME; //?????
    FDCAN1_TxHeader.DataLength = len;               //???????
    FDCAN1_TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    FDCAN1_TxHeader.BitRateSwitch = FDCAN_BRS_OFF;           //????????л?
    FDCAN1_TxHeader.FDFormat = FDCAN_CLASSIC_CAN;            //?????CAN??
    FDCAN1_TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS; //????????
    FDCAN1_TxHeader.MessageMarker = 0;

    if (HAL_FDCAN_AddMessageToTxFifoQ(&FDCAN1_Handler, &FDCAN1_TxHeader, msg) != HAL_OK)
        return 1; //????
    return 0;
}

void Set_TPDO1(void)
{
    u32 msg_id = 0x000;
    msg_id = 0x601;
    static unsigned char msg_1[8] = {0x2F, 0x00, 0x1A, 0x00, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_1, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_2[8] = {0x23, 0x00, 0x1A, 0x01, 0x10, 0x01, 0x26, 0x00};
    MYFDCAN1_Send(msg_2, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_3[8] = {0x23, 0x00, 0x1A, 0x02, 0x10, 0x02, 0x26, 0x00};
    MYFDCAN1_Send(msg_3, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_4[8] = {0x23, 0x00, 0x1A, 0x03, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_4, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_5[8] = {0x23, 0x00, 0x1A, 0x04, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_5, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_6[8] = {0x23, 0x00, 0x1A, 0x05, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_6, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_7[8] = {0x23, 0x00, 0x1A, 0x06, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_7, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_8[8] = {0x23, 0x00, 0x1A, 0x07, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_8, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_9[8] = {0x23, 0x00, 0x1A, 0x08, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_9, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_10[8] = {0x2F, 0x00, 0x1A, 0x00, 0x02, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_10, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_11[8] = {0x2F, 0x00, 0x18, 0x02, 0xFE, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_11, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_12[8] = {0x2B, 0x00, 0x18, 0x03, 0x0A, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_12, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_13[8] = {0x2B, 0x00, 0x18, 0x05, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_13, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);

    msg_id = 0x602;
    static unsigned char msg_14[8] = {0x2F, 0x00, 0x1A, 0x00, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_14, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_15[8] = {0x23, 0x00, 0x1A, 0x01, 0x10, 0x01, 0x26, 0x00};
    MYFDCAN1_Send(msg_15, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_16[8] = {0x23, 0x00, 0x1A, 0x02, 0x10, 0x02, 0x26, 0x00};
    MYFDCAN1_Send(msg_16, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_17[8] = {0x23, 0x00, 0x1A, 0x03, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_17, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_18[8] = {0x23, 0x00, 0x1A, 0x04, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_18, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_19[8] = {0x23, 0x00, 0x1A, 0x05, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_19, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_20[8] = {0x23, 0x00, 0x1A, 0x06, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_20, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_21[8] = {0x23, 0x00, 0x1A, 0x07, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_21, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_22[8] = {0x23, 0x00, 0x1A, 0x08, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_22, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_23[8] = {0x2F, 0x00, 0x1A, 0x00, 0x02, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_23, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_24[8] = {0x2F, 0x00, 0x18, 0x02, 0xFE, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_24, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_25[8] = {0x2B, 0x00, 0x18, 0x03, 0x0A, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_25, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_26[8] = {0x2B, 0x00, 0x18, 0x05, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_26, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);

    msg_id = 0x601;
    static unsigned char msg_27[8] = {0x23, 0x00, 0x18, 0x01, 0x81, 0x01, 0x00, 0x00};
    MYFDCAN1_Send(msg_27, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    msg_id = 0x602;
    static unsigned char msg_28[8] = {0x23, 0x00, 0x18, 0x01, 0x82, 0x01, 0x00, 0x00};
    MYFDCAN1_Send(msg_28, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
}

void Set_TPDO2(void)
{
    u32 msg_id = 0x000;
    msg_id = 0x601;
    static unsigned char msg_1[8] = {0x2F, 0x01, 0x1A, 0x00, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_1, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_2[8] = {0x23, 0x01, 0x1A, 0x01, 0x20, 0x6C, 0x60, 0x00};
    MYFDCAN1_Send(msg_2, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_3[8] = {0x23, 0x01, 0x1A, 0x02, 0x20, 0x63, 0x60, 0x00};
    MYFDCAN1_Send(msg_3, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_4[8] = {0x23, 0x01, 0x1A, 0x03, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_4, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_5[8] = {0x23, 0x01, 0x1A, 0x04, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_5, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_6[8] = {0x23, 0x01, 0x1A, 0x05, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_6, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_7[8] = {0x23, 0x01, 0x1A, 0x06, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_7, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_8[8] = {0x23, 0x01, 0x1A, 0x07, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_8, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_9[8] = {0x23, 0x01, 0x1A, 0x08, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_9, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_10[8] = {0x2F, 0x01, 0x1A, 0x00, 0x02, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_10, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_11[8] = {0x2F, 0x01, 0x18, 0x02, 0xFE, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_11, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_12[8] = {0x2B, 0x01, 0x18, 0x03, 0x0A, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_12, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_13[8] = {0x2B, 0x01, 0x18, 0x05, 0x0A, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_13, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);

    msg_id = 0x602;
    static unsigned char msg_14[8] = {0x2F, 0x01, 0x1A, 0x00, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_14, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_15[8] = {0x23, 0x01, 0x1A, 0x01, 0x20, 0x6C, 0x60, 0x00};
    MYFDCAN1_Send(msg_15, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_16[8] = {0x23, 0x01, 0x1A, 0x02, 0x20, 0x63, 0x60, 0x00};
    MYFDCAN1_Send(msg_16, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_17[8] = {0x23, 0x01, 0x1A, 0x03, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_17, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_18[8] = {0x23, 0x01, 0x1A, 0x04, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_18, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_19[8] = {0x23, 0x01, 0x1A, 0x05, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_19, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_20[8] = {0x23, 0x01, 0x1A, 0x06, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_20, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_21[8] = {0x23, 0x01, 0x1A, 0x07, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_21, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_22[8] = {0x23, 0x01, 0x1A, 0x08, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_22, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_23[8] = {0x2F, 0x01, 0x1A, 0x00, 0x02, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_23, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_24[8] = {0x2F, 0x01, 0x18, 0x02, 0xFE, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_24, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_25[8] = {0x2B, 0x01, 0x18, 0x03, 0x0A, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_25, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_26[8] = {0x2B, 0x01, 0x18, 0x05, 0x0A, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_26, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);

    msg_id = 0x601;
    static unsigned char msg_27[8] = {0x23, 0x01, 0x18, 0x01, 0x81, 0x01, 0x00, 0x00};
    MYFDCAN1_Send(msg_27, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    msg_id = 0x602;
    static unsigned char msg_28[8] = {0x23, 0x01, 0x18, 0x01, 0x82, 0x01, 0x00, 0x00};
    MYFDCAN1_Send(msg_28, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
}

void Set_RPDO1(void)
{
    u32 msg_id = 0x000;
    msg_id = 0x601;
    static unsigned char msg_1[8] = {0x2F, 0x00, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_1, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_2[8] = {0x23, 0x00, 0x16, 0x01, 0x20, 0xFF, 0x60, 0x00};
    MYFDCAN1_Send(msg_2, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_3[8] = {0x23, 0x00, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_3, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_4[8] = {0x23, 0x00, 0x16, 0x03, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_4, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_5[8] = {0x23, 0x00, 0x16, 0x04, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_5, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_6[8] = {0x23, 0x00, 0x16, 0x05, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_6, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_7[8] = {0x23, 0x00, 0x16, 0x06, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_7, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_8[8] = {0x23, 0x00, 0x16, 0x07, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_8, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_9[8] = {0x23, 0x00, 0x16, 0x08, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_9, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_10[8] = {0x23, 0x00, 0x16, 0x00, 0x02, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_10, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_11[8] = {0x2F, 0x00, 0x14, 0x02, 0xFE, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_11, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_12[8] = {0x2B, 0x00, 0x14, 0x03, 0x64, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_12, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);

    msg_id = 0x602;
    static unsigned char msg_13[8] = {0x2F, 0x00, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_13, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_14[8] = {0x23, 0x00, 0x16, 0x01, 0x20, 0xFF, 0x60, 0x00};
    MYFDCAN1_Send(msg_14, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_15[8] = {0x23, 0x00, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_15, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_16[8] = {0x23, 0x00, 0x16, 0x03, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_16, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_17[8] = {0x23, 0x00, 0x16, 0x04, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_17, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_18[8] = {0x23, 0x00, 0x16, 0x05, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_18, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_19[8] = {0x23, 0x00, 0x16, 0x06, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_19, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_20[8] = {0x23, 0x00, 0x16, 0x07, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_20, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_21[8] = {0x23, 0x00, 0x16, 0x08, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_21, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_22[8] = {0x23, 0x00, 0x16, 0x00, 0x02, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_22, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_23[8] = {0x2F, 0x00, 0x14, 0x02, 0xFE, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_23, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_24[8] = {0x2B, 0x00, 0x14, 0x03, 0x64, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_24, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);

    msg_id = 0x601;
    static unsigned char msg_25[8] = {0x23, 0x00, 0x14, 0x01, 0x01, 0x02, 0x00, 0x00};
    MYFDCAN1_Send(msg_25, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    msg_id = 0x602;
    static unsigned char msg_26[8] = {0x23, 0x00, 0x14, 0x01, 0x02, 0x02, 0x00, 0x00};
    MYFDCAN1_Send(msg_26, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
}

void PDO_start(void)
{
    static u32 msg_id = 0x000;
    static unsigned char msg_1[8] = {0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_1, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_2[8] = {0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_2, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
}

void init_motor(void)
{
    connected = true;
    alarm_count = 0;
    u32 msg_id = 0x000;
    static unsigned char msg_1[8] = {0x81, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_1, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_2[8] = {0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_2, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_3[8] = {0x81, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_3, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    static unsigned char msg_4[8] = {0x80, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    MYFDCAN1_Send(msg_4, FDCAN_DLC_BYTES_8, msg_id);
    delay_ms(5);
    for (int i = 0; i < 2; i++)
    {
        msg_id = 0x601 + i;
        static unsigned char msg_5[8] = {0x2F, 0x60, 0x60, 0x00, 0x03, 0x00, 0x00, 0x00};
        MYFDCAN1_Send(msg_5, FDCAN_DLC_BYTES_8, msg_id);
        delay_ms(5);
        static unsigned char msg_6[8] = {0x23, 0x83, 0x60, 0x00, 0x00, 0x40, 0x00, 0x00};
        MYFDCAN1_Send(msg_6, FDCAN_DLC_BYTES_8, msg_id);
        delay_ms(5);
        static unsigned char msg_7[8] = {0x23, 0x84, 0x60, 0x00, 0x00, 0x40, 0x00, 0x00};
        MYFDCAN1_Send(msg_7, FDCAN_DLC_BYTES_8, msg_id);
        delay_ms(5);
        static unsigned char msg_8[8] = {0x2B, 0x40, 0x60, 0x00, 0x06, 0x00, 0x00, 0x00};
        MYFDCAN1_Send(msg_8, FDCAN_DLC_BYTES_8, msg_id);
        delay_ms(5);
        static unsigned char msg_9[8] = {0x23, 0xFF, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
        MYFDCAN1_Send(msg_9, FDCAN_DLC_BYTES_8, msg_id);
        delay_ms(5);
        static unsigned char msg_10[8] = {0x2B, 0x40, 0x60, 0x00, 0x0F, 0x00, 0x00, 0x00};
        MYFDCAN1_Send(msg_10, FDCAN_DLC_BYTES_8, msg_id);
        delay_ms(5);
    }
    Set_RPDO1();
    Set_TPDO1();
    Set_TPDO2();
    PDO_start();
}

void Speed_To_Can(u32 *speed)
{
    /*    static unsigned char msg_1[8], msg_2[8];
    static float v_left, v_right, rpm_left, rpm_right;
    static u16 high, low;
    static u32 id_1, id_2;
    id_1 = 0x201;
    id_2 = 0x202;
    v_left = twist - angular * WHEEL_INTERVAL / 2;
    v_right = twist + angular * WHEEL_INTERVAL / 2;
    rpm_left = v_left * WHEEL_DIAMETER * PI * KINCO_RPM_HEX * DECEL_RATIO;
    rpm_right = v_right * WHEEL_DIAMETER * PI * KINCO_RPM_HEX * DECEL_RATIO;
    high = ((u32)(rpm_left)) / 65536;
    low = ((u32)(rpm_left)) % 65536;
    msg_1[0] = low % 256;
    msg_1[1] = low / 256;
    msg_1[2] = high % 256;
    msg_1[3] = high / 256;
    msg_1[4] = 0x00;
    msg_1[5] = 0x00;
    msg_1[6] = 0x00;
    msg_1[7] = 0x00;
    MYFDCAN1_Send(msg_1, FDCAN_DLC_BYTES_8, id_1);
    delay_ms(1);
    high = ((u32)(rpm_right)) / 65536;
    low = ((u32)(rpm_right)) % 65536;
    msg_1[0] = low % 256;
    msg_1[1] = low / 256;
    msg_1[2] = high % 256;
    msg_1[3] = high / 256;
    msg_1[4] = 0x00;
    msg_1[5] = 0x00;
    msg_1[6] = 0x00;
    msg_1[7] = 0x00;
    MYFDCAN1_Send(msg_2, FDCAN_DLC_BYTES_8, id_2);*/
    static u16 high, low;
    static u32 id_1, id_2;
    static unsigned char msg_1[8], msg_2[8];
    int32_t vel = 1e-3;
    id_1 = 0x201;
    id_2 = 0x202;
    vel = speed[0] * KINCO_RPM_HEX * DIR_LEFT;
    high = ((uint32_t)(vel)) / 65536;
    low = ((uint32_t)(vel)) % 65536;
    msg_1[0] = low % 256;
    msg_1[1] = low / 256;
    msg_1[2] = high % 256;
    msg_1[3] = high / 256;
    msg_1[4] = 0x00;
    msg_1[5] = 0x00;
    msg_1[6] = 0x00;
    msg_1[7] = 0x00;
    MYFDCAN1_Send(msg_1, FDCAN_DLC_BYTES_8, id_1);
    delay_ms(1);
    vel = speed[1] * KINCO_RPM_HEX * DIR_RIGHT;
    high = ((uint32_t)(vel)) / 65536;
    low = ((uint32_t)(vel)) % 65536;
    msg_2[0] = low % 256;
    msg_2[1] = low / 256;
    msg_2[2] = high % 256;
    msg_2[3] = high / 256;
    msg_2[4] = 0x00;
    msg_2[5] = 0x00;
    msg_2[6] = 0x00;
    msg_2[7] = 0x00;
    MYFDCAN1_Send(msg_2, FDCAN_DLC_BYTES_8, id_2);
    delay_ms(1);
}

u8 FDCAN1_Receive_Msg(u8 *buf)
{
    if (HAL_FDCAN_GetRxMessage(&FDCAN1_Handler, FDCAN_RX_FIFO0, &FDCAN1_RxHeader, buf) != HAL_OK)
        return 0; //????????
    return FDCAN1_RxHeader.DataLength >> 16;
}

#if FDCAN1_RX0_INT_ENABLE

void motor_process_right(u8 *data)
{
    static u32 low_1, high_1;
    low_1 = data[1] * 256 + data[0];
    high_1 = data[3] * 256 + data[2];
    info_right.speed = (int)(high_1 * 65536 + low_1);
    low_1 = data[5] * 256 + data[4];
    high_1 = data[7] * 256 + data[6];
    info_right.pos = (int)(high_1 * 65536 + low_1);
    ck_rh = true;
    printf("pos_right is %d\n", info_right.pos);
}

void motor_process_left(u8 *data)
{
    static u32 low_1, high_1;
    low_1 = data[1] * 256 + data[0];
    high_1 = data[3] * 256 + data[2];
    info_left.speed = (int)(high_1 * 65536 + low_1);
    low_1 = data[5] * 256 + data[4];
    high_1 = data[7] * 256 + data[6];
    info_left.pos = (int)(high_1 * 65536 + low_1);
    ck_lf = true;
    printf("pos_left is %d\n", info_left.pos);
}

static void odometry_Cal_Init(void)
{
    x = 0;
    y = 0;
    th = 0;
    vxy = 0;
    vth = 0;
    odom.pose.x = x;
    odom.pose.y = y;
    odom.pose.z = 0;
    odom.twist.linear_x = 0;
    odom.twist.linear_y = 0;
    odom.twist.angualr_z = 0;
    //prev = clock() / CLOCKS_PER_SEC;
    told = HAL_GetTick() / 1000;
    prev_left_pos = info_left.pos;
    prev_right_pos = info_right.pos;
}

void connection_handler(void)
{
    if (connection_check == true)
    {
        connection_check = false;
        disconnect_cnt = 0;
    }
    else
    {
        disconnect_cnt++;
        if (disconnect_cnt > 251)
        {
            alarm_count++;
            disconnect_cnt = -251;
            connected = false; //CAN disconnec;
            if (connected == false)
            {
                static ALARM_MSG alarm_con;
                alarm_con.seq = alarm_count;
                alarm_con.alarm_id = 3;
                strcpy(alarm_con.alarm_msg, "CAN DISCONNECTED");
                strcpy(alarm_con.node_id, "MOTOR");
                alarm_con.sensor_id = 3;
                connected = true;
                printf("lose connection!\n\r");
            }
        }
    }
}

void warning_frame_handler(u32 id, u8 *rxdata)
{
    static int32_t temp, err_1, err_2, cnt = 0, warn_count = 0;
    temp = rxdata[1] << 8;
    err_1 = temp + rxdata[0];
    temp = rxdata[3] << 8;
    err_2 = temp + rxdata[2];

    while (err_1)
    {
        alarm_count++;
        cnt = err_1 & 0x01;
        err_1 = (err_1 >> 1);
        if (cnt == 1)
        {
            static ALARM_MSG alarm_err1;
            alarm_err1.seq = alarm_count;
            strcpy(alarm_err1.alarm_msg, warn_group1[warn_count]);
            strcpy(alarm_err1.node_id, "MOTOR");
            alarm_err1.sensor_id = 3;
            printf("alarm_msg is: %s\n\r", alarm_err1.alarm_msg);
            //here pub the struct alarm_err1;
        }
        warn_count++;
    }

    warn_count = 0;

    while (err_2)
    {
        alarm_count++;
        cnt = err_2 & 0x01;
        err_2 = (err_2 >> 1);
        if (cnt == 1)
        {
            static ALARM_MSG alarm_err2;
            alarm_err2.seq = alarm_count;
            strcpy(alarm_err2.alarm_msg, warn_group2[warn_count]);
            strcpy(alarm_err2.node_id, "MOTOR");
            alarm_err2.sensor_id = 3;
            printf("alarm_msg is: %s\n\r", alarm_err2.alarm_msg);
            //here pub the struct alarm_err2;
        }
        warn_count++;
    }
}

void odometry_cal(int32_t pos_left, int32_t pos_right)
{
    float dt;
    static u32 dt_temp;
    static float dxy_ave, dx, dy, dth;
    static float dright, dleft;
    static int32_t dright_temp, dleft_temp;
    u32 reload = 10000;
    //clock_t now;
    //u32 tnow;
    static bool ck_xy, ck_th;
    ck_xy = false;
    ck_th = false;
    //now = clock() / CLOCKS_PER_SEC;
    if (first_cal)
    {
        alarm_msg.seq = 0;
        odometry_Cal_Init();
        first_cal = false;
    }
    tnow = HAL_GetTick() / 1000; //ms

    //dt = 0.01f;
    if (told > tnow)
    {
        dt_temp = reload + told - tnow;
    }
    else
    {
        dt_temp = told - tnow;
    }
    dt = dt_temp / 1000;
    //prev = now;
    told = tnow;
    
    dright_temp = info_right.pos - prev_right_pos;
    if((dright_temp -(MAX_POSE/2.0)) > 1e-3)
    {
        dright_temp -= MAX_POSE;
    }
    if((dright_temp + (MAX_POSE/2.0))<=1e-3)
    {
        dright_temp +=MAX_POSE;
    }
    
    dleft_temp = info_left.pos - prev_left_pos;
    if((dleft_temp -(MAX_POSE/2.0)) > 1e-3)
    {
        dleft_temp -= MAX_POSE;
    }
    if((dleft_temp + (MAX_POSE/2.0))<=1e-3)
    {
        dleft_temp +=MAX_POSE;
    }
    
    if (info_right.pos - prev_right_pos < 0)
    {
        dright_temp = info_right.pos - prev_right_pos + ENCODER_RELOAD;
    }
    else
    {
        dright_temp = info_right.pos - prev_right_pos;
    }

    if (info_left.pos - prev_left_pos < 0)
    {
        dleft_temp = info_left.pos - prev_left_pos + ENCODER_RELOAD;
    }
    else
    {
        dleft_temp = info_left.pos - prev_left_pos;
    }

    dright = ((float)dright_temp) * DISTANCE_PER_PULSE;
    dleft = ((float)dleft_temp) * DISTANCE_PER_PULSE;
    prev_left_pos = info_left.pos;
    prev_right_pos = info_right.pos;

    dxy_ave = (dright + dleft) * 0.5f;
    dth = (dright - dleft) / WHEEL_INTERVAL;
    vxy = dxy_ave / 0.02f; //dt; //小车线速度

    if (vxy >= 1.5f)
        vxy = 1.5f;
    if (vxy <= -1.5f)
        vxy = -1.5f;

    vth = dth / 0.02f; //小车角速度

    if (dxy_ave != 0)
    {
        dx = cos(dth) * dxy_ave;
        dy = -sin(dth) * dxy_ave;
        temp_x += dx;
        temp_y += dy;
        x += (cos(th) * dx - sin(th) * dy);
        y += (sin(th) * dx + cos(th) * dy);
        ck_xy = true;
    }

    if (dth != 0)
    {
        th += dth;
        ck_th = true;
    }

    if (ck_xy || ck_th)
    {
        odom.pose.x = x;
        odom.pose.y = y;
        odom.pose.z = 0;
        odom.pose.th = th;
        odom.twist.linear_x = vxy;
        odom.twist.linear_y = 0;
        odom.twist.angualr_z = vth;
        printf("pos_x is %f\n\r", x);
        printf("pos_y is %f\n\r", y);
        printf("pos_th is %f\n\r", th);
        printf("speed_x is %f\n\r", vxy);
        printf("speed_th is %f\n\r", vth);
        ck_xy = false;
        ck_th = false;
    }
}

//FDCAN1?ж??????
void FDCAN1_IT0_IRQHandler(void)
{
    HAL_FDCAN_IRQHandler(&FDCAN1_Handler);
}

//FIFO0???????
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
    static u8 rxdata[8];
    static int id, len;

    if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
    {
        HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &FDCAN1_RxHeader, rxdata);
        id = FDCAN1_RxHeader.Identifier;
        len = FDCAN1_RxHeader.DataLength;
        test_ss++;

        if (len == FDCAN_DLC_BYTES_8)
        {
            switch (id)
            {
            case MOTOR_PROCESS_LEFT:
                motor_process_left(rxdata);
                connection_check = true;
                break;
            case MOTOR_PROCESS_RIGHT:
                motor_process_right(rxdata);
                connection_check = true;
                break;
            case WARNING_FRAME_ONE:
            case WARNING_FRAME_TWO:
                warning_frame_handler(id, rxdata);
            default:
                break;
            }
        }
        if (ck_lf && ck_rh)
        {
            odometry_cal(info_left.pos, info_right.pos);
            ck_lf = false;
            ck_rh = false;
        }
    }
}
#endif
