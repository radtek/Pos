/*
  ZBDlg.h - Header file fr DialogBox with buttons from language strings
  TZipBuilder VCL by Chris Vleghert and Eric W. Engler
  v1.79
  Copyright (C) 2005  Roger Aelbrecht

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License (licence.txt) for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  contact: Zipbuilder AT drehoeksw DOT net
  DelphiZip maillist subscribe at http://www.geocities.com/SiliconValley/Orchard/8607/ 
  updates: http://www.drehoeksw.net

  last modified 2008-10-17
---------------------------------------------------------------------------*/

#ifndef ZBDlgH
#define ZBDlgH
//---------------------------------------------------------------------------
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include "ZBMsgStr.h"
#include "ZipUtils.h" // for min and max templates

#if __BORLANDC__ < 1328		// BCB1=0x0520(1312)
#define mrNoToAll (Byte)(9)
#endif

namespace Zipdlg
{

const int PWLEN = 80;	// Maximum no. of characters in a password; Do not change!
const int zmtWarning  = 0x100000;
const int zmtError    = 0x110000;
const int zmtInformation = 0x120000;
const int zmtConfirmation = 0x130000;
const int zmtPassword = 0x140000;
const int NumBtns = 11;

#if __BORLANDC__ < 1344
#pragma option -a4
#else
#pragma option -a8
#endif
class  TZipDialogBox : public TForm
{
private:
	SIZE AvDlgUnits;
	int BeepID;
	int ctx;
	TLabel* DxText;
	const TCHAR* IconID;
	TEdit* PwdEdit;
  int __fastcall GetDlgType(void);
  String __fastcall GetPWrd(void);
  void __fastcall SetPWrd(const String Value);
public:
  __fastcall TZipDialogBox( TComponent *Owner, int context);
	__fastcall virtual ~TZipDialogBox( void ) {};
		// And just to keep BCB1 happy...
	__fastcall virtual TZipDialogBox( TComponent *Owner ) : TForm( Owner ) { };
  void __fastcall Build(const String& Title, const String& msg,
                        TMsgDlgButtons Btns);
  int __fastcall ShowModal(void);
  __property int DlgType={read= GetDlgType};
  __property String PWrd={read= GetPWrd, write= SetPWrd};
};

#pragma option -a.

}/* namespace Zipdlg*/
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Zipdlg;
#endif
#endif

