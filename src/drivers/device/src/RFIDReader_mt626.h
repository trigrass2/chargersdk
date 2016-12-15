#ifndef __RFIDREADER_MT626_H
#define __RFIDREADER_MT626_H

#include <stdio.h>
#include "stm32f4xx.h"

#define MT626_CMD_MAX 10
#define MT626_SENDBUFF_MAX      100
#define MT626_RECVBUFF_MAX      100


#define MT626_CMD_STX                   0x02
#define MT626_CMD_ETX                   0x03
#define MT626_CMD_TYPE                  0x34

#define MT626_FIND_CMD                  0   //#0  Ѱ��
#define MT626_READ_UID_CMD              1   //#1  ��ȡUID       
#define MT626_AUTH_KEYA_CMD             2   //#3  ��֤KeyA
#define MT626_AUTH_KEYB_CMD             3   //#4  ��֤KeyB
#define MT626_READ_CMD                  4   //#5  ������������
#define MT626_WRITE_CMD                 5   //#6  д����������
#define MT626_CHANGE_KEY_CMD            6   //#7  ��������
#define MT626_INC_CMD                   7   //#8  ��ֵ
#define MT626_DEC_CMD                   8   //#9  ��ֵ
#define MT626_INIT_CMD                  9   //#10 ��ʼ��



typedef enum 
{
    MT_SUCCEED,
    MT_NOTAG,
    MT_LOGIN_SUCCEED,
    MT_LOGIN_FAIL,
    MT_READ_FAIL,
    MT_WRITE_FAIL,
    
    /*
    0x00 Operation succeed
0x01 No tag
0x02 Login succeed
0x03 Login fail
0x04 Read fail
0x05 Write fail
0x06 Unable to read after write
0x08 Address overflow
0x09 Download Key fail
0x0D Not authenticate
0x0E Not a value block or Halt selected card fail
0x0F Invalid command length parameter
0xF0 Checksum error
0xF1 Command code error
    */
}MT_RESULT;

typedef    int (*pMT626_MAKE_PROC)  (void *pObj, uint8_t ucSendID, uint8_t *pOptionData,uint8_t ucOptionLen,uint8_t *pucSendLength);
typedef    int (*pMT626_ANALY_PROC) (void *pObj, uint8_t ucSendID, uint8_t ucState,uint8_t *pRcvData, uint8_t ucRecvLen);


typedef struct _MT626CMD
{
    uint8_t     ucParam;
    uint16_t    usLenght;
    
    pMT626_MAKE_PROC  makeProc;
    pMT626_ANALY_PROC analyProc;
    
}MT626CMD_t;

typedef struct _MT626COM
{
    MT626CMD_t *pMT626CMD[MT626_CMD_MAX];
    uint8_t *pucSendBuffer;
    uint8_t *pucRecvBuffer;
    int (*sendCommand)(void *pObj,uint8_t ucSendID, uint8_t ucSendLenght);
    int (*recvResponse)(uint8_t ucSendID, uint8_t ucState,uint8_t *pRcvData, uint8_t ucRecvLen);
    pMT626_MAKE_PROC makeStCmd;
    pMT626_ANALY_PROC analyStRes;
    void (*deleteCOM)(void *pObj);
}MT626COM_t;


MT626COM_t *MT626COMCreate(void);

#endif