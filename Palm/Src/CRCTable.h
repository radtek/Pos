/***************************** C HEADER FILE ********************************
*
*
*  PROJECT:    Any that require XMODEM CRC generation.
*
*  SYS MODULE: CRC table.
*
*  FILENAME:   CRCTable.h
*
*  PURPOSE:    To allow users to generate XMODEM CRCs using a lookup table
*              method. The CRC polynomial is x^16+x^12+x^5+1.
*
*  CONTENTS:   Macros to start and update the CRC. 
*
*  HISTORY:     
*
****************************************************************************/
#ifndef  CRC_TBL_H
#define  CRC_TBL_H

/* HEADER FILES ===========================================================*/


/* DEFINITIONS ============================================================*/

/* Macro to start the 16 bit CRC off. Must be invoked with the first UInt8 of 
   data the CRC is being calculated for (and also before invoking 
   UpdateCRC) */
#define StartCRC(FirstByte) \
        ( XMODEMCRCTable[ (unsigned char)(FirstByte)] )

/* Macro to update the current value of the 16 bit CRC with a new 8 bit data
   element. Returns the updated 16 bit CRC value. */
#define UpdateCRC(OldCRC, NewByte) \
        ( ((OldCRC)<< 8 )^ XMODEMCRCTable[( (unsigned short)(OldCRC) >> 8 )^\
          (unsigned char)(NewByte)] )
 
/* The CRC lookup table. */
extern const unsigned short XMODEMCRCTable[256];

/* FUNCTION PROTOTYPES ====================================================*/


/*=========================================================================*/

#endif

/* END OF HEADER FILE =====================================================*/

