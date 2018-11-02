// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DeviceRealTerminal.h"
#include "Message.h"
#include "MMLogging.h"
#include "MMTouchKeyboard.h"
#include "ManagerDiscount.h"
#include "RunManager.h"
#include "MessageManager.h"
#include "TableManager.h"
#include "ListPaymentSystem.h"
#include "DBHappyHour.h"
#include "ManagerHappyHour.h"
#include "GlobalSettings.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

#define NUMBER_OF_MSG_IN_VIEW 8

// ---------------------------------------------------------------------------
__fastcall TfrmMessage::TfrmMessage(TComponent* Owner, Database::TDBControl &inDBControl) : TZForm(Owner), DBControl(inDBControl),
TextResults(new TStringList), allow_combo(false)
{
	ShowPointsAsDiscount = true;
}

// ---------------------------------------------------------------------------

void __fastcall TfrmMessage::FormShow(TObject *Sender)
{
   FormResize(NULL);

   int ButtonWidth;
   TextResult = "";
   Key = 0;
   int x = pnlCourses->Left;
   int y = pnlCourses->Left;
   const int PixelGap = pnlCourses->Left;
   ButtonWidth = pnlCourses->Width - (pnlCourses->Left * 2);

   TMessageBtn *CloseButton = new TMessageBtn(this);
   CloseButton->Parent = this->sbMessages;
   CloseButton->Visible = true;
   CloseButton->Font->Color = clWhite;
   CloseButton->Title = "Cancel";
   CloseButton->Font->Name = "Tahoma";
   CloseButton->Font->Size = 14;
   CloseButton->Font->Style = TFontStyles() << fsBold;
   CloseButton->Caption = CloseButton->Title;
   CloseButton->TextResult = "";
   CloseButton->Left = x;
   CloseButton->Top = y;
   CloseButton->Height = sbMessages->ClientHeight / NUMBER_OF_MSG_IN_VIEW;
   CloseButton->Width = ButtonWidth;
   CloseButton->ButtonColor = 0x000098F5;
   CloseButton->OnMouseClick = BtnCloseClick;

   y += CloseButton->Height + PixelGap;

   if (MessageType != eHHProfile && MessageType != eCashDrawer)
   {
       TMessageBtn *TopButton = new TMessageBtn(this);
       TopButton->Parent = this->sbMessages;
       TopButton->Font->Color = clWhite;

       if (MessageType == ePaymentTypeDisplay || MessageType == eDiscountReason)
       {
          TopButton->Title = "Clear";
          TopButton->Tag = -1;
          TopButton->ShowKeyboard = false;
          TopButton->BtnType = eSelectionComplete;
          TopButton->Visible = true;
       }
       else if (MessageType == eRunProgram)
       {
          TopButton->Title = "Run...";
          TopButton->ShowKeyboard = true;
          TopButton->Tag = -1;
          TopButton->Visible = true;
          TopButton->BtnType = eSelectionComplete;
       }
       else if (MessageType == eCallAway)
       {
          TopButton->Title = "Send...";
          TopButton->ShowKeyboard = false;
          TopButton->Tag = -1;
          TopButton->BtnType = eSelectionComplete;
          TopButton->Visible = true;
       }
       else if (MessageType == eCancelReason)
       {
          TopButton->Title = "Custom Cancel";
          TopButton->Tag = -1;
          TopButton->ShowKeyboard = true;
          TopButton->BtnType = eSelectionComplete;
          TopButton->Visible = true;
       }
       else if (MessageType == eCustomerTypes)
       {
          TopButton->Title = "Custom Order Type";
          TopButton->Tag = -1;
          TopButton->ShowKeyboard = true;
          TopButton->BtnType = eSelectionComplete;
          TopButton->Visible = true;
       }
       else
       {
          TopButton->Title = "Notes :";
          TopButton->ShowKeyboard = true;
          TopButton->Tag = -1;
          TopButton->BtnType = eSelectionComplete;
          TopButton->Visible = true;
       }
       TopButton->Caption = TopButton->Title;
       TopButton->TextResult = "";
       TopButton->ButtonColor = clTeal;
       TopButton->Left = x;
       TopButton->Top = y;
       TopButton->Height = sbMessages->ClientHeight / NUMBER_OF_MSG_IN_VIEW;
       TopButton->Width = ButtonWidth;
       TopButton->OnMouseClick = BtnMesasgeClick;

       if (TopButton->Visible)
       {
          y += TopButton->Height + PixelGap;
       }

    }

   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

          if(MessageType == ePaymentTypeDisplay)
          {
            DisplayPaymentList(DBTransaction,x,y,ButtonWidth,PixelGap);
          }
          else if (MessageType == eDiscountReason)
	      {
             TStringList *List = new TStringList;
             try
             {

              std::vector<eDiscountFilter> discountFilter;
              if(allow_combo)
                 discountFilter.push_back(only_combos);
              else
                 discountFilter.push_back(exclude_combos);

            ManagerDiscount->GetDiscountList(DBTransaction,List,discountFilter,ShowPointsAsDiscount);

			for (int i = 0; i < List->Count; i++)
			{
			   TMessageBtn *NewButton = new TMessageBtn(this);
			   NewButton->Parent = sbMessages;
			   NewButton->Visible = true;
			   NewButton->Font->Color = clWhite;
			   NewButton->Title = List->Strings[i];
			   NewButton->Tag = (int)List->Objects[i];
			   NewButton->TextResult = ManagerDiscount->GetDiscountDescription(DBTransaction, NewButton->Tag);
			   NewButton->ButtonColor = clNavy;
			   NewButton->Caption = NewButton->Title;
			   NewButton->Font->Name = "Tahoma";
			   NewButton->Font->Size = 14;
			   NewButton->Font->Style = TFontStyles() << fsBold;

			   NewButton->Left = x;
			   NewButton->Top = y;
			   NewButton->Height = sbMessages->ClientHeight / NUMBER_OF_MSG_IN_VIEW;
			   NewButton->Width = ButtonWidth;
			   NewButton->OnMouseClick = BtnMesasgeClick;
			   y += NewButton->Height + PixelGap;
			}
		 }
		 __finally
		 {
			delete List;
		 }
	  }
	  else if (MessageType == eRunProgram)
	  {
		 TStringList *List = new TStringList;
		 try
		 {
			ManagerRun->GetListTitle(DBTransaction, List, 0);
			for (int i = 1; i < List->Count; i++)
			{
			   TMessageBtn *NewButton = new TMessageBtn(this);
			   NewButton->Parent = this->sbMessages;
			   NewButton->Visible = true;
			   NewButton->Font->Color = clWhite;
			   NewButton->Title = List->Strings[i];
			   NewButton->Tag = (int)List->Objects[i];
			   NewButton->TextResult = ManagerRun->GetContent(DBTransaction, NewButton->Tag);
			   NewButton->ButtonColor = clNavy;
			   NewButton->Caption = NewButton->Title;
			   NewButton->Font->Name = "Tahoma";
			   NewButton->Font->Size = 14;
			   NewButton->Font->Style = TFontStyles() << fsBold;

			   NewButton->Left = x;
			   NewButton->Top = y;
			   NewButton->Height = 65;
			   NewButton->Width = ButtonWidth;
			   NewButton->OnMouseClick = BtnMesasgeClick;

			   y += NewButton->Height + PixelGap;
			}
		 }
		 __finally
		 {
			delete List;
		 }
	  }
	  else if (MessageType == eCallAway && TDeviceRealTerminal::Instance().Menus->VisibleMenu != NULL)
	  {

		 std::vector <TServingCourse> ::iterator itServingCourse;

		 for (itServingCourse = TDeviceRealTerminal::Instance().Menus->VisibleMenu->ServingCourses.begin();
			itServingCourse != TDeviceRealTerminal::Instance().Menus->VisibleMenu->ServingCourses.end(); itServingCourse++)
		 {
			TMessageBtn *NewButton = new TMessageBtn(this);

			NewButton->Parent = this->sbMessages;
			NewButton->Visible = true;
			NewButton->Font->Color = clWhite;
			NewButton->Title = itServingCourse->Name;
			NewButton->Tag = eMultiSelect;
			NewButton->TextResult = itServingCourse->KitchenName;
			NewButton->ButtonColor = clNavy;
			NewButton->Caption = NewButton->Title;
			NewButton->Font->Name = "Tahoma";
			NewButton->Font->Size = 14;
			NewButton->Font->Style = TFontStyles() << fsBold;

			NewButton->Left = x;
			NewButton->Top = y;
			NewButton->Height = 65;
			NewButton->Width = ButtonWidth;
			NewButton->OnMouseClick = BtnMesasgeClick;
			NewButton->BtnType = eMultiSelect;

			y += NewButton->Height + PixelGap;
		 }
	  }
      else if(MessageType == eHHProfile)
      {
           HHProfileList(DBTransaction,x,y,ButtonWidth,PixelGap);
      }
	  else
	  {
		 TStringList *List = new TStringList;
		 try
		 {
			ManagerMessage->GetListTitle(DBTransaction, List, MessageType);
			for (int i = 1; i < List->Count; i++)
			{
			   TMessageBtn *NewButton = new TMessageBtn(this);
			   NewButton->Parent = this->sbMessages;
			   NewButton->Visible = true;
			   NewButton->Font->Color = clWhite;
			   NewButton->Title = List->Strings[i];
			   NewButton->Tag = (int)List->Objects[i];
			   NewButton->TextResult = ManagerMessage->GetContent(DBTransaction, NewButton->Tag);
			   NewButton->ButtonColor = clNavy;
			   NewButton->Font->Name = "Tahoma";
			   NewButton->Font->Size = 14;
			   NewButton->Font->Style = TFontStyles() << fsBold;
			   NewButton->Caption = NewButton->Title;
			   NewButton->Left = x;
			   NewButton->Top = y;
			   NewButton->Height = 65;
			   NewButton->Width = ButtonWidth;
			   NewButton->OnMouseClick = BtnMesasgeClick;
			   y += NewButton->Height + PixelGap;
			}
		 }
		 __finally
		 {
			delete List;
		 }
	  }
	  DBTransaction.Commit();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
   }
}


void  TfrmMessage::DisplayPaymentList(Database::TDBTransaction &DBTransaction,
                                      int cordX,int cordY,int ButtonWidth,int PixelGap)
{
    TListPaymentSystem *PaymentSystem;
    std::auto_ptr <TStringList> PaymentList(new TStringList);
    PaymentSystem->PaymentLoad(DBTransaction, PaymentList.get());
    for (int i = 1; i < PaymentList->Count; i++)
	{
           if(PaymentList->Strings[i] != "Voucher" && PaymentList->Strings[i] != "Gift Card")
           {
               TMessageBtn *NewButton = new TMessageBtn(this);
               NewButton->Parent = this->sbMessages;
               NewButton->Visible = true;
               NewButton->Font->Color = clWhite;
               NewButton->Title = PaymentList->Strings[i];
               NewButton->Tag = (int)PaymentList->Objects[i];
               NewButton->TextResult = PaymentList->Strings[i];//ManagerMessage->GetContent(DBTransaction, NewButton->Tag);
               NewButton->ButtonColor = clNavy;
               NewButton->Font->Name = "Tahoma";
               NewButton->Font->Size = 14;
               NewButton->Font->Style = TFontStyles() << fsBold;
               NewButton->Caption = NewButton->Title;
               NewButton->Left = cordX;
               NewButton->Top = cordY;
               NewButton->Height = 65;
               NewButton->Width = ButtonWidth;
               NewButton->OnMouseClick = BtnMesasgeClick;
               cordY += NewButton->Height + PixelGap;
           }
        }
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMessage::BtnMesasgeClick(TObject *Sender)
{
   TMessageBtn *Button = (TMessageBtn*)Sender;
   std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
   frmTouchKeyboard->MaxLength = 200;
   frmTouchKeyboard->AllowCarriageReturn = false;
   frmTouchKeyboard->StartWithShiftDown = true;
   frmTouchKeyboard->KeyboardText = Button->TextResult;

   TextSelected = Button->Title;

   if (MessageType == eDiscountReason)
   {
	  frmTouchKeyboard->Caption = Button->Title + " Enter Discount Reason";
   }
   if (MessageType == eRunProgram)
   {
	  frmTouchKeyboard->Caption = Button->Title + " Enter Command Line to run or Hit Ok";
   }
   else if (MessageType == eCallAway)
   {
	  frmTouchKeyboard->Caption = Button->Title + " Enter Call Away Course";
   }
   else
   {
	  frmTouchKeyboard->Caption = Button->Title + " Enter Message";
   }

   bool Proceed = true;
   if (Button->ShowKeyboard)
   {
	  if (frmTouchKeyboard->ShowModal() == mrOk)
	  {
		 TextResult = frmTouchKeyboard->KeyboardText;
	  }
	  else
	  {
		 ModalResult = mrCancel;
		 Proceed = false;
	  }
   }



   if (Proceed)
   {
     if ( MessageType != eHHProfile)
     {
              if (Button->BtnType == eSelectionComplete)
              {
                 ModalResult = mrOk;
              }
              else if (Button->BtnType == eMultiSelect)
              {
                 if (Button->Selected)
                 {
                    Button->Selected = false;
                    Button->ButtonColor = clMaroon;
                    int Index = TextResults->IndexOf(Button->TextResult);
                    if (Index != -1)
                    {
                          TextResults->Delete(Index);
                    }
                 }
                 else
                 {
                    Button->Selected = true;
                    Button->ButtonColor = clGreen;
                    TextResults->Add(Button->TextResult);
                 }
              }
              else
              {
                 TextResult = Button->TextResult;
                 ModalResult = mrOk;
              }

     }
	else
     {

          ModalResult = mrOk;
     }
	  Key = Button->Tag;
   }
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMessage::WMDisplayChange(TWMDisplayChange& Message)
{
   FormResize(this);
}

void __fastcall TfrmMessage::BtnCloseClick(TObject *Sender)
{
   ModalResult = mrCancel;
   // Close();
}

__fastcall TMessageBtn::TMessageBtn(Classes::TComponent* AOwner) : TTouchBtn(AOwner) // TCustomStaticText(AOwner)
{
   TextResult = "";
   Title = "";
   Font->Color = clBlack;
   Font->Name = "Tahoma";
   Font->Style = TFontStyles() << fsBold;
   Font->Size = 14;
   BevelInner = bvNone;
   Selected = false;
   ShowKeyboard = false;
   BtnType = eBtnNormal;
}

void __fastcall TfrmMessage::FormResize(TObject *Sender)
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
   Left = (Screen->Width - Width) / 2;
   Top = (Screen->Height - Height) / 2;
   pnlCourses->Width = ClientWidth - pnlCourses->Left - pnlCourses->Left;
   pnlCourses->Height = ClientHeight - pnlCourses->Left - pnlCourses->Left;
   btnMsgUp->Top = pnlCourses->Height - btnMsgUp->Height - pnlCourses->Left;
   btnMsgDown->Top = pnlCourses->Height - btnMsgDown->Height - pnlCourses->Left;
   btnMsgUp->Left = pnlCourses->Left;
   btnMsgDown->Left = pnlCourses->Width - btnMsgDown->Width - pnlCourses->Left;
   sbMessages->Height = btnMsgDown->Top - pnlCourses->Left;
}
// ---------------------------------------------------------------------------

void __fastcall TfrmMessage::FormClose(TObject *Sender, TCloseAction &Action)
{
   while (sbMessages->ControlCount > 0)
   {
	  delete sbMessages->Controls[0];
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmMessage::pnlCoursesResize(TObject *Sender)
{
   sbMessages->Align = alClient;
   sbMessages->Align = alNone;
   sbMessages->Width += 20; // sbCourses->VertScrollBar->Size;
   sbMessages->Height = btnMsgUp->Top - pnlCourses->Left; // sbCourses->VertScrollBar->Size;
}
// ---------------------------------------------------------------------------

void __fastcall TfrmMessage::btnMsgDownMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
   int StartScrollPos = sbMessages->VertScrollBar->Position;
   int ButtonHeight = sbMessages->ClientHeight / NUMBER_OF_MSG_IN_VIEW;
   int EndScrollPos = StartScrollPos + sbMessages->ClientHeight - ButtonHeight;
   if (EndScrollPos > sbMessages->VertScrollBar->Range)
	  EndScrollPos = sbMessages->VertScrollBar->Range;

   for (int i = StartScrollPos; i <= EndScrollPos; i += 8)
   {
	  sbMessages->VertScrollBar->Position = i;
	  sbMessages->Update();
   }
   sbMessages->VertScrollBar->Position = EndScrollPos;
}
// ---------------------------------------------------------------------------

void __fastcall TfrmMessage::btnMsgUpMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
   int StartScrollPos = sbMessages->VertScrollBar->Position;
   int ButtonHeight = sbMessages->ClientHeight / NUMBER_OF_MSG_IN_VIEW;
   int EndScrollPos = StartScrollPos - sbMessages->ClientHeight + ButtonHeight;
   if (EndScrollPos < 0)
	  EndScrollPos = 0;

   for (int i = StartScrollPos; i >= EndScrollPos; i -= 8)
   {
	  sbMessages->VertScrollBar->Position = i;
	  sbMessages->Update();
   }
   sbMessages->VertScrollBar->Position = EndScrollPos;
}
// ---------------------------------------------------------------------------


void TfrmMessage::HHProfileList(Database::TDBTransaction &DBTransaction,int cordX,int cordY,int ButtonWidth,int PixelGap)
{
     std::vector<THappyHour> HappyHourProfiles;
      //get the available profiles  in list
    TManagerHappyHour *loadProfiles= new TManagerHappyHour();
    HappyHourProfiles = loadProfiles->LoadHappyHourProfiles(false);

   for (int profile = 0; profile < HappyHourProfiles.size(); profile++)
      	{
           TMessageBtn *NewButton = new TMessageBtn(this);
           NewButton->Parent = this->sbMessages;
           NewButton->Visible = true;
           NewButton->Font->Color = clWhite;
           NewButton->Title =HappyHourProfiles[profile].ProfileName;
           NewButton->Tag = HappyHourProfiles[profile].ProfileKey;
           NewButton->TextResult = HappyHourProfiles[profile].ProfileName;//ManagerMessage->GetContent(DBTransaction, NewButton->Tag);
           NewButton->ButtonColor = clNavy;
           NewButton->Font->Name = "Tahoma";
           NewButton->Font->Size = 14;
           NewButton->Font->Style = TFontStyles() << fsBold;
           NewButton->Caption = NewButton->Title;
           NewButton->Left = cordX;
           NewButton->Top = cordY;
           NewButton->Height = 65;
           NewButton->Width = ButtonWidth;
           NewButton->OnMouseClick = BtnMesasgeClick;
           cordY += NewButton->Height + PixelGap;
        }
}


