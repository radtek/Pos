#ifndef SmartCardDEFH
#define SmartCardDEFH

#include "SmartCardVer0API.h"
#include "SmartCardVer1API.h"
#include "SmartCardVer2API.h"
#include "SmartCardVer3API.h"
#include "SmartCardVer4API.h"
#include "SmartCardVer5API.h"

#define SMART_CARD_CURRENT_VERSION                          SMART_CARD_VERSION_SIX
#define SMART_CARD_CURRENT_VERSION_NUMBER                   6
#define SMART_CARD_CURRENT_VERSION_CLASS                    TSmartCardVer6
#define SMART_CARD_CURRENT_VERSION_CARD_MEMBER_DATA_LENGTH  V6_CARD_MEMBER_DATA_LENGTH

// Space must be a multiple of 8 for Blowfish encryption. Currently 208 Bytes.

#define CARD_BLOCK_START 0x00
#define CARD_TOTAL_MEMORY_LENGTH 0x100

#define CARD_END 0xFF
#define CARD_CRC_LENGTH 0x02
// Excludes the 4 bytes for Security Data & Error counter.
#define CARD_RESTORE_POINT_LENGTH 256

#define SMARTCARD_CARDREMOVED 0x01
#define SMARTCARD_CARDINSERTED 0x02

#define SMARTCARD_CARD_STATUS_BANNED  0x00
#define SMARTCARD_CARD_STATUS_ACTIVE  0x01
#define SMARTCARD_CARD_STATUS_BACKUP  0x02
#define SMARTCARD_CARD_STATUS_UNKNOWN 0x03

#endif
