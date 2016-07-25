//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ChefmateSettings.h"

#include "MMTouchKeyboard.h"
#include "MMTouchNumpad.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------

__fastcall TfrmChefmateSettings::TfrmChefmateSettings(TComponent* Owner)
	: TZForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmChefmateSettings::FormCreate(TObject *Sender)
{
	initSettings();
}
//---------------------------------------------------------------------------
void __fastcall TfrmChefmateSettings::FormShow(TObject *Sender)
{
	refreshSettings();
}
//---------------------------------------------------------------------------
void __fastcall TfrmChefmateSettings::btnOKMouseClick(TObject *Sender)
{
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TfrmChefmateSettings::btnCancelMouseClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TfrmChefmateSettings::chkbActiveClick(TObject *Sender)
{
	refreshSettings();
}
//---------------------------------------------------------------------------
void __fastcall TfrmChefmateSettings::edIPAddrClick(TObject *Sender)
{
	if( !( ( TControl *)Sender )->Enabled )
		return;

	UnicodeString text = checkIPAddr( IPAddr );

	if( editText( "Chefmate IP address", text ) )
	{
		IPAddr = text;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmChefmateSettings::edIPPortClick(TObject *Sender)
{
	if( !( ( TControl *)Sender )->Enabled )
		return;

	UnicodeString text = IntToStr( ( __int32 )checkIPPort( IPPort ) );

	if( editNumText( "Chefmate IP port", text ) )
	{
		IPPort = StrToInt( checkIPPortStr( text ) );
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmChefmateSettings::chkbDynamicClick(TObject *Sender)
{
	if( !( ( TControl *)Sender )->Enabled )
		return;

	Dynamic = ( ( TRadioButton* )Sender )->Checked;
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC METHODS
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void TfrmChefmateSettings::Set(
						bool       inActive,
						AnsiString inIPAddr,
						__int32    inIPPort,
						bool       inDynamic )
{
	Active  = inActive;
	IPAddr  = inIPAddr;
	IPPort  = inIPPort;
	Dynamic = inDynamic;

	chkbActive->Checked = inActive;
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//:PRIVATE METHODS
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void TfrmChefmateSettings::initSettings()
{
	// MULTI-CHEFMATE
	chkbActive->Enabled = false;
	chkbActive->Checked = true;

	enableAndClearEdit( true, edIPAddr, checkIPAddr( FIPAddr ) );
	enableAndClearEdit( false, edIPPort, checkIPPort( FIPPort ) );

	chkbDynamic->Checked = true;
	chkbDynamic->Enabled = false;

/*
	// SINGLE-CHEFMATE
	chkbActive->Checked = false;

	enableAndClearEdit( chkbActive->Checked, edIPAddr, checkIPAddr( FIPAddr ) );
	enableAndClearEdit( chkbActive->Checked, edIPPort, checkIPPort( FIPPort ) );

	chkbDynamic->Checked = false;
	chkbDynamic->Enabled = chkbActive->Checked;
*/
}
//---------------------------------------------------------------------------
void TfrmChefmateSettings::refreshSettings()
{
	// MULTI-CHEFMATE
	// Do nothing


/*
	// SINGLE-CHEFMATE
	enableAndClearEdit( chkbActive->Checked, edIPAddr, checkIPAddr( FIPAddr ) );
	enableAndClearEdit( chkbActive->Checked, edIPPort, checkIPPort( FIPPort ) );

	chkbDynamic->Enabled = chkbActive->Checked;
*/
}
//---------------------------------------------------------------------------
void TfrmChefmateSettings::enableAndClearEdit(
							bool           inEnabled,
							TEdit*         inEdit,
							UnicodeString  inText )
{
	inEdit->Enabled = inEnabled;

	inEdit->Text        = inText;
	inEdit->Font->Color = inEnabled ? clBlack : clBlack;
	inEdit->Color       = inEnabled ? clWhite : clSilver;
}
//---------------------------------------------------------------------------
UnicodeString TfrmChefmateSettings::checkIPAddr( UnicodeString inIPAddr )
{
	UnicodeString result = ( inIPAddr == "" ) ?
						   ( UnicodeString )_DEFAULT_CM_IP_ADDR : inIPAddr;
	return result;
}
//---------------------------------------------------------------------------
UnicodeString TfrmChefmateSettings::checkIPPortStr( UnicodeString inIPPortStr )
{
	UnicodeString result = IntToStr( _DEFAULT_CM_IP_PORT );

	//::::::::::::::::::::::::::::::::::::::

	try
	{
		unsigned int ipPort = StrToInt( inIPPortStr );

		return inIPPortStr;
	}
	catch( ... )
	{
	}

	//::::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------
unsigned int TfrmChefmateSettings::checkIPPort( unsigned int inIPPort )
{
	unsigned int result = _DEFAULT_CM_IP_PORT;

	//::::::::::::::::::::::::::::::::::::::

	if( inIPPort > 0 )
	{
		result = inIPPort;
	}

	//::::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------
bool TfrmChefmateSettings::editText( UnicodeString inCaption, UnicodeString &outText )
{
	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength           = 250;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown  = false;
	frmTouchKeyboard->KeyboardText        = outText;

	frmTouchKeyboard->Caption = inCaption;
	if( frmTouchKeyboard->ShowModal() == mrOk )
	{
		outText = frmTouchKeyboard->KeyboardText;

		return true;
	}
	else
	{
        return false;
    }
}
//---------------------------------------------------------------------------
bool TfrmChefmateSettings::editNumText( UnicodeString inCaption, UnicodeString &outText )
{
	std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
	frmTouchNumpad->btnSurcharge->Caption = "Ok";
	frmTouchNumpad->btnSurcharge->Visible = true;
	frmTouchNumpad->btnDiscount->Visible  = false;
	frmTouchNumpad->Mode                  = pmNumber;
	frmTouchNumpad->INTInitial            = StrToInt( checkIPPortStr( outText ) );

	frmTouchNumpad->Caption = inCaption;
	if( frmTouchNumpad->ShowModal() == mrOk)
	{
		outText = IntToStr( frmTouchNumpad->INTResult );

		return true;
	}
	else
	{
		return false;
	}
}
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PROTECTED METHODS
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

bool TfrmChefmateSettings::_getActive()
{
	return chkbActive->Checked;
}
//---------------------------------------------------------------------------
UnicodeString TfrmChefmateSettings::_getIPAddr()
{
	return edIPAddr->Text;
}
//---------------------------------------------------------------------------
unsigned int TfrmChefmateSettings::_getIPPort()
{
	return StrToInt( edIPPort->Text );
}
//---------------------------------------------------------------------------
bool TfrmChefmateSettings::_getDynamic()
{
	return chkbDynamic->Checked;
}
//---------------------------------------------------------------------------
void TfrmChefmateSettings::_setActive( bool inActive )
{
	chkbActive->Checked = inActive;
	refreshSettings();
}
//---------------------------------------------------------------------------
void TfrmChefmateSettings::_setIPAddr( UnicodeString inIPAddr )
{
	if( chkbActive->Checked )
	{
		edIPAddr->Text = checkIPAddr( inIPAddr );
	}
}
//---------------------------------------------------------------------------
void TfrmChefmateSettings::_setIPPort( UnicodeString inIPPort )
{
	if( chkbActive->Checked )
	{
		edIPPort->Text = checkIPPortStr( inIPPort );
	}
}
//---------------------------------------------------------------------------
void TfrmChefmateSettings::_setDynamic( bool inDynamic )
{
	if( chkbActive->Checked )
	{
		chkbDynamic->Checked = inDynamic;
	}
}
//---------------------------------------------------------------------------

