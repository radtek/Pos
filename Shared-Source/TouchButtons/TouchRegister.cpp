//---------------------------------------------------------------------------
#pragma hdrstop

#include "TouchEditors.h"
#include "TouchBtn.h"
#include "TouchGrid.h"
#include "TouchPages.h"
#include "TouchKeyboard.h"
#include "TouchNumpad.h"
#include "TouchTime.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
namespace Touchbtn     
{
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes[1] = {__classid(TTouchBtn)};
		RegisterComponents("TouchControls", classes, 0);
	}
}
//---------------------------------------------------------------------------
namespace Touchgrid
{
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes[1] = {__classid(TTouchGrid)};
		RegisterComponents("TouchControls", classes, 0);
		RegisterComponentEditor(__classid(TTouchGrid),__classid(TTouchGridEditor));
	}
}
//---------------------------------------------------------------------------
namespace Touchtime
{
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes[1] = {__classid(TTouchTime)};
		RegisterComponents("TouchControls", classes, 0);
	}
}
//---------------------------------------------------------------------------
namespace Touchpages
{
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes_1[1] = {__classid(TTouchPages)};
		TComponentClass classes_2[1] = {__classid(TTouchSheet)};
		RegisterComponents("TouchControls", classes_1, 0); 
		RegisterNoIcon(classes_2, 0);
		RegisterClass(classes_2[0]);
		RegisterComponentEditor(classes_1[0], __classid(TTouchPagesEditor));
		RegisterComponentEditor(classes_2[0], __classid(TTouchPagesEditor));
	}
}
//---------------------------------------------------------------------------
namespace Touchnumpad
{
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes[2] = {__classid(TTouchNumpad), __classid(TNumpadDisplay)};
		RegisterComponents("TouchControls", classes, 1);
	}
}
//---------------------------------------------------------------------------
namespace Touchkeyboard
{
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes[1] = {__classid(TMMTouchKeyboard)};
		RegisterComponents("TouchControls", classes, 0);
	}
}
//---------------------------------------------------------------------------

