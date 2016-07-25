//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DiscountTimes.h"
#include <DateUtils.hpp>
#include "GUIScale.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma link "TouchTime"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TfrmDiscountTimes::TfrmDiscountTimes(TComponent* Owner,Database::TDBControl &inDBControl)
	: TZForm(Owner), DBControl(inDBControl)
{
   DiscountKey = 0;
}

void __fastcall TfrmDiscountTimes::FormShow(TObject *Sender)
{
	FormResize(Sender);
	DrawCalendar();
	UpdateDiscountList();
}

//---------------------------------------------------------------------------

void __fastcall TfrmDiscountTimes::FormResize(TObject *Sender)
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
        /*
        this->Height = Screen->Height;
        GUIScale::ParentHeight(Panel1, 1);
        GUIScale::ParentHeight(Panel28, 0.15);
        GUIScale::ChildrenHeight(Panel28, 0.1);
        GUIScale::ParentHeight(Panel4, 0.4);
        GUIScale::ParentHeight(Panel5, 0.4);
        GUIScale::ParentHeight(Panel7, 0.2);
        //GUIScale::ChildrenPercentageHeight(Panel5, 0.5);

        GUIScale::ParentWidth(Panel2, 0.2);
        GUIScale::ParentWidth(Panel5, 0.3);   */
	Top = 0;
	Left = 0;
	Panel3->Left = Panel1->Left + Panel1->Width;
	SetGridColors(tgYear);
	SetGridColors(tgMonth);
	SetGridColors(tgDay);
	SetGridColors(tgridSelection);
}
//---------------------------------------------------------------------------

void TfrmDiscountTimes::DrawCalendar()
{
   tgYear->RowCount = 0;
	tgYear->ColCount = 10;
   tgYear->RowCount = 1;
   int TheCurrentYear = CurrentYear();
   for (int i = 0; i <= tgYear->ColCount-1 ; i++)
   {
      tgYear->Buttons[0][i]->Caption = IntToStr(TheCurrentYear);
      tgYear->Buttons[0][i]->Tag = TheCurrentYear++;
   }
   tgYear->Buttons[0][0]->Latched = true;

	tgMonth->RowCount = 0;
   tgMonth->ColCount = 12;
   tgMonth->RowCount = 1;
   for (int i = 0; i < 12 ; i++)
   {
      int CurrentMonth = i+1;
      tgMonth->Buttons[0][i]->Tag = CurrentMonth;
      tgMonth->Buttons[0][i]->Caption = ShortMonthNames[i];
      if(tgMonth->Buttons[0][i]->Tag == MonthOfTheYear(Now())) // Latch March.
      {
         tgMonth->Buttons[0][i]->Latched = true;
         tgMonthMouseClick(NULL,TMouseButton() << mbLeft, TShiftState(), tgMonth->Buttons[0][i]);
      }
   }
}


void __fastcall TfrmDiscountTimes::tgMonthMouseClick(TObject *Sender,
      TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
   // Retrive the Year and the Month;
   int Year, Month;
   for (int i = 0; i < tgYear->ColCount ; i++)
   {
      if(tgYear->Buttons[0][i]->Latched)
      {
         Year = tgYear->Buttons[0][i]->Tag;
      }
   }

   for (int i = 0; i < 12 ; i++)
   {
      if(tgMonth->Buttons[0][i]->Latched)
      {
         Month = tgMonth->Buttons[0][i]->Tag;
      }
   }

   // Draw up the Days.
   int TheDaysInAMonth = DaysInAMonth(Year, Month);

	tgDay->RowCount = 0;
   tgDay->ColCount = 7;
	tgDay->RowCount = 5;

   for (int i = 0; i < tgDay->RowCount ; i++)
   {
      for (int j = 0; j < tgDay->ColCount; j++)
      {
         int CurrentDay = (i * tgDay->ColCount )+j +1;
         tgDay->Buttons[i][j]->Caption = IntToStr(CurrentDay);
         tgDay->Buttons[i][j]->Tag = CurrentDay;
			tgDay->Buttons[i][j]->Visible = true;

         if(tgDay->Buttons[i][j]->Tag == DayOfTheMonth(Now()))
         {
            tgDay->Buttons[i][j]->Latched = true;
         }
      }
   }

   for (int k = 0; k < tgDay->RowCount ; k++)
   {
      for (int j = 0; j < tgDay->ColCount; j++)
      {
         if(tgDay->Buttons[k][j]->Tag > TheDaysInAMonth)
         {
             tgDay->Buttons[k][j]->Visible = false;
         }
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscountTimes::TouchGridButtonsMouseClick(TObject *Sender,
      TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
   // Retrive the Year and the Month;
   int Year, Month, Day;
   for (int i = 0; i < tgYear->ColCount ; i++)
   {
      if(tgYear->Buttons[0][i]->Latched)
      {
         Year = tgYear->Buttons[0][i]->Tag;
      }
   }

   for (int i = 0; i < 12 ; i++)
   {
      if(tgMonth->Buttons[0][i]->Latched)
      {
         Month = tgMonth->Buttons[0][i]->Tag;
      }
   }

   for (int i = 0; i < tgDay->RowCount ; i++)
   {
      for (int j = 0; j < tgDay->ColCount; j++)
      {
         if(tgDay->Buttons[i][j]->Latched)
         {
            Day = tgDay->Buttons[i][j]->Tag;
         }
      }
   }


   TDateTime CurrentTime = EncodeDate(Year, Month, Day) +  TouchTime->Time;
   switch(TouchGridButtons->Row(GridButton))
   {
      case 0:
         DiscountTimes.StartTime = CurrentTime;
         GridButton->Caption = "Set Start Time\r" + DiscountTimes.StartTime.DateTimeString();
      break;
      case 1:
         DiscountTimes.EndTime = CurrentTime;
         GridButton->Caption = "Set End Time\r" + DiscountTimes.EndTime.DateTimeString();
      break;
      case 2:
         AddDiscountTime();
      break;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscountTimes::TouchBtn1MouseClick(TObject *Sender)
{
   Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmDiscountTimes::AddDiscountTime()
{
   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();
   DiscountTimes.DiscountKey = DiscountKey;
   ManagerDiscount->AddDiscountTime(DBTransaction,DiscountTimes);
   DBTransaction.Commit();
   UpdateDiscountList();
}
void __fastcall TfrmDiscountTimes::btnDeleteMouseClick(TObject *Sender)
{
   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();
   ManagerDiscount->DeleteDiscountTime(DBTransaction,DiscountTimes);
   DBTransaction.Commit();
   UpdateDiscountList();
}
//---------------------------------------------------------------------------
void __fastcall TfrmDiscountTimes::UpdateDiscountList()
{
   std::vector<TDiscountTimes> DiscountTimes;

   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();
   ManagerDiscount->GetDiscountTimes(DBTransaction,DiscountKey,DiscountTimes);
   TDiscount Discount;
   ManagerDiscount->GetDiscount(DBTransaction,DiscountKey,Discount);
   DBTransaction.Commit();

   tgridSelection->RowCount = 0;
   tgridSelection->ColCount = 1;
   tgridSelection->RowCount = DiscountTimes.size();

   for (int i = 0; i <= tgridSelection->RowCount-1 ; i++)
   {
      TDiscountTimes CurrentDiscountTimes = DiscountTimes[i];
      tgridSelection->Buttons[i][0]->Caption = Discount.Name + " Start : " + CurrentDiscountTimes.StartTime.DateTimeString() + " End : " + CurrentDiscountTimes.EndTime.DateTimeString();
      tgridSelection->Buttons[i][0]->Tag = CurrentDiscountTimes.DiscountTimeKey;
   }
}
void __fastcall TfrmDiscountTimes::tgridSelectionMouseClick(
      TObject *Sender, TMouseButton Button, TShiftState Shift,
      TGridButton *GridButton)
{
   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();
   ManagerDiscount->GetDiscountTime(DBTransaction,GridButton->Tag,DiscountTimes);
   DBTransaction.Commit();
   TouchGridButtons->Buttons[0][0]->Caption = "Set Start Time\r" + DiscountTimes.StartTime.DateTimeString();
   TouchGridButtons->Buttons[1][0]->Caption = "Set End Time\r" + DiscountTimes.EndTime.DateTimeString();
   TouchTime->Time = DiscountTimes.EndTime;
}
//---------------------------------------------------------------------------

