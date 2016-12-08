//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ListManager.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

void TListManagerEvents::Occured(int ItemIndex, int ColIndex = -1)
{
	ErrorMessage = "";
	ErrorTitle = "";
	HaltProceedingEvents = false;
	EventsFailed = false;
	try
	{
      try
      {
         if(Asleep)
			{
            FiredWhileAsleep = true;
         }
         else if(Enabled)
         {  // Prevents recursion of events.
            if(false == RecursionProtected)
            {
               RecursionProtected = true;
					for (unsigned int i = 0; i < Events.size() ; i++)
               {
                  if(!HaltProceedingEvents)
                  {
							Events[i](ItemIndex, ColIndex);
                  }
               }
            }
         }
      }
      __finally
      {
         RecursionProtected = false;
      }
	}
	catch(Exception &E)
	{
		ErrorMessage = E.Message;
      EventsFailed = true;
	}
}

void TListManagerEvents::RegisterForEvent(TMMListManagerEvent Function)
{
	std::vector<TMMListManagerEvent>::iterator itFunction = std::find(Events.begin(), Events.end(), Function);
	if(itFunction == Events.end())
	{
   	Events.push_back(Function);
   }
}

void TListManagerEvents::DeregisterForEvent(TMMListManagerEvent Function)
{
	std::vector<TMMListManagerEvent>::iterator RemoveFunction = std::find(Events.begin(), Events.end(), Function);
	if(RemoveFunction != Events.end())
	{
		Events.erase(RemoveFunction);
	}
}

TListManagerEvents::TListManagerEvents()
{
   Enabled = true;
   Asleep = false;
   FiredWhileAsleep = false;
   RecursionProtected = false;
}

void TListManagerEvents::Enable()
{
   Enabled = true;
}

void TListManagerEvents::Disable()
{
   Enabled = false;
}

int TListManagerEvents::EventCount()
{
   return Events.size();
}

void TListManagerEvents::Sleep()
{
   FiredWhileAsleep = false;
   Asleep = true;
}

void TListManagerEvents::Wakeup()
{
   Asleep = false;
   if(FiredWhileAsleep)
   {
      Occured(-1);
   }
}


//---------------------------------------------------------------------------
__fastcall TfrmListManager::TfrmListManager(TComponent* Owner)
   : TZForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmListManager::FormResize(TObject *Sender)
{
    if (Tag != Screen->Width)
    {
        int Temp = Tag;
        Tag = Screen->Width;
        if(double(Screen->Width) / Screen->Height < 1.4)
        {
           ScaleBy(Screen->Width, Temp);
        }
    }
    Left = (Screen->Width - Width) / 2;
    Top	 = (Screen->Height - Height) / 2;
    pnlMajor->Width = pnlMinor->Left - this->BorderWidth;
}
//---------------------------------------------------------------------------
void __fastcall TfrmListManager::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(this);
}

void __fastcall TfrmListManager::FormShow(TObject *Sender)
{
	FormResize(this);

    if(sgDisplay->ColCount > 1)
    {
        int width = (int)((sgDisplay->Width - 40) /sgDisplay->ColCount);
        for(int i =0 ; i < sgDisplay->ColCount; i++)
        {
            sgDisplay->ColWidths[i] = width;
        }
    }
    else
    {
		sgDisplay->ColWidths[0] = sgDisplay->ClientWidth;
    }
}

void TfrmListManager::DoCustomDrawing()
{
   sgDisplay->DefaultDrawing = false;
}

//---------------------------------------------------------------------------
void __fastcall TfrmListManager::tbtnAddMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   Add.Occured((int)sgDisplay->Objects[0][sgDisplay->Row]);
}
//---------------------------------------------------------------------------

void __fastcall TfrmListManager::tbtnEditMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   Edit.Occured((int)sgDisplay->Objects[0][sgDisplay->Row]);
}
//---------------------------------------------------------------------------

void __fastcall TfrmListManager::tbtnDeleteMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   Delete.Occured((int)sgDisplay->Objects[0][sgDisplay->Row]);
}
//---------------------------------------------------------------------------

void __fastcall TfrmListManager::btnCloseMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	Close.Occured((int)sgDisplay->Objects[0][sgDisplay->Row]);
}
//---------------------------------------------------------------------------
void TfrmListManager::SetCaption(AnsiString inCaption)
{
   pnlLabel->Caption = inCaption;
}

void __fastcall TfrmListManager::btnActiveMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   Active.Occured((int)sgDisplay->Objects[0][sgDisplay->Row]);
}
//---------------------------------------------------------------------------

void __fastcall TfrmListManager::sgDisplaySelectCell(TObject *Sender, int ACol, int ARow,
          bool &CanSelect)
{
	Select.Occured(ARow, ACol);
}
//---------------------------------------------------------------------------

void __fastcall TfrmListManager::sgDisplayDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State)
{
	AState = State;
	DrawCell.Occured(ARow, ACol);
}
//---------------------------------------------------------------------------


