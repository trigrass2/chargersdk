/**
* @file evse_globals.h
* @brief
* @author rgw
* @version v1.0
* @date 2017-01-22
*/

#ifndef  __EVSE_GLOBALS_H
#define  __EVSE_GLOBALS_H

#include "EVSE.h"
#include "interface_rfid.h"
#include "userlib_list.h"
#include "libEcharge/ech_protocol.h"

extern EVSE_t *pEVSE;
extern UserList_t *pListCON;
extern RFIDDev_t *pRFIDDev;
extern echProtocol_t *pechProto;

#endif
