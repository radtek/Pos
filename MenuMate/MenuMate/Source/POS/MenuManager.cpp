//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <Filectrl.hpp>
#include "MenuManager.h"
#include "SelectActiveMenus.h"
#include "Processing.h"
#include "Secure.h"
#include "MMMessageBox.h"
#include "ContactStaff.h"
#include "NetMessageMenuAltered.h"
#include "ManagerNet.h"
#include "MMLogging.h"
#include "ListCourse.h"
#include "DBGroups.h"

#include "MenuManagerItemSizeSelection.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma resource "*.dfm"

TForm * TfrmMenuManager::WinOwner = NULL;
//---------------------------------------------------------------------------
__fastcall TfrmMenuManager::TfrmMenuManager(TComponent* Owner,TManagerMenusPOS *inManagerMenus,Database::TDBControl &inIBDatabase)
	: TForm(Owner), IBDatabase(inIBDatabase), CL_UNSELECTED_COURSE(RGB(128, 129, 129)), CL_SELECTED_COURSE(RGB(0, 157, 191))
{
	CurrentCourseBtn = NULL;
	ManagerMenus = inManagerMenus;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TfrmMenuManager::CreateParams(Controls::TCreateParams &params)
{
	TForm::CreateParams( params );
	if( WinOwner )
	{
		params.WndParent = WinOwner->Handle;
		WinOwner = NULL;
	}
}
//---------------------------------------------------------------------------
TfrmMenuManager *TfrmMenuManager::Create(TForm* Owner,TManagerMenusPOS *inManagerMenus,Database::TDBControl &inIBDatabase)
{
	WinOwner = Owner;
	return new TfrmMenuManager(Owner,inManagerMenus,inIBDatabase);
}

void __fastcall TfrmMenuManager::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
   {
		int Temp = Tag;
		Tag = Screen->Width;
   	ScaleBy(Screen->Width, Temp);
      Realign();
   }
   btnCourseUp->Top = pnlCourses->ClientHeight - btnCourseUp->Height - btnDrinksDishes->Top;
   btnCourseDown->Top = btnCourseUp->Top;
   sbCourses->Top = pnlCourses->Left;
   sbCourses->Height = btnCourseUp->Top - (pnlCourses->Left*2);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuManager::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(this);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuManager::FormShow(TObject *Sender)
{
	if (CurrentCourseBtn)
    {
        CurrentCourseBtn->Color = CL_UNSELECTED_COURSE;
    }
	FormResize(this);
	if(ManagerMenus->VisibleMenu == NULL)
    {
	    if(ManagerMenus->Current->MenuGet(0) != NULL)
        {
            ManagerMenus->VisibleMenu = ManagerMenus->Current->MenuGet(0);
        }
    }

	menuItemGrid->Font->Size = static_cast<int>(TGlobalSettings::Instance().posButtonFontSize & ~0x80);
	menuItemGrid->ColCount = TGlobalSettings::Instance().DefaultColumns;

    RedrawMenus();
    MenuEdited = false;
}

void __fastcall TfrmMenuManager::CheckBoxClick(TObject *Sender)
{
    using item_management::TfrmItemSizeSelection;

    TGridButton &buttonClicked = *reinterpret_cast<TGridButton *>(Sender);

    std::auto_ptr<TfrmItemSizeSelection> selectSizeToEditDialog( new TfrmItemSizeSelection(this, buttonClicked.Caption, buttonClicked.Tag, buttonClicked.Latched));

    if(selectSizeToEditDialog->ShowModal() == mrOk)
    {
        buttonClicked.Latched = selectSizeToEditDialog->cbItemAvailable->Checked;
        MenuEdited = true;
    }
}

void TfrmMenuManager::RefreshCourseButtons()
{
    int Count = 0;
    sbCourses->VertScrollBar->Range = 0;
    sbCourses->VertScrollBar->Position = 0;
    sbCourses->Update();

    if(TDeviceRealTerminal::Instance().Menus->VisibleMenu != NULL)
    {
        for (int i=0; i < sbCourses->ControlCount; i++)
        {
            TTouchBtn *CourseButton = (TTouchBtn *)sbCourses->Controls[i];
            CourseButton->Visible = false;
        }

        btnDrinksDishes->Caption = MenuName;
        for (int j = 0; j < TDeviceRealTerminal::Instance().Menus->VisibleMenu->Count; j++)
        {
            TListCourse *Course = TDeviceRealTerminal::Instance().Menus->VisibleMenu->CourseGet(j);
            if(sbCourses->ControlCount > Count)
            {
                TTouchBtn *CourseButton = (TTouchBtn *)sbCourses->Controls[Count];
                CourseButton->Height = 65;
                CourseButton->Top = ((Count * 8) + (Count * CourseButton->Height));
                CourseButton->Left = TouchButton->Left;
                CourseButton->Width = pnlCourses->Width - (TouchButton->Left*2);
                CourseButton->Height = 65;
                CourseButton->OnMouseDown = TouchButtonMouseDown;
                CourseButton->Visible = true;
                CourseButton->ButtonColor  = CL_UNSELECTED_COURSE;
                CourseButton->LatchedColor = CL_SELECTED_COURSE;
                CourseButton->Color = sbCourses->Color;
                CourseButton->Font = TouchButton->Font;
                CourseButton->Caption = Course->Course_Name;
                CourseButton->Tag = Course->Course_Key;
                CourseButton->Font->Color = clWhite;

                sbCourses->VertScrollBar->Range = CourseButton->Top + CourseButton->Height;
                Count++;
            }
            else // New Button required.
            {
                TTouchBtn *CourseButton =  new TTouchBtn(this);
                CourseButton->Parent = sbCourses;
                CourseButton->Height = 65;
                CourseButton->Top = ((Count * 8) + (Count * CourseButton->Height));
                CourseButton->Left = TouchButton->Left;
                CourseButton->Width = pnlCourses->Width - (TouchButton->Left*2);
                CourseButton->Height = 65;
                CourseButton->OnMouseDown = TouchButtonMouseDown;
                CourseButton->Visible = true;
                CourseButton->ButtonColor  = CL_UNSELECTED_COURSE;
                CourseButton->LatchedColor = CL_SELECTED_COURSE;
                CourseButton->LatchingProperties = TouchButton->LatchingProperties;
                CourseButton->LatchingType = TouchButton->LatchingType;
                CourseButton->Color = sbCourses->Color;
                CourseButton->Font = TouchButton->Font;
                CourseButton->Name = "TouchButton" + IntToStr(Count);
                CourseButton->Caption = Course->Course_Name;
                CourseButton->Tag = Course->Course_Key;
                CourseButton->Visible = true;
                CourseButton->Font->Color = clWhite;

                sbCourses->VertScrollBar->Range = CourseButton->Top + CourseButton->Height;
                Count++;
            }
        }
    }

    CurrentCourseBtn = TouchButton;
    CurrentCourseBtn->ButtonColor = CL_UNSELECTED_COURSE;
    if (CurrentCourseBtn->Visible)
    {
        TouchButtonMouseDown(CurrentCourseBtn, mbLeft, TShiftState(), 0, 0);
    }
    else
    {
        ShowItems();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuManager::TouchButtonMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if (CurrentCourseBtn)
    {
        //CurrentCourseBtn->ButtonColor = CL_UNSELECTED_COURSE;
        CurrentCourseBtn->Latched = false;
    }
    CurrentCourseBtn = (TTouchBtn *)Sender;
    //CurrentCourseBtn->ButtonColor = CL_SELECTED_COURSE;
    CurrentCourseBtn->Latched = true;
    if (CurrentCourseBtn)
    {
        if (CurrentCourseBtn->Top + CurrentCourseBtn->Height >= sbCourses->Height)
        {
            btnCourseDownMouseDown(NULL,mbLeft, TShiftState(),0,0);
        }
        else if (CurrentCourseBtn->Top < 0)
        {
            btnCourseUpMouseDown(NULL,mbLeft, TShiftState(),0,0);
        }
    }
    ShowItems();
}
//---------------------------------------------------------------------------
void TfrmMenuManager::ShowItems()
{
	TDeviceRealTerminal &ref_dtrm = TDeviceRealTerminal::Instance();
	TListCourse *course;
	TItem *item;
	int i = 1, j = 0, k = 0;

   if (!ref_dtrm.Menus->VisibleMenu || !ref_dtrm.Menus->VisibleMenu->Count)
		return;

	course = ref_dtrm.Menus->VisibleMenu->CourseGet(0);
	while (i < ref_dtrm.Menus->VisibleMenu->Count
	       && course->Course_Key != CurrentCourseBtn->Tag)
		course = ref_dtrm.Menus->VisibleMenu->CourseGet(i++);

	menuItemGrid->RowCount = course->Count / menuItemGrid->ColCount
	                         + (course->Count % menuItemGrid->ColCount != 0);

	for (i = j = 0; k < course->Count; k = i + j * menuItemGrid->ColCount) {
		menuItemGrid->Buttons[j][i]->Caption = (item = course->ItemGet(k))->Item;
		menuItemGrid->Buttons[j][i]->Latched = item->Enabled;
		menuItemGrid->Buttons[j][i]->Tag = item->ItemKey;
		menuItemGrid->Buttons[j][i]->Visible = true;
		j += (++i %= menuItemGrid->ColCount) == 0;
	}

	for ( ; j < menuItemGrid->RowCount; j += (i %= menuItemGrid->ColCount) == 0)
		menuItemGrid->Buttons[j][i++]->Visible = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuManager::btnCloseClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMenuManager::sbDishesClick(TObject *Sender)
{

	ShowItems();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMenuManager::btnTimerTimer(TObject *Sender)
{
	btnDrinksDishes->Tag = 1;
	btnTimer->Enabled = false;

	Database::TDBTransaction DBTransaction(IBDatabase);
	DBTransaction.StartTransaction();

	if (!ManagerMenus->GetMenusExist(DBTransaction))
	{
		MessageBox("There are no menus to change to. Add One to the Database.", "Error", MB_OK + MB_ICONERROR);
		return;
	}

	TDeviceRealTerminal::Instance().Menus->SwapInNewMenus();
	std::auto_ptr<TfrmSelectActiveMenus> frmSelectActiveMenus(TfrmSelectActiveMenus::Create(this,ManagerMenus,DBTransaction));
	if (frmSelectActiveMenus->ShowModal() == mrOk)
	{
		bool Broadcast = false;
		if(MessageBox("Do you wish all terminals to use this menu configuration?",
				"Update all terminals",
				MB_YESNO + MB_ICONQUESTION) == IDYES)
		{
			TMMContactInfo TempUserInfo;
			std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
			TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckDisable);
			if (Result == lsAccepted)
			{
				Broadcast = true;
			}
			else if (Result == lsDenied)
			{
				MessageBox("You do not have access to change Menus system wide.", "Error", MB_OK + MB_ICONERROR);
			}
			else if (Result == lsPINIncorrect)
			{
				MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
			}
		}

		std::auto_ptr<TNetMessageMenuChanged> dbRequest(new TNetMessageMenuChanged);
		for(int i=0; i<frmSelectActiveMenus->pnlMenus->ControlCount; i ++)
		{
			if (((TTouchBtn *)frmSelectActiveMenus->pnlMenus->Controls[i])->Color == clGreen)
			{
				dbRequest->Menu_Names[(((TTouchBtn *)frmSelectActiveMenus->pnlMenus->Controls[i])->Caption)] = eMenuAddReplace;
			}
		}
		dbRequest->Broadcast = Broadcast;
		TDeviceRealTerminal::Instance().Menus->MenuChanged(DBTransaction,dbRequest.get());
		TDeviceRealTerminal::Instance().Menus->SwapInNewMenus();
	}
	DBTransaction.Commit();
	RedrawMenus(); // Redraw Menus.
}
//---------------------------------------------------------------------------

void __fastcall TfrmMenuManager::btnDrinksDishesMouseDown(TObject *Sender,
		TMouseButton Button, TShiftState Shift, int X, int Y)
{
	btnDrinksDishes->Tag = 0;
	btnTimer->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMenuManager::btnDrinksDishesMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   btnTimer->Enabled = false;
   if(btnDrinksDishes->Tag == 0) // Toggle
   {
      TDeviceRealTerminal::Instance().Menus->ToggleMenus();
   }
   RedrawMenus(); // Redraw Menus.
}
//---------------------------------------------------------------------------



void TfrmMenuManager::RedrawMenus()
{
	try
	{
		TDeviceRealTerminal::Instance().Menus->SwapInNewMenus();
		if(TDeviceRealTerminal::Instance().Menus->VisibleMenu == NULL)
		{
			TListMenu *CurrentMenu = TDeviceRealTerminal::Instance().Menus->Current->MenuGet(0);
			if(CurrentMenu != NULL)
			{
				TDeviceRealTerminal::Instance().Menus->VisibleMenu = CurrentMenu;
			}
		}

		if(TDeviceRealTerminal::Instance().Menus->VisibleMenu != NULL)
		{
			Variant locvalues = ShowingMenu;
			lbeMenuName->Caption = "MENU : " + TDeviceRealTerminal::Instance().Menus->VisibleMenu->MenuName;
			MenuName = TDeviceRealTerminal::Instance().Menus->VisibleMenu->MenuName;
			btnDrinksDishes->Caption = MenuName;
			if (CurrentCourseBtn)
			{
				CurrentCourseBtn->Color = clMaroon;
			}
			CurrentCourseBtn = TouchButton;
			RefreshCourseButtons();
		}
		else
		{
			Database::TDBTransaction DBTransaction(IBDatabase);
			DBTransaction.StartTransaction();
			if (!ManagerMenus->GetMenusExist(DBTransaction))
			{
				btnDrinksDishes->Caption = " No Menu";
				lbeMenuName->Caption = "No Menus Loaded.";
				return;
			}
			else
			{
				btnDrinksDishes->Caption = " No Menu";
				lbeMenuName->Caption = "No Menus Selected.";
			}
			DBTransaction.Commit();			
			RefreshCourseButtons();
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}


void __fastcall TfrmMenuManager::btnRemoveMenuClick(TObject *Sender)
{
	Database::TDBTransaction DBTransaction(IBDatabase);
	DBTransaction.StartTransaction();
	std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));

	TMMContactInfo TempUserInfo;
	TempUserInfo = TDeviceRealTerminal::Instance().User;
	bool AllowDisable = Staff->TestAccessLevel(TempUserInfo, CheckMenuEditor);
	// If not, prompt for a login.
	if (!AllowDisable)
	{
		TempUserInfo.Clear();
		TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckMenuEditor);
		if (Result == lsAccepted)
		{
			AllowDisable = true;
		}
		else if (Result == lsDenied)
		{
			MessageBox("You do not have access rights to Menu Import (Which Includes Remove)", "Error", MB_OK + MB_ICONERROR);
		}
		else if (Result == lsPINIncorrect)
		{
			MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
		}
	}
	DBTransaction.Commit();

	if(AllowDisable)
	{
		if(TDeviceRealTerminal::Instance().Menus->VisibleMenu!= NULL)
		{
			if (MessageBox("This will completely remove " + TDeviceRealTerminal::Instance().Menus->VisibleMenu->MenuName + "! \rDo you wish to continue?",
					"Warning",
					MB_OKCANCEL	+ MB_ICONWARNING + MB_DEFBUTTON2) == IDOK)
			{
            if (MessageBox("This really will delete " + TDeviceRealTerminal::Instance().Menus->VisibleMenu->MenuName + " from the system!\rOnly Tech support will be able to recover it.\rDo you wish to continue?",
                  "Warning",
                  MB_OKCANCEL	+ MB_ICONWARNING + MB_DEFBUTTON2) == IDOK)
            {
               try
               {
                  DBTransaction.StartTransaction();
                  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

                  IBInternalQuery->Close();
                  IBInternalQuery->SQL->Text = "DELETE FROM MENU WHERE MENU_KEY = :LOC_MENU_KEY";
                  IBInternalQuery->ParamByName("LOC_MENU_KEY")->AsInteger = TDeviceRealTerminal::Instance().Menus->VisibleMenu->MenuKey;
                  IBInternalQuery->ExecQuery();
                  if(IBInternalQuery->RowsAffected == -1 || IBInternalQuery->RowsAffected == 0)
                  {
                     MessageBox("Menu has already been removed.", "Error", MB_ICONERROR + MB_OK);
                  }
                  DBTransaction.Commit();

                  DBTransaction.StartTransaction();
                  TNetMessageMenuChanged * Request = new TNetMessageMenuChanged;
                  try
                  {
                     Request->Broadcast = true;
                     Request->CompareToDataBase = true;
                     TDeviceRealTerminal::Instance().Menus->MenuAddedRemoved(DBTransaction,Request);
                  }
                  __finally
                  {
                     delete Request;
                  }

                  TDeviceRealTerminal::Instance().Menus->VisibleMenu = NULL;
                  TDeviceRealTerminal::Instance().Menus->SwapInNewMenus();
				  TDeviceRealTerminal::Instance().Menus->SetMenuList(DBTransaction, TDeviceRealTerminal::Instance().ID.DeviceKey);
                  RedrawMenus(); // Redraw Menus.
                  if (ManagerMenus->GetMenusExist(DBTransaction))
                  {
                     if(TDeviceRealTerminal::Instance().Menus->VisibleMenu == NULL)
                     {
                        btnTimerTimer(Sender);
                     }
                  }
                  DBTransaction.Commit();
               }
               catch (Exception &E)
               {
                  MessageBox(E.Message.w_str(), _T("Error"), MB_OK + MB_ICONERROR);
               }
            }
			}
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMenuManager::tbtnAddMenuClick(TObject *Sender)
{
	Database::TDBTransaction DBTransaction(IBDatabase);
	DBTransaction.StartTransaction();

	std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));

	TMMContactInfo TempUserInfo;
	TempUserInfo = TDeviceRealTerminal::Instance().User;
	bool AllowDisable = Staff->TestAccessLevel(TempUserInfo, CheckMenuEditor);
	// If not, prompt for a login.
	if (!AllowDisable)
	{
		TempUserInfo.Clear();
		TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckMenuEditor);
		if (Result == lsAccepted)
		{
			AllowDisable = true;
		}
		else if (Result == lsDenied)
		{
			MessageBox("You do not have access rights to Menu Import.", "Error", MB_OK + MB_ICONERROR);
		}
		else if (Result == lsPINIncorrect)
		{
			MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
		}
	}
	DBTransaction.Commit();

	if(AllowDisable)
	{
		if(!DirectoryExists(ExtractFilePath(Application->ExeName) + "Menus"))
		{
			CreateDir(ExtractFilePath(Application->ExeName) + "Menus");
		}

		OpenDialog1->Title = "Load Menus";
		OpenDialog1->Filter = "Menu files (*.csv)|*.CSV|All files (*.*)|*.*";
		OpenDialog1->InitialDir = ExtractFilePath(Application->ExeName) + "Menus";
		if (OpenDialog1->Execute())
		{
			std::auto_ptr<TStringList> Menu(new TStringList);
			Menu->LoadFromFile(OpenDialog1->FileName);
			if(!DirectoryExists(ExtractFilePath(Application->ExeName) + "Menus\\Imported"))
			{
				CreateDir(ExtractFilePath(Application->ExeName) + "Menus\\Imported");
			}
			Menu->SaveToFile(ExtractFilePath(Application->ExeName) + "Menus\\Imported\\" + Now().FormatString("yyyy-mm-dd - hh-nn-ss ") + ExtractFileName(OpenDialog1->FileName));

			if (TDeviceRealTerminal::Instance().Menus->LoadMenu(IBDatabase,Menu.get(), false))
			{
				RedrawMenus();
				btnTimerTimer(Sender);

				if(TDeviceRealTerminal::Instance().IMManager->Registered)
				{
				   std::auto_ptr<TfrmProcessing>(frmProcessing)(TfrmProcessing::Create<TfrmProcessing>(Screen->ActiveForm));
				   frmProcessing->CanCancel = false;
				   frmProcessing->Message = "Exporting Products...";
				   frmProcessing->ShowProgress = false;
				   frmProcessing->Show();

				   // Update IntaMate.
				   TPOS_XMLBase POSXML("Product Export");
				   TDeviceRealTerminal::Instance().Menus->BuildXMLMenu(TDeviceRealTerminal::Instance().DBControl,POSXML,TGlobalSettings::Instance().SiteID);
				   TDeviceRealTerminal::Instance().IMManager->Export(POSXML);
				   POSXML.SaveToFile();
				   frmProcessing->Close();

				   frmProcessing->CanCancel = false;
				   frmProcessing->Message = "Exporting Categories and Groups...";
				   frmProcessing->ShowProgress = false;
				   frmProcessing->Show();

				   POSXML.Reset("Categories Export");
				   DBTransaction.StartTransaction();
				   TDeviceRealTerminal::Instance().Menus->BuildXMLListCategories(DBTransaction,POSXML,TGlobalSettings::Instance().SiteID);
				   DBTransaction.Commit();
				   TDeviceRealTerminal::Instance().IMManager->Export(POSXML);
				   POSXML.SaveToFile();

				   POSXML.Reset("List Category Groups Export");
				   DBTransaction.StartTransaction();
				   TDeviceRealTerminal::Instance().Menus->BuildXMLListGroup(DBTransaction,POSXML,TGlobalSettings::Instance().SiteID);
				   DBTransaction.Commit();
				   TDeviceRealTerminal::Instance().IMManager->Export(POSXML);
				   POSXML.SaveToFile();
				   frmProcessing->Close();
				}
			}
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMenuManager::FormCreate(TObject *Sender)
{
   ShowingMenu = -1;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMenuManager::pnlCoursesResize(TObject *Sender)
{
   sbCourses->Align = alClient;
   sbCourses->Align = alNone;
   sbCourses->Width += 20;
   sbCourses->Top = pnlCourses->Left;
   sbCourses->Height = btnCourseUp->Top - (pnlCourses->Left*2);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMenuManager::btnCourseUpMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if (sbCourses->VertScrollBar->Position > 0)
   {
      int StopAt = (sbCourses->VertScrollBar->Position - sbCourses->Height) + TouchButton->Height;
      if(StopAt < 30) StopAt = 0;
      
      for (int i=sbCourses->VertScrollBar->Position; i>= StopAt; i-=3)
      {
         sbCourses->VertScrollBar->Position = i;
         sbCourses->Update();
      }
   }
   
}
//---------------------------------------------------------------------------

void __fastcall TfrmMenuManager::btnCourseDownMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if (sbCourses->VertScrollBar->Position < (sbCourses->VertScrollBar->Range - sbCourses->ClientHeight))
   {
      int StopAt = (sbCourses->VertScrollBar->Position + sbCourses->Height) - TouchButton->Height;
      if( StopAt > (sbCourses->VertScrollBar->Range - sbCourses->ClientHeight))
      {
         StopAt = sbCourses->VertScrollBar->Range - sbCourses->ClientHeight;
      }
      
      for (; sbCourses->VertScrollBar->Position < StopAt; sbCourses->VertScrollBar->Position += 3)
      {
         sbCourses->Update();
      }
	}
   
}
//---------------------------------------------------------------------------

void __fastcall TfrmMenuManager::menuItemGridMouseClick(TObject *Sender,
  TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
    GridButton->Latched = !GridButton->Latched;
	CheckBoxClick(reinterpret_cast<TObject *>(GridButton));
}
//---------------------------------------------------------------------------

