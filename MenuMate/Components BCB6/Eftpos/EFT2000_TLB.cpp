// ************************************************************************ //
// WARNING                                                                    
// -------                                                                    
// The types declared in this file were generated from data read from a       
// Type Library. If this type library is explicitly or indirectly (via        
// another type library referring to this type library) re-imported, or the   
// 'Refresh' command of the Type Library Editor activated while editing the   
// Type Library, the contents of this file will be regenerated and all        
// manual modifications will be lost.                                         
// ************************************************************************ //

// $Rev: 18756 $
// File generated on 5/11/2010 12:33:16 p.m. from Type Library described below.

// ************************************************************************  //
// Type Lib: C:\PROGRA~2\Syncro\EFT2000.exe (1)
// LIBID: {AC17F141-6A16-11D3-ABEA-00105A6CA437}
// LCID: 0
// Helpfile: 
// HelpString: EFT2000 1.3 Type Library
// DepndLst: 
//   (1) v2.0 stdole, (C:\Windows\system32\stdole2.tlb)
// ************************************************************************ //

#include <vcl.h>
#pragma hdrstop

#include "EFT2000_TLB.h"

#if !defined(__PRAGMA_PACKAGE_SMART_INIT)
#define      __PRAGMA_PACKAGE_SMART_INIT
#pragma package(smart_init)
#endif

namespace Eft2000_tlb
{


// *********************************************************************//
// GUIDS declared in the TypeLibrary                                      
// *********************************************************************//
const GUID LIBID_EFT2000 = {0xAC17F141, 0x6A16, 0x11D3,{ 0xAB, 0xEA, 0x00,0x10, 0x5A, 0x6C,0xA4, 0x37} };
const GUID DIID_IPOSEvent = {0xAC17F14F, 0x6A16, 0x11D3,{ 0xAB, 0xEA, 0x00,0x10, 0x5A, 0x6C,0xA4, 0x37} };
const GUID CLSID_POSInterface = {0xAC17F14E, 0x6A16, 0x11D3,{ 0xAB, 0xEA, 0x00,0x10, 0x5A, 0x6C,0xA4, 0x37} };
const GUID IID_IPOSProductData = {0xE97AEB91, 0x9B3C, 0x11D5,{ 0x85, 0xA9, 0x00,0xC0, 0x4F, 0x07,0x8C, 0xAD} };
const GUID IID_IPOS = {0xAC17F14D, 0x6A16, 0x11D3,{ 0xAB, 0xEA, 0x00,0x10, 0x5A, 0x6C,0xA4, 0x37} };
const GUID IID_IPOS2 = {0x4D32AEA0, 0xB997, 0x11D4,{ 0xB5, 0xC3, 0x00,0xC0, 0x4F, 0x07,0x8D, 0xAE} };
const GUID IID_IPOS3 = {0x30CB4D32, 0x3C6B, 0x470A,{ 0x93, 0xE2, 0x4C,0x99, 0xCE, 0x56,0x18, 0x46} };
const GUID IID_IPOS4 = {0xD7E5A7E2, 0xE648, 0x4F26,{ 0xB6, 0x64, 0xFF,0xFE, 0x3A, 0xD4,0x47, 0xAB} };
const GUID DIID_IEFTEvent = {0xAC17F152, 0x6A16, 0x11D3,{ 0xAB, 0xEA, 0x00,0x10, 0x5A, 0x6C,0xA4, 0x37} };
const GUID CLSID_EFTInterface = {0xAC17F151, 0x6A16, 0x11D3,{ 0xAB, 0xEA, 0x00,0x10, 0x5A, 0x6C,0xA4, 0x37} };
const GUID IID_IDialogData = {0x7850B3D4, 0xF92F, 0x4AF0,{ 0xB2, 0xAB, 0x6B,0xFD, 0xFB, 0xF5,0x26, 0x48} };
const GUID IID_IEFTProductData = {0x1A9FB261, 0x9A92, 0x11D5,{ 0x85, 0xA8, 0x00,0xC0, 0x4F, 0x07,0x8C, 0xAD} };
const GUID IID_IEFT = {0xAC17F150, 0x6A16, 0x11D3,{ 0xAB, 0xEA, 0x00,0x10, 0x5A, 0x6C,0xA4, 0x37} };
const GUID IID_IEFT2 = {0x9C801080, 0xB906, 0x11D4,{ 0xB5, 0xC3, 0x00,0xC0, 0x4F, 0x07,0x8D, 0xAE} };
const GUID IID_IEFTIF = {0xE35B8937, 0x66C9, 0x11D3,{ 0xA0, 0x6A, 0x00,0x08, 0xC7, 0xBC,0x6F, 0xEA} };

};     // namespace Eft2000_tlb
