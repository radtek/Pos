//---------------------------------------------------------------------------
#pragma hdrstop

#include "ZForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
TForm * TZForm::WinOwner = NULL;
//---------------------------------------------------------------------------
__fastcall TZForm::TZForm(TComponent* Owner) : TForm(Owner)
{
   try
   {
/*    TManagerTheme ManagerTheme;
      ManagerTheme.GenerateTheme(this);
      ManagerTheme.ProcessControl(this); */

      ButtonColors[BUTTONTYPE_UNSELECTED][ATTRIB_BUTTONCOLOR] = CL_BUTTONCOLOUR_UNSELECTED;
      ButtonColors[BUTTONTYPE_UNSELECTED][ATTRIB_FONTCOLOR] = CL_BUTTONCOLOUR_UNSELECTED_FONT_COLOR;
      ButtonColors[BUTTONTYPE_UNSELECTED][ATTRIB_BUTTONCOLOR_CANCELLED] = CL_BUTTONCOLOUR_CANCELLED;
      ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR] = CL_BUTTONCOLOUR_EMPTY_SEAT;
      ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_FONTCOLOR] = CL_BUTTONCOLOUR_EMPTY_SEAT_FONT_COLOR;
      ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR_CANCELLED] = CL_BUTTONCOLOUR_CANCELLED;
      ButtonColors[BUTTONTYPE_FULL][ATTRIB_BUTTONCOLOR] = CL_BUTTONCOLOUR_FULL_SEAT;
      ButtonColors[BUTTONTYPE_FULL][ATTRIB_FONTCOLOR] = CL_BUTTONCOLOUR_FULL_SEAT_FONT_COLOR;
      ButtonColors[BUTTONTYPE_LOCKED][ATTRIB_BUTTONCOLOR] = CL_BUTTONCOLOUR_LOCKED_SEAT;
      ButtonColors[BUTTONTYPE_LOCKED][ATTRIB_FONTCOLOR] = CL_BUTTONCOLOUR_LOCKED_SEAT_FONT_COLOR;
      ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_BUTTONCOLOR] = CL_BUTTONCOLOUR_SELECTED_SEAT;
      ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_FONTCOLOR] = CL_BUTTONCOLOUR_SELECTED_SEAT_FONT_COLOR;
      ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_BUTTONCOLOR_CANCELLED] = CL_BUTTONCOLOUR_CANCELLED;
      ButtonColors[BUTTONTYPE_SEC_SELECTED][ATTRIB_BUTTONCOLOR] = CL_BUTTONCOLOUR_SELECTED_SECONDARY_FUNC;
      ButtonColors[BUTTONTYPE_SEC_SELECTED][ATTRIB_FONTCOLOR] = CL_BUTTONCOLOUR_SELECTED_SECONDARY_FUNC_FONT_COLOR;
      ButtonColors[BUTTONTYPE_SEC_UNSELECTED][ATTRIB_BUTTONCOLOR] = CL_BUTTONCOLOUR_UNSELECTED_SECONDARY_FUNC;
      ButtonColors[BUTTONTYPE_SEC_UNSELECTED][ATTRIB_FONTCOLOR] = CL_BUTTONCOLOUR_UNSELECTED_SECONDARY_FUNC_FONT_COLOR;

      //ManagerTheme.LoadButtonTheme(ButtonColors);
   }
   catch(Exception &E)
   {

   }
}

void TZForm::SetGridColors(TTouchGrid *Grid)
{
	Grid->ScrollBtnDown->Color 	= CL_BUTTONCOLOUR_SCROLLBTNS;
	Grid->ScrollBtnUp->Color 	= CL_BUTTONCOLOUR_SCROLLBTNS;
	Grid->ScrollBtnLeft->Color 	= CL_BUTTONCOLOUR_SCROLLBTNS;
	Grid->ScrollBtnRight->Color = CL_BUTTONCOLOUR_SCROLLBTNS;
	Grid->ArrowBorderColor 		= CL_BUTTONCOLOUR_ARROWS;
	Grid->ArrowColor 			= CL_BUTTONCOLOUR_ARROWS;
	Grid->GridColor 			= CL_GRIDCOLOUR;
	Grid->Color		 			= CL_GRIDCOLOUR;
	Grid->Font->Color			= CL_BUTTONCOLOUR_UNSELECTED_FONT_COLOR;
}

void TZForm::SetNumpadColors(TTouchNumpad *Pad)
{
	Pad->ButtonColor					= CL_NUMPADCOLOUR;
}

//---------------------------------------------------------------------------
void __fastcall TZForm::CreateParams(Controls::TCreateParams &params)
{ 
	TForm::CreateParams( params );
	if( WinOwner )
	{
		params.WndParent = WinOwner->Handle;
		WinOwner = NULL;
	}
} 
//---------------------------------------------------------------------------
