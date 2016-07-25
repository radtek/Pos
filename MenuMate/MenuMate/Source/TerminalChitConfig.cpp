//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TerminalChitConfig.h"
#include "Devices.h"
#include "MMLogging.h"

#define CONTAINER_LIST_DEVICE_COLUMN 0

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TfrmTerminalChitConfig::TfrmTerminalChitConfig(TComponent* Owner,Database::TDBTransaction &inDBTransaction)
	: TZForm(Owner), DBTransaction(inDBTransaction)
{
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void __fastcall TfrmTerminalChitConfig::FormShow(TObject *Sender)
{
    FormResize(Sender);
    UpdateDeviceDisplay();
	UpdateProfileListDisplay();
}
//---------------------------------------------------------------------------

void __fastcall TfrmTerminalChitConfig::tbtnOkMouseClick(TObject *Sender)
{
   SaveDeviceChitProfiles();
   Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmTerminalChitConfig::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
   {
	  int Temp = Tag;
	  Tag = Screen->Width;
          if((double)Screen->Width / Screen->Height < 1.4)
          {
	  	ScaleBy(Screen->Width, Temp);
          }
   }

   Height = Screen->Height;
   Width = Screen->Width;
   pnlButtons->Top = pnl1->Left;
   pnlButtons->Left = ClientWidth - pnlButtons->Width - pnl1->Left;
   pnlButtons->Height = ClientHeight - (pnl1->BorderWidth * 2);

   pnl1->Top = pnl1->Left;
   pnl1->Width = pnlButtons->Left - (pnl1->BorderWidth * 2);
   pnl1->Height = ClientHeight - (pnl1->BorderWidth * 2);

}
//---------------------------------------------------------------------------
void __fastcall TfrmTerminalChitConfig::WMDisplayChange(TWMDisplayChange& Message)
{
   FormResize(this);
}

void TfrmTerminalChitConfig::SaveDeviceChitProfiles()
{
   try
   {
	  if (CurrentDeviceKey != 0)
	  {
		 std::auto_ptr <TManagerDevices> ManagerDevices(new TManagerDevices);

		 for (DeviceChitsList::iterator ptrDevice = DevicesChitProfile.begin();
			ptrDevice != DevicesChitProfile.end(); advance(ptrDevice, 1))
		 {
			ManagerDevices->SetChitProfilesList(DBTransaction, ptrDevice->first, ptrDevice->second);
		 }
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

void __fastcall TfrmTerminalChitConfig::tgridDeviceListMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift,
          TGridButton *GridButton)
{
   try
   {
	  SaveDeviceChitProfiles();
	  CurrentDeviceKey = GridButton->Tag;
	  UpdateProfileListDisplay();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }

}
//---------------------------------------------------------------------------
void TfrmTerminalChitConfig::UpdateDeviceDisplay()
{
   try
   {

	  tgridDeviceList->RowCount = 0;
	  tgridDeviceList->ColCount = 1;

	  std::auto_ptr <TStringList> DeviceList(new TStringList);
	  std::auto_ptr <TManagerDevices> ManagerDevices(new TManagerDevices);
	  ManagerDevices->GetDeviceList(DBTransaction, 1, DeviceList.get());

	  tgridDeviceList->RowCount = DeviceList->Count;

	  for (int i = 0; i < DeviceList->Count; i++)
	  {
		 tgridDeviceList->Buttons[i][CONTAINER_LIST_DEVICE_COLUMN]->Color = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR];
		 tgridDeviceList->Buttons[i][CONTAINER_LIST_DEVICE_COLUMN]->LatchedColor = ButtonColors[BUTTONTYPE_FULL][ATTRIB_BUTTONCOLOR];
		 tgridDeviceList->Buttons[i][CONTAINER_LIST_DEVICE_COLUMN]->FontColor = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_FONTCOLOR];
		 tgridDeviceList->Buttons[i][CONTAINER_LIST_DEVICE_COLUMN]->LatchedFontColor = ButtonColors[BUTTONTYPE_FULL][ATTRIB_FONTCOLOR];
		 tgridDeviceList->Buttons[i][CONTAINER_LIST_DEVICE_COLUMN]->Caption = DeviceList->Strings[i];
		 tgridDeviceList->Buttons[i][CONTAINER_LIST_DEVICE_COLUMN]->Tag = (int)DeviceList->Objects[i];
	  }
	  UpdateProfileListDisplay();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

void TfrmTerminalChitConfig::UpdateProfileListDisplay()
{
   if (CurrentDeviceKey != 0)
   {
	  std::auto_ptr <TManagerDevices> ManagerDevices(new TManagerDevices);
	  ManagerDevices->GetChitProfilesList(DBTransaction, CurrentDeviceKey, DevicesChitProfile[CurrentDeviceKey]);

	  tgridProfileList->RowCount = 0; // Clears all the Latching.
	  tgridProfileList->ColCount = 2;
	  tgridProfileList->RowCount = TManagerChitNumber::Instance().Size();

      int i = 0;
	  for (TManagerChitNumber::Instance().First(false); !TManagerChitNumber::Instance().Eof(); TManagerChitNumber::Instance().Next(false),i++)
	  {
   		 TChitNumber ChitNumber = TManagerChitNumber::Instance().ChitNumber();
		 tgridProfileList->Buttons[i][0]->Caption = ChitNumber.Name;
		 tgridProfileList->Buttons[i][0]->Tag = ChitNumber.ChitNumberKey;

		 tgridProfileList->Buttons[i][0]->Color = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR];
		 tgridProfileList->Buttons[i][0]->LatchedColor = ButtonColors[BUTTONTYPE_FULL][ATTRIB_BUTTONCOLOR];
		 tgridProfileList->Buttons[i][0]->FontColor = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_FONTCOLOR];
		 tgridProfileList->Buttons[i][0]->LatchedFontColor = ButtonColors[BUTTONTYPE_FULL][ATTRIB_FONTCOLOR];

		 tgridProfileList->Buttons[i][1]->Caption = "Default";
		 tgridProfileList->Buttons[i][1]->Tag = ChitNumber.ChitNumberKey;

		 tgridProfileList->Buttons[i][1]->Color = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR];
		 tgridProfileList->Buttons[i][1]->LatchedColor = ButtonColors[BUTTONTYPE_FULL][ATTRIB_BUTTONCOLOR];
		 tgridProfileList->Buttons[i][1]->FontColor = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_FONTCOLOR];
		 tgridProfileList->Buttons[i][1]->LatchedFontColor = ButtonColors[BUTTONTYPE_FULL][ATTRIB_FONTCOLOR];

		 if (DevicesChitProfile[CurrentDeviceKey].find(tgridProfileList->Buttons[i][0]->Tag) == DevicesChitProfile[CurrentDeviceKey].end())
		 { // Not Found.
			tgridProfileList->Buttons[i][0]->Latched = false;
		 }
		 else
		 {
			tgridProfileList->Buttons[i][0]->Latched = true;
            typChitDefaultMap ChitDefaultMap = DevicesChitProfile[CurrentDeviceKey];
            if (ChitDefaultMap[tgridProfileList->Buttons[i][0]->Tag] == true)
            { // Not Found.
               tgridProfileList->Buttons[i][1]->Latched = true;
            }
            else
            {
               tgridProfileList->Buttons[i][1]->Latched = false;
            }

		 }
	  }
   }
   else // Clear Display.
   {
	  tgridProfileList->RowCount = 0;
   }
}
void __fastcall TfrmTerminalChitConfig::tgridProfileListMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift,
          TGridButton *GridButton)
{
   try
   {
	  if (CurrentDeviceKey != 0)
	  {
		 DevicesChitProfile[CurrentDeviceKey].clear();

         if (tgridProfileList->Col(GridButton) == 1)
         { // Hit the default button
	         if(GridButton->Latched)
             {  // If we Latched a default latch it's Chit.
             	int Row = tgridProfileList->Row(GridButton);
               	tgridProfileList->Buttons[Row][0]->Latched = GridButton->Latched;
                // Unlatch all other defaults.
               	for (int i = 0; i < tgridProfileList->RowCount; i++)
               	{
                	if(i != Row)
                    {
                    	tgridProfileList->Buttons[i][1]->Latched = false;
                    }
               	}
             }
         }

         if (tgridProfileList->Col(GridButton) == 0)
         { // Hit the Main Button button
         	int Row = tgridProfileList->Row(GridButton);
            if(!GridButton->Latched)
            { // If it's being unlatched then it cant be the default.
            	tgridProfileList->Buttons[Row][1]->Latched = GridButton->Latched;
            }
         }


         typChitDefaultMap ChitDefaultMap = DevicesChitProfile[CurrentDeviceKey];
		 for (int i = 0; i < tgridProfileList->RowCount; i++)
		 {
			if (tgridProfileList->Buttons[i][0]->Latched)
			{
               ChitDefaultMap[tgridProfileList->Buttons[i][0]->Tag] = tgridProfileList->Buttons[i][1]->Latched;
			}
		 }
         DevicesChitProfile[CurrentDeviceKey] = ChitDefaultMap;
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}
//---------------------------------------------------------------------------

