/*******************************************************************
 Copyright © 1995 - 1998, 3Com Corporation or its subsidiaries ("3Com").  
 All rights reserved.
   
 This software may be copied and used solely for developing products for 
 the Palm Computing platform and for archival and backup purposes.  Except 
 for the foregoing, no part of this software may be reproduced or transmitted 
 in any form or by any means or used to make any derivative work (such as 
 translation, transformation or adaptation) without express written consent 
 from 3Com.

 3Com reserves the right to revise this software and to make changes in content 
 from time to time without obligation on the part of 3Com to provide notification 
 of such revision or changes.  
 3COM MAKES NO REPRESENTATIONS OR WARRANTIES THAT THE SOFTWARE IS FREE OF ERRORS 
 OR THAT THE SOFTWARE IS SUITABLE FOR YOUR USE.  THE SOFTWARE IS PROVIDED ON AN 
 "AS IS" BASIS.  3COM MAKES NO WARRANTIES, TERMS OR CONDITIONS, EXPRESS OR IMPLIED, 
 EITHER IN FACT OR BY OPERATION OF LAW, STATUTORY OR OTHERWISE, INCLUDING WARRANTIES, 
 TERMS, OR CONDITIONS OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND 
 SATISFACTORY QUALITY.

 TO THE FULL EXTENT ALLOWED BY LAW, 3COM ALSO EXCLUDES FOR ITSELF AND ITS SUPPLIERS 
 ANY LIABILITY, WHETHER BASED IN CONTRACT OR TORT (INCLUDING NEGLIGENCE), FOR 
 DIRECT, INCIDENTAL, CONSEQUENTIAL, INDIRECT, SPECIAL, OR PUNITIVE DAMAGES OF 
 ANY KIND, OR FOR LOSS OF REVENUE OR PROFITS, LOSS OF BUSINESS, LOSS OF INFORMATION 
 OR DATA, OR OTHER FINANCIAL LOSS ARISING OUT OF OR IN CONNECTION WITH THIS SOFTWARE, 
 EVEN IF 3COM HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

 3Com, HotSync, Palm Computing, and Graffiti are registered trademarks, and 
 Palm III and Palm OS are trademarks of 3Com Corporation or its subsidiaries.

 IF THIS SOFTWARE IS PROVIDED ON A COMPACT DISK, THE OTHER SOFTWARE AND 
 DOCUMENTATION ON THE COMPACT DISK ARE SUBJECT TO THE LICENSE AGREEMENT 
 ACCOMPANYING THE COMPACT DISK.

 *-------------------------------------------------------------------   
 * FileName:
 *		Hardware.h
 *
 * Description:
 *		General Hardware Equates. This header file
 *  contains general hardware configuration equates that are not dependent on
 *  the version of the hardware we're running on. Equates specific to
 *  a certain component like the 68328, etc. is contained in separate
 *  header files.
 *
 * History:
 *   	4/27/95  RM - Created by Ron Marianetti
 *
 *******************************************************************/  

#ifdef	NON_PORTABLE	   // So app's don't mistakenly include this

#ifndef __HARDWARE_H__
#define __HARDWARE_H__



/**************************************************************************
 * General Equates
 ***************************************************************************/
 
// Bits in the low memory global GHwrWakeUp
#define	hwrWakeUpGeneral			0x0001
#define	hwrWakeUpPWM				0x0002
#define	hwrWakeUpFromKey			0x0004				// set if we woke from a key press
#define	hwrWakeUpReturnToSleep	0x0008				// set if we woke from 24 hr. interrupt
																	//  in which case we want to go right back to sleep


// Bits in the low memory global GHwrMiscFlags
#define	hwrMiscFlagHasBacklight	0x0001				// set if backlight is present
#define	hwrMiscFlagHasMbdIrDA	0x0002				// set if IrDA is present (on main board)
#define	hwrMiscFlagHasCardIrDA	0x0004				// set if IrDA is present (on memory card)
#define	hwrMiscFlagHasBurrBrown	0x0008				// set if BurrBrown A/D is present
#define	hwrMiscFlagHasJerryHW	0x0010				// set if Jerry Hardware is present
			// The following bit flags are set by HwrIdentifyFeatures.
			// They allow software to read the hardware ID without poking at hardware.
			// They also provide some isolation from different ID detection schemes
			// such as if the ID detection mechanism should change with EZ...
#define	hwrMiscFlagID1				0x1000				// set if ID bit keyBitHard1 was set
#define	hwrMiscFlagID2				0x2000				// set if ID bit keyBitHard2 was set
#define	hwrMiscFlagID3				0x4000				// set if ID bit keyBitHard3 was set
#define	hwrMiscFlagID4				0x8000				// set if ID bit keyBitHard4 was set
#define	hwrMiscFlagIDMask			0xF000



// Bits for the call HwrDockSignals
#define	hwrDockInSyncButton	0x0001					// Dock button
#define	hwrDockInGeneric1		0x0002					// General Purpose input #1

#define	hwrDockOutGeneric0	0x0001					// General Purpose output #0



// Define # of card slots
#if EMULATION_LEVEL == EMULATION_NONE
#define	hwrNumCardSlots	1								// 1 card slot
#else
#define	hwrNumCardSlots	2								// 2 card slots for emulation
#endif


// Screen Dimensions
#define	hwrDisplayWidth		160					// Width of display
#define	hwrDisplayHeight		160					// Height of display

// Screen depth info
#define	hwrDisplayBootDepth	1						// Boot time bits per pixel (system default)
#define	hwrDisplayMaxDepth	2						// Maximum bits per pixel hardware supports
#define	hwrDisplayAllDepths	0x0003				// All display depths (in bit map format)

// Screen refresh rate
#define	hwrDisplayPeriod		12						// frame period in ms.


// Battery scaling:
//    Voltage = (level + hwrVoltStepsOffset) / hwrStepsPerVolt
#define	hwrStepsPerVolt		78						// 78 = 1 volt.
#define	hwrVoltStepsOffset	0						// Add to level before scaling		

//----------------------------------------------------------------------------
// Digitizer Dimensions
//----------------------------------------------------------------------------
#define	hwrSystemTop			(hwrDisplayHeight+4)
#define	hwrSystemHeight		56

// Graffiti Area
#define	hwrGraffitiTop			hwrSystemTop			// Top coordinate of Graffiti area
#define	hwrGraffitiLeft		27							// Left coordinate of Graffiti area
#define	hwrGraffitiWidth		106						// Width of Graffiti area
#define	hwrGraffitiHeight		hwrSystemHeight		// Height of Graffiti area
#define	hwrGraffitiSplit		(hwrGraffitiLeft+62)	// Split mode dividing line

// Launch Button
#define	hwrLaunchBtnLeft		0							
#define	hwrLaunchBtnTop		hwrSystemTop						
#define	hwrLaunchBtnWidth		27
#define	hwrLaunchBtnHeight	(hwrSystemHeight/2)

// Menu Button
#define	hwrMenuBtnLeft			0							
#define	hwrMenuBtnTop			(hwrSystemTop+hwrMenuBtnHeight)  							
#define	hwrMenuBtnWidth		27
#define	hwrMenuBtnHeight		(hwrSystemHeight/2)

// Calculator Button
#define	hwrCalcBtnLeft			133							
#define	hwrCalcBtnTop			hwrSystemTop 						
#define	hwrCalcBtnWidth		27
#define	hwrCalcBtnHeight		(hwrSystemHeight/2)

// Find Button
#define	hwrFindBtnLeft			133							
#define	hwrFindBtnTop			(hwrSystemTop+hwrCalcBtnHeight)	 						
#define	hwrFindBtnWidth		27
#define	hwrFindBtnHeight		(hwrSystemHeight/2)

// Keyboard Alpha Button
#define	hwrKeyboardAlphaBtnLeft			27							
#define	hwrKeyboardAlphaBtnTop			(hwrSystemTop+hwrSystemHeight-hwrSystemHeight/4)	 						
#define	hwrKeyboardAlphaBtnWidth		18
#define	hwrKeyboardAlphaBtnHeight		(hwrSystemHeight/4)

// Keyboard Numeric Button
#define	hwrKeyboardNumericBtnLeft		(hwrFindBtnLeft-hwrKeyboardNumericBtnWidth)						
#define	hwrKeyboardNumericBtnTop		hwrKeyboardAlphaBtnTop	 						
#define	hwrKeyboardNumericBtnWidth		18
#define	hwrKeyboardNumericBtnHeight	hwrKeyboardAlphaBtnHeight

// HwrLCDGetDepth requests
#define	hwrLCDGetCurrentDepth			0				// Get depth display is currently set to


//----------------------------------------------------------------------------
// The ROM token list is an array of zero or more HwrROMTokenRec records
// terminated by a record having token 0xFFFFFFFF. Each record is variable
// length (6 bytes minimum) and must begin on an even byte boundry.
// 
// WARNING: DO NOT CHANGE THE STRUCTURE OF THIS RECORD WITHOUT MODIFYING THE
//          ASSOCIATED ASSEMBLY ROUTINES IN ROMHardwareTD1.c !!!
//          PrvGetROMToken, HwrGetROMToken, PrvExecuteROMToken1
//----------------------------------------------------------------------------
typedef struct {
	ULong	token;
	UInt	len;
	UChar	data[];		// variable length data goes here (0 or more chars)
} HwrROMTokenRec, *HwrROMTokenRecPtr;


#define	hwrROMTokenSnum		'snum'	// MUST be the first token, should be 12 bytes long
													//   Snum == Memory Card Flash ID (serial number)
#define	hwrROMTokenIrda		'irda'	// present ONLY on memory card w/IrDA (Gordon)
#define	hwrROMTokenFlex		'flex'	// present ONLY on memory card w/Flex pager (Cessna)
#define	hwrROMTokenBoot1		'cd01'	// boot time code plug-in #1 (See RomBoot.c)
#define	hwrROMTokenBoot2		'cd02'	// boot time code plug-in #2 (See RomBoot.c)
#define	hwrROMTokenBoot3		'cd03'	// boot time code plug-in #3 (See RomBoot.c)


// The following include has to appear here, because MemoryPrvNew.h
// needs hwrNumCardSlots, which is defined above...

#include <MemoryPrvNew.h>	// for CardHeaderPtr below


/**************************************************************************
 * Prototypes of functions used only when running on the real hardware
 ***************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif


	// These routines are only accessed from the Debugger or boot code,
	//  so they don't need traps.
	void		HwrPreRAMInit(void); 
	
	void		HwrPreDebugInit(void); 
	
	Err		HwrChecksumBigROM (CardHeaderPtr cardP);

	void 		HwrInit(void); 

	void		HwrPostDebugInit(void); 
	
	void		HwrIdentifyFeatures(void); 
	
	void		HwrResetNMI(void);
	
	DWord		HwrGetRAMSize(UInt cardNo, DWord limit);
	
	void		HwrDrawSplashScreen(void);

	void   	HwrDisableInts(void);
	
	void		HwrShutDownPLL(void);
	
	void		HwrLowBatteryHandler(void);

#ifndef __GNUC__
	asm void	PrvGetROMTokenListByCardNo(void);
	
	asm void	PrvGetROMToken(void);
	
	asm void	PrvExecuteROMToken1(void);
#endif

	// Interrupt handlers should NOT patch the low-memory interrupt vectors.
	// Instead, they should be installed using the following traps, which the
	// kernel will call whenever the given interrupt triggers.
	// Additionally, since the handlers are limited in what they can do (no calls
	// to Memory Manager or other routines that use it, including FeatureMgr),
	// a pointer to globals should be stored in the corresponding LowMem global.
	// See irq1GlobalsP, irq2GlobalsP, irq3GlobalsP, & irq6GlobalsP in <Globals.h>
	void		HwrIRQ1Handler(DWord param)
							SYS_TRAP(sysTrapHwrIRQ1Handler);
	void		HwrIRQ2Handler(DWord param)
							SYS_TRAP(sysTrapHwrIRQ2Handler);
	void		HwrIRQ3Handler(DWord param)
							SYS_TRAP(sysTrapHwrIRQ3Handler);
	void		HwrIRQ4Handler(DWord param)
							SYS_TRAP(sysTrapHwrIRQ4Handler);
	void		HwrIRQ5Handler(DWord param)
							SYS_TRAP(sysTrapHwrIRQ5Handler);
	void		HwrIRQ6Handler(DWord param)
							SYS_TRAP(sysTrapHwrIRQ6Handler);
							
	
	// These routines are for "general" use.
	
	void 		HwrGetRAMMapping(Ptr /*CardInfoPtr*/ cardInfoP, UInt* numBlocksP, 
							DWordPtr physBlockInfoP, DWordPtr logBlockInfoP)
							SYS_TRAP(sysTrapHwrGetRAMMapping);
				
	DWord		HwrMemWritable(VoidPtr addr)
							SYS_TRAP(sysTrapHwrMemWritable);
	
	DWord		HwrMemReadable(VoidPtr addr)
							SYS_TRAP(sysTrapHwrMemReadable);
							
	Err		HwrCursor(Boolean set, Int* modeP, PointType* whereP, PointType* sizeP, 
						UInt* blinkP)
							SYS_TRAP(sysTrapHwrCursor);
	
	void		HwrDelay(ULong	microseconds)
							SYS_TRAP(sysTrapHwrDelay);
	
	void		HwrDoze(Boolean onlyNMI)
							SYS_TRAP(sysTrapHwrDoze);
	
	void		HwrSleep(Boolean untilReset, Boolean emergency)
							SYS_TRAP(sysTrapHwrSleep);
	
	Err		HwrWake(void)
							SYS_TRAP(sysTrapHwrWake);
							
	Err		HwrSetSystemClock(ULong*	freqP)
							SYS_TRAP(sysTrapHwrSetSystemClock);
							
	Err		HwrSetCPUDutyCycle(UInt*	dutyP)
							SYS_TRAP(sysTrapHwrSetCPUDutyCycle);
							
	Err		HwrLCDInit(UInt framePeriod, VoidPtr baseP, UInt width,
							UInt height, UInt depth)
							SYS_TRAP(sysTrapHwrLCDInit);
							
	Err		HwrLCDBaseAddr(Ptr* newAddrP)
							SYS_TRAP(sysTrapHwrLCDBaseAddr);
							
	Err		HwrLCDDrawBitmap(UInt x, UInt y, Ptr bitmapP, Boolean clearFirst)
							SYS_TRAP(sysTrapHwrLCDDrawBitmap);
							
	Err		HwrLCDSleep(Boolean untilReset, Boolean emergency)
							SYS_TRAP(sysTrapHwrLCDSleep);
							
	Err		HwrLCDWake(void)
							SYS_TRAP(sysTrapHwrLCDWake);
							
	Word		HwrLCDGetDepth(Word request)
							SYS_TRAP(sysTrapHwrLCDGetDepth);
							
	Err		HwrTimerInit(UInt timer)
							SYS_TRAP(sysTrapHwrTimerInit);
							
	Err		HwrTimerSleep(UInt timer)
							SYS_TRAP(sysTrapHwrTimerSleep);
							
	Err		HwrTimerWake(UInt timer)
							SYS_TRAP(sysTrapHwrTimerWake);
							
	UInt		HwrBatteryLevel(void)
							SYS_TRAP(sysTrapHwrBatteryLevel);
							
	Boolean	HwrPluggedIn(void)
							SYS_TRAP(sysTrapHwrPluggedIn);
							
	Boolean	HwrBacklight(Boolean set, Boolean newState)
							SYS_TRAP(sysTrapHwrBacklight);
							
	Boolean	HwrEnableDataWrites(void)
							SYS_TRAP(sysTrapHwrEnableDataWrites);
	
	void		HwrDisableDataWrites(void)
							SYS_TRAP(sysTrapHwrDisableDataWrites);
							
	Err		HwrDockSignals(WordPtr inputsP, Word outputSet, Word outputClr)
							SYS_TRAP(sysTrapHwrDockSignals);
	
	Err		HwrGetROMToken (Word cardNo, DWord token, BytePtr *dataP, WordPtr sizeP )
							SYS_TRAP(sysTrapHwrGetROMToken);

							
#ifdef __cplusplus 
}
#endif


/************************************************************
 * Assembly Function Prototypes
 *************************************************************/
#define	_HwrWake		\
				ASM_SYS_TRAP(sysTrapHwrWake)

#define	_HwrDelay	\
				ASM_SYS_TRAP(sysTrapHwrDelay)


#endif 	//__HARDWARE_H__

#endif 	// NON_PORTABLE
