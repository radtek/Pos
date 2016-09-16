// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
#ifdef  PalmMate
#include "Palm.h"
#endif

#include "PrinterMaintenance.h"
#include "Main.h"
#include "MMMessageBox.h"
#include "MMLogging.h"
#include "SelectDish.h"
#include "PSectionInstructionEdit.h"
#include "MMTouchKeyboard.h"
#include "ManagerVariable.h"
#include "VirtualPrintersInterface.h"
#include "GUIScale.h"

#include "Devices.h"
#include <iterator>
#include <algorithm>
#include "VerticalSelect.h"
#include "StrUtils.hpp"
#include "ThemeManager.h"
#include "Printing.h"
#include "Comms.h"
#include "PrintOut.h"
#include "PrinterDefs.h"
#include "ReqPrintJob.h"
#include "WindowsPrinters.h"
#include "MMUtilFunc.h"
#include "SeatOrders.h"
#include "ParkedSale.h"
#include "DBOrder.h"

#include "ChefMateInterface.h"

#define CONTAINER_LIST_DEVICE_COLUMN 0
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma link "TouchPages"
#pragma resource "*.dfm"

#pragma warn -par

// ---------------------------------------------------------------------------
__fastcall TfrmPrinterMaintenance::TfrmPrinterMaintenance(TComponent* Owner, Database::TDBControl &inDBControl) : TZForm(Owner),
DBControl(inDBControl)
{
   Color = clBlack;

   pgControl->ActivePage = tsPrinting;

   CurrentDeviceKey = 0;
   CurrentDeviceKey = 0;
   CurrentPrinterProfileKey = 0;
   CurrentMenuKey = 0;

   EventPhysicalPropertiesChanged.RegisterForEvent(OnPhysicalPropertiesChanged);

   if (TDeviceRealTerminal::Instance().Menus != NULL)
   {
	  TDeviceRealTerminal::Instance().Menus->OnNewMenus.RegisterForEvent(OnNewMenus);
   }
}

// ---------------------------------------------------------------------------
void __fastcall TfrmPrinterMaintenance::FormDestroy(TObject *Sender)
{
   while (lbPrintOpt->Count > 0)
   {
	  delete(TPSectionInstruction*)lbPrintOpt->Items->Objects[0];
	  lbPrintOpt->Items->Delete(0);
   }

   while (lbReceiptPrintOpt->Count > 0)
   {
	  delete(TPSectionInstruction*)lbReceiptPrintOpt->Items->Objects[0];
	  lbReceiptPrintOpt->Items->Delete(0);
   }

   while (lbPrintConfig->Count > 0)
   {
	  delete(TPSectionInstruction*)lbPrintConfig->Items->Objects[0];
	  lbPrintConfig->Items->Delete(0);
   }

   while (lbReceiptPrintConfig->Count > 0)
   {
	  delete(TPSectionInstruction*)lbReceiptPrintConfig->Items->Objects[0];
	  lbReceiptPrintConfig->Items->Delete(0);
   }

   for (int i = 0; i < lbPrinterModels->Items->Count; i++)
   {
	  delete(TPrinterModelSettings*)lbPrinterModels->Items->Objects[i];
   }

   EventPhysicalPropertiesChanged.DeregisterForEvent(OnPhysicalPropertiesChanged);

   if (TDeviceRealTerminal::Instance().Menus != NULL)
   {
	  TDeviceRealTerminal::Instance().Menus->OnNewMenus.DeregisterForEvent(OnNewMenus);
   }
}

// ---------------------------------------------------------------------------
void __fastcall TfrmPrinterMaintenance::FormCloseQuery(TObject *Sender, bool &CanClose)
{
   CanClose = true;

   if (pgControl->ActivePage == tsPrinting)
   {
	  SaveCoursePrinterProfiles();
   }

   if (pgControl->ActivePage == tsBreakdownCats)
   {
	  SaveBreakDownPrinterProfiles();
   }


   if (pgControl->ActivePage == tsServingCoursePrinting)
   {
	  SaveCurrentServingCourseSettings();
   }

   if (KitchenTemplateModified && pgControl->ActivePage == tsKitchenFormat)
   {
	  SaveKitchenTemplate();
   }

   if (ReceiptTemplateModified && pgControl->ActivePage == tsReceiptOptions)
   {
	  SaveReceiptTemplate();
   }

   if (CurrentDeviceKey != 0 && pgControl->ActivePage == tsDevicePrinterProfiles)
   {
	  SaveDevicePrinterProfiles();
   }

   if (pgControl->ActivePage == tsReceiptOptions)
   {
	  if (memFooter->Modified || memHeader->Modified || memPHeader->Modified)
	  {
		 if (MessageBox(UnicodeString("Do you wish to save your changes to your header and footer?").w_str(),
			   UnicodeString("Confirmation").w_str(), MB_YESNO + MB_DEFBUTTON1 + MB_ICONINFORMATION) == IDYES)
		 {
			btnHeaderFooterSaveMouseClick(Sender);
		 }
	  }
     SaveHeaderLabels();
   }

}

// ---------------------------------------------------------------------------
void __fastcall TfrmPrinterMaintenance::FormShow(TObject *Sender)
{
   try
   {
	  FormResize(this);
      IsDBRegistered = false;
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

	  if (pgControl->ActivePage == tsPrinting)
	  {
		 PrinterProfileCourses.clear();
		 // Course -> Printer Profile Assignment.
		 UpdatePrinterDisplay();
		 UpdateMenuDisplay();
	  }
	  else if (pgControl->ActivePage == tsBreakdownCats)
	  {
		 PrinterProfileBreakDownCategories.clear();
		 UpdatePrinterBreakDownDisplay();
	  }
	  else if (pgControl->ActivePage == tsKitchenFormat)
	  {
		 UpdateKitchenFormatProfileListDisplay();
		 DisplayKitchenPrinterInstructionSet();
		 // Kitchen Docket Template Display.
		 DisplayKitchenTemplate();
		 DrawKitchenDocket();
         cbPrintNoticeOnTransfer->Checked = TGlobalSettings::Instance().PrintNoticeOnTransfer;
	  }
	  else if (pgControl->ActivePage == tsPrinters)
	  {
		 btnRefreshPrinterModelsMouseClick(NULL);
		 UpdatePhysicalPrinterList(DBTransaction);
	  }
	  else if (pgControl->ActivePage == tsServingCoursePrinting)
	  {
		 // Serving Course -> Printer Profile Assignment.
		 PrinterProfileServingCourses.clear();
		 UpdateSCPrinterDisplay();
	  }
	  else if (pgControl->ActivePage == tsDevicePrinterProfiles)
	  {
		 DevicesPrinterProfile.clear();
		 // Device -> Printer Profile Assignment.
		 UpdateDeviceDisplay();
	  }
	  else if (pgControl->ActivePage == tsReceiptOptions)
	  {
		 UpdateReceiptFormatProfileListDisplay();
		 DisplayReceiptPrinterInstructionSet();
		 // Kitchen Docket Template Display.
		 DisplayReceiptTemplate();
		 DrawReceiptDocket();
		 cbAlwaysPrintReceiptTenderedSales->Checked = Receipt->AlwaysPrintReceiptTenderedSales;
		 cbAlwaysPrintCashSales->Checked = Receipt->AlwaysPrintReceiptCashSales;
		 cbReceiptBarCodes->Checked = Receipt->DisplayBarCodes;
		 cbDuplicateReceipts->Checked = TGlobalSettings::Instance().DuplicateReceipts;
		 cbDoublePrint->Checked = TGlobalSettings::Instance().PrintSignatureReceiptsTwice;
		 cbSaveAndPrint->Checked = TGlobalSettings::Instance().SaveAndPrintPrintsPartialOrder;
         cbPrintZeroTaxes->Checked = TGlobalSettings::Instance().PrintZeroTaxes;
		 cbAPTROS->Checked = TGlobalSettings::Instance().AutoPrintTabReceipts;
		 cbAPSROS->Checked = TGlobalSettings::Instance().AutoPrintStaffReceipts;
		 cbAPMOS->Checked = TGlobalSettings::Instance().AutoPrintMemberReceipts;
		 cbAPTAROS->Checked = TGlobalSettings::Instance().AutoPrintTableReceipts;
		 cbAPCROS->Checked = TGlobalSettings::Instance().AutoPrintCashReceipts;
		 cbAPRROS->Checked = TGlobalSettings::Instance().AutoPrintRoomReceipts;
		 cbAPIROS->Checked = TGlobalSettings::Instance().AutoPrintInvoiceReceipts;
		 cbAPWOOS->Checked = TGlobalSettings::Instance().AutoPrintWebReceipts;

		 bool PrintNote = TManagerVariable::Instance().GetBool(DBTransaction, vmPrintNoteWithDiscount);
		 cbPrintNoteWithDiscount->Checked = PrintNote;

		 TManagerPhysicalPrinter ManagerPhysicalPrinter;
		 cbReceiptPrinter->Items->AddObject("None", static_cast <TObject*> (0));
		 ManagerPhysicalPrinter.DBGetPrinterList(DBTransaction, cbReceiptPrinter->Items);

		 cbReceiptPrinter->ItemIndex = 0;
		 if (TComms::Instance().ReceiptPrinter.PhysicalPrinterKey != 0)
		 {
			int PrinterIndex = 0;
			for (int i = 0; i < cbReceiptPrinter->Items->Count; i++)
			{
			   if ((int)(cbReceiptPrinter->Items->Objects[i]) == TComms::Instance().ReceiptPrinter.PhysicalPrinterKey)
			   {
				  PrinterIndex = i;
			   }
			}
			cbReceiptPrinter->ItemIndex = PrinterIndex;
          TDeviceRealTerminal::Instance().Registered(&IsDBRegistered, NULL);
         if(IsDBRegistered)
         {
             memHeader->Text = TGlobalSettings::Instance().Header->Text;
             memPHeader->Text = TGlobalSettings::Instance().PHeader->Text;
         }
         else
         {
             memHeader->Text = "NOT FOR RESALE"
                               "THIS SOFTWARE VERSION IS NOT REGISTERED"
                                "CONTACT MENUMATE on SUPPORT@MENUMATE.COM "
                                "TO BE REGISTERED CORRECTLY "
                                "USE OF NON REGISTERED SOFTWARE MAY RESULT "
                                "IN ADDITIONAL LEGAL COSTS AND EXPENSES";
            memPHeader->Text =  memHeader->Text;
         }
         memFooter->Text = TGlobalSettings::Instance().Footer->Text;
         memHeader->Modified = false;
         memPHeader->Modified = false;
         memFooter->Modified = false;

      }

	  Modified = false;
	  KitchenTemplateModified = false;

	  AnsiString BulletSide = TManagerVariable::Instance().GetStr(DBTransaction, vmKitchenBulletSide);
	  edSideBullet->Text = BulletSide;

	  AnsiString BulletOpt = TManagerVariable::Instance().GetStr(DBTransaction, vmKitchenBulletOpt);
	  edOptionsBullet->Text = BulletOpt;

	  AnsiString BulletSetMenuItem = TManagerVariable::Instance().GetStr(DBTransaction, vmKitchenBulletSetMenuItem);
	  edSetMenuItemBullet->Text = BulletSetMenuItem;

	  AnsiString NoteHeader = TManagerVariable::Instance().GetStr(DBTransaction, vmKitchenHeaderNote);
	  edNoteHeader->Text = NoteHeader;

	  AnsiString SideHeader = TManagerVariable::Instance().GetStr(DBTransaction, vmKitchenHeaderSide);
	  edSidesHeader->Text = SideHeader;

	  AnsiString MixHeader = TManagerVariable::Instance().GetStr(DBTransaction, vmKitchenHeaderMix);
	  edMixHeader->Text = MixHeader;

	  AnsiString DishesPerSeatMultiplerString = TManagerVariable::Instance().GetStr(DBTransaction, vmKitchenPerSeatMultiplerString);
	  edPerSeatMultiplier->Text = DishesPerSeatMultiplerString;

	  tbChefMateColour->Color = (TColor)TGlobalSettings::Instance().KitchenColour;

	  if (TDeviceRealTerminal::Instance().KitchenMod->Enabled)
	  {
		 tbChefMateIP->Enabled = true;
	  }
	  else
	  {
		 tbChefMateIP->Enabled = false;
	  }
	  DBTransaction.Commit();
   }
   }
   catch(Exception & E)
   {
	  Modified = false;
	  KitchenTemplateModified = false;
	  Application->ShowException(&E);
	  PostMessage(Handle, WM_CLOSE, 0, 0);
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
   }
}

// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::WMDisplayChange(TWMDisplayChange& Message)
{
   FormResize(this);
}

// ---------------------------------------------------------------------------
void TfrmPrinterMaintenance::SaveHeaderLabels()
{
    //Save the receipt number label to database.
    Database::TDBTransaction DBTransaction(DBControl);
    DBTransaction.StartTransaction();
    TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmReceiptNumberLabel,TGlobalSettings::Instance().ReceiptNumberLabel);
    //Save the reprint receipt label to database.
    TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmReprintReceiptLabel,TGlobalSettings::Instance().ReprintReceiptLabel);
    DBTransaction.Commit();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmPrinterMaintenance::FormResize(TObject *Sender)
{
 // if (Tag != Screen->Width)
//  {
	  int Temp = Tag;
	  Tag = Screen->Width;
        if((double)Screen->Width / Screen->Height < 1.4)
        {
        ScaleBy(Screen->Width, Temp);
        }
       /* else
        {
            GUIScale::ChildrenTop(Panel43, 0.9);
        }  */
  // }

   Height = Screen->Height;
   Width = Screen->Width;
   pnlButtons->Top = pgControl->Left;
   pnlButtons->Left = ClientWidth - pnlButtons->Width - pgControl->Left;
   pnlButtons->Height = ClientHeight - (pgControl->Left * 2);
   pgControl->Top = pgControl->Left;
   pgControl->Width = pnlButtons->Left - (pgControl->Left * 2);
   pgControl->Height = ClientHeight - (pgControl->Left * 2);
  // Panel44->Left = tbReceiptProperties->Left + tbReceiptProperties->Right +20;
  // tbReceiptTemplateUp->Left = tbtnIndentSectionRight->Left + tbtnIndentSectionRight->Width + 10;;
}

// ---------------------------------------------------------------------------
void TfrmPrinterMaintenance::UpdateMenuDisplay()
{
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

	  CurrentMenuKey = 0;

	  std::auto_ptr <TStringList> MenuList(new TStringList);
	  TDeviceRealTerminal::Instance().Menus->GetMenuList(DBTransaction, MenuList.get());

	  GetCourseInfo(DBTransaction);

	  DBTransaction.Commit();

	  tgridMenu->ColCount = 1;
	  tgridMenu->RowCount = 0;
	  tgridMenu->RowCount = MenuList->Count;

	  for (int i = 0; i < MenuList->Count; i++)
	  {
		 tgridMenu->Buttons[i][0]->Color = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR];
		 tgridMenu->Buttons[i][0]->LatchedColor = ButtonColors[BUTTONTYPE_FULL][ATTRIB_BUTTONCOLOR];
		 tgridMenu->Buttons[i][0]->FontColor = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_FONTCOLOR];
		 tgridMenu->Buttons[i][0]->LatchedFontColor = ButtonColors[BUTTONTYPE_FULL][ATTRIB_FONTCOLOR];

		 tgridMenu->Buttons[i][0]->Latched = false;
		 tgridMenu->Buttons[i][0]->Caption = MenuList->Strings[i];
		 tgridMenu->Buttons[i][0]->Tag = (int)MenuList->Objects[i];
	  }

	  if (tgridMenu->RowCount > 0)
	  {
		 tgridMenuMouseClick(NULL, TMouseButton() << mbLeft, TShiftState(), tgridMenu->Buttons[0][0]);
	  }

	  UpdateCourseDisplay();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}
// ---------------------------------------------------------------------------

void TfrmPrinterMaintenance::UpdateCourseDisplay()
{
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

	  std::auto_ptr <TStringList> CourseList(new TStringList());

	  // TDeviceRealTerminal::Instance().Menus->GetCourseList(DBTransaction,CurrentMenuKey,CourseList.get());
	  TDeviceRealTerminal::Instance().Menus->GetCourseKitchenNameList(DBTransaction, CurrentMenuKey, CourseList.get());

	  DBTransaction.Commit();

	  tgridCourse->RowCount = 0; // Clears all the Latching.
	  tgridCourse->ColCount = 1;
	  tgridCourse->RowCount = CourseList->Count;

	  if (CourseList->Count > 0)
	  {
		 int Index = 0;
		 // Add all the Selected Courses.
		 for (std::vector <int> ::iterator ptrCourseKey = PrinterProfileCourses[CurrentPrinterProfileKey][CurrentMenuKey].begin();
			ptrCourseKey != PrinterProfileCourses[CurrentPrinterProfileKey][CurrentMenuKey].end(); ptrCourseKey++, Index++)
		 {
			tgridCourse->Buttons[Index][0]->Caption = CourseInfo[*ptrCourseKey].CourseKitchenName;
			tgridCourse->Buttons[Index][0]->Tag = *ptrCourseKey;

			tgridCourse->Buttons[Index][0]->Color = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR];
			tgridCourse->Buttons[Index][0]->LatchedColor = ButtonColors[BUTTONTYPE_FULL][ATTRIB_BUTTONCOLOR];
			tgridCourse->Buttons[Index][0]->FontColor = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_FONTCOLOR];
			tgridCourse->Buttons[Index][0]->LatchedFontColor = ButtonColors[BUTTONTYPE_FULL][ATTRIB_FONTCOLOR];
			tgridCourse->Buttons[Index][0]->Latched = true;
		 }

		 for (int i = 0; (i < CourseList->Count) && (Index < tgridCourse->RowCount); i++)
		 {
			if (CurrentPrinterProfileKey != 0)
			{
			   // Add all the unslected Courses on the bottom.
			   if (std::find(PrinterProfileCourses[CurrentPrinterProfileKey][CurrentMenuKey].begin(),
					 PrinterProfileCourses[CurrentPrinterProfileKey][CurrentMenuKey].end(), (int)CourseList->Objects[i])
				  == PrinterProfileCourses[CurrentPrinterProfileKey][CurrentMenuKey].end())
			   { // Not Found.
				  tgridCourse->Buttons[Index][0]->Caption = CourseList->Strings[i];
				  tgridCourse->Buttons[Index][0]->Tag = (int)CourseList->Objects[i];

				  tgridCourse->Buttons[Index][0]->Color = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR];
				  tgridCourse->Buttons[Index][0]->LatchedColor = ButtonColors[BUTTONTYPE_FULL][ATTRIB_BUTTONCOLOR];
				  tgridCourse->Buttons[Index][0]->FontColor = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_FONTCOLOR];
				  tgridCourse->Buttons[Index][0]->LatchedFontColor = ButtonColors[BUTTONTYPE_FULL][ATTRIB_FONTCOLOR];
				  tgridCourse->Buttons[Index][0]->Latched = false;
				  Index++;
			   }
			}
			else
			{
			   tgridCourse->Buttons[Index][0]->Caption = CourseList->Strings[i];
			   tgridCourse->Buttons[Index][0]->Tag = (int)CourseList->Objects[i];

			   tgridCourse->Buttons[Index][0]->Color = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR];
			   tgridCourse->Buttons[Index][0]->LatchedColor = ButtonColors[BUTTONTYPE_FULL][ATTRIB_BUTTONCOLOR];
			   tgridCourse->Buttons[Index][0]->FontColor = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_FONTCOLOR];
			   tgridCourse->Buttons[Index][0]->LatchedFontColor = ButtonColors[BUTTONTYPE_FULL][ATTRIB_FONTCOLOR];
			   tgridCourse->Buttons[Index][0]->Latched = false;
			   Index++;
			}
		 }
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

// ---------------------------------------------------------------------------

void TfrmPrinterMaintenance::SaveCoursePrinterProfiles()
{
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  for (std::map <int, std::map <int, std::vector <int> > > ::iterator ptrProfile = PrinterProfileCourses.begin();
		 ptrProfile != PrinterProfileCourses.end(); advance(ptrProfile, 1))
	  {
		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text = "Delete from PRNORDER where VIRTUALPRINTER_KEY = :VIRTUALPRINTER_KEY";
		 IBInternalQuery->ParamByName("VIRTUALPRINTER_KEY")->AsInteger = ptrProfile->first;
		 IBInternalQuery->ExecQuery();

		 for (std::map <int, std::vector <int> > ::iterator ptrMenu = ptrProfile->second.begin(); ptrMenu != ptrProfile->second.end();
			ptrMenu++)
		 {

			for (std::vector <int> ::iterator ptrCourseKey = ptrMenu->second.begin(); ptrCourseKey != ptrMenu->second.end();
			   std::advance(ptrCourseKey, 1))
			{
			   if (TDeviceRealTerminal::Instance().Menus->IsAValidMenu(DBTransaction, CourseInfo[*ptrCourseKey].MenuKey))
			   {
				  IBInternalQuery->Close();
				  IBInternalQuery->SQL->Text =
					  "INSERT INTO PRNORDER (" "PRNORD_KEY, " "VIRTUALPRINTER_KEY, " "MENU_KEY, " "COURSE_KEY, "
					  "COURSE_NAME) " "VALUES (" "(SELECT GEN_ID(GEN_PRNORDER , 1) FROM RDB$DATABASE), "
					  ":VIRTUALPRINTER_KEY, " ":MENU_KEY, " ":COURSE_KEY, " ":COURSE_NAME); ";
				  IBInternalQuery->ParamByName("VIRTUALPRINTER_KEY")->AsInteger = ptrProfile->first;
				  IBInternalQuery->ParamByName("MENU_KEY")->AsInteger = CourseInfo[*ptrCourseKey].MenuKey;

				  IBInternalQuery->ParamByName("COURSE_KEY")->AsInteger = *ptrCourseKey;
                  IBInternalQuery->ParamByName("COURSE_NAME")->AsString	= CourseInfo[*ptrCourseKey].CourseKitchenName; 
				  IBInternalQuery->ExecQuery();
			   }

			}
		 }
	  }
	  DBTransaction.Commit();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}
// ---------------------------------------------------------------------------


void TfrmPrinterMaintenance::SaveBreakDownPrinterProfiles()
{
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  for (std::map <int, std::vector <int> > ::iterator ptrProfile = PrinterProfileBreakDownCategories.begin();
		 ptrProfile != PrinterProfileBreakDownCategories.end(); advance(ptrProfile, 1))
	  {
		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text = "Delete from PRNCAT where VIRTUALPRINTER_KEY = :VIRTUALPRINTER_KEY";
		 IBInternalQuery->ParamByName("VIRTUALPRINTER_KEY")->AsInteger = ptrProfile->first;
		 IBInternalQuery->ExecQuery();

		 for (std::vector <int> ::iterator ptrBreakDownCat = ptrProfile->second.begin(); ptrBreakDownCat != ptrProfile->second.end();
			ptrBreakDownCat++)
		 {
              IBInternalQuery->Close();
              IBInternalQuery->SQL->Text =
                  "INSERT INTO PRNCAT (" "PRNCAT_KEY, " "VIRTUALPRINTER_KEY, " "CATEGORY_KEY ) "
                  "VALUES (" "(SELECT GEN_ID(GEN_PRNCAT , 1) FROM RDB$DATABASE), "
                  ":VIRTUALPRINTER_KEY, :CATEGORY_KEY); ";
              IBInternalQuery->ParamByName("VIRTUALPRINTER_KEY")->AsInteger = ptrProfile->first;
              IBInternalQuery->ParamByName("CATEGORY_KEY")->AsInteger = *ptrBreakDownCat;
              IBInternalQuery->ExecQuery();
		 }
	  }
	  DBTransaction.Commit();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}
// ---------------------------------------------------------------------------

void TfrmPrinterMaintenance::UpdateDeviceDisplay()
{
   try
   {

	  tgridDeviceList->RowCount = 0;
	  tgridDeviceList->ColCount = 1;

	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

	  std::auto_ptr <TStringList> DeviceList(new TStringList);
	  std::auto_ptr <TManagerDevices> ManagerDevices(new TManagerDevices);
	  ManagerDevices->GetDeviceList(DBTransaction, 1, DeviceList.get());

	  DBTransaction.Commit();

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

void TfrmPrinterMaintenance::UpdateProfileListDisplay()
{
   if (CurrentDeviceKey != 0)
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

	  std::auto_ptr <TStringList> PrinterProfiles(new TStringList);
	  std::auto_ptr <TManagerPhysicalPrinter> ManagerPhysicalPrinter(new TManagerPhysicalPrinter);

	  TManagerVirtualPrinter::GetVirtualPrinterList(DBTransaction, PrinterProfiles.get());

	  // Add Physical Printer Connection Info.
	  // Add Physical Printer Connection Info.
	  for (int i = 0; i < PrinterProfiles->Count; i++)
	  {
		 int VirtualPrinterKey = (int)PrinterProfiles->Objects[i];
		 TPrinterVirtual *vPrinter = TManagerVirtualPrinter::GetVirtualPrinterByKey(DBTransaction,VirtualPrinterKey);
		 if (vPrinter)
		 {
			TPrinterPhysical PrinterPhysical = ManagerPhysicalPrinter->GetPhysicalPrinter(DBTransaction, vPrinter->PhysicalPrinterKey);
			PrinterProfiles->Strings[i] = PrinterProfiles->Strings[i] + " [" + PrinterPhysical.ServerName + " : " +
				PrinterPhysical.ShareName + "]";
		 }
	  }

	  std::auto_ptr <TManagerDevices> ManagerDevices(new TManagerDevices);
	  ManagerDevices->GetPrinterProfilesList(DBTransaction, CurrentDeviceKey, DevicesPrinterProfile[CurrentDeviceKey]);

	  tgridProfileList->RowCount = 0; // Clears all the Latching.
	  tgridProfileList->ColCount = 1;
	  tgridProfileList->RowCount = PrinterProfiles->Count;
	  for (int i = 0; i < PrinterProfiles->Count; i++)
	  {
		 tgridProfileList->Buttons[i][0]->Caption = PrinterProfiles->Strings[i];
		 tgridProfileList->Buttons[i][0]->Tag = (int)PrinterProfiles->Objects[i];

		 tgridProfileList->Buttons[i][0]->Color = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR];
		 tgridProfileList->Buttons[i][0]->LatchedColor = ButtonColors[BUTTONTYPE_FULL][ATTRIB_BUTTONCOLOR];
		 tgridProfileList->Buttons[i][0]->FontColor = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_FONTCOLOR];
		 tgridProfileList->Buttons[i][0]->LatchedFontColor = ButtonColors[BUTTONTYPE_FULL][ATTRIB_FONTCOLOR];

		 if (DevicesPrinterProfile[CurrentDeviceKey].find(tgridProfileList->Buttons[i][0]->Tag) == DevicesPrinterProfile[CurrentDeviceKey]
			.end())
		 { // Not Found.
			tgridProfileList->Buttons[i][0]->Latched = false;
		 }
		 else
		 {
			tgridProfileList->Buttons[i][0]->Latched = true;
		 }
	  }
	  DBTransaction.Commit();
   }
   else // Clear Display.
   {
	  tgridProfileList->RowCount = 0;
   }
}

void __fastcall TfrmPrinterMaintenance::tgridDeviceListMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift,
   TGridButton *GridButton)
{
   try
   {
	  SaveDevicePrinterProfiles();

	  CurrentDeviceKey = GridButton->Tag;
	  UpdateProfileListDisplay();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

// ---------------------------------------------------------------------------
void __fastcall TfrmPrinterMaintenance::tgridProfileListMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift,
   TGridButton *GridButton)
{
   try
   {
	  if (CurrentDeviceKey != 0)
	  {
		 DevicesPrinterProfile[CurrentDeviceKey].clear();

		 for (int i = 0; i < tgridProfileList->RowCount; i++)
		 {
			if (tgridProfileList->Buttons[i][0]->Latched)
			{
			   DevicesPrinterProfile[CurrentDeviceKey].insert(tgridProfileList->Buttons[i][0]->Tag);

               Database::TDBTransaction DBTransaction(DBControl);
               DBTransaction.StartTransaction();

			   TPrinterVirtual *Printer = TManagerVirtualPrinter::GetVirtualPrinterByKey(DBTransaction,tgridProfileList->Buttons[i][0]->Tag);
			   if (Printer != NULL)
			   {

				  // Create the Windos printer and add the connection if required.
				  TManagerPhysicalPrinter ManagerPhysicalPrinter;
				  TPrinterPhysical PrinterPhysical = ManagerPhysicalPrinter.GetPhysicalPrinter(DBTransaction, Printer->PhysicalPrinterKey);
				  if (PrinterPhysical.Type == ptWindows_Printer)
				  {
					 if (TDeviceRealTerminal::Instance().ID.DeviceKey == CurrentDeviceKey)
					 {
						int ErrorCode = AddPrinterConnection(PrinterPhysical.UNCName().t_str());
						if (ErrorCode != 0)
						{
						   MessageBox("The printer could not be added. Make sure that " + PrinterPhysical.ServerName +
							  " is turned on.\rError Code " + IntToStr(ErrorCode), "Error", MB_ICONERROR + MB_OK);
						}
					 }
					 else
					 {
						std::auto_ptr <TManagerDevices> ManagerDevices(new TManagerDevices);
						AnsiString DeviceName = ManagerDevices->GetDeviceName(DBTransaction, CurrentDeviceKey);
						MessageBox("The Selected printer may not have been configured under Windows on " + DeviceName +
						   ".\r" "You will need to Restart MenuMate on " + DeviceName +
						   " to ensure the window driver has been added.", "Error", MB_ICONWARNING + MB_OK);
					 }
				  }
			   }
  			   DBTransaction.Commit();
			}
		 }
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}
// ---------------------------------------------------------------------------

void TfrmPrinterMaintenance::SaveDevicePrinterProfiles()
{
   try
   {
	  if (CurrentDeviceKey != 0)
	  {
		 std::auto_ptr <TManagerDevices> ManagerDevices(new TManagerDevices);

		 Database::TDBTransaction DBTransaction(DBControl);
		 DBTransaction.StartTransaction();

		 for (std::map <__int64, typProfileKeyList> ::iterator ptrDevice = DevicesPrinterProfile.begin();
			ptrDevice != DevicesPrinterProfile.end(); advance(ptrDevice, 1))
		 {
			ManagerDevices->SetPrinterProfilesList(DBTransaction, ptrDevice->first, ptrDevice->second);
		 }
		 DBTransaction.Commit();
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

// ---------------------------------------------------------------------------
void TfrmPrinterMaintenance::UpdatePrinterDisplay()
{
   try
   {
	  CurrentPrinterProfileKey = 0;
	  PrinterProfileCourses.clear();

	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

	  std::auto_ptr <TStringList> PrinterProfiles(new TStringList);
	  std::auto_ptr <TManagerPhysicalPrinter> ManagerPhysicalPrinter(new TManagerPhysicalPrinter);

	  TManagerVirtualPrinter::GetVirtualPrinterList(DBTransaction, PrinterProfiles.get());

	  // Add Physical Printer Connection Info.
	  for (int i = 0; i < PrinterProfiles->Count; i++)
	  {
		 int VirtualPrinterKey = (int)PrinterProfiles->Objects[i];
		 TPrinterVirtual *vPrinter = TManagerVirtualPrinter::GetVirtualPrinterByKey(DBTransaction,VirtualPrinterKey);
		 if (vPrinter)
		 {
			TPrinterPhysical PrinterPhysical = ManagerPhysicalPrinter->GetPhysicalPrinter(DBTransaction, vPrinter->PhysicalPrinterKey);
			PrinterProfiles->Strings[i] = PrinterProfiles->Strings[i] + " [" + PrinterPhysical.ServerName + " : " +
				PrinterPhysical.ShareName + "]";
		 }
	  }

	  tgridPrinterProfiles->RowCount = 0; // Clears all the Latching.
	  tgridPrinterProfiles->ColCount = 1;
	  tgridPrinterProfiles->RowCount = PrinterProfiles->Count;
	  for (int i = 0; i < PrinterProfiles->Count; i++)
	  {
		 tgridPrinterProfiles->Buttons[i][0]->Caption = PrinterProfiles->Strings[i];
		 tgridPrinterProfiles->Buttons[i][0]->Tag = (int)PrinterProfiles->Objects[i];
		 tgridPrinterProfiles->Buttons[i][0]->Color = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR];
		 tgridPrinterProfiles->Buttons[i][0]->LatchedColor = ButtonColors[BUTTONTYPE_FULL][ATTRIB_BUTTONCOLOR];
		 tgridPrinterProfiles->Buttons[i][0]->FontColor = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_FONTCOLOR];
		 tgridPrinterProfiles->Buttons[i][0]->LatchedFontColor = ButtonColors[BUTTONTYPE_FULL][ATTRIB_FONTCOLOR];
		 GetPrinterProfileCourses(tgridPrinterProfiles->Buttons[i][0]->Tag,
			PrinterProfileCourses[tgridPrinterProfiles->Buttons[i][0]->Tag]);
	  }
	  DBTransaction.Commit();

	  if (tgridPrinterProfiles->RowCount > 0)
	  {
		 tgridPrinterProfilesMouseClick(NULL, TMouseButton() << mbLeft, TShiftState(), tgridPrinterProfiles->Buttons[0][0]);
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

// ---------------------------------------------------------------------------

void TfrmPrinterMaintenance::UpdateSCPrinterDisplay()
{
   try
   {
	  CurrentPrinterProfileKey = 0;
	  PrinterProfileServingCourses.clear();

	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

	  std::auto_ptr <TStringList> PrinterProfiles(new TStringList);
	  std::auto_ptr <TManagerPhysicalPrinter> ManagerPhysicalPrinter(new TManagerPhysicalPrinter);

	  TManagerVirtualPrinter::GetVirtualPrinterList(DBTransaction, PrinterProfiles.get());

	  // Add Physical Printer Connection Info.
	  for (int i = 0; i < PrinterProfiles->Count; i++)
	  {
		 int VirtualPrinterKey = (int)PrinterProfiles->Objects[i];
		 TPrinterVirtual *vPrinter = TManagerVirtualPrinter::GetVirtualPrinterByKey(DBTransaction,VirtualPrinterKey);
		 if (vPrinter)
		 {
			TPrinterPhysical PrinterPhysical = ManagerPhysicalPrinter->GetPhysicalPrinter(DBTransaction, vPrinter->PhysicalPrinterKey);
			PrinterProfiles->Strings[i] = PrinterProfiles->Strings[i] + " [" + PrinterPhysical.ServerName + " : " +
				PrinterPhysical.ShareName + "]";
		 }
	  }

	  tgridSCPrinterProfiles->RowCount = 0; // Clears all the Latching.
	  tgridSCPrinterProfiles->ColCount = 1;
	  tgridSCPrinterProfiles->RowCount = PrinterProfiles->Count;
	  for (int i = 0; i < PrinterProfiles->Count; i++)
	  {
		 tgridSCPrinterProfiles->Buttons[i][0]->Caption = PrinterProfiles->Strings[i];
		 tgridSCPrinterProfiles->Buttons[i][0]->Tag = (int)PrinterProfiles->Objects[i];
		 tgridSCPrinterProfiles->Buttons[i][0]->Color = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR];
		 tgridSCPrinterProfiles->Buttons[i][0]->LatchedColor = ButtonColors[BUTTONTYPE_FULL][ATTRIB_BUTTONCOLOR];
		 tgridSCPrinterProfiles->Buttons[i][0]->FontColor = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_FONTCOLOR];
		 tgridSCPrinterProfiles->Buttons[i][0]->LatchedFontColor = ButtonColors[BUTTONTYPE_FULL][ATTRIB_FONTCOLOR];
		 GetPrinterProfileServingCourses(tgridSCPrinterProfiles->Buttons[i][0]->Tag, PrinterProfileServingCourses);
	  }
	  DBTransaction.Commit();

	  if (tgridSCPrinterProfiles->RowCount > 0)
	  {
		 tgridSCPrinterProfilesMouseClick(NULL, TMouseButton() << mbLeft, TShiftState(), tgridSCPrinterProfiles->Buttons[0][0]);
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

// ---------------------------------------------------------------------------

void TfrmPrinterMaintenance::UpdateServingCourseDisplay()
{
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

	  std::auto_ptr <TStringList> ServingCourseList(new TStringList);
	  std::map <int, UnicodeString> ServingCourseInfo;
	  GetDistinctServingCourseList(ServingCourseList.get());
	  for (int i = 0; i < ServingCourseList->Count; i++)
	  {
		 ServingCourseInfo[(int)ServingCourseList->Objects[i]] = ServingCourseList->Strings[i];
	  }

	  DBTransaction.Commit();

	  tgridServingCourses->RowCount = 0; // Clears all the Latching.
	  tgridServingCourses->ColCount = 1;
	  tgridServingCourses->RowCount = ServingCourseList->Count;

	  int Index = 0;
	  for (std::vector <int> ::iterator ptrServingCourseKey = PrinterProfileServingCourses[CurrentPrinterProfileKey].begin();
		 ptrServingCourseKey != PrinterProfileServingCourses[CurrentPrinterProfileKey].end(); ptrServingCourseKey++, Index++)
	  {
		 tgridServingCourses->Buttons[Index][0]->Caption = ServingCourseInfo[*ptrServingCourseKey];
		 tgridServingCourses->Buttons[Index][0]->Tag = *ptrServingCourseKey;

		 tgridServingCourses->Buttons[Index][0]->Color = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR];
		 tgridServingCourses->Buttons[Index][0]->LatchedColor = ButtonColors[BUTTONTYPE_FULL][ATTRIB_BUTTONCOLOR];
		 tgridServingCourses->Buttons[Index][0]->FontColor = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_FONTCOLOR];
		 tgridServingCourses->Buttons[Index][0]->LatchedFontColor = ButtonColors[BUTTONTYPE_FULL][ATTRIB_FONTCOLOR];
		 tgridServingCourses->Buttons[Index][0]->Latched = true;
	  }

	  for (int i = 0; i < ServingCourseList->Count; i++)
	  {
		 if (CurrentPrinterProfileKey != 0)
		 {
			if (std::find(PrinterProfileServingCourses[CurrentPrinterProfileKey].begin(),
				  PrinterProfileServingCourses[CurrentPrinterProfileKey].end(), (int)ServingCourseList->Objects[i])
			   == PrinterProfileServingCourses[CurrentPrinterProfileKey].end())
			{ // Not Found.
			   tgridServingCourses->Buttons[Index][0]->Caption = ServingCourseList->Strings[i];
			   tgridServingCourses->Buttons[Index][0]->Tag = (int)ServingCourseList->Objects[i];

			   tgridServingCourses->Buttons[Index][0]->Color = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR];
			   tgridServingCourses->Buttons[Index][0]->LatchedColor = ButtonColors[BUTTONTYPE_FULL][ATTRIB_BUTTONCOLOR];
			   tgridServingCourses->Buttons[Index][0]->FontColor = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_FONTCOLOR];
			   tgridServingCourses->Buttons[Index][0]->LatchedFontColor = ButtonColors[BUTTONTYPE_FULL][ATTRIB_FONTCOLOR];
			   tgridServingCourses->Buttons[Index][0]->Latched = false;
			   Index++;
			}
		 }
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

void TfrmPrinterMaintenance::GetDistinctServingCourseList(TStringList *ServingCourses)
{
   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
	   "Select distinct DISPLAY_ORDER,cast(SERVINGCOURSE_KITCHEN_NAME as varchar(200)) SERVINGCOURSE_NAME, SERVINGCOURSES_KEY "
	   " From SERVINGCOURSES Where SERVINGCOURSE_KITCHEN_NAME IS NOT NULL and SERVINGCOURSE_KITCHEN_NAME != ''";
   IBInternalQuery->ExecQuery();
   for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
   {
	  ServingCourses->AddObject(UTF8ToUnicodeString((AnsiString)IBInternalQuery->FieldByName("SERVINGCOURSE_NAME")->AsString),
		 (System::TObject*)(IBInternalQuery->FieldByName("SERVINGCOURSES_KEY")->AsInteger));
   }

   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
	   " Select distinct DISPLAY_ORDER,cast(SERVINGCOURSE_NAME as varchar(200)) SERVINGCOURSE_NAME, SERVINGCOURSES_KEY From"
	   " SERVINGCOURSES Where (SERVINGCOURSE_KITCHEN_NAME IS NULL OR SERVINGCOURSE_KITCHEN_NAME = '') Order by 1";
   IBInternalQuery->ExecQuery();
   for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
   {
        ServingCourses->AddObject(UTF8ToUnicodeString((AnsiString)IBInternalQuery->FieldByName("SERVINGCOURSE_NAME")->AsString),
        (System::TObject*)(IBInternalQuery->FieldByName("SERVINGCOURSES_KEY")->AsInteger));
   }

   DBTransaction.Commit();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmPrinterMaintenance::btnDeleteDeviceClick(TObject *Sender)
{
   if (CurrentDeviceKey != 0)
   {
	  AnsiString Query = "Are you sure you wish to delete the terminal." "All printer configurations for this terminal will be lost.\n\n"
		  "If this device is currently active, its printing configurations will be removed and it will cease to print.\n\n"
		  "If the device removed is a PC, restarting MenuMate on that PC will restore the terminal.\n\n"
		  "Remove Palm devices from PalmMate.";

	  if (MessageBox(Query, "Confirmation", MB_YESNO + MB_DEFBUTTON2 + MB_ICONWARNING) == IDYES)
	  {
		 try
		 {
			Database::TDBTransaction DBTransaction(DBControl);
			DBTransaction.StartTransaction();

			TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = "Delete from CONNECTIONS where DEVICE_KEY = :DEVICE_KEY";
			IBInternalQuery->ParamByName("DEVICE_KEY")->AsInteger = CurrentDeviceKey;
			IBInternalQuery->ExecQuery();

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = "Delete from DEVICES where DEVICE_KEY = :DEVICE_KEY";
			IBInternalQuery->ParamByName("DEVICE_KEY")->AsInteger = CurrentDeviceKey;
			IBInternalQuery->ExecQuery();

			DBTransaction.Commit();

			UpdateDeviceDisplay();
		 }
		 catch(Exception & E)
		 {
			TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
			throw;
		 }
	  }
   }
}

void __fastcall TfrmPrinterMaintenance::tbPrinterPropertiesClick(TObject *Sender)
{
   SaveCoursePrinterProfiles();
   std::auto_ptr <TfrmVirtualPrintersInterface> frmVirtualPrintersInterface
	   (TfrmVirtualPrintersInterface::Create <TfrmVirtualPrintersInterface> (this, DBControl));
   frmVirtualPrintersInterface->ShowModal();
   UpdatePrinterDisplay();
}

// ---------------------------------------------------------------------------
void TfrmPrinterMaintenance::DisplayReceiptPrinterInstructionSet()
{
   int lbPrintOptOldIndex = lbReceiptPrintOpt->ItemIndex;
   int lbPrintConfigOldIndex = lbReceiptPrintConfig->ItemIndex;

   while (lbReceiptPrintOpt->Count > 0)
   {
	  delete(TPSectionInstruction*)lbReceiptPrintOpt->Items->Objects[0];
	  lbReceiptPrintOpt->Items->Delete(0);
   }

   TPrintOutFormatInstructions Instructions;
   typeInstructions::iterator itInstructions = Instructions.Instructions.begin();
   for (; itInstructions != Instructions.Instructions.end(); itInstructions++)
   {
	  TPSectionInstruction *InstructionOption = new TPSectionInstruction(itInstructions->second.first);
	  lbReceiptPrintOpt->Items->AddObject(InstructionOption->InstructionCaption, (TObject*)InstructionOption);
   }

   if (lbPrintConfigOldIndex < lbReceiptPrintConfig->Count)
   {
	  lbReceiptPrintConfig->ItemIndex = lbPrintConfigOldIndex;
   }

   if (lbPrintOptOldIndex < lbReceiptPrintOpt->Count)
   {
	  lbReceiptPrintOpt->ItemIndex = lbPrintOptOldIndex;
   }
}

void TfrmPrinterMaintenance::DisplayKitchenPrinterInstructionSet()
{
   int lbPrintConfigOldIndex = lbPrintConfig->ItemIndex;
   int lbPrintOptOldIndex = lbPrintOpt->ItemIndex;

   while (lbPrintOpt->Count > 0)
   {
	  delete(TPSectionInstruction*)lbPrintOpt->Items->Objects[0];
	  lbPrintOpt->Items->Delete(0);
   }

   // Add all Instructions to LHS
   TPrintOutFormatInstructions Instructions;
   typeInstructions::iterator itInstructions = Instructions.Instructions.begin();
   for (; itInstructions != Instructions.Instructions.end(); itInstructions++)
   {
	  TPSectionInstruction *InstructionOption = new TPSectionInstruction(itInstructions->second.first);
	  lbPrintOpt->Items->AddObject(InstructionOption->InstructionCaption, (TObject*)InstructionOption);
   }

   if (lbPrintConfigOldIndex < lbPrintConfig->Count)
   {
	  lbPrintConfig->ItemIndex = lbPrintConfigOldIndex;
   }

   if (lbPrintOptOldIndex < lbPrintOpt->Count)
   {
	  lbPrintOpt->ItemIndex = lbPrintOptOldIndex;
   }
}

void __fastcall TfrmPrinterMaintenance::tbInsertPrintConfigMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if (CurrentPrinterProfileKey != 0)
   {
	  if (lbPrintOpt->ItemIndex > -1)
	  {
		 TPSectionInstruction *Instruction = new TPSectionInstruction();
		 *Instruction = *((TPSectionInstruction*)lbPrintOpt->Items->Objects[lbPrintOpt->ItemIndex]);

		 if (lbPrintConfig->ItemIndex < 0)
		 {
			lbPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);
		 }
		 else
		 {
			lbPrintConfig->Items->InsertObject(lbPrintConfig->ItemIndex + 1, Instruction->Caption, (TObject*)Instruction);
		 }

		 if (lbPrintOpt->ItemIndex < lbPrintOpt->Items->Count - 1)
		 {
			lbPrintOpt->ItemIndex++;
		 }
		 lbPrintConfig->ItemIndex++;
		 KitchenTemplateModified = true;
	  }
	  DrawKitchenDocket();
   }
   else
   {
	  MessageBox("Please select a printer profile.", "Error", MB_ICONERROR + MB_OK);
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::tbRemovePrintConfigMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if (CurrentPrinterProfileKey != 0)
   {
	  if (lbPrintConfig->ItemIndex > -1)
	  {
		 delete(TPSectionInstruction*)lbPrintConfig->Items->Objects[lbPrintConfig->ItemIndex];
		 lbPrintConfig->Items->Delete(lbPrintConfig->ItemIndex);
		 if (lbPrintConfig->ItemIndex < lbPrintConfig->Items->Count - 1)
		 {
			lbPrintConfig->ItemIndex++;
		 }
		 KitchenTemplateModified = true;
	  }
	  DisplayKitchenPrinterInstructionSet();
	  DrawKitchenDocket();
   }
   else
   {
	  MessageBox("Please select a printer profile.", "Error", MB_ICONERROR + MB_OK);
   }
}

// ---------------------------------------------------------------------------
void TfrmPrinterMaintenance::DisplayKitchenTemplate()
{
   while (lbPrintConfig->Count > 0)
   {
	  delete(TPSectionInstruction*)lbPrintConfig->Items->Objects[0];
	  lbPrintConfig->Items->Delete(0);
   }
   lbPrintConfig->Clear();

   if (CurrentPrinterProfileKey != 0)
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

	  TSectionInstructStorage KitchenTemplate;
	  TKitchen::LoadKitchenTemplate(DBTransaction, CurrentPrinterProfileKey, KitchenTemplate);
	  for (int i = 0; i < KitchenTemplate.size(); i++)
	  {
		 TPSectionInstruction *Instruction = new TPSectionInstruction();
		 *Instruction = KitchenTemplate[i];
		 lbPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);
	  }
	  DBTransaction.Commit();
   }
}

void TfrmPrinterMaintenance::SaveKitchenTemplate()
{
   TSectionInstructStorage KitchenTemplate;
   KitchenTemplate.clear();
   for (int i = 0; i < lbPrintConfig->Count; i++)
   {
	  TPSectionInstruction Instruction = *(TPSectionInstruction*)lbPrintConfig->Items->Objects[i];
	  KitchenTemplate.push_back(Instruction);
   }

   if (CurrentPrinterProfileKey != 0)
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();
	  TKitchen::SaveKitchenTemplate(DBTransaction, CurrentPrinterProfileKey, KitchenTemplate);
	  DBTransaction.Commit();
   }
}

void __fastcall TfrmPrinterMaintenance::DrawKitchenDocket()
{
  Database::TDBTransaction DBTransaction(DBControl);
  DBTransaction.StartTransaction();

   if (frmSelectDish->ParkedSales->GetCount(DBTransaction) > 0)
   {

	  TPrinterVirtual *vPrinter = TManagerVirtualPrinter::GetVirtualPrinterByKey(DBTransaction,CurrentPrinterProfileKey);
	  if (vPrinter)
	  {
		 std::vector <TSeatOrders*> SeatOrders;

		 TSeatOrders *Temp = new TSeatOrders(0);
		 SeatOrders.push_back(Temp);
		 for (int i = 1; i < 10; i++)
		 {
			Temp = new TSeatOrders(0);
			SeatOrders.push_back(Temp);
		 }

		 std::auto_ptr <TPaymentTransaction> Transaction(new TPaymentTransaction(DBTransaction));

		 TParkedSale *Sale = (TParkedSale*)frmSelectDish->ParkedSales->GetFirstParkedSale(DBTransaction);
		 Sale->GetSaleCopy(SeatOrders);

		 int SelectedSeat;
		 int SelectedTable;
		 AnsiString SelectedTabContainerName;
		 AnsiString SelectedParty;
		 int SelectedTab = 0;

		 SelectedSeat = Sale->SelectedSeat;
		 SelectedTable = Sale->SelectedTable;
		 SelectedTabContainerName = Sale->SelectedTabContainerName;
		 SelectedParty = Sale->SelectedParty;

		 TDateTime OrderedTimeStamp = Now();

		 for (UINT iSeat = 0; iSeat < SeatOrders.size(); iSeat++)
		 {
			for (int i = 0; i < SeatOrders[iSeat]->Orders->Count; i++)
			{
			   TItemComplete *Order = SeatOrders[iSeat]->Orders->Items[i];
			   Order->Terminal = TDeviceRealTerminal::Instance().ID.Name;
			   // Order->TabKey					= iSeat; // This is faked to get ravel correctly
			   Order->TimeStamp = Now();
			   Order->OrderType = NormalOrder;
			   Order->OrderedLocation = TDeviceRealTerminal::Instance().ID.Location;
			   if (iSeat == 0)
			   {
				  Order->TabName = "Cash Sale";
				  Order->TableNo = 0;
				  Order->SeatNo = 0;
				  Order->TabType = TabNone;
			   }
			   else
			   {
				  Order->TabName = SeatOrders[iSeat]->SeatName;
				  Order->TableNo = SelectedTable;
				  Order->SeatNo = iSeat;
				  Order->TabType = TabTableSeat;
				  Order->TabKey = iSeat;
			   }
			   Transaction->Orders->Add(Order);
			}
		 }

		 if (Transaction->Orders->Count > 0)
		 {
			Display1->Clear();

			std::auto_ptr <TStringList> OrderDisplay(new TStringList());
			OrderDisplay->Clear();

			std::auto_ptr <TReqPrintJob> Request(new TReqPrintJob(&TDeviceRealTerminal::Instance()));

			Request->Transaction = Transaction.get();
			Request->Waiter = "Demo";
			Request->BarCodeData = 1; // So Barcode Section Prints
			Request->MiscData["PartyName"] = SelectedParty;

			TSectionInstructStorage NewKitchenTemplate;
			NewKitchenTemplate.clear();
			for (int i = 0; i < lbPrintConfig->Count; i++)
			{
			   TPSectionInstruction Instruction = *(TPSectionInstruction*)lbPrintConfig->Items->Objects[i];
			   NewKitchenTemplate.push_back(Instruction);
			}
			Request->Transaction->Money.Recalc(*Request->Transaction);

            std::auto_ptr<TKitchen> Kitchen(new TKitchen());
            Kitchen->Initialise(DBTransaction);
			Kitchen->GetPrintoutsPreview(DBTransaction, Request.get(), vPrinter, &NewKitchenTemplate);
			Request->Printouts->PrintToStrings(OrderDisplay.get());

			for (int i = 0; i < OrderDisplay->Count; i++)
			{
			   AnsiString Temp = OrderDisplay->Strings[i];
			   Display1->Lines->Add(Temp);
			}
		 }

		 for (UINT iSeat = 0; iSeat < SeatOrders.size(); iSeat++)
		 {
			for (int i = 0; i < SeatOrders[iSeat]->Orders->Count; i++)
			{
			   TItemComplete *Order = SeatOrders[iSeat]->Orders->Items[i];
			   Order->TabKey = 0;
			}
		 }

		 for (UINT i = 0; i < SeatOrders.size(); i++)
		 {
			delete SeatOrders[i];
		 }
	  }
   }
	DBTransaction.Commit();
}

void __fastcall TfrmPrinterMaintenance::tbKitchenTemplateUpClick(TObject *Sender)
{
   int OldIndex = lbPrintConfig->ItemIndex;
   if (lbPrintConfig->ItemIndex > 0)
   {
	  lbPrintConfig->Items->Move(OldIndex, OldIndex - 1);
   }
   KitchenTemplateModified = true;
   DrawKitchenDocket();
   lbPrintConfig->ItemIndex = OldIndex - 1;
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::tbKitchenTemplateDownClick(TObject *Sender)
{
   int OldIndex = lbPrintConfig->ItemIndex;
   if (lbPrintConfig->ItemIndex != -1 && lbPrintConfig->ItemIndex < lbPrintConfig->Items->Count - 1)
   {
	  lbPrintConfig->Items->Move(OldIndex, OldIndex + 1);
   }
   KitchenTemplateModified = true;
   DrawKitchenDocket();
   lbPrintConfig->ItemIndex = OldIndex + 1;
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::tbPropertiesMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{

   if (lbPrintConfig->ItemIndex != -1 && lbPrintConfig->ItemIndex < lbPrintConfig->Items->Count)
   {
	  TPSectionInstruction OldInstruction = *(TPSectionInstruction*)lbPrintConfig->Items->Objects[lbPrintConfig->ItemIndex];

	  std::auto_ptr <TfrmPSectionInstructionEdit> frmPSectionInstructionEdit
		  (TfrmPSectionInstructionEdit::Create <TfrmPSectionInstructionEdit> (this));

	  frmPSectionInstructionEdit->AfterPropertyChanged.RegisterForEvent(OnKitchenPropertyChanged);

	  frmPSectionInstructionEdit->Instruction = (TPSectionInstruction*)lbPrintConfig->Items->Objects[lbPrintConfig->ItemIndex];
	  if (frmPSectionInstructionEdit->ShowModal() != mrOk)
	  {
		 *(TPSectionInstruction*)lbPrintConfig->Items->Objects[lbPrintConfig->ItemIndex] = OldInstruction;
		 DrawKitchenDocket();
	  }
	  else
	  {
		 KitchenTemplateModified = true;
	  }
	  frmPSectionInstructionEdit->AfterPropertyChanged.DeregisterForEvent(OnKitchenPropertyChanged);
	  DrawKitchenDocket();
   }
}
// ---------------------------------------------------------------------------

void TfrmPrinterMaintenance::OnKitchenPropertyChanged(TSystemEvents *Sender)
{
   DrawKitchenDocket();
}

void TfrmPrinterMaintenance::OnReceiptPropertyChanged(TSystemEvents *Sender)
{
   DrawReceiptDocket();
}

void __fastcall TfrmPrinterMaintenance::tbPrintToClick(TObject *Sender)
{
   try
   {
    Database::TDBTransaction DBTransaction(DBControl);
    DBTransaction.StartTransaction();

	  if (frmSelectDish->ParkedSales->GetCount(DBTransaction) > 0)
	  {

		 std::vector <TSeatOrders*> SeatOrders;

		 TSeatOrders *Temp = new TSeatOrders(0);
		 SeatOrders.push_back(Temp);
		 for (int i = 1; i < 10; i++)
		 {
			Temp = new TSeatOrders(0);
			SeatOrders.push_back(Temp);
		 }

		 std::auto_ptr <TPaymentTransaction> Transaction(new TPaymentTransaction(DBTransaction));

		 TParkedSale *Sale = (TParkedSale*)frmSelectDish->ParkedSales->GetFirstParkedSale(DBTransaction);
		 Sale->GetSaleCopy(SeatOrders);

		 int SelectedSeat;
		 int SelectedTable;
		 AnsiString SelectedTabContainerName;
		 AnsiString SelectedParty;
		 int SelectedTab = 0;

		 SelectedSeat = Sale->SelectedSeat;
		 SelectedTable = Sale->SelectedTable;
		 SelectedTabContainerName = Sale->SelectedTabContainerName;
		 SelectedParty = Sale->SelectedParty;

		 TDateTime OrderedTimeStamp = Now();

		 for (UINT iSeat = 0; iSeat < SeatOrders.size(); iSeat++)
		 {
			for (int i = 0; i < SeatOrders[iSeat]->Orders->Count; i++)
			{
			   TItemComplete *Order = SeatOrders[iSeat]->Orders->Items[i];
			   Order->Terminal = TDeviceRealTerminal::Instance().ID.Name;
			   // Order->TabKey					= iSeat; // This is faked to get ravel correctly
			   Order->TimeStamp = Now();
			   Order->OrderType = NormalOrder;
			   Order->OrderedLocation = TDeviceRealTerminal::Instance().ID.Location;
			   if (iSeat == 0)
			   {
				  Order->TabName = "Cash Sale";
				  Order->TableNo = 0;
				  Order->SeatNo = 0;
				  Order->TabType = TabNone;
			   }
			   else
			   {
				  Order->TabName = SeatOrders[iSeat]->SeatName;
				  Order->TableNo = SelectedTable;
				  Order->SeatNo = iSeat;
				  Order->TabType = TabTableSeat;
				  Order->TabKey = iSeat;
			   }
			   Transaction->Orders->Add(Order);
			}
		 }

		 if (Transaction->Orders->Count > 0)
		 {
			Display1->Clear();
			TReqPrintJob *Request = new TReqPrintJob(&TDeviceRealTerminal::Instance());
			Request->Transaction = Transaction.get();
			Request->Waiter = "Demo";
			Request->BarCodeData = 1; // So Barcode Section Prints
			TSectionInstructStorage NewKitchenTemplate;
			NewKitchenTemplate.clear();
			for (int i = 0; i < lbPrintConfig->Count; i++)
			{
			   TPSectionInstruction Instruction = *(TPSectionInstruction*)lbPrintConfig->Items->Objects[i];
			   NewKitchenTemplate.push_back(Instruction);
			}
			Request->Transaction->Money.Recalc(*Request->Transaction);
            std::auto_ptr<TKitchen> Kitchen(new TKitchen());
            Kitchen->Initialise(DBTransaction);
			Kitchen->GetPrintouts(DBTransaction, Request, pfAll_Printers, &NewKitchenTemplate, CurrentPrinterProfileKey);

			// Forces Redirects.
			Request->Printouts->ClearPrinters();
			Request->Printouts->Print(devPC);

			delete Request;
		 }

		 for (UINT iSeat = 0; iSeat < SeatOrders.size(); iSeat++)
		 {
			for (int i = 0; i < SeatOrders[iSeat]->Orders->Count; i++)
			{
			   TItemComplete *Order = SeatOrders[iSeat]->Orders->Items[i];
			   Order->TabKey = 0;
			}
		 }

		 for (UINT i = 0; i < SeatOrders.size(); i++)
		 {
			delete SeatOrders[i];
		 }

	  }
	  DBTransaction.Commit();
   }
   __finally
   {
   }
   DrawKitchenDocket();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::edPerSeatMultiplierMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();
   std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
   frmTouchKeyboard->MaxLength = 255;
   frmTouchKeyboard->AllowCarriageReturn = false;
   frmTouchKeyboard->StartWithShiftDown = false;
   frmTouchKeyboard->KeyboardText = TManagerVariable::Instance().GetStr(DBTransaction, vmKitchenPerSeatMultiplerString, " on ");
   frmTouchKeyboard->Caption = "Enter the per seat multiplier ( 3 on 4 ).";
   if (frmTouchKeyboard->ShowModal() == mrOk)
   {
	  TManagerVariable::Instance().SetDeviceStr(DBTransaction, vmKitchenPerSeatMultiplerString, frmTouchKeyboard->KeyboardText);
	  edPerSeatMultiplier->Text = frmTouchKeyboard->KeyboardText;
	  DrawKitchenDocket();
   }
   DBTransaction.Commit();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::edNoteHeaderMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();
   std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
   frmTouchKeyboard->MaxLength = 255;
   frmTouchKeyboard->AllowCarriageReturn = false;
   frmTouchKeyboard->StartWithShiftDown = false;
   frmTouchKeyboard->KeyboardText = TManagerVariable::Instance().GetStr(DBTransaction, vmKitchenHeaderNote, "Note: ");
   frmTouchKeyboard->Caption = "Enter the header that precedes a note.";
   if (frmTouchKeyboard->ShowModal() == mrOk)
   {
	  TManagerVariable::Instance().SetDeviceStr(DBTransaction, vmKitchenHeaderNote, frmTouchKeyboard->KeyboardText);
	  edNoteHeader->Text = frmTouchKeyboard->KeyboardText;
	  DrawKitchenDocket();
   }
   DBTransaction.Commit();
}
// ---------------------------------------------------------------------------

#pragma warn -8057

void __fastcall TfrmPrinterMaintenance::edSidesHeaderMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();
   std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
   frmTouchKeyboard->MaxLength = 255;
   frmTouchKeyboard->AllowCarriageReturn = false;
   frmTouchKeyboard->StartWithShiftDown = false;
   frmTouchKeyboard->KeyboardText = TManagerVariable::Instance().GetStr(DBTransaction, vmKitchenHeaderSide, "  Sides: ");
   frmTouchKeyboard->Caption = "Enter the header that precedes the sides.";
   if (frmTouchKeyboard->ShowModal() == mrOk)
   {
	  TManagerVariable::Instance().SetDeviceStr(DBTransaction, vmKitchenHeaderSide, frmTouchKeyboard->KeyboardText);
	  edSidesHeader->Text = frmTouchKeyboard->KeyboardText;
	  DrawKitchenDocket();
   }
   DBTransaction.Commit();

}

#pragma warn +8057

// ---------------------------------------------------------------------------

#pragma warn -8057

void __fastcall TfrmPrinterMaintenance::edMixHeaderMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();
   std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
   frmTouchKeyboard->MaxLength = 255;
   frmTouchKeyboard->AllowCarriageReturn = false;
   frmTouchKeyboard->StartWithShiftDown = false;
   frmTouchKeyboard->KeyboardText = TManagerVariable::Instance().GetStr(DBTransaction, vmKitchenHeaderMix, "  Serve In: ");
   frmTouchKeyboard->Caption = "Enter the header that precedes the beverage mix.";
   if (frmTouchKeyboard->ShowModal() == mrOk)
   {
	  TManagerVariable::Instance().SetDeviceStr(DBTransaction, vmKitchenHeaderMix, frmTouchKeyboard->KeyboardText);
	  edMixHeader->Text = frmTouchKeyboard->KeyboardText;
	  DrawKitchenDocket();
   }
   DBTransaction.Commit();
}

#pragma warn +8057

// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::edSideBulletMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();
   std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
   frmTouchKeyboard->MaxLength = 255;
   frmTouchKeyboard->AllowCarriageReturn = false;
   frmTouchKeyboard->StartWithShiftDown = false;
   frmTouchKeyboard->KeyboardText = TManagerVariable::Instance().GetStr(DBTransaction, vmKitchenBulletSide, " > ");
   frmTouchKeyboard->Caption = "Enter the Bullet to go in front of all sides.";
   if (frmTouchKeyboard->ShowModal() == mrOk)
   {
	  TManagerVariable::Instance().SetDeviceStr(DBTransaction, vmKitchenBulletSide, frmTouchKeyboard->KeyboardText);
	  edSideBullet->Text = frmTouchKeyboard->KeyboardText;
	  DrawKitchenDocket();
   }
   DBTransaction.Commit();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::edOptionsBulletMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();
   std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
   frmTouchKeyboard->MaxLength = 255;
   frmTouchKeyboard->AllowCarriageReturn = false;
   frmTouchKeyboard->StartWithShiftDown = false;
   frmTouchKeyboard->KeyboardText = TManagerVariable::Instance().GetStr(DBTransaction, vmKitchenBulletOpt, "    ");
   frmTouchKeyboard->Caption = "Enter the Bullet to go in front of all Options.";
   if (frmTouchKeyboard->ShowModal() == mrOk)
   {
	  TManagerVariable::Instance().SetDeviceStr(DBTransaction, vmKitchenBulletOpt, frmTouchKeyboard->KeyboardText);
	  edOptionsBullet->Text = frmTouchKeyboard->KeyboardText;
	  DrawKitchenDocket();
   }
   DBTransaction.Commit();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::lbPrintersClick(TObject *Sender)
{
   lbPrinterModels->ItemIndex = -1;
   ShowPrinterDetails();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::btnSelectPrinterTypeClick(TObject *Sender)
{
   ShowPrinterDetails();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::btnNormalFontUpMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
   int CurrentCPL = StrToInt(pnlNormalFontCPL->Caption);
   if (Sender == btnNormalFontUp)
   {
	  if (CurrentCPL < PRINTER_WIDTH_MAX)
		 CurrentCPL++;
   }
   else if (Sender == btnNormalFontDown)
   {
	  if (CurrentCPL > PRINTER_WIDTH_MIN)
		 CurrentCPL--;
   }
   pnlNormalFontCPL->Caption = IntToStr(CurrentCPL);

   if (lbPrinters->ItemIndex != -1)
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

	  TManagerPhysicalPrinter ManagerPhysicalPrinter;
	  TPrinterPhysical Printer = ManagerPhysicalPrinter.GetPhysicalPrinter(DBTransaction,
		 (int)(lbPrinters->Items->Objects[lbPrinters->ItemIndex]));
	  if (Printer.PhysicalPrinterKey != 0)
	  {
		 if (Printer.PhysicalProfileKey == 0)
		 {
			Printer.PhysicalProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction, eEquipmentProfiles,
			   Printer.ServerName + Printer.ShareName);
		 }
		 TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmNormalCPL, CurrentCPL);
		 TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmPrinterModel, 1);
		 EventPhysicalPropertiesChanged.Occured();
	  }
	  DBTransaction.Commit();
	  lbPrinterModels->ItemIndex = -1;
	  ShowPrinterDetails();
   }
   else
   {
	  Application->MessageBox(UnicodeString("Please Select a Printer.").w_str(), UnicodeString("Information").w_str(),
		 MB_OK + MB_ICONINFORMATION);
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::btnBoldFontUpMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
   int CurrentCPL = StrToInt(pnlBoldFontCPL->Caption);
   if (Sender == btnBoldFontUp)
   {
	  if (CurrentCPL < PRINTER_WIDTH_MAX)
		 CurrentCPL++;
   }
   else if (Sender == btnBoldFontDown)
   {
	  if (CurrentCPL > PRINTER_WIDTH_MIN)
		 CurrentCPL--;
   }
   pnlBoldFontCPL->Caption = IntToStr(CurrentCPL);

   if (lbPrinters->ItemIndex != -1)
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();
	  TManagerPhysicalPrinter ManagerPhysicalPrinter;
	  TPrinterPhysical Printer = ManagerPhysicalPrinter.GetPhysicalPrinter(DBTransaction,
		 (int)(lbPrinters->Items->Objects[lbPrinters->ItemIndex]));
	  if (Printer.PhysicalPrinterKey != 0)
	  {
		 if (Printer.PhysicalProfileKey == 0)
		 {
			Printer.PhysicalProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction, eEquipmentProfiles,
			   Printer.ServerName + Printer.ShareName);
		 }
		 TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmNormalBoldCPL, CurrentCPL);
		 TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmPrinterModel, 1);
		 EventPhysicalPropertiesChanged.Occured();
	  }
	  DBTransaction.Commit();
	  lbPrinterModels->ItemIndex = -1;
	  ShowPrinterDetails();
   }
   else
   {
	  MessageBox(_T("Please Select a Printer."), _T("Information"), MB_OK + MB_ICONINFORMATION);
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::btnCutterGapUpMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
   int CurrentGap = StrToInt(pnlCutterGap->Caption);
   if (Sender == btnCutterGapUp)
   {
	  if (CurrentGap < 100)
		 CurrentGap++;
   }
   else if (Sender == btnCutterGapDown)
   {
	  if (CurrentGap > 0)
		 CurrentGap--;
   }
   pnlCutterGap->Caption = IntToStr(CurrentGap);

   if (lbPrinters->ItemIndex != -1)
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

	  TManagerPhysicalPrinter ManagerPhysicalPrinter;
	  TPrinterPhysical Printer = ManagerPhysicalPrinter.GetPhysicalPrinter(DBTransaction,
		 (int)(lbPrinters->Items->Objects[lbPrinters->ItemIndex]));
	  if (Printer.PhysicalPrinterKey != 0)
	  {
		 if (Printer.PhysicalProfileKey == 0)
		 {
			Printer.PhysicalProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction, eEquipmentProfiles,
			   Printer.ServerName + Printer.ShareName);
		 }
		 TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmCutterGap, CurrentGap);
		 TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmPrinterModel, 1);
		 EventPhysicalPropertiesChanged.Occured();
	  }
	  DBTransaction.Commit();
	  lbPrinterModels->ItemIndex = -1;
	  ShowPrinterDetails();
   }
   else
   {
	  MessageBox(_T("Please Select a Printer."), _T("Information"), MB_OK + MB_ICONINFORMATION);
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::btnTopMarginUpMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
   int CurrentTop = StrToInt(pnlTopMargin->Caption);
   if (Sender == btnTopMarginUp)
   {
	  if (CurrentTop < 10)
		 CurrentTop++;
   }
   else if (Sender == btnTopMarginDown)
   {
	  if (CurrentTop > 0)
		 CurrentTop--;
   }
   pnlTopMargin->Caption = IntToStr(CurrentTop);

   if (lbPrinters->ItemIndex != -1)
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

	  TManagerPhysicalPrinter ManagerPhysicalPrinter;
	  TPrinterPhysical Printer = ManagerPhysicalPrinter.GetPhysicalPrinter(DBTransaction,
		 (int)(lbPrinters->Items->Objects[lbPrinters->ItemIndex]));
	  if (Printer.PhysicalPrinterKey != 0)
	  {
		 if (Printer.PhysicalProfileKey == 0)
		 {
			Printer.PhysicalProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction, eEquipmentProfiles,
			   Printer.ServerName + Printer.ShareName);
		 }
		 TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmTopMargin, CurrentTop);
		 TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmPrinterModel, 1);
		 EventPhysicalPropertiesChanged.Occured();
	  }
	  DBTransaction.Commit();
	  lbPrinterModels->ItemIndex = -1;
	  ShowPrinterDetails();
   }
   else
   {
	  MessageBox(_T("Please Select a Printer."), _T("Information"), MB_OK + MB_ICONINFORMATION);
   }

}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::btnKitchenTopMarginUpMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
   int Y)
{
   int CurrentTop = StrToInt(pnlKitchenTopMargin->Caption);
   if (Sender == btnKitchenTopMarginUp)
   {
	  if (CurrentTop < 10)
		 CurrentTop++;
   }
   else if (Sender == btnKitchenTopMarginDown)
   {
	  if (CurrentTop > 0)
		 CurrentTop--;
   }
   pnlKitchenTopMargin->Caption = IntToStr(CurrentTop);

   if (lbPrinters->ItemIndex != -1)
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

	  TManagerPhysicalPrinter ManagerPhysicalPrinter;
	  TPrinterPhysical Printer = ManagerPhysicalPrinter.GetPhysicalPrinter(DBTransaction,
		 (int)(lbPrinters->Items->Objects[lbPrinters->ItemIndex]));
	  if (Printer.PhysicalPrinterKey != 0)
	  {
		 if (Printer.PhysicalProfileKey == 0)
		 {
			Printer.PhysicalProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction, eEquipmentProfiles,
			   Printer.ServerName + Printer.ShareName);
		 }
		 TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmKitchenTopMargin, CurrentTop);
		 TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmPrinterModel, 1);
		 EventPhysicalPropertiesChanged.Occured();
	  }
	  DBTransaction.Commit();
	  lbPrinterModels->ItemIndex = -1;
	  ShowPrinterDetails();
   }
   else
   {
	  MessageBox(_T("Please Select a Printer."), _T("Information"), MB_OK + MB_ICONINFORMATION);
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::btnNormalFontDownMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
   int CurrentCPL = StrToInt(pnlNormalFontCPL->Caption);
   if (Sender == btnNormalFontUp)
   {
	  if (CurrentCPL < PRINTER_WIDTH_MAX)
		 CurrentCPL++;
   }
   else if (Sender == btnNormalFontDown)
   {
	  if (CurrentCPL > PRINTER_WIDTH_MIN)
		 CurrentCPL--;
   }
   pnlNormalFontCPL->Caption = IntToStr(CurrentCPL);

   if (lbPrinters->ItemIndex != -1)
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

	  TManagerPhysicalPrinter ManagerPhysicalPrinter;
	  TPrinterPhysical Printer = ManagerPhysicalPrinter.GetPhysicalPrinter(DBTransaction,
		 (int)(lbPrinters->Items->Objects[lbPrinters->ItemIndex]));
	  if (Printer.PhysicalPrinterKey != 0)
	  {
		 if (Printer.PhysicalProfileKey == 0)
		 {
			Printer.PhysicalProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction, eEquipmentProfiles,
			   Printer.ServerName + Printer.ShareName);
		 }
		 TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmNormalCPL, CurrentCPL);
		 TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmPrinterModel, 1);
		 EventPhysicalPropertiesChanged.Occured();
	  }
	  DBTransaction.Commit();
	  lbPrinterModels->ItemIndex = -1;
	  ShowPrinterDetails();
   }
   else
   {
	  MessageBox(_T("Please Select a Printer."), _T("Information"), MB_OK + MB_ICONINFORMATION);
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::btnBoldFontDownMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
   int CurrentCPL = StrToInt(pnlBoldFontCPL->Caption);
   if (Sender == btnBoldFontUp)
   {
	  if (CurrentCPL < PRINTER_WIDTH_MAX)
		 CurrentCPL++;
   }
   else if (Sender == btnBoldFontDown)
   {
	  if (CurrentCPL > PRINTER_WIDTH_MIN)
		 CurrentCPL--;
   }
   pnlBoldFontCPL->Caption = IntToStr(CurrentCPL);

   if (lbPrinters->ItemIndex != -1)
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

	  TManagerPhysicalPrinter ManagerPhysicalPrinter;
	  TPrinterPhysical Printer = ManagerPhysicalPrinter.GetPhysicalPrinter(DBTransaction,
		 (int)(lbPrinters->Items->Objects[lbPrinters->ItemIndex]));
	  if (Printer.PhysicalPrinterKey != 0)
	  {
		 if (Printer.PhysicalProfileKey == 0)
		 {
			Printer.PhysicalProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction, eEquipmentProfiles,
			   Printer.ServerName + Printer.ShareName);
		 }
		 TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmNormalBoldCPL, CurrentCPL);
		 TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmPrinterModel, 1);
		 EventPhysicalPropertiesChanged.Occured();
	  }
	  DBTransaction.Commit();
	  lbPrinterModels->ItemIndex = -1;
	  ShowPrinterDetails();
   }
   else
   {
	  MessageBox(_T("Please Select a Printer."), _T("Information"), MB_OK + MB_ICONINFORMATION);
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::btnCutterGapDownMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
   int CurrentGap = StrToInt(pnlCutterGap->Caption);
   if (Sender == btnCutterGapUp)
   {
	  if (CurrentGap < 100)
		 CurrentGap++;
   }
   else if (Sender == btnCutterGapDown)
   {
	  if (CurrentGap > 0)
		 CurrentGap--;
   }
   pnlCutterGap->Caption = IntToStr(CurrentGap);

   if (lbPrinters->ItemIndex != -1)
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

	  TManagerPhysicalPrinter ManagerPhysicalPrinter;
	  TPrinterPhysical Printer = ManagerPhysicalPrinter.GetPhysicalPrinter(DBTransaction,
		 (int)(lbPrinters->Items->Objects[lbPrinters->ItemIndex]));
	  if (Printer.PhysicalPrinterKey != 0)
	  {
		 if (Printer.PhysicalProfileKey == 0)
		 {
			Printer.PhysicalProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction, eEquipmentProfiles,
			   Printer.ServerName + Printer.ShareName);
		 }
		 TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmCutterGap, CurrentGap);
		 TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmPrinterModel, 1);
		 EventPhysicalPropertiesChanged.Occured();
	  }
	  DBTransaction.Commit();
	  lbPrinterModels->ItemIndex = -1;
	  ShowPrinterDetails();
   }
   else
   {
	  MessageBox(_T("Please Select a Printer."), _T("Information"), MB_OK + MB_ICONINFORMATION);
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::btnTopMarginDownMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
   int CurrentTop = StrToInt(pnlTopMargin->Caption);
   if (Sender == btnTopMarginUp)
   {
	  if (CurrentTop < 10)
		 CurrentTop++;
   }
   else if (Sender == btnTopMarginDown)
   {
	  if (CurrentTop > 0)
		 CurrentTop--;
   }
   pnlTopMargin->Caption = IntToStr(CurrentTop);

   if (lbPrinters->ItemIndex != -1)
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

	  TManagerPhysicalPrinter ManagerPhysicalPrinter;
	  TPrinterPhysical Printer = ManagerPhysicalPrinter.GetPhysicalPrinter(DBTransaction,
		 (int)(lbPrinters->Items->Objects[lbPrinters->ItemIndex]));
	  if (Printer.PhysicalPrinterKey != 0)
	  {
		 if (Printer.PhysicalProfileKey == 0)
		 {
			Printer.PhysicalProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction, eEquipmentProfiles,
			   Printer.ServerName + Printer.ShareName);
		 }
		 TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmTopMargin, CurrentTop);
		 TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmPrinterModel, 1);
		 EventPhysicalPropertiesChanged.Occured();
	  }
	  DBTransaction.Commit();
	  lbPrinterModels->ItemIndex = -1;
	  ShowPrinterDetails();
   }
   else
   {
	  MessageBox(_T("Please Select a Printer."), _T("Information"), MB_OK + MB_ICONINFORMATION);
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::btnKitchenTopMarginDownMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
   int Y)
{
   int CurrentTop = StrToInt(pnlKitchenTopMargin->Caption);
   if (Sender == btnKitchenTopMarginUp)
   {
	  if (CurrentTop < 10)
		 CurrentTop++;
   }
   else if (Sender == btnKitchenTopMarginDown)
   {
	  if (CurrentTop > 0)
		 CurrentTop--;
   }
   pnlKitchenTopMargin->Caption = IntToStr(CurrentTop);

   if (lbPrinters->ItemIndex != -1)
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

	  TManagerPhysicalPrinter ManagerPhysicalPrinter;
	  TPrinterPhysical Printer = ManagerPhysicalPrinter.GetPhysicalPrinter(DBTransaction,
		 (int)(lbPrinters->Items->Objects[lbPrinters->ItemIndex]));
	  if (Printer.PhysicalPrinterKey != 0)
	  {
		 if (Printer.PhysicalProfileKey == 0)
		 {
			Printer.PhysicalProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction, eEquipmentProfiles,
			   Printer.ServerName + Printer.ShareName);
		 }
		 TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmKitchenTopMargin, CurrentTop);
		 TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmPrinterModel, 1);
		 EventPhysicalPropertiesChanged.Occured();
	  }
	  DBTransaction.Commit();
	  lbPrinterModels->ItemIndex = -1;
	  ShowPrinterDetails();
   }
   else
   {
	  MessageBox(_T("Please Select a Printer."), _T("Information"), MB_OK + MB_ICONINFORMATION);
   }
}
// ---------------------------------------------------------------------------

void TfrmPrinterMaintenance::ShowPrinterDetails()
{
   if (lbPrinters->ItemIndex != -1)
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

	  TManagerPhysicalPrinter ManagerPhysicalPrinter;
	  TPrinterPhysical Printer = ManagerPhysicalPrinter.GetPhysicalPrinter(DBTransaction,
		 (int)(lbPrinters->Items->Objects[lbPrinters->ItemIndex]));
	  if (Printer.PhysicalPrinterKey != 0)
	  {
		 if (Printer.PrinterModel != 0)
		 {
			lbPrinterModels->ItemIndex = -1;
			for (int i = 0; i < lbPrinterModels->Items->Count; i++)
			{
			   TPrinterModelSettings *PrinterModelSettings = (TPrinterModelSettings*)lbPrinterModels->Items->Objects[i];
			   if (PrinterModelSettings->PrinterModel == Printer.PrinterModel)
			   {
				  lbPrinterModels->ItemIndex = i;
			   }
			}

			pnlNormalFontCPL->Caption = Printer.NormalCharPerLine;
			pnlBoldFontCPL->Caption = Printer.BoldCharPerLine;
			pnlCutterGap->Caption = Printer.CutterGap;
			pnlTopMargin->Caption = Printer.TopMargin;
			pnlKitchenTopMargin->Caption = Printer.KitchenTopMargin;
			rbFullCut->Checked = Printer.AlwaysUseFullCut;
			rbPartialCut->Checked = !Printer.AlwaysUseFullCut;
		 }
		 else
		 {
			lbPrinterModels->ItemIndex = -1;
		 }
	  }
	  DBTransaction.Commit();
   }
}
// ---------------------------------------------------------------------------

void TfrmPrinterMaintenance::UpdatePhysicalPrinterList(Database::TDBTransaction &DBTransaction)
{
   lbPrinters->Clear();
   TManagerPhysicalPrinter ManagerPhysicalPrinter;
   ManagerPhysicalPrinter.DBGetPrinterList(DBTransaction, lbPrinters->Items);
   if (lbPrinters->Items->Count > 0)
	  lbPrinters->ItemIndex = 0;
   lbPrintersClick(NULL);
}

void TfrmPrinterMaintenance::OnNewMenus(TSystemEvents *Sender)
{
   if (pgControl->ActivePage == tsPrinting)
   {
	  MessageBox("The menus have changed and need to be reloaded.", "Warning", MB_ICONWARNING + MB_OK);
	  PrinterProfileCourses.clear();
	  UpdatePrinterDisplay();
	  UpdateMenuDisplay();
   }
}

void TfrmPrinterMaintenance::OnPhysicalPropertiesChanged(TSystemEvents *Sender)
{
   // Update the Current Receipt Printer if its the one that has changed.
   TManagerPhysicalPrinter ManagerPhysicalPrinter;
   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();
   TPrinterPhysical Printer = ManagerPhysicalPrinter.GetPhysicalPrinter(DBTransaction,
	  (int)(lbPrinters->Items->Objects[lbPrinters->ItemIndex]));
   DBTransaction.Commit();
   if (Printer.PhysicalPrinterKey != 0 && Printer.PhysicalPrinterKey == TComms::Instance().ReceiptPrinter.PhysicalPrinterKey)
   {
	  TComms::Instance().ReceiptPrinter = Printer;
   }
}

void TfrmPrinterMaintenance::SaveCurrentServingCourseSettings()
{
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  // Remove All Printer Connections For this Menu.
	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "Delete from PRNSERVINGCOURSEORDER;";
	  IBInternalQuery->ExecQuery();

	  for (std::map <int, std::vector <int> > ::iterator ptrProfile = PrinterProfileServingCourses.begin();
		 ptrProfile != PrinterProfileServingCourses.end(); std::advance(ptrProfile, 1))
	  {
		 for (std::vector <int> ::iterator ptrServingCourseKey = ptrProfile->second.begin();
			ptrServingCourseKey != ptrProfile->second.end(); std::advance(ptrServingCourseKey, 1))
		 {
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_PRNSERVINGCOURSEORDER , 1) FROM RDB$DATABASE";
			IBInternalQuery->ExecQuery();
			int LocalKey = IBInternalQuery->Fields[0]->AsInteger;

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
				"INSERT INTO PRNSERVINGCOURSEORDER (" "PRNSERVINGCOURSEORDER_KEY, "
				"VIRTUALPRINTER_KEY, " "SERVINGCOURSES_KEY) " "VALUES (" ":PRNSERVINGCOURSEORDER_KEY, "
				":VIRTUALPRINTER_KEY, " ":SERVINGCOURSES_KEY); ";
			IBInternalQuery->ParamByName("PRNSERVINGCOURSEORDER_KEY")->AsInteger = LocalKey;
			IBInternalQuery->ParamByName("VIRTUALPRINTER_KEY")->AsInteger = ptrProfile->first;
			IBInternalQuery->ParamByName("SERVINGCOURSES_KEY")->AsInteger = *ptrServingCourseKey;
			IBInternalQuery->ExecQuery();
		 }
	  }
	  DBTransaction.Commit();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
   }
}

// ---------------------------------------------------------------------------
void __fastcall TfrmPrinterMaintenance::tbtnOkMouseClick(TObject *Sender)
{
   Close();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::tgridMenuMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift,
   TGridButton *GridButton)
{
   try
   {
	  CurrentMenuKey = GridButton->Tag;
	  GridButton->Latched = true;
	  UpdateCourseDisplay();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::tgridCourseMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift,
   TGridButton *GridButton)
{
   try
   {
	  if (CurrentPrinterProfileKey != 0 && CurrentMenuKey != 0)
	  {
		 std::vector <int> ::iterator ptrCourse;
		 ptrCourse = std::find(PrinterProfileCourses[CurrentPrinterProfileKey][CurrentMenuKey].begin(),
			PrinterProfileCourses[CurrentPrinterProfileKey][CurrentMenuKey].end(), GridButton->Tag);
		 if (ptrCourse == PrinterProfileCourses[CurrentPrinterProfileKey][CurrentMenuKey].end())
		 { // Not Found.
			PrinterProfileCourses[CurrentPrinterProfileKey][CurrentMenuKey].push_back(GridButton->Tag);
			GridButton->Latched = true;
		 }
		 else
		 {
			PrinterProfileCourses[CurrentPrinterProfileKey][CurrentMenuKey].erase(ptrCourse);
			GridButton->Latched = false;
		 }
		 UpdateCourseDisplay();
	  }
	  else
	  {
		 MessageBox("Please Select a Printer Profile.", "Error", MB_ICONERROR | MB_OK);
		 GridButton->Latched = false;
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}
// ---------------------------------------------------------------------------

void TfrmPrinterMaintenance::GetPrinterProfileCourses(int VirtualPrinterKey, std::map <int, std::vector <int> > &MenuCourseList)
{
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text =
		  "SELECT MENU_KEY,COURSE_KEY,PRNORD_KEY FROM PRNORDER WHERE VIRTUALPRINTER_KEY = :VIRTUALPRINTER_KEY"
		  " GROUP BY MENU_KEY, COURSE_KEY,PRNORD_KEY ORDER BY PRNORD_KEY";
	  IBInternalQuery->ParamByName("VIRTUALPRINTER_KEY")->AsInteger = VirtualPrinterKey;
	  IBInternalQuery->ExecQuery();
	  for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	  {
		 MenuCourseList[IBInternalQuery->FieldByName("MENU_KEY")->AsInteger].push_back
			 (IBInternalQuery->FieldByName("COURSE_KEY")->AsInteger);
	  }
	  DBTransaction.Commit();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

void TfrmPrinterMaintenance::GetPrinterProfileBreakDown(int VirtualPrinterKey,  std::map <int, std::vector <int> > &List)
{
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text =
		  "SELECT PRNCAT_KEY, VIRTUALPRINTER_KEY, CATEGORY_KEY, PRNORD FROM PRNCAT WHERE VIRTUALPRINTER_KEY = :VIRTUALPRINTER_KEY"
		  " ORDER BY PRNORD";
	  IBInternalQuery->ParamByName("VIRTUALPRINTER_KEY")->AsInteger = VirtualPrinterKey;
	  IBInternalQuery->ExecQuery();
	  for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	  {
		 List[IBInternalQuery->FieldByName("VIRTUALPRINTER_KEY")->AsInteger].push_back
			 (IBInternalQuery->FieldByName("CATEGORY_KEY")->AsInteger);
	  }
	  DBTransaction.Commit();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

void TfrmPrinterMaintenance::GetPrinterProfileServingCourses(int VirtualPrinterKey, std::map <int,
   std::vector <int> > &PrinterServingCourse)
{
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text =
		  "SELECT VIRTUALPRINTER_KEY,SERVINGCOURSES_KEY"
		  " FROM PRNSERVINGCOURSEORDER" " WHERE VIRTUALPRINTER_KEY = :VIRTUALPRINTER_KEY";
	  IBInternalQuery->ParamByName("VIRTUALPRINTER_KEY")->AsInteger = VirtualPrinterKey;
	  IBInternalQuery->ExecQuery();
	  for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	  {
		 PrinterServingCourse[IBInternalQuery->FieldByName("VIRTUALPRINTER_KEY")->AsInteger].push_back
			 (IBInternalQuery->FieldByName("SERVINGCOURSES_KEY")->AsInteger);
	  }
	  DBTransaction.Commit();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

void TfrmPrinterMaintenance::GetCourseInfo(Database::TDBTransaction &DBTransaction)
{
   try
   {
	  CourseInfo.clear();

	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT MENU_KEY,COURSE_KEY,COURSE_NAME,COURSE_KITCHEN_NAME FROM COURSE";
	  IBInternalQuery->ExecQuery();

	  for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	  {
		 TSUCouseInfo TempCourseInfo;
		 TempCourseInfo.CourseKey = IBInternalQuery->FieldByName("COURSE_KEY")->AsInteger;

		 TempCourseInfo.CourseName = IBInternalQuery->FieldByName("COURSE_NAME")->AsString;
		 TempCourseInfo.CourseKitchenName = UTF8ToUnicodeString((AnsiString)IBInternalQuery->FieldByName("COURSE_KITCHEN_NAME")->AsString);
		 if (TempCourseInfo.CourseKitchenName == UnicodeString(""))
		 {
			TempCourseInfo.CourseKitchenName = TempCourseInfo.CourseName;
		 }

		 TempCourseInfo.MenuKey = IBInternalQuery->FieldByName("MENU_KEY")->AsInteger;

		 CourseInfo[IBInternalQuery->FieldByName("COURSE_KEY")->AsInteger] = TempCourseInfo;
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

void __fastcall TfrmPrinterMaintenance::tbtnSelectAllMouseClick(TObject *Sender)
{
   if (CurrentPrinterProfileKey != 0 && CurrentMenuKey != 0)
   {
	  for (int i = 0; i < tgridCourse->RowCount; i++)
	  {
		 std::vector <int> ::iterator ptrCourse;
		 ptrCourse = std::find(PrinterProfileCourses[CurrentPrinterProfileKey][CurrentMenuKey].begin(),
			PrinterProfileCourses[CurrentPrinterProfileKey][CurrentMenuKey].end(), tgridCourse->Buttons[i][0]->Tag);
		 if (ptrCourse == PrinterProfileCourses[CurrentPrinterProfileKey][CurrentMenuKey].end())
		 { // Not Found.
			PrinterProfileCourses[CurrentPrinterProfileKey][CurrentMenuKey].push_back(tgridCourse->Buttons[i][0]->Tag);
		 }
	  }
	  UpdateCourseDisplay();
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::tbtnClearAllMouseClick(TObject *Sender)
{
   if (CurrentPrinterProfileKey != 0 && CurrentMenuKey != 0)
   {
	  PrinterProfileCourses[CurrentPrinterProfileKey][CurrentMenuKey].clear();
	  UpdateCourseDisplay();
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::tgridSCPrinterProfilesMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift,
   TGridButton *GridButton)
{
   try
   {
	  CurrentPrinterProfileKey = GridButton->Tag;
	  GridButton->Latched = true;
	  UpdateServingCourseDisplay();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::tbtnSCSelectAllMouseClick(TObject *Sender)
{
   if (CurrentPrinterProfileKey != 0)
   {
	  for (int i = 0; i < tgridServingCourses->RowCount; i++)
	  {
		 std::vector <int> ::iterator ptrServingCourse;
		 ptrServingCourse = std::find(PrinterProfileServingCourses[CurrentPrinterProfileKey].begin(),
			PrinterProfileServingCourses[CurrentPrinterProfileKey].end(), tgridServingCourses->Buttons[i][0]->Tag);
		 if (ptrServingCourse == PrinterProfileServingCourses[CurrentPrinterProfileKey].end())
		 { // Not Found.
			PrinterProfileServingCourses[CurrentPrinterProfileKey].push_back(tgridServingCourses->Buttons[i][0]->Tag);
		 }
	  }
	  UpdateServingCourseDisplay();
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::tgridServingCoursesMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift,
   TGridButton *GridButton)
{
   try
   {
	  if (CurrentPrinterProfileKey != 0)
	  {
		 std::vector <int> ::iterator iterServingCourseKey;
		 iterServingCourseKey = std::find(PrinterProfileServingCourses[CurrentPrinterProfileKey].begin(),
			PrinterProfileServingCourses[CurrentPrinterProfileKey].end(), GridButton->Tag);

		 if (iterServingCourseKey == PrinterProfileServingCourses[CurrentPrinterProfileKey].end())
		 { // Not Found.
			PrinterProfileServingCourses[CurrentPrinterProfileKey].push_back(GridButton->Tag);
			GridButton->Latched = true;
		 }
		 else
		 {
			PrinterProfileServingCourses[CurrentPrinterProfileKey].erase(iterServingCourseKey);
			GridButton->Latched = false;
		 }
		 UpdateServingCourseDisplay();
	  }
	  else
	  {
		 MessageBox("Please Select a Printer Profile.", "Error", MB_ICONERROR | MB_OK);
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::tbtnAddEditProfileMouseClick(TObject *Sender)
{
   std::auto_ptr <TfrmVirtualPrintersInterface> frmVirtualPrintersInterface
	   (TfrmVirtualPrintersInterface::Create <TfrmVirtualPrintersInterface> (this, DBControl));
   frmVirtualPrintersInterface->ShowModal();
   UpdateSCPrinterDisplay();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::tbtnServingCourseClearMouseClick(TObject *Sender)
{
   if (CurrentPrinterProfileKey != 0)
   {
	  PrinterProfileServingCourses[CurrentPrinterProfileKey].clear();
	  UpdateServingCourseDisplay();
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::edSetMenuItemBulletMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();
   std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
   frmTouchKeyboard->MaxLength = 255;
   frmTouchKeyboard->AllowCarriageReturn = false;
   frmTouchKeyboard->StartWithShiftDown = false;
   frmTouchKeyboard->KeyboardText = TManagerVariable::Instance().GetStr(DBTransaction, vmKitchenBulletSetMenuItem, "  ");
   frmTouchKeyboard->Caption = "Enter the Bullet to go in front of all Options.";
   if (frmTouchKeyboard->ShowModal() == mrOk)
   {
	  TManagerVariable::Instance().SetDeviceStr(DBTransaction, vmKitchenBulletSetMenuItem, frmTouchKeyboard->KeyboardText);
	  edSetMenuItemBullet->Text = frmTouchKeyboard->KeyboardText;
	  DrawKitchenDocket();
   }
   DBTransaction.Commit();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::cbDuplicateReceiptsClick(TObject *Sender)
{
   TGlobalSettings::Instance().DuplicateReceipts = cbDuplicateReceipts->Checked;
   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();
   TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmDuplicateReceipts, TGlobalSettings::Instance().DuplicateReceipts);
   DBTransaction.Commit();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::cbPrintNoteWithDiscountClick(TObject *Sender)
{
   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();
   TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmPrintNoteWithDiscount, cbPrintNoteWithDiscount->Checked);
   DBTransaction.Commit();
   if (cbPrintNoteWithDiscount->Checked)
   {
	  if (Receipt)
	  {
		 Receipt->PrintNoteWithDiscount = true;
	  }
   }
   else
   {
	  if (Receipt)
	  {
		 Receipt->PrintNoteWithDiscount = false;
	  }
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::cbReceiptPrinterChange(TObject *Sender)
{
   if (cbReceiptPrinter->ItemIndex != -1)
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

	  if (cbReceiptPrinter->ItemIndex == 0)
	  {
		 TPrinterPhysical Blank;
		 TComms::Instance().ReceiptPrinter = Blank;
		 TManagerVariable::Instance().SetDeviceInt(DBTransaction, vmReceiptPrinter, 0);
	  }
	  else
	  {
		 TComboBox *pCB = (TComboBox*)Sender;
			TManagerPhysicalPrinter ManagerPhysicalPrinter;
			TComms::Instance().ReceiptPrinter = ManagerPhysicalPrinter.GetPhysicalPrinter(DBTransaction,
			   (int)cbReceiptPrinter->Items->Objects[cbReceiptPrinter->ItemIndex]);
			TManagerVariable::Instance().SetDeviceInt(DBTransaction, vmReceiptPrinter, TComms::Instance().ReceiptPrinter.PhysicalPrinterKey);
			if (TComms::Instance().ReceiptPrinter.Type == ptWindows_Printer)
			{
			   HANDLE hPrinter = NULL;
			   bool PrinterExists = OpenPrinter(TComms::Instance().ReceiptPrinter.UNCName().t_str(), &hPrinter, NULL);
			   if (!PrinterExists)
			   {
            	AddPrinterConnection(TComms::Instance().ReceiptPrinter.UNCName().t_str());
			   }
			}
	  }
	  DBTransaction.Commit();
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::cbReceiptPrinterDropDown(TObject *Sender)
{
   cbReceiptPrinter->Clear();
   TManagerPhysicalPrinter ManagerPhysicalPrinter;
   cbReceiptPrinter->Items->AddObject("None", static_cast <TObject*> (0));
   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();
   ManagerPhysicalPrinter.DBGetPrinterList(DBTransaction, cbReceiptPrinter->Items);
   DBTransaction.Commit();
   cbReceiptPrinter->ItemIndex = 0;
   if (TComms::Instance().ReceiptPrinter.PhysicalPrinterKey != 0)
   {
	  int PrinterIndex = 0;
	  for (int i = 0; i < cbReceiptPrinter->Items->Count; i++)
	  {
		 if ((int)(cbReceiptPrinter->Items->Objects[i]) == TComms::Instance().ReceiptPrinter.PhysicalPrinterKey)
		 {
			PrinterIndex = i;
		 }
	  }
	  cbReceiptPrinter->ItemIndex = PrinterIndex;
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::btnLoadPrinterGraphicMouseClick(TObject *Sender)
{
   if (TComms::Instance().ReceiptPrinter.PhysicalPrinterKey != 0)
   {
	  TDeviceRealTerminal::Instance().LoadPrinterGraphic();
	  MessageBox("Download complete.", "Complete", MB_OK + MB_ICONINFORMATION);
   }
   else
   {
	  MessageBox("Please select a receipt printer first.", "Error", MB_OK + MB_ICONERROR);
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::btnRefreshMouseClick(TObject *Sender)
{
   if (FileExists(ExtractFilePath(Application->ExeName) + "Header.bmp"))
   {
	  Image1->Picture->LoadFromFile(ExtractFilePath(Application->ExeName) + "Header.bmp");
   }
   else
   {
	  Image1->Picture->Assign(NULL);
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::memFooterMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
   std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
   frmTouchKeyboard->MaxLength = 0;
   frmTouchKeyboard->AllowCarriageReturn = true;
   frmTouchKeyboard->CloseOnDoubleCarriageReturn = false;
   frmTouchKeyboard->StartWithShiftDown = false;
   frmTouchKeyboard->KeyboardText = memFooter->Lines->Text;
   frmTouchKeyboard->Caption = "Enter receipt footer";
   if (frmTouchKeyboard->ShowModal() == mrOk)
   {
	  memFooter->Lines->Text = frmTouchKeyboard->KeyboardText;
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::btnHeaderFooterSaveMouseClick(TObject *Sender)
{
   // Save to Text Files.
   TDeviceRealTerminal::Instance().SaveHdrFtr(memHeader->Lines, memPHeader->Lines, memFooter->Lines);
   memHeader->Modified = false;
   memPHeader->Modified = false;
   memFooter->Modified = false;
   btnLoadHdrFtr->Enabled = false;
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::tbtnTestHeaderMouseClick(TObject *Sender)
{
   if (memHeader->Modified || memFooter->Modified || memPHeader->Modified)
   {
	  if (MessageBox("Do you wish to save your changes to the header and footer first?", "Confirmation",
			MB_OKCANCEL + MB_DEFBUTTON1 + MB_ICONINFORMATION) == IDOK)
	  {
		 btnHeaderFooterSaveMouseClick(Sender);
	  }
   }

   if (TComms::Instance().ReceiptPrinter.PhysicalPrinterKey == 0)
   {
	  MessageBox("Please select a receipt printer first.", "Error", MB_OK + MB_ICONERROR);
   }
   else
   {
	  std::auto_ptr <TPrintout> Printout(new TPrintout);
	  Printout->Printer = TComms::Instance().ReceiptPrinter;

	  Printout->PrintFormat->PrintGraphic();
	  Printout->PrintFormat->Line->ColCount = 1;
	  Printout->PrintFormat->Line->FontInfo.Bold = false;
	  Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
	  Printout->PrintFormat->Line->FontInfo.Width = fsNormalSize;
	  Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width;
	  Printout->PrintFormat->Line->Columns[0]->Alignment = taCenter;

	  if (tpReceipt->ActivePage == tpPreliminary)
	  {
		 for (int i = 0; i < TGlobalSettings::Instance().PHeader->Count; i++)
		 {
			Printout->PrintFormat->Line->Columns[0]->Text = TGlobalSettings::Instance().PHeader->Strings[i];
			Printout->PrintFormat->AddLine();
		 }
	  }
	  else
	  {
		 for (int i = 0; i < TGlobalSettings::Instance().Header->Count; i++)
		 {
			Printout->PrintFormat->Line->Columns[0]->Text = TGlobalSettings::Instance().Header->Strings[i];
			Printout->PrintFormat->AddLine();
		 }
	  }
	  Printout->PrintFormat->DrawLine();

	  Printout->PrintFormat->NewLine();
	  Printout->PrintFormat->Line->ColCount = 1;
	  Printout->PrintFormat->Line->FontInfo.Bold = false;
	  Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
	  Printout->PrintFormat->Line->FontInfo.Width = fsNormalSize;
	  Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width;
	  Printout->PrintFormat->Line->Columns[0]->Alignment = taCenter;

	  for (int i = 0; i < TGlobalSettings::Instance().Footer->Count; i++)
	  {
		 Printout->PrintFormat->Line->Columns[0]->Text = TGlobalSettings::Instance().Footer->Strings[i];
		 Printout->PrintFormat->AddLine();
	  }
	  Printout->PrintFormat->PartialCut();

	  std::auto_ptr <TPrintouts> Printouts(new TPrintouts);
	  Printouts->Add(Printout.get());
	  Printouts->Print(TDeviceRealTerminal::Instance().ID.Type);
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::btnLoadHdrFtrMouseClick(TObject *Sender)
{
   TDeviceRealTerminal::Instance().LoadHdrFtr();
   memHeader->Lines = TGlobalSettings::Instance().Header.get();
   memPHeader->Lines = TGlobalSettings::Instance().PHeader.get();
   memFooter->Lines = TGlobalSettings::Instance().Footer.get();
   memHeader->Modified = false;
   memFooter->Modified = false;
   memPHeader->Modified = false;
   if (FileExists(ExtractFilePath(Application->ExeName) + "Header.bmp"))
   {
	  Image1->Picture->LoadFromFile(ExtractFilePath(Application->ExeName) + "Header.bmp");
   }
   else
   {
	  Image1->Picture->Assign(NULL);
   }
   btnLoadHdrFtr->Enabled = false;
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::memHeaderChange(TObject *Sender)
{
   btnLoadHdrFtr->Enabled = true;
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::memHeaderMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{

  TDeviceRealTerminal::Instance().Registered(&IsDBRegistered, NULL);
   if(IsDBRegistered)
   {
       std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
       frmTouchKeyboard->MaxLength = 0;
       frmTouchKeyboard->AllowCarriageReturn = true;
       frmTouchKeyboard->CloseOnDoubleCarriageReturn = false;
       frmTouchKeyboard->StartWithShiftDown = false;
       frmTouchKeyboard->KeyboardText = memHeader->Lines->Text;
       frmTouchKeyboard->Caption = "Enter receipt header";
       if (frmTouchKeyboard->ShowModal() == mrOk)
       {
          memHeader->Lines->Text = frmTouchKeyboard->KeyboardText;
       }
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::memPHeaderMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TDeviceRealTerminal::Instance().Registered(&IsDBRegistered, NULL);
   if(IsDBRegistered)
   {
       std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
       frmTouchKeyboard->MaxLength = 0;
       frmTouchKeyboard->AllowCarriageReturn = true;
       frmTouchKeyboard->CloseOnDoubleCarriageReturn = false;
       frmTouchKeyboard->StartWithShiftDown = false;
       frmTouchKeyboard->KeyboardText = memPHeader->Lines->Text;
       frmTouchKeyboard->Caption = "Enter preliminary receipt header";
       if (frmTouchKeyboard->ShowModal() == mrOk)
       {
          memPHeader->Lines->Text = frmTouchKeyboard->KeyboardText;
       }
    }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::cbAlwaysPrintReceiptTenderedSalesClick(TObject *Sender)
{
   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();
   Receipt->AlwaysPrintReceiptTenderedSales = cbAlwaysPrintReceiptTenderedSales->Checked;
   TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmAlwaysPrintReceiptTenderedSales, Receipt->AlwaysPrintReceiptTenderedSales);
   DBTransaction.Commit();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::cbAlwaysPrintCashSalesClick(TObject *Sender)
{
   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();
   Receipt->AlwaysPrintReceiptCashSales = cbAlwaysPrintCashSales->Checked;
   TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmAlwaysPrintReceiptCashSales, Receipt->AlwaysPrintReceiptCashSales);
   DBTransaction.Commit();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::cbReceiptBarCodesClick(TObject *Sender)
{
   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();
   Receipt->DisplayBarCodes = cbReceiptBarCodes->Checked;
   TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmDisplayBarCodesOnReceipt, Receipt->DisplayBarCodes);
   DBTransaction.Commit();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::cbDoublePrintClick(TObject *Sender)
{
   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();
   TGlobalSettings::Instance().PrintSignatureReceiptsTwice = cbDoublePrint->Checked;
   TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmDuplicateSignatureReceipts, TGlobalSettings::Instance().PrintSignatureReceiptsTwice);
   DBTransaction.Commit();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::cbSaveAndPrintClick(TObject *Sender)
{
   TGlobalSettings::Instance().SaveAndPrintPrintsPartialOrder = cbSaveAndPrint->Checked;
   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();
   TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmSaveAndPrintPrintsPartialOrder, TGlobalSettings::Instance().SaveAndPrintPrintsPartialOrder);
   DBTransaction.Commit();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::cbAPTROSClick(TObject *Sender)
{
   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();
   TGlobalSettings::Instance().AutoPrintTabReceipts = cbAPTROS->Checked;
   TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmAutoPrintTabReceipts, TGlobalSettings::Instance().AutoPrintTabReceipts);
   DBTransaction.Commit();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::cbAPSROSClick(TObject *Sender)
{
   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();
   TGlobalSettings::Instance().AutoPrintStaffReceipts = cbAPSROS->Checked;
   TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmAutoPrintStaffReceipts, TGlobalSettings::Instance().AutoPrintStaffReceipts);
   DBTransaction.Commit();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::cbAPMOSClick(TObject *Sender)
{
   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();
   TGlobalSettings::Instance().AutoPrintMemberReceipts = cbAPMOS->Checked;
   TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmAutoPrintMemberReceipts, TGlobalSettings::Instance().AutoPrintMemberReceipts);
   DBTransaction.Commit();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::cbAPTAROSClick(TObject *Sender)
{
   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();
   TGlobalSettings::Instance().AutoPrintTableReceipts = cbAPTAROS->Checked;
   TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmAutoPrintTableReceipts, TGlobalSettings::Instance().AutoPrintTableReceipts);
   DBTransaction.Commit();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::cbAPCROSClick(TObject *Sender)
{
   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();
   TGlobalSettings::Instance().AutoPrintCashReceipts = cbAPCROS->Checked;
   TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmAutoPrintCashReceipts, TGlobalSettings::Instance().AutoPrintCashReceipts);
   DBTransaction.Commit();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::cbAPRROSClick(TObject *Sender)
{
   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();
   TGlobalSettings::Instance().AutoPrintRoomReceipts = cbAPRROS->Checked;
   TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmAutoPrintRoomReceipts, TGlobalSettings::Instance().AutoPrintRoomReceipts);
   DBTransaction.Commit();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::cbAPIROSClick(TObject *Sender)
{
   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();
   TGlobalSettings::Instance().AutoPrintInvoiceReceipts = cbAPIROS->Checked;
   TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmAutoPrintInvoiceReceipts, TGlobalSettings::Instance().AutoPrintInvoiceReceipts);
   DBTransaction.Commit();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::ManuallyAddPrinterMouseClick(TObject *Sender)
{
   try
   {
	  AnsiString PrinterName = "";
	  AnsiString ServerName = "";
	  DWORD PrinterFlags;

	  std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
	  frmTouchKeyboard->MaxLength = 255;
	  frmTouchKeyboard->AllowCarriageReturn = false;
	  frmTouchKeyboard->StartWithShiftDown = false;
	  frmTouchKeyboard->KeyboardText = "";
	  frmTouchKeyboard->Caption = "Enter printer server computer name.";
	  if (frmTouchKeyboard->ShowModal() == mrOk)
	  {
		 ServerName = frmTouchKeyboard->KeyboardText;
	  }

	  frmTouchKeyboard->MaxLength = 255;
	  frmTouchKeyboard->AllowCarriageReturn = false;
	  frmTouchKeyboard->StartWithShiftDown = false;
	  frmTouchKeyboard->KeyboardText = "";
	  frmTouchKeyboard->Caption = "Enter printer share name.";
	  if (frmTouchKeyboard->ShowModal() == mrOk)
	  {
		 PrinterName = frmTouchKeyboard->KeyboardText;
	  }

	  TPrinterPhysical PrinterPhysical;
	  PrinterPhysical.ServerName = ServerName;
	  PrinterPhysical.ShareName = PrinterName;

	  TManagerPhysicalPrinter ManagerPhysicalPrinter;

	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();
	  ManagerPhysicalPrinter.DBAddPrinter(DBTransaction, PrinterPhysical);
	  ManagerPhysicalPrinter.LoadProfile(DBTransaction, PrinterPhysical);
	  UpdatePhysicalPrinterList(DBTransaction);

	  int ErrorCode = AddPrinterConnection(PrinterPhysical.UNCName().t_str());
	  if (ErrorCode != 0)
	  {
		 MessageBox("The printer could not be added. Make sure that " + PrinterPhysical.ServerName + " is turned on.\r Error Code" + IntToStr
			(ErrorCode), "Error", MB_ICONERROR + MB_OK);
	  }

	  DBTransaction.Commit();

   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}
// ---------------------------------------------------------------------------

void TfrmPrinterMaintenance::MMEnumPrinters(UnicodeString PrinterContainer, std::map <UnicodeString, DWORD> &PrinterData)
{
   DWORD dwNeeded, dwNumPrinters;

   EnumPrinters(PRINTER_ENUM_REMOTE || PRINTER_ENUM_SHARED || PRINTER_ENUM_NETWORK || PRINTER_ENUM_NAME, PrinterContainer.t_str(), 1, NULL,
	  0, &dwNeeded, &dwNumPrinters);

   unsigned char *buffer = NULL;
   buffer = new unsigned char[dwNeeded];

   try
   {
	  setmem(buffer, dwNeeded, 0);
	  EnumPrinters(PRINTER_ENUM_REMOTE || PRINTER_ENUM_SHARED || PRINTER_ENUM_NETWORK || PRINTER_ENUM_NAME, PrinterContainer.t_str(), 1,
		 buffer, dwNeeded, &dwNeeded, &dwNumPrinters);

	  LPPRINTER_INFO_1 lppi1;

	  std::map <UnicodeString, std::set <UnicodeString> > PrinterMap;
	  for (int index = 0; index < (int)dwNumPrinters; index++)
	  {
		 lppi1 = (LPPRINTER_INFO_1)(buffer + index*sizeof(PRINTER_INFO_1));
		 if (UnicodeString(lppi1->pName).SubString(1, 2) == "\\\\")
		 {
			PrinterData[lppi1->pName] = lppi1->Flags;
		 }
	  }
   }
   __finally
   {
	  delete[]buffer;
   }
}

void __fastcall TfrmPrinterMaintenance::btnRefreshPrinterModelsMouseClick(TObject *Sender)
{
   for (int i = 0; i < lbPrinterModels->Items->Count; i++)
   {
	  delete(TPrinterModelSettings*)lbPrinterModels->Items->Objects[i];
   }
   lbPrinterModels->Items->Clear();

   TIniFile* PrinterDat = new TIniFile(ExtractFilePath(Application->ExeName) + "Printers.dat");
   try
   {
	  PrinterDat->ReadSections(lbPrinterModels->Items);
	  for (int i = 0; i < lbPrinterModels->Items->Count; i++)
	  {
		 TPrinterModelSettings *PrinterModelSettings = new TPrinterModelSettings;
		 PrinterModelSettings->PrinterModel = PrinterDat->ReadInteger(lbPrinterModels->Items->Strings[i], "PrinterModel", 0);
		 PrinterModelSettings->NormalFontCPL = PrinterDat->ReadInteger(lbPrinterModels->Items->Strings[i], "NormalFontCPL", 0);
		 PrinterModelSettings->BoldFontCPL = PrinterDat->ReadInteger(lbPrinterModels->Items->Strings[i], "BoldFontCPL", 0);
		 PrinterModelSettings->CutterGap = PrinterDat->ReadInteger(lbPrinterModels->Items->Strings[i], "CutterGap", 0);
		 PrinterModelSettings->TopMargin = PrinterDat->ReadInteger(lbPrinterModels->Items->Strings[i], "TopMargin", 0);
		 PrinterModelSettings->AlwaysUseFullCut = PrinterDat->ReadInteger(lbPrinterModels->Items->Strings[i], "AlwaysUseFullCut", 0);
		 PrinterModelSettings->KanjiPrinter = PrinterDat->ReadInteger(lbPrinterModels->Items->Strings[i], "KanjiPrinter", 0);
		 PrinterModelSettings->PrinterCodePage = PrinterDat->ReadInteger(lbPrinterModels->Items->Strings[i], "PrinterCodePage", 0);

		 PrinterModelSettings->PrinterInitCmd = PrinterDat->ReadString(lbPrinterModels->Items->Strings[i], "PrinterInitCmd", "");
		 PrinterModelSettings->PrinterCodePageOn = PrinterDat->ReadString(lbPrinterModels->Items->Strings[i], "CodePageStringOn", "");
		 PrinterModelSettings->PrinterCodePageOff = PrinterDat->ReadString(lbPrinterModels->Items->Strings[i], "CodePageStringOff", "");

         if(PrinterModelSettings->PrinterCodePageOn.Length() > 1)
         {
		 	std::vector <char> Buffer(PrinterModelSettings->PrinterCodePageOn.Length()/2, 0x00);
		 	HexToBin(PrinterModelSettings->PrinterCodePageOn.LowerCase().c_str(), (char *)&Buffer[0], Buffer.size());
         }

         if(PrinterModelSettings->PrinterCodePageOff.Length() > 1)
         {
		 	std::vector <char> Buffer(PrinterModelSettings->PrinterCodePageOff.Length()/2, 0x00);
		 	HexToBin(PrinterModelSettings->PrinterCodePageOff.LowerCase().c_str(), (char *)&Buffer[0], Buffer.size());
         }

		 PrinterModelSettings->SingleLineChar = PrinterDat->ReadString(lbPrinterModels->Items->Strings[i], "SingleLineChar",
			AnsiString(FONT_SOLID_LINE_TEXT))[1];
		 PrinterModelSettings->DoubleLineChar = PrinterDat->ReadString(lbPrinterModels->Items->Strings[i], "DoubleLineChar",
			AnsiString(FONT_DOUBLE_LINE_TEXT))[1];
		 PrinterModelSettings->GraphicSettings.GraphicPrinter = PrinterDat->ReadInteger(lbPrinterModels->Items->Strings[i],
			"GraphicPrinter", 0);
		 PrinterModelSettings->GraphicSettings.GraphicFontName = PrinterDat->ReadString(lbPrinterModels->Items->Strings[i],
			"GraphicFontName", "Terminal");
		 PrinterModelSettings->GraphicSettings.GraphicFontSize = PrinterDat->ReadInteger(lbPrinterModels->Items->Strings[i],
			"GraphicFontSize", 12);
		 PrinterModelSettings->GraphicSettings.GraphicHeaderFile = PrinterDat->ReadString(lbPrinterModels->Items->Strings[i],
			"GraphicHeaderFile", "GraphicHeader.bmp");
		 PrinterModelSettings->GraphicSettings.GraphicHeaderX = PrinterDat->ReadInteger(lbPrinterModels->Items->Strings[i],
			"GraphicHeaderX", 0);
		 PrinterModelSettings->GraphicSettings.GraphicHeaderY = PrinterDat->ReadInteger(lbPrinterModels->Items->Strings[i],
			"GraphicHeaderY", 0);
		 PrinterModelSettings->GraphicSettings.GraphicHeaderWidth = PrinterDat->ReadInteger(lbPrinterModels->Items->Strings[i],
			"GraphicHeaderWidth", 0);
		 PrinterModelSettings->GraphicSettings.GraphicHeaderHeight = PrinterDat->ReadInteger(lbPrinterModels->Items->Strings[i],
			"GraphicHeaderHeight", 0);

		 PrinterModelSettings->GraphicSettings.GraphicReceiptX = PrinterDat->ReadInteger(lbPrinterModels->Items->Strings[i],
			"GraphicReceiptX", 0);
		 PrinterModelSettings->GraphicSettings.GraphicReceiptY = PrinterDat->ReadInteger(lbPrinterModels->Items->Strings[i],
			"GraphicReceiptY", 0);
		 PrinterModelSettings->GraphicSettings.GraphicReceiptMaxLines = PrinterDat->ReadInteger(lbPrinterModels->Items->Strings[i],
			"GraphicReceiptMaxLines", 0);
		 PrinterModelSettings->GraphicSettings.GraphicBackGroundFile = PrinterDat->ReadString(lbPrinterModels->Items->Strings[i],
			"GraphicBackGroundFile", "ReceiptBackGround.bmp");
		 PrinterModelSettings->GraphicSettings.GraphicBackGroundX = PrinterDat->ReadInteger(lbPrinterModels->Items->Strings[i],
			"GraphicBackGroundX", 0);
		 PrinterModelSettings->GraphicSettings.GraphicBackGroundY = PrinterDat->ReadInteger(lbPrinterModels->Items->Strings[i],
			"GraphicBackGroundY", 0);
		 PrinterModelSettings->GraphicSettings.GraphicBackGroundWidth = PrinterDat->ReadInteger(lbPrinterModels->Items->Strings[i],
			"GraphicBackGroundWidth", 0);
		 PrinterModelSettings->GraphicSettings.GraphicBackGroundHeight = PrinterDat->ReadInteger(lbPrinterModels->Items->Strings[i],
			"GraphicBackGroundHeight", 0);
		 PrinterModelSettings->GraphicSettings.GraphicPageCountX = PrinterDat->ReadInteger(lbPrinterModels->Items->Strings[i],
			"GraphicPageCountX", -1);
		 PrinterModelSettings->GraphicSettings.GraphicPageCountY = PrinterDat->ReadInteger(lbPrinterModels->Items->Strings[i],
			"GraphicPageCountY", -1);

		 AnsiString Emulation = PrinterDat->ReadString(lbPrinterModels->Items->Strings[i], "Emulation", "Epson");
		 if (UpperCase(Emulation) == "EPSON")
		 {
			PrinterModelSettings->Emulation = emEpson;
			if (!PrinterDat->ValueExists(lbPrinterModels->Items->Strings[i], "SingleLineChar"))
			{
			   PrinterModelSettings->SingleLineChar = FONT_SOLID_LINE_EPSON;
			}
			if (!PrinterDat->ValueExists(lbPrinterModels->Items->Strings[i], "DoubleLineChar"))
			{
			   PrinterModelSettings->DoubleLineChar = FONT_DOUBLE_LINE_EPSON;
			}
		 }
		 else if (UpperCase(Emulation) == "TEXT")
			PrinterModelSettings->Emulation = emTextOnly;
		 else if (UpperCase(Emulation) == "STAR")
			PrinterModelSettings->Emulation = emStar;
		 else if (UpperCase(Emulation) == "SCRIPT")
			PrinterModelSettings->Emulation = emScript;
		 else
			PrinterModelSettings->Emulation = emEpson;

		 lbPrinterModels->Items->Objects[i] = PrinterModelSettings;
	  }
   }
   __finally
   {
	  delete PrinterDat;
   }
   if (Sender != NULL)
	  ShowPrinterDetails(); // Only refresh if a button was pressed.
}
// ---------------------------------------------------------------------------

void __fastcall
TfrmPrinterMaintenance::tbChefMateColourMouseClick(
TObject *Sender)
{
	TManagerVariable &mv = TManagerVariable::Instance();

	int global_profile_key;
   Database::TDBTransaction tr(DBControl);

   ColorDialog->Execute();

   TGlobalSettings::Instance().KitchenColour = ColorDialog->Color;

   tr.StartTransaction();

	if (!(global_profile_key =
	      mv.GetProfile(tr, eSystemProfiles, "Globals")))
		global_profile_key = mv.SetProfile(tr, eSystemProfiles, "Globals");

   mv.SetProfileInt(tr, global_profile_key, vmChefMateColour,
	                 ColorDialog->Color);
   tr.Commit();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::tbChefMateIPMouseClick(TObject *Sender)
{
    TChefmateSettings ChefmateSettings;
	ChefmateSettings.SetDatabase( DBControl );
	ChefmateSettings.Edit( this );

	if( ChefmateSettings.Active )
	{
	  TManagerPhysicalPrinter ManagerPhysicalPrinter;

	  TPrinterPhysical ChefMateScreenPrinter;
	  ChefMateScreenPrinter.Type = ptChefMate_Printer;
	  ChefMateScreenPrinter.ServerName = ChefmateSettings.IPAddr;
	  ChefMateScreenPrinter.ShareName = "ChefMate Display";
	  ChefMateScreenPrinter.NormalCharPerLine = 25;
	  ChefMateScreenPrinter.BoldCharPerLine = 25;
	  ChefMateScreenPrinter.SingleLineChar = '-';
	  ChefMateScreenPrinter.DoubleLineChar = '=';
	  ChefMateScreenPrinter.EmulationMode = emTextOnly;
	  ChefMateScreenPrinter.KanjiPrinter = true;
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();
	  ManagerPhysicalPrinter.DBAddPrinter(DBTransaction, ChefMateScreenPrinter);
	  UpdatePhysicalPrinterList(DBTransaction);
	  DBTransaction.Commit();
	}

/*
   std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
   frmTouchKeyboard->MaxLength = 15;
   frmTouchKeyboard->AllowCarriageReturn = false;
   frmTouchKeyboard->StartWithShiftDown = false;
   frmTouchKeyboard->KeyboardText = "";
   frmTouchKeyboard->Caption = "IP address for ChefMate Packets.(e.g 192.168.1.2)";
   if (frmTouchKeyboard->ShowModal() == mrOk)
   {
	  TManagerPhysicalPrinter ManagerPhysicalPrinter;

	  TPrinterPhysical ChefMateScreenPrinter;
	  ChefMateScreenPrinter.Type = ptChefMate_Printer;
	  ChefMateScreenPrinter.ServerName = frmTouchKeyboard->KeyboardText;
	  ChefMateScreenPrinter.ShareName = "ChefMate Display";
	  ChefMateScreenPrinter.NormalCharPerLine = 25;
	  ChefMateScreenPrinter.BoldCharPerLine = 25;
	  ChefMateScreenPrinter.SingleLineChar = '-';
	  ChefMateScreenPrinter.DoubleLineChar = '=';
	  ChefMateScreenPrinter.EmulationMode = emTextOnly;
	  ChefMateScreenPrinter.KanjiPrinter = true;
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();
	  ManagerPhysicalPrinter.DBAddPrinter(DBTransaction, ChefMateScreenPrinter);
	  UpdatePhysicalPrinterList(DBTransaction);
	  DBTransaction.Commit();
   }
*/
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::btnDelChefPrinterMouseClick(TObject *Sender)
{
   if (lbPrinters->ItemIndex != -1)
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();
	  TManagerPhysicalPrinter ManagerPhysicalPrinter;
	  TPrinterPhysical Printer = ManagerPhysicalPrinter.GetPhysicalPrinter(DBTransaction,
		 (int)(lbPrinters->Items->Objects[lbPrinters->ItemIndex]));
	  if (Printer.PhysicalPrinterKey != 0)
	  {
		 if (Printer.Type == ptChefMate_Printer)
		 {
			ManagerPhysicalPrinter.DBDeletePrinter(DBTransaction, Printer);
			UpdatePhysicalPrinterList(DBTransaction);
		 }
		 else
		 {
			MessageBox("The selected printer is not a ChefMate Printer.", "Error", MB_OK);
		 }
	  }
	  else
	  {
		 MessageBox("No ChefMate Printer Selected.", "Error", MB_OK);
	  }
	  DBTransaction.Commit();
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::btnTestMouseClick(TObject *Sender)
{
   if (lbPrinters->ItemIndex != -1)
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

	  TManagerPhysicalPrinter ManagerPhysicalPrinter;
	  TPrinterPhysical Printer = ManagerPhysicalPrinter.GetPhysicalPrinter(DBTransaction,
		 (int)(lbPrinters->Items->Objects[lbPrinters->ItemIndex]));
	  if (Printer.PhysicalPrinterKey != 0)
	  {
		 TPrintout *Printout = new TPrintout;
		 Printout->Printer = Printer;
		 try
		 {
			Printout->PrintFormat->WordWrap = false;

			Printout->PrintFormat->Line->ColCount = 1;
			Printout->PrintFormat->Line->FontInfo.Bold = false;
			Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
			Printout->PrintFormat->Line->FontInfo.Width = fsNormalSize;
			Printout->PrintFormat->Line->Columns[0]->Width = 90;
			Printout->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;

			Printout->PrintFormat->Line->Columns[0]->Text = "First Line";
			Printout->PrintFormat->AddLine();
			Printout->PrintFormat->NewLine();
			Printout->PrintFormat->NewLine();
			Printout->PrintFormat->Line->Columns[0]->Text =
				"Normal Font       20-234567-30-234567-40-234567-50-234567-60-234567-70-234567-80-234567-90";
			Printout->PrintFormat->AddLine();
			Printout->PrintFormat->NewLine();
			Printout->PrintFormat->Line->FontInfo.Bold = true;
			Printout->PrintFormat->Line->Columns[0]->Text =
				"Bold Font         20-234567-30-234567-40-234567-50-234567-60-234567-70-234567-80-234567-90";
			Printout->PrintFormat->AddLine();
			Printout->PrintFormat->NewLine();
			Printout->PrintFormat->NewLine();
			Printout->PrintFormat->Line->FontInfo.Bold = false;
			Printout->PrintFormat->Line->Columns[0]->Text = "Last Line";
			Printout->PrintFormat->AddLine();

            Printout->Print();

           AnsiString LineStr = "";
           /*std::vector <char> BufferInit(Printer.PrinterInitCmd.Length()/2, 0x00);
           HexToBin(Printer.PrinterInitCmd.LowerCase().c_str(), &BufferInit[0], BufferInit.size());
           LineStr = AnsiString(&BufferInit[0], BufferInit.size());
           Printout->PrintFormat->Line->Columns[0]->Text = LineStr;
           Printout->PrintFormat->AddLine();*/
		 }
		 __finally
		 {
			delete Printout;
		 }
	  }
	  DBTransaction.Commit();
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::DeletePrinterMouseClick(TObject *Sender)
{
   if (MessageBox("Are you sure you wish to remove this printer and All profiles that use it?", "Remove Printer.",
		 MB_ICONQUESTION | MB_OKCANCEL) == ID_OK)
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

	  TManagerPhysicalPrinter ManagerPhysicalPrinter;
	  TPrinterPhysical Printer = ManagerPhysicalPrinter.GetPhysicalPrinter(DBTransaction,
		 (int)(lbPrinters->Items->Objects[lbPrinters->ItemIndex]));
	  if (Printer.PhysicalPrinterKey != 0)
	  {
		 ManagerPhysicalPrinter.DBDeletePrinter(DBTransaction, Printer);
		 UpdatePhysicalPrinterList(DBTransaction);
	  }
	  DBTransaction.Commit();
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::AssignPrinterConfigMouseClick(TObject *Sender)
{
   if (lbPrinters->ItemIndex != -1)
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

	  TManagerPhysicalPrinter ManagerPhysicalPrinter;
	  TPrinterPhysical Printer = ManagerPhysicalPrinter.GetPhysicalPrinter(DBTransaction,
		 (int)(lbPrinters->Items->Objects[lbPrinters->ItemIndex]));
	  if (Printer.PhysicalPrinterKey != 0)
	  {
		 TPrinterModelSettings *PrinterModelSettings = (TPrinterModelSettings*)lbPrinterModels->Items->Objects[lbPrinterModels->ItemIndex];
		 if (PrinterModelSettings)
		 {
			Printer.PrinterModel = PrinterModelSettings->PrinterModel;
			Printer.NormalCharPerLine = PrinterModelSettings->NormalFontCPL;
			Printer.BoldCharPerLine = PrinterModelSettings->BoldFontCPL;
			Printer.CutterGap = PrinterModelSettings->CutterGap;
			Printer.TopMargin = PrinterModelSettings->TopMargin;
			Printer.KitchenTopMargin = PrinterModelSettings->TopMargin;
			Printer.AlwaysUseFullCut = PrinterModelSettings->AlwaysUseFullCut;
			Printer.KanjiPrinter = PrinterModelSettings->KanjiPrinter;
			Printer.PrinterCodePage = PrinterModelSettings->PrinterCodePage;
			Printer.PrinterCodePageOn = PrinterModelSettings->PrinterCodePageOn;
			Printer.PrinterCodePageOff = PrinterModelSettings->PrinterCodePageOff;
            Printer.PrinterInitCmd = PrinterModelSettings->PrinterInitCmd;
			Printer.SingleLineChar = PrinterModelSettings->SingleLineChar;
			Printer.DoubleLineChar = PrinterModelSettings->DoubleLineChar;
			Printer.EmulationMode = PrinterModelSettings->Emulation;
			Printer.Graphics = PrinterModelSettings->GraphicSettings;
		 }
		 else
		 {
			Printer.PrinterModel = 0;
		 }

		 if (Printer.PhysicalProfileKey == 0)
		 {
			Printer.PhysicalProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction, eEquipmentProfiles,
			   Printer.ServerName + Printer.ShareName);
		 }

		 TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmPrinterModel, Printer.PrinterModel);
		 if (Printer.PrinterModel != 0)
		 {
			TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmNormalCPL, Printer.NormalCharPerLine);
			TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmNormalBoldCPL, Printer.BoldCharPerLine);
			TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmCutterGap, Printer.CutterGap);
			TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmTopMargin, Printer.TopMargin);
			TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmKitchenTopMargin, Printer.KitchenTopMargin);
			TManagerVariable::Instance().SetProfileBool(DBTransaction, Printer.PhysicalProfileKey, vmAlwaysUseFullCut, Printer.AlwaysUseFullCut);
			TManagerVariable::Instance().SetProfileBool(DBTransaction, Printer.PhysicalProfileKey, vmKanjiPrinter, Printer.KanjiPrinter);
			TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmPrinterCodePage, Printer.PrinterCodePage);

            TManagerVariable::Instance().SetProfileStr(DBTransaction, Printer.PhysicalProfileKey, vmPrinterInitCmd, Printer.PrinterInitCmd);
			TManagerVariable::Instance().SetProfileStr(DBTransaction, Printer.PhysicalProfileKey, vmPrinterCodePageOn, Printer.PrinterCodePageOn);
			TManagerVariable::Instance().SetProfileStr(DBTransaction, Printer.PhysicalProfileKey, vmPrinterCodePageOff, Printer.PrinterCodePageOff);

			TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmSingleLineChar, Printer.SingleLineChar);
			TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmDoubleLineChar, Printer.DoubleLineChar);
			TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmPrinterEmulationMode, Printer.EmulationMode);
			TManagerVariable::Instance().SetProfileBool(DBTransaction, Printer.PhysicalProfileKey, vmGraphicPrinter,
			   Printer.Graphics.GraphicPrinter);
			TManagerVariable::Instance().SetProfileStr(DBTransaction, Printer.PhysicalProfileKey, vmGraphicFontName,
			   Printer.Graphics.GraphicFontName);
			TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmGraphicFontSize,
			   Printer.Graphics.GraphicFontSize);
			TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmGraphicHeaderX, Printer.Graphics.GraphicHeaderX);
			TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmGraphicHeaderY, Printer.Graphics.GraphicHeaderY);
			TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmGraphicHeaderWidth,
			   Printer.Graphics.GraphicHeaderWidth);
			TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmGraphicHeaderHeight,
			   Printer.Graphics.GraphicHeaderHeight);
			TManagerVariable::Instance().SetProfileStr(DBTransaction, Printer.PhysicalProfileKey, vmGraphicHeaderFile,
			   Printer.Graphics.GraphicHeaderFile);

			TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmGraphicReceiptX,
			   Printer.Graphics.GraphicReceiptX);
			TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmGraphicReceiptY,
			   Printer.Graphics.GraphicReceiptY);

			TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmGraphicReceiptMaxLines,
			   Printer.Graphics.GraphicReceiptMaxLines);
			TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmGraphicBackGroundX,
			   Printer.Graphics.GraphicBackGroundX);
			TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmGraphicBackGroundY,
			   Printer.Graphics.GraphicBackGroundY);
			TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmGraphicBackGroundWidth,
			   Printer.Graphics.GraphicBackGroundWidth);
			TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmGraphicBackGroundHeight,
			   Printer.Graphics.GraphicBackGroundHeight);
			TManagerVariable::Instance().SetProfileStr(DBTransaction, Printer.PhysicalProfileKey, vmGraphicBackGroundFile,
			   Printer.Graphics.GraphicBackGroundFile);
			TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmGraphicPageCountX,
			   Printer.Graphics.GraphicPageCountX);
			TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmGraphicPageCountY,
			   Printer.Graphics.GraphicPageCountY);

		 }
	  }
	  DBTransaction.Commit();
	  ShowPrinterDetails();
	  EventPhysicalPropertiesChanged.Occured();
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::lbPrinterModelsClick(TObject *Sender)
{
   if (lbPrinterModels->ItemIndex != -1)
   {
	  AnsiString PrinterTypeName = lbPrinterModels->Items->Strings[lbPrinterModels->ItemIndex];
	  if (MessageBox("Are you sure you wish to assign " + PrinterTypeName + " to this printer.", "Printer Assignment",
			MB_YESNO + MB_ICONQUESTION) == ID_YES)
	  {
		 AssignPrinterConfigMouseClick(NULL);
	  }
	  else
	  {
		 lbPrinterModels->ItemIndex = -1;
	  }
   }
   ShowPrinterDetails();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::rbFullCutMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();

   TManagerPhysicalPrinter ManagerPhysicalPrinter;
   TPrinterPhysical Printer = ManagerPhysicalPrinter.GetPhysicalPrinter(DBTransaction,
	  (int)(lbPrinters->Items->Objects[lbPrinters->ItemIndex]));
   if (Printer.PhysicalPrinterKey != 0)
   {
	  if (Printer.PhysicalProfileKey == 0)
	  {
		 Printer.PhysicalProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction, eEquipmentProfiles,
			Printer.ServerName + Printer.ShareName);
	  }
	  TManagerVariable::Instance().SetProfileBool(DBTransaction, Printer.PhysicalProfileKey, vmAlwaysUseFullCut, true);
	  TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmPrinterModel, 1);
	  EventPhysicalPropertiesChanged.Occured();
   }
   DBTransaction.Commit();
   lbPrinterModels->ItemIndex = -1;
   ShowPrinterDetails();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::rbPartialCutMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();

   TManagerPhysicalPrinter ManagerPhysicalPrinter;
   TPrinterPhysical Printer = ManagerPhysicalPrinter.GetPhysicalPrinter(DBTransaction,
	  (int)(lbPrinters->Items->Objects[lbPrinters->ItemIndex]));
   if (Printer.PhysicalPrinterKey != 0)
   {
	  if (Printer.PhysicalProfileKey == 0)
	  {
		 Printer.PhysicalProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction, eEquipmentProfiles,
			Printer.ServerName + Printer.ShareName);
	  }
	  TManagerVariable::Instance().SetProfileBool(DBTransaction, Printer.PhysicalProfileKey, vmAlwaysUseFullCut, false);
	  TManagerVariable::Instance().SetProfileInt(DBTransaction, Printer.PhysicalProfileKey, vmPrinterModel, 1);
	  EventPhysicalPropertiesChanged.Occured();
   }
   DBTransaction.Commit();
   lbPrinterModels->ItemIndex = -1;
   ShowPrinterDetails();
}
// ---------------------------------------------------------------------------

void TfrmPrinterMaintenance::UpdateKitchenFormatProfileListDisplay()
{
   if (CurrentPrinterProfileKey == 0)
   {
	  std::auto_ptr <TStringList> PrinterProfiles(new TStringList);

	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();
	  TManagerVirtualPrinter::GetVirtualPrinterList(DBTransaction, PrinterProfiles.get());
	  if (PrinterProfiles->Count > 0)
	  {
		 CurrentPrinterProfileKey = (int)PrinterProfiles->Objects[0];
		 tbtnPrinterProfile->Caption = "Printer Profile\r" + PrinterProfiles->Strings[0];
	  }
	  DBTransaction.Commit();
	  DisplayKitchenTemplate();
	  DrawKitchenDocket();
   }
   else
   {
	  tbtnPrinterProfile->Caption = "Printer Profile\r None Selected";
	  DisplayKitchenTemplate();
	  DrawKitchenDocket();
   }
}

void __fastcall TfrmPrinterMaintenance::tbtnPrinterProfileMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
   try
   {
	  if (CurrentPrinterProfileKey != 0)
	  {
		 SaveKitchenTemplate();
	  }

	  CurrentPrinterProfileKey = 0;

	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

	  std::auto_ptr <TStringList> PrinterProfiles(new TStringList);
	  std::auto_ptr <TManagerPhysicalPrinter> ManagerPhysicalPrinter(new TManagerPhysicalPrinter);

	  TManagerVirtualPrinter::GetVirtualPrinterList(DBTransaction, PrinterProfiles.get());

	  // Add Physical Printer Connection Info.
	  for (int i = 0; i < PrinterProfiles->Count; i++)
	  {
		 int VirtualPrinterKey = (int)PrinterProfiles->Objects[i];
		 TPrinterVirtual *vPrinter = TManagerVirtualPrinter::GetVirtualPrinterByKey(DBTransaction,VirtualPrinterKey);
		 if (vPrinter)
		 {
			TPrinterPhysical PrinterPhysical = ManagerPhysicalPrinter->GetPhysicalPrinter(DBTransaction, vPrinter->PhysicalPrinterKey);
			PrinterProfiles->Strings[i] = PrinterProfiles->Strings[i] + " [" + PrinterPhysical.ServerName + " : " +
				PrinterPhysical.ShareName + "]";
		 }
	  }

	  std::auto_ptr <TfrmVerticalSelect> SelectionForm(TfrmVerticalSelect::Create <TfrmVerticalSelect> (this));
	  TVerticalSelection Item;
	  Item.Title = "Cancel";
	  Item.Properties["Color"] = IntToStr(clMaroon);
	  Item.CloseSelection = true;
	  SelectionForm->Items.push_back(Item);

	  for (int i = 0; i < PrinterProfiles->Count; i++)
	  {
		 Item.Title = PrinterProfiles->Strings[i];
		 Item.Properties["Color"] = IntToStr(clMaroon);
		 Item.Properties["Key"] = (int)PrinterProfiles->Objects[i];
		 Item.CloseSelection = true;
		 SelectionForm->Items.push_back(Item);
	  }

	  SelectionForm->ShowModal();
	  TVerticalSelection SelectedItem;
	  if (SelectionForm->GetFirstSelectedItem(SelectedItem) && SelectedItem.Title != "Cancel")
	  {
		 CurrentPrinterProfileKey = int(SelectedItem.Properties["Key"]);
		 tbtnPrinterProfile->Caption = "Printer Profile\r" + SelectedItem.Title;
	  }
	  else
	  {
		 tbtnPrinterProfile->Caption = "Printer Profile\r None Selected";
	  }

	  DBTransaction.Commit();

	  DisplayKitchenTemplate();
	  DrawKitchenDocket();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

// ---------------------------------------------------------------------------
void TfrmPrinterMaintenance::UpdateReceiptFormatProfileListDisplay()
{
   if (CurrentPrinterProfileKey == 0)
   {
	  std::auto_ptr <TStringList> PrinterProfiles(new TStringList);
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

	  std::auto_ptr <TStringList> ReceiptPrinters(new TStringList);
	  TManagerPhysicalPrinter ManagerPhysicalPrinter;
	  ManagerPhysicalPrinter.DBGetPrinterList(DBTransaction, ReceiptPrinters.get());
	  if (ReceiptPrinters->Count > 0)
	  {
		 CurrentPrinterProfileKey = (int)ReceiptPrinters->Objects[0];
		 tbtnPrinterProfile->Caption = "Printer Profile\r" + ReceiptPrinters->Strings[0];
	  }
	  DBTransaction.Commit();
	  DisplayReceiptTemplate();
	  DrawReceiptDocket();
   }
   else
   {
	  tbtnPrinterProfile->Caption = "Receipt Printer\r None Selected";
	  DisplayReceiptTemplate();
	  DrawReceiptDocket();
   }
}

void __fastcall TfrmPrinterMaintenance::tbSelectReceiptPrinterMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
   int Y)
{
   try
   {
	  if (CurrentReceiptPrinterKey != 0)
	  {
		 SaveReceiptTemplate();
	  }

	  CurrentReceiptPrinterKey = 0;

	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

	  std::auto_ptr <TStringList> ReceiptPrinters(new TStringList);
	  TManagerPhysicalPrinter ManagerPhysicalPrinter;
	  ManagerPhysicalPrinter.DBGetPrinterList(DBTransaction, ReceiptPrinters.get());

	  std::auto_ptr <TfrmVerticalSelect> SelectionForm(TfrmVerticalSelect::Create <TfrmVerticalSelect> (this));
	  TVerticalSelection Item;
	  Item.Title = "Cancel";
	  Item.Properties["Color"] = IntToStr(clMaroon);
	  Item.CloseSelection = true;
	  SelectionForm->Items.push_back(Item);

	  for (int i = 0; i < ReceiptPrinters->Count; i++)
	  {
		 Item.Title = ReceiptPrinters->Strings[i];
		 Item.Properties["Color"] = IntToStr(clMaroon);
		 Item.Properties["Key"] = (int)ReceiptPrinters->Objects[i];
		 Item.CloseSelection = true;
		 SelectionForm->Items.push_back(Item);
	  }

	  SelectionForm->ShowModal();
	  TVerticalSelection SelectedItem;
	  if (SelectionForm->GetFirstSelectedItem(SelectedItem) && SelectedItem.Title != "Cancel")
	  {
		 CurrentReceiptPrinterKey = int(SelectedItem.Properties["Key"]);
		 tbSelectReceiptPrinter->Caption = "Receipt Printer\r" + SelectedItem.Title;
	  }
	  else
	  {
		 tbSelectReceiptPrinter->Caption = "Receipt Printer\r None Selected";
	  }

	  DBTransaction.Commit();

	  DisplayReceiptTemplate();
	  DrawReceiptDocket();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

// ---------------------------------------------------------------------------
void TfrmPrinterMaintenance::DisplayReceiptTemplate()
{
   while (lbReceiptPrintConfig->Count > 0)
   {
	  delete(TPSectionInstruction*)lbReceiptPrintConfig->Items->Objects[0];
	  lbReceiptPrintConfig->Items->Delete(0);
   }
   lbReceiptPrintConfig->Clear();

   if (CurrentReceiptPrinterKey != 0)
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

	  TSectionInstructStorage ReceiptTemplate;
	  Receipt->LoadTemplate(DBTransaction, CurrentReceiptPrinterKey, ReceiptTemplate);
	  for (int i = 0; i < ReceiptTemplate.size(); i++)
	  {
		 TPSectionInstruction *Instruction = new TPSectionInstruction();
		 *Instruction = ReceiptTemplate[i];
		 lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);
	  }
	  DBTransaction.Commit();
   }
}

void TfrmPrinterMaintenance::SaveReceiptTemplate()
{
   TSectionInstructStorage ReceiptTemplate;
   ReceiptTemplate.clear();
   for (int i = 0; i < lbReceiptPrintConfig->Count; i++)
   {
	  TPSectionInstruction Instruction = *(TPSectionInstruction*)lbReceiptPrintConfig->Items->Objects[i];
	  ReceiptTemplate.push_back(Instruction);
   }

   if (CurrentReceiptPrinterKey != 0)
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();
	  Receipt->SaveTemplate(DBTransaction, CurrentReceiptPrinterKey, ReceiptTemplate);
	  DBTransaction.Commit();
   }
}

void __fastcall TfrmPrinterMaintenance::DrawReceiptDocket()
{
   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();

   if (frmSelectDish->ParkedSales->GetCount(DBTransaction) > 0)
   {

	  std::auto_ptr <TReqPrintJob> TempReceipt(new TReqPrintJob(&TDeviceRealTerminal::Instance()));
	  TPaymentTransaction ReceiptTransaction(DBTransaction);
	  TempReceipt->Transaction = &ReceiptTransaction;
	  TempReceipt->JobType = pjReceiptReceipt;
	  TempReceipt->SenderType = devPC;
	  TempReceipt->Waiter = TDeviceRealTerminal::Instance().User.Name;
	  TempReceipt->PaymentType = ptPreliminary;

	  std::vector <TSeatOrders*> SeatOrders;

	  TSeatOrders *Temp = new TSeatOrders(0);
	  SeatOrders.push_back(Temp);
	  for (int i = 1; i < 10; i++)
	  {
		 Temp = new TSeatOrders(0);
		 SeatOrders.push_back(Temp);
	  }

	  std::auto_ptr <TStringList> Data(new TStringList);
	  frmSelectDish->ParkedSales->GetParkedSales(DBTransaction, Data.get());
	  TParkedSale *Sale = (TParkedSale*)frmSelectDish->ParkedSales->GetParkedSale(DBTransaction, (int)Data->Objects[0]);
	  Sale->GetSaleCopy(SeatOrders);

	  int SelectedSeat;
	  int SelectedTable;
	  AnsiString SelectedTabContainerName;
	  AnsiString SelectedParty;
	  int SelectedTab = 0;
	  SelectedSeat = Sale->SelectedSeat;
	  SelectedTable = Sale->SelectedTable;
	  SelectedTabContainerName = Sale->SelectedTabContainerName;
	  SelectedParty = Sale->SelectedParty;

	  TDateTime OrderedTimeStamp = Now();

	  for (UINT iSeat = 0; iSeat < SeatOrders.size(); iSeat++)
	  {
		 for (int i = 0; i < SeatOrders[iSeat]->Orders->Count; i++)
		 {
			TItemComplete *Order = SeatOrders[iSeat]->Orders->Items[i];
			Order->Terminal = TDeviceRealTerminal::Instance().ID.Name;
			// Order->TabKey					= iSeat; // This is faked to get ravel correctly
			Order->TimeStamp = Now();
			Order->OrderType = NormalOrder;
			Order->OrderedLocation = TDeviceRealTerminal::Instance().ID.Location;
			if (iSeat == 0)
			{
			   Order->TabName = "Cash Sale";
			   Order->TableNo = 0;
			   Order->SeatNo = 0;
			   Order->TabType = TabNone;
			}
			else
			{
			   Order->TabName = SeatOrders[iSeat]->SeatName;
			   Order->TableNo = SelectedTable;
			   Order->SeatNo = iSeat;
			   Order->TabType = TabTableSeat;
			   Order->TabKey = iSeat;
			}
			ReceiptTransaction.Orders->Add(Order);
		 }
	  }

	  TSectionInstructStorage NewTemplate;
	  NewTemplate.clear();
	  for (int i = 0; i < lbReceiptPrintConfig->Count; i++)
	  {
		 TPSectionInstruction Instruction = *(TPSectionInstruction*)lbReceiptPrintConfig->Items->Objects[i];
		 NewTemplate.push_back(Instruction);
	  }

	  TPrinterPhysical DefaultScreenPrinter;
	  DefaultScreenPrinter.NormalCharPerLine = 30;
	  DefaultScreenPrinter.BoldCharPerLine = 30;

	  Receipt->GetPrintoutsPreview(DBTransaction, TempReceipt.get(), DefaultScreenPrinter, &NewTemplate);
	  reReceiptDisplay->Lines->Clear();
	  std::auto_ptr <TStringList> OrderDisplay(new TStringList());
	  TempReceipt->Printouts->PrintToStrings(OrderDisplay.get());

	  for (int i = 0; i < OrderDisplay->Count; i++)
	  {
		 AnsiString Temp = OrderDisplay->Strings[i];
		 reReceiptDisplay->Lines->Add(Temp);
	  }

	  for (UINT iSeat = 0; iSeat < SeatOrders.size(); iSeat++)
	  {
		 for (int i = 0; i < SeatOrders[iSeat]->Orders->Count; i++)
		 {
			TItemComplete *Order = SeatOrders[iSeat]->Orders->Items[i];
			Order->TabKey = 0;
		 }
	  }

	  for (UINT i = 0; i < SeatOrders.size(); i++)
	  {
		 delete SeatOrders[i];
	  }
   }
   DBTransaction.Commit();
}

void __fastcall TfrmPrinterMaintenance::tbReceiptTemplateUpMouseClick(TObject *Sender)
{
   int OldIndex = lbReceiptPrintConfig->ItemIndex;
   if (lbReceiptPrintConfig->ItemIndex > 0)
   {
	  lbReceiptPrintConfig->Items->Move(OldIndex, OldIndex - 1);
   }
   ReceiptTemplateModified = true;
   DrawReceiptDocket();
   lbReceiptPrintConfig->ItemIndex = OldIndex - 1;
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::tbReceiptTemplateDownMouseClick(TObject *Sender)
{
   int OldIndex = lbReceiptPrintConfig->ItemIndex;
   if (lbReceiptPrintConfig->ItemIndex != -1 && lbReceiptPrintConfig->ItemIndex < lbReceiptPrintConfig->Items->Count - 1)
   {
	  lbReceiptPrintConfig->Items->Move(OldIndex, OldIndex + 1);
   }
   ReceiptTemplateModified = true;
   DrawReceiptDocket();
   lbReceiptPrintConfig->ItemIndex = OldIndex + 1;

}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::tbReceiptPropertiesMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if (lbReceiptPrintConfig->ItemIndex != -1 && lbReceiptPrintConfig->ItemIndex < lbReceiptPrintConfig->Items->Count)
   {
	  TPSectionInstruction OldInstruction = *(TPSectionInstruction*)lbReceiptPrintConfig->Items->Objects[lbReceiptPrintConfig->ItemIndex];

	  std::auto_ptr <TfrmPSectionInstructionEdit> frmPSectionInstructionEdit
		  (TfrmPSectionInstructionEdit::Create <TfrmPSectionInstructionEdit> (this));

	  frmPSectionInstructionEdit->AfterPropertyChanged.RegisterForEvent(OnReceiptPropertyChanged);

	  frmPSectionInstructionEdit->Instruction = (TPSectionInstruction*)
		  lbReceiptPrintConfig->Items->Objects[lbReceiptPrintConfig->ItemIndex];
	  if (frmPSectionInstructionEdit->ShowModal() != mrOk)
	  {
		 *(TPSectionInstruction*)lbReceiptPrintConfig->Items->Objects[lbReceiptPrintConfig->ItemIndex] = OldInstruction;
		 DrawReceiptDocket();
	  }
	  else
	  {
		 ReceiptTemplateModified = true;
	  }
	  frmPSectionInstructionEdit->AfterPropertyChanged.DeregisterForEvent(OnReceiptPropertyChanged);
	  DrawReceiptDocket();
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::tbReceiptInsertMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if (CurrentReceiptPrinterKey != 0)
   {
	  if (lbReceiptPrintOpt->ItemIndex > -1)
	  {
		 TPSectionInstruction *Instruction = new TPSectionInstruction();
		 *Instruction = *((TPSectionInstruction*)lbReceiptPrintOpt->Items->Objects[lbReceiptPrintOpt->ItemIndex]);

		 if (lbReceiptPrintConfig->ItemIndex < 0)
		 {
			lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);
		 }
		 else
		 {
			lbReceiptPrintConfig->Items->InsertObject(lbReceiptPrintConfig->ItemIndex + 1, Instruction->Caption, (TObject*)Instruction);
		 }

		 if (lbReceiptPrintOpt->ItemIndex < lbReceiptPrintOpt->Items->Count - 1)
		 {
			lbReceiptPrintOpt->ItemIndex++;
		 }
		 lbReceiptPrintConfig->ItemIndex++;
		 ReceiptTemplateModified = true;
	  }
	  DrawReceiptDocket();
   }
   else
   {
	  MessageBox("Please select a printer profile.", "Error", MB_ICONERROR + MB_OK);
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::tbReceiptRemoveMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if (CurrentReceiptPrinterKey != 0)
   {
	  if (lbReceiptPrintConfig->ItemIndex > -1)
	  {
		 delete(TPSectionInstruction*)lbReceiptPrintConfig->Items->Objects[lbReceiptPrintConfig->ItemIndex];
		 int CurrentIndex = lbReceiptPrintConfig->ItemIndex;
		 lbReceiptPrintConfig->Items->Delete(lbReceiptPrintConfig->ItemIndex);
		 if (CurrentIndex < lbReceiptPrintConfig->Items->Count)
		 {
			lbReceiptPrintConfig->ItemIndex = CurrentIndex;
		 }
		 else if (CurrentIndex = lbReceiptPrintConfig->Items->Count)
		 {
			lbReceiptPrintConfig->ItemIndex = lbReceiptPrintConfig->Items->Count - 1;
		 }
		 else if (lbReceiptPrintConfig->ItemIndex < lbReceiptPrintConfig->Items->Count - 1)
		 {
			lbReceiptPrintConfig->ItemIndex++;
		 }
		 ReceiptTemplateModified = true;
	  }
	  DisplayReceiptPrinterInstructionSet();
	  DrawReceiptDocket();
   }
   else
   {
	  MessageBox("Please select a printer profile.", "Error", MB_ICONERROR + MB_OK);
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::tbtnReceiptTemplatesMouseClick(TObject *Sender)
{
   if (CurrentReceiptPrinterKey != 0)
   {
	  // Display Launch List
	  std::auto_ptr <TfrmVerticalSelect> SelectionForm(TfrmVerticalSelect::Create <TfrmVerticalSelect> (this));

	  TVerticalSelection Item;
	  Item.Title = "Cancel";
	  Item.Properties["Color"] = IntToStr(clMaroon);
	  Item.CloseSelection = true;
	  SelectionForm->Items.push_back(Item);

	  TVerticalSelection Item1;
	  Item1.Title = "Default";
	  Item1.Properties["Action"] = IntToStr(1);
	  Item1.Properties["Color"] = IntToStr(clNavy);
	  Item1.CloseSelection = true;
	  SelectionForm->Items.push_back(Item1);

	  Item1.Title = "Split Food Drink";
	  Item1.Properties["Action"] = IntToStr(2);
	  Item1.Properties["Color"] = IntToStr(clNavy);
	  Item1.CloseSelection = true;
	  SelectionForm->Items.push_back(Item1);

	  SelectionForm->ShowModal();
	  TVerticalSelection SelectedItem;
	  if (SelectionForm->GetFirstSelectedItem(SelectedItem) && SelectedItem.Title != "Cancel")
	  {
		 int Action = StrToIntDef(SelectedItem.Properties["Action"], 0);
		 switch(Action)
		 {
		 case 1:
			{
			   while (lbReceiptPrintConfig->Items->Count != 0)
			   {
				  delete(TPSectionInstruction*)lbReceiptPrintConfig->Items->Objects[0];
				  lbReceiptPrintConfig->Items->Delete(0);
			   }

			   TPSectionInstruction *Instruction = new TPSectionInstruction(epofiPrintReceiptLogo);
			   Instruction->Cut = true;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintSaleTypeDetail);
			   Instruction->GroupNo = 1;
                           Instruction->LineAbove = true;
                           Instruction->LineBelow = false;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintReceiptHeader);
			   Instruction->GroupNo = 1;
                           Instruction->LineAbove = false;
			   Instruction->DrawLineBelow = true;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiFilterOutCancels);
			   Instruction->GroupNo = 1;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintTable);
			   Instruction->GroupNo = 2;
			   Instruction->DrawLineBelow = true;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintReceiptInfo);
			   Instruction->GroupNo = 3;
			   Instruction->LineAbove = false;
			   Instruction->LineBelow = true;
			   Instruction->DrawLineBelow = true;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintCurrencySymbol);
			   Instruction->GroupNo = 4;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintSeat);
			   Instruction->GroupNo = 4;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintItemsTotal);
			   Instruction->GroupNo = 5;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintTotalEx);
			   Instruction->LineBelow = true;
			   Instruction->DrawLineAbove = true;
			   Instruction->GroupNo = 4;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintTotalDiscountsName);
			   Instruction->GroupNo = 1;
			   Instruction->DrawLineAbove = true;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintPaymentSurcharges);
			   Instruction->GroupNo = 1;
			   Instruction->DrawLineAbove = true;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintTotal);
			   Instruction->GroupNo = 1;
			   Instruction->DrawLineAbove = true;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintPaymentTotals);
			   Instruction->GroupNo = 1;
			   Instruction->DrawLineAbove = true;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintSeparatedTaxTotals);
			   Instruction->GroupNo = 1;
			   Instruction->DrawLineAbove = true;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintAccCreditInfo);
			   Instruction->GroupNo = 1;
			   Instruction->DrawLineAbove = true;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintMembersName);
			   Instruction->GroupNo = 1;
			   Instruction->DrawLineAbove = true;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintPointsDetails);
			   Instruction->GroupNo = 1;
			   Instruction->DrawLineAbove = true;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintReceiptFooter);
			   Instruction->GroupNo = 1;
			   Instruction->DrawLineAbove = true;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   ReceiptTemplateModified = true;
			   DrawReceiptDocket();
			}break;
		 case 2:
			{
			   while (lbReceiptPrintConfig->Items->Count != 0)
			   {
				  delete(TPSectionInstruction*)lbReceiptPrintConfig->Items->Objects[0];
				  lbReceiptPrintConfig->Items->Delete(0);
			   }

			   TPSectionInstruction *Instruction = new TPSectionInstruction(epofiPrintReceiptLogo);
			   Instruction->Cut = true;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintSaleTypeDetail);
			   Instruction->GroupNo = 1;
               Instruction->LineAbove = true;
               Instruction->LineBelow = false;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

               Instruction = new TPSectionInstruction(epofiPrintReceiptHeader);
			   Instruction->GroupNo = 1;
			   Instruction->LineAbove = false;
			   Instruction->DrawLineBelow = true;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiFilterOutCancels);
			   Instruction->GroupNo = 1;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintTable);
			   Instruction->GroupNo = 2;
			   Instruction->DrawLineBelow = true;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintReceiptInfo);
			   Instruction->GroupNo = 3;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintServingCourse);
			   Instruction->GroupNo = 4;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintItemsTotal);
			   Instruction->GroupNo = 4;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintTotalEx);
			   Instruction->LineBelow = true;
			   Instruction->DrawLineAbove = true;
			   Instruction->GroupNo = 4;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintTotalDiscountsName);
			   Instruction->GroupNo = 1;
			   Instruction->DrawLineAbove = true;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintPaymentSurcharges);
			   Instruction->GroupNo = 1;
			   Instruction->DrawLineAbove = true;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintTotal);
			   Instruction->GroupNo = 1;
			   Instruction->DrawLineAbove = true;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintPaymentTotals);
			   Instruction->GroupNo = 1;
			   Instruction->DrawLineAbove = true;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintSeparatedTaxTotals);
			   Instruction->GroupNo = 1;
			   Instruction->DrawLineAbove = true;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintAccCreditInfo);
			   Instruction->GroupNo = 1;
			   Instruction->DrawLineAbove = true;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintMembersName);
			   Instruction->GroupNo = 1;
			   Instruction->DrawLineAbove = true;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintPointsDetails);
			   Instruction->GroupNo = 1;
			   Instruction->DrawLineAbove = true;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

               Instruction = new TPSectionInstruction(epofiPickUpPrintCustomerName);
			   Instruction->GroupNo = 1;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

               Instruction = new TPSectionInstruction(epofiPickUpPrintCustomerPhone);
			   Instruction->GroupNo = 1;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintReceiptFooter);
			   Instruction->GroupNo = 1;
			   Instruction->DrawLineAbove = true;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   ReceiptTemplateModified = true;
			   DrawReceiptDocket();
			}break;
		 case 3:
			{
			   while (lbReceiptPrintConfig->Items->Count != 0)
			   {
				  delete(TPSectionInstruction*)lbReceiptPrintConfig->Items->Objects[0];
				  lbReceiptPrintConfig->Items->Delete(0);
			   }

			   TPSectionInstruction *Instruction = new TPSectionInstruction(epofiPrintReceiptLogo);
			   Instruction->Cut = true;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintSaleTypeDetail);
			   Instruction->GroupNo = 1;
               Instruction->LineAbove = true;
               Instruction->LineBelow = false;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintReceiptHeader);
			   Instruction->GroupNo = 1;
			   Instruction->LineAbove = false;
			   Instruction->DrawLineBelow = true;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiFilterOutCancels);
			   Instruction->GroupNo = 1;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintTable);
			   Instruction->GroupNo = 2;
			   Instruction->DrawLineBelow = true;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintReceiptInfo);
			   Instruction->GroupNo = 3;
			   Instruction->LineAbove = false;
			   Instruction->LineBelow = true;
			   Instruction->DrawLineBelow = true;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiSortByFoodDrink);
			   Instruction->GroupNo = 3;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintServingCourse);
			   Instruction->GroupNo = 4;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintItemsTotal);
			   Instruction->GroupNo = 5;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   /* Instruction = new TPSectionInstruction(epofiPrintTotalEx);
			   Instruction->LineBelow = true;
			   Instruction->DrawLineAbove = true;
			   Instruction->GroupNo = 3;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject *)Instruction); */

			   Instruction = new TPSectionInstruction(epofiPrintTotalDiscountsName);
			   Instruction->GroupNo = 1;
			   Instruction->DrawLineAbove = true;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintPaymentSurcharges);
			   Instruction->GroupNo = 1;
			   Instruction->DrawLineAbove = true;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintTotal);
			   Instruction->GroupNo = 1;
			   Instruction->DrawLineAbove = true;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintPaymentTotals);
			   Instruction->GroupNo = 1;
			   Instruction->DrawLineAbove = true;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintSeparatedTaxTotals);
			   Instruction->GroupNo = 1;
			   Instruction->DrawLineAbove = true;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintAccCreditInfo);
			   Instruction->GroupNo = 1;
			   Instruction->DrawLineAbove = true;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintMembersName);
			   Instruction->GroupNo = 1;
			   Instruction->DrawLineAbove = true;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintPointsDetails);
			   Instruction->GroupNo = 1;
			   Instruction->DrawLineAbove = true;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

               Instruction = new TPSectionInstruction(epofiPickUpPrintCustomerName);
			   Instruction->GroupNo = 1;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

               Instruction = new TPSectionInstruction(epofiPickUpPrintCustomerPhone);
			   Instruction->GroupNo = 1;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintReceiptFooter);
			   Instruction->GroupNo = 1;
			   Instruction->DrawLineAbove = true;
			   lbReceiptPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   ReceiptTemplateModified = true;
			   DrawReceiptDocket();
			}break;
		 }
	  }
   }
   else
   {
	  MessageBox("Please select a printer profile.", "Error", MB_ICONERROR + MB_OK);
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::tbtnReceiptTestPrintMouseClick(TObject *Sender)
{
   try
   {
     Database::TDBTransaction DBTransaction(DBControl);
     DBTransaction.StartTransaction();
	  if (frmSelectDish->ParkedSales->GetCount(DBTransaction) > 0)
	  {

		 std::vector <TSeatOrders*> SeatOrders;

		 TSeatOrders *Temp = new TSeatOrders(0);
		 SeatOrders.push_back(Temp);
		 for (int i = 1; i < 10; i++)
		 {
			Temp = new TSeatOrders(0);
			SeatOrders.push_back(Temp);
		 }

		 std::auto_ptr <TPaymentTransaction> Transaction(new TPaymentTransaction(DBTransaction));

		 TParkedSale *Sale = (TParkedSale*)frmSelectDish->ParkedSales->GetFirstParkedSale(DBTransaction);
		 Sale->GetSaleCopy(SeatOrders);

		 int SelectedTable;
		 AnsiString SelectedTabContainerName;
		 AnsiString SelectedParty;

		 SelectedTable = Sale->SelectedTable;
		 SelectedTabContainerName = Sale->SelectedTabContainerName;
		 SelectedParty = Sale->SelectedParty;

		 TDateTime OrderedTimeStamp = Now();

		 for (UINT iSeat = 0; iSeat < SeatOrders.size(); iSeat++)
		 {
			for (int i = 0; i < SeatOrders[iSeat]->Orders->Count; i++)
			{
			   TItemComplete *Order = SeatOrders[iSeat]->Orders->Items[i];
			   Order->Terminal = TDeviceRealTerminal::Instance().ID.Name;
			   // Order->TabKey					= iSeat; // This is faked to get ravel correctly
			   Order->TimeStamp = Now();
			   Order->OrderType = NormalOrder;
			   Order->OrderedLocation = TDeviceRealTerminal::Instance().ID.Location;
			   if (iSeat == 0)
			   {
				  Order->TabName = "Cash Sale";
				  Order->TableNo = 0;
				  Order->SeatNo = 0;
				  Order->TabType = TabNone;
			   }
			   else
			   {
				  Order->TabName = SeatOrders[iSeat]->SeatName;
				  Order->TableNo = SelectedTable;
				  Order->SeatNo = iSeat;
				  Order->TabType = TabTableSeat;
				  Order->TabKey = iSeat;
			   }
			   Transaction->Orders->Add(Order);
			}
		 }

		 if (Transaction->Orders->Count > 0)
		 {
			Display1->Clear();
			TReqPrintJob *TempReceipt = new TReqPrintJob(&TDeviceRealTerminal::Instance());
			TempReceipt->PaymentType = ptPreliminary;
			TempReceipt->Transaction = Transaction.get();
			TempReceipt->Waiter = "Demo";
			TempReceipt->BarCodeData = 1; // So Barcode Section Prints

			TSectionInstructStorage NewReceiptTemplate;
			NewReceiptTemplate.clear();
			for (int i = 0; i < lbReceiptPrintConfig->Count; i++)
			{
			   TPSectionInstruction Instruction = *(TPSectionInstruction*)lbReceiptPrintConfig->Items->Objects[i];
			   NewReceiptTemplate.push_back(Instruction);
			}

			Receipt->GetPrintoutsPreview(DBTransaction, TempReceipt, TComms::Instance().ReceiptPrinter, &NewReceiptTemplate);
			// Forces Redirects.
			TempReceipt->Printouts->Print(devPC);
			delete TempReceipt;
		 }

		 for (UINT iSeat = 0; iSeat < SeatOrders.size(); iSeat++)
		 {
			for (int i = 0; i < SeatOrders[iSeat]->Orders->Count; i++)
			{
			   TItemComplete *Order = SeatOrders[iSeat]->Orders->Items[i];
			   Order->TabKey = 0;
			}
		 }

		 for (UINT i = 0; i < SeatOrders.size(); i++)
		 {
			delete SeatOrders[i];
		 }
	  }
	  DBTransaction.Commit();
   }
   __finally
   {
   }
   DrawKitchenDocket();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::tbtnIndentSectionLeftMouseClick(TObject *Sender)
{
   TListBox *ListBox = lbReceiptPrintConfig;
   if (ListBox->ItemIndex != -1 && ListBox->ItemIndex < ListBox->Items->Count)
   {
	  TPSectionInstruction OldInstruction = *(TPSectionInstruction*)ListBox->Items->Objects[ListBox->ItemIndex];
	  OldInstruction.GroupNo--;
	  if (OldInstruction.GroupNo < 0)
	  {
		 OldInstruction.GroupNo = 0;
	  }
	  *(TPSectionInstruction*)ListBox->Items->Objects[ListBox->ItemIndex] = OldInstruction;

	  ReceiptTemplateModified = true;
	  DrawReceiptDocket();
	  ListBox->Refresh();
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::tbtnIndentSectionRightMouseClick(TObject *Sender)
{
   TListBox *ListBox = lbReceiptPrintConfig;
   if (ListBox->ItemIndex != -1 && ListBox->ItemIndex < ListBox->Items->Count)
   {
	  TPSectionInstruction OldInstruction = *(TPSectionInstruction*)ListBox->Items->Objects[lbReceiptPrintConfig->ItemIndex];
	  OldInstruction.GroupNo++;
	  if (OldInstruction.GroupNo > 10)
	  {
		 OldInstruction.GroupNo = 10;
	  }
	  *(TPSectionInstruction*)ListBox->Items->Objects[ListBox->ItemIndex] = OldInstruction;

	  ReceiptTemplateModified = true;
	  DrawReceiptDocket();
	  ListBox->Refresh();
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::lbReceiptPrintConfigDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State)
{
   TCanvas *pCanvas = ((TListBox*)Control)->Canvas;
//   TColor Color = pCanvas->Brush->Color;
//   TBrushStyle Style = pCanvas->Brush->Style;
   pCanvas->FillRect(Rect); // clear the rectangle

   AnsiString Caption = ((TListBox*)Control)->Items->Strings[Index];
   TPSectionInstruction Instruction = *(TPSectionInstruction*)((TListBox*)Control)->Items->Objects[Index];
   switch(Instruction.GroupNo)
   {
   case 1:
	  pCanvas->Font->Color = (TColor)clNavy;
	  break;

   case 2:
	  pCanvas->Font->Color = (TColor)clTeal;
	  break;

   case 3:
	  pCanvas->Font->Color = (TColor)clMaroon;
	  break;
   case 4:
	  pCanvas->Font->Color = (TColor)clPurple;
	  break;
   case 5:
	  pCanvas->Font->Color = (TColor)clAqua;
	  break;
   default:
	  pCanvas->Font->Color = (TColor)clBlack;
	  break;
   }
   int Indent = 3 * Instruction.GroupNo;

   UnicodeString AnsiIndent = UnicodeString::StringOfChar(' ', Indent) + Caption;

   int TextHeight = DrawTextExW(pCanvas->Handle, AnsiIndent.w_str(), AnsiIndent.Length(),
	  &TRect(Rect.left, Rect.top, Rect.right, Rect.bottom), DT_CALCRECT | DT_WORDBREAK | DT_NOPREFIX, NULL) + 4;

   TRect CentredRect = Rect;
   int RowHeight = Rect.Bottom - Rect.Top;
   if (RowHeight > TextHeight)
   {
	  CentredRect.Top += (RowHeight - TextHeight) / 2;
	  CentredRect.Bottom = CentredRect.Top + TextHeight;
   }
   DrawTextExW(pCanvas->Handle, AnsiIndent.w_str(), AnsiIndent.Length(), &CentredRect, DT_WORDBREAK | DT_NOPREFIX, NULL);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::tbtnKitchenIndentLeftMouseClick(TObject *Sender)
{
   TListBox *ListBox = lbPrintConfig;
   if (ListBox->ItemIndex != -1 && ListBox->ItemIndex < ListBox->Items->Count)
   {
	  TPSectionInstruction OldInstruction = *(TPSectionInstruction*)ListBox->Items->Objects[ListBox->ItemIndex];
	  OldInstruction.GroupNo--;
	  if (OldInstruction.GroupNo < 0)
	  {
		 OldInstruction.GroupNo = 0;
	  }
	  *(TPSectionInstruction*)ListBox->Items->Objects[ListBox->ItemIndex] = OldInstruction;

	  KitchenTemplateModified = true;
	  DrawKitchenDocket();
	  ListBox->Refresh();
   }

}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::tbtnKitchenIndentRightMouseClick(TObject *Sender)
{
   TListBox *ListBox = lbPrintConfig;
   if (ListBox->ItemIndex != -1 && ListBox->ItemIndex < ListBox->Items->Count)
   {
	  TPSectionInstruction OldInstruction = *(TPSectionInstruction*)ListBox->Items->Objects[ListBox->ItemIndex];
	  OldInstruction.GroupNo++;
	  if (OldInstruction.GroupNo > 10)
	  {
		 OldInstruction.GroupNo = 10;
	  }
	  *(TPSectionInstruction*)ListBox->Items->Objects[ListBox->ItemIndex] = OldInstruction;

	  KitchenTemplateModified = true;
	  DrawKitchenDocket();
	  ListBox->Refresh();
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::tbtnKitchenTemplatesMouseClick(TObject *Sender)
{
   if (CurrentPrinterProfileKey != 0)
   {
	  // Display Launch List
	  std::auto_ptr <TfrmVerticalSelect> SelectionForm(TfrmVerticalSelect::Create <TfrmVerticalSelect> (this));

	  TVerticalSelection Item;
	  Item.Title = "Cancel";
	  Item.Properties["Color"] = IntToStr(clMaroon);
	  Item.CloseSelection = true;
	  SelectionForm->Items.push_back(Item);

	  TVerticalSelection Item1;
	  Item1.Title = "Default";
	  Item1.Properties["Action"] = IntToStr(1);
	  Item1.Properties["Color"] = IntToStr(clNavy);
	  Item1.CloseSelection = true;
	  SelectionForm->Items.push_back(Item1);

	  Item1.Title = "By Course";
	  Item1.Properties["Action"] = IntToStr(2);
	  Item1.Properties["Color"] = IntToStr(clNavy);
	  Item1.CloseSelection = true;
	  SelectionForm->Items.push_back(Item1);

	  Item1.Title = "Expanded";
	  Item1.Properties["Action"] = IntToStr(3);
	  Item1.Properties["Color"] = IntToStr(clNavy);
	  Item1.CloseSelection = true;
	  SelectionForm->Items.push_back(Item1);

	  SelectionForm->ShowModal();
	  TVerticalSelection SelectedItem;
	  if (SelectionForm->GetFirstSelectedItem(SelectedItem) && SelectedItem.Title != "Cancel")
	  {
		 int Action = StrToIntDef(SelectedItem.Properties["Action"], 0);
		 switch(Action)
		 {
		 case 1:
			{
			   while (lbPrintConfig->Items->Count != 0)
			   {
				  delete(TPSectionInstruction*)lbPrintConfig->Items->Objects[0];
				  lbPrintConfig->Items->Delete(0);
			   }

			   TPSectionInstruction *Instruction = new TPSectionInstruction(epofiPrintChitNumber);
			   Instruction->GroupNo = 0;
			   Instruction->FontInfo.Bold = true;
			   Instruction->FontInfo.Width = fsDoubleSize;
			   Instruction->FontInfo.Height = fsDoubleSize;
			   Instruction->DrawLineAbove = true;
			   lbPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintTable);
			   Instruction->GroupNo = 0;
			   Instruction->FontInfo.Bold = true;
			   Instruction->FontInfo.Width = fsDoubleSize;
			   Instruction->FontInfo.Height = fsDoubleSize;
			   Instruction->DrawLineAbove = true;
			   lbPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintKitchenInfo);
			   Instruction->Cut = true;
			   Instruction->GroupNo = 1;
			   Instruction->DrawLineAbove = true;
			   Instruction->FontInfo.Bold = true;
			   lbPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintServingCourse);
			   Instruction->GroupNo = 2;
			   Instruction->FontInfo.Bold = true;
			   Instruction->FontInfo.Width = fsDoubleSize;
			   Instruction->FontInfo.Height = fsDoubleSize;
			   Instruction->DrawLineAbove = true;
			   lbPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintItems);
			   Instruction->GroupNo = 3;
			   lbPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   KitchenTemplateModified = true;
			   DrawKitchenDocket();
			}break;
		 case 2:
			{
			   while (lbPrintConfig->Items->Count != 0)
			   {
				  delete(TPSectionInstruction*)lbPrintConfig->Items->Objects[0];
				  lbPrintConfig->Items->Delete(0);
			   }

			   TPSectionInstruction *Instruction = new TPSectionInstruction(epofiPrintChitNumber);
			   Instruction->GroupNo = 0;
			   Instruction->FontInfo.Bold = true;
			   Instruction->FontInfo.Width = fsDoubleSize;
			   Instruction->FontInfo.Height = fsDoubleSize;
			   Instruction->DrawLineAbove = true;
			   lbPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintTable);
			   Instruction->GroupNo = 0;
			   Instruction->FontInfo.Bold = true;
			   Instruction->FontInfo.Width = fsDoubleSize;
			   Instruction->FontInfo.Height = fsDoubleSize;
			   Instruction->DrawLineAbove = true;
			   lbPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintKitchenInfo);
			   Instruction->Cut = true;
			   Instruction->GroupNo = 1;
			   Instruction->DrawLineAbove = true;
			   Instruction->FontInfo.Bold = true;
			   lbPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintCourse);
			   Instruction->GroupNo = 2;
			   Instruction->FontInfo.Bold = true;
			   Instruction->FontInfo.Width = fsDoubleSize;
			   Instruction->FontInfo.Height = fsDoubleSize;
			   Instruction->DrawLineAbove = true;
			   lbPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintItems);
			   Instruction->GroupNo = 3;
			   lbPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   KitchenTemplateModified = true;
			   DrawKitchenDocket();
			}break;
		 case 3:
			{
			   while (lbPrintConfig->Items->Count != 0)
			   {
				  delete(TPSectionInstruction*)lbPrintConfig->Items->Objects[0];
				  lbPrintConfig->Items->Delete(0);
			   }

			   TPSectionInstruction *Instruction = new TPSectionInstruction(epofiPrintChitNumber);
			   Instruction->GroupNo = 0;
			   Instruction->FontInfo.Bold = true;
			   Instruction->FontInfo.Width = fsDoubleSize;
			   Instruction->FontInfo.Height = fsDoubleSize;
			   Instruction->DrawLineAbove = true;
			   lbPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintTable);
			   Instruction->GroupNo = 0;
			   Instruction->FontInfo.Bold = true;
			   Instruction->FontInfo.Width = fsDoubleSize;
			   Instruction->FontInfo.Height = fsDoubleSize;
			   Instruction->DrawLineAbove = true;
			   lbPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintKitchenInfo);
			   Instruction->Cut = true;
			   Instruction->GroupNo = 1;
			   Instruction->DrawLineAbove = true;
			   Instruction->FontInfo.Bold = true;
			   lbPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintServingCourse);
			   Instruction->GroupNo = 2;
			   Instruction->FontInfo.Bold = true;
			   Instruction->FontInfo.Width = fsDoubleSize;
			   Instruction->FontInfo.Height = fsDoubleSize;
			   Instruction->DrawLineAbove = true;
			   lbPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintItem);
			   Instruction->GroupNo = 3;
			   lbPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintItemOptions);
			   Instruction->GroupNo = 4;
			   lbPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintItemSides);
			   Instruction->GroupNo = 4;
			   lbPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   Instruction = new TPSectionInstruction(epofiPrintItemNote);
			   Instruction->GroupNo = 4;
			   lbPrintConfig->Items->AddObject(Instruction->Caption, (TObject*)Instruction);

			   KitchenTemplateModified = true;
			   DrawKitchenDocket();
			}break;
		 }
	  }
   }
   else
   {
	  MessageBox("Please select a printer profile.", "Error", MB_ICONERROR + MB_OK);
   }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmPrinterMaintenance::tbtnTestCharSetsMouseClick(TObject *Sender)
{
   if (lbPrinters->ItemIndex != -1)
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

	  TManagerPhysicalPrinter ManagerPhysicalPrinter;
	  TPrinterPhysical Printer = ManagerPhysicalPrinter.GetPhysicalPrinter(DBTransaction,
		 (int)(lbPrinters->Items->Objects[lbPrinters->ItemIndex]));
	  if (Printer.PhysicalPrinterKey != 0)
	  {
      	if(Printer.PrinterInitCmd != "")
         {
            int JobID = 0;
            AnsiString LineStr = "";
            std::vector <char> BufferInit(Printer.PrinterInitCmd.Length()/2, 0x00);
            HexToBin(Printer.PrinterInitCmd.LowerCase().c_str(), &BufferInit[0], BufferInit.size());
            LineStr = AnsiString(&BufferInit[0], BufferInit.size());
            WinPrinters::PrintRaw("Receipt","My Job0", LineStr, JobID);
				MessageBox("Init Command Sent, Restart printer if necessary.", "Error", MB_ICONERROR + MB_OK);
         }
         else
         {
			  MessageBox("This Printer has no Init Command.", "Error", MB_ICONERROR + MB_OK);
         }
	  }
	  DBTransaction.Commit();
   }
}

void __fastcall TfrmPrinterMaintenance::cbAPWOOSClick(TObject *Sender)
{
   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();
   TGlobalSettings::Instance().AutoPrintWebReceipts = cbAPWOOS->Checked;
   TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmAutoPrintWebReceipts, TGlobalSettings::Instance().AutoPrintWebReceipts);
   DBTransaction.Commit();
}

void TfrmPrinterMaintenance::UpdatePrinterBreakDownDisplay()
{
   try
   {
	  CurrentPrinterProfileKey = 0;
	  PrinterProfileBreakDownCategories.clear();

	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

	  std::auto_ptr <TStringList> PrinterProfiles(new TStringList);
	  std::auto_ptr <TManagerPhysicalPrinter> ManagerPhysicalPrinter(new TManagerPhysicalPrinter);

	  TManagerVirtualPrinter::GetVirtualPrinterList(DBTransaction, PrinterProfiles.get());

	  // Add Physical Printer Connection Info.
	  for (int i = 0; i < PrinterProfiles->Count; i++)
	  {
		 int VirtualPrinterKey = (int)PrinterProfiles->Objects[i];
		 TPrinterVirtual *vPrinter = TManagerVirtualPrinter::GetVirtualPrinterByKey(DBTransaction,VirtualPrinterKey);
		 if (vPrinter)
		 {
			TPrinterPhysical PrinterPhysical = ManagerPhysicalPrinter->GetPhysicalPrinter(DBTransaction, vPrinter->PhysicalPrinterKey);
			PrinterProfiles->Strings[i] = PrinterProfiles->Strings[i] + " [" + PrinterPhysical.ServerName + " : " +
				PrinterPhysical.ShareName + "]";
		 }
	  }

	  tgPrnProfileBreakDwn->RowCount = 0; // Clears all the Latching.
	  tgPrnProfileBreakDwn->ColCount = 1;
	  tgPrnProfileBreakDwn->RowCount = PrinterProfiles->Count;
	  for (int i = 0; i < PrinterProfiles->Count; i++)
	  {
		 tgPrnProfileBreakDwn->Buttons[i][0]->Caption = PrinterProfiles->Strings[i];
		 tgPrnProfileBreakDwn->Buttons[i][0]->Tag = (int)PrinterProfiles->Objects[i];
		 tgPrnProfileBreakDwn->Buttons[i][0]->Color = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR];
		 tgPrnProfileBreakDwn->Buttons[i][0]->LatchedColor = ButtonColors[BUTTONTYPE_FULL][ATTRIB_BUTTONCOLOR];
		 tgPrnProfileBreakDwn->Buttons[i][0]->FontColor = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_FONTCOLOR];
		 tgPrnProfileBreakDwn->Buttons[i][0]->LatchedFontColor = ButtonColors[BUTTONTYPE_FULL][ATTRIB_FONTCOLOR];
		 GetPrinterProfileBreakDown(tgPrnProfileBreakDwn->Buttons[i][0]->Tag, PrinterProfileBreakDownCategories);
	  }
	  DBTransaction.Commit();

	  if (tgPrnProfileBreakDwn->RowCount > 0)
	  {
		 tgPrnProfileBreakDwnMouseClick(NULL, TMouseButton() << mbLeft, TShiftState(), tgridPrinterProfiles->Buttons[0][0]);
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}



void __fastcall TfrmPrinterMaintenance::tgridPrinterProfilesMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
   try
   {
	  CurrentPrinterProfileKey = GridButton->Tag;
	  GridButton->Latched = true;
	  UpdateCourseDisplay();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

void __fastcall TfrmPrinterMaintenance::tgPrnProfileBreakDwnMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
   try
   {
	  CurrentPrinterProfileKey = GridButton->Tag;
	  GridButton->Latched = true;
	  UpdateBreakDownDisplay();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

void TfrmPrinterMaintenance::UpdateBreakDownDisplay()
{
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

	  std::map<int,UnicodeString > CatList;
      TDBOrder::GetArchiveCategories(DBTransaction,CatList);

	  DBTransaction.Commit();

	  tgridBreakDwn->ColCount = 1;
	  tgridBreakDwn->RowCount = 0;
	  tgridBreakDwn->RowCount = CatList.size();

	  if (CatList.size() > 0)
	  {
		 int Index = 0;
		 // Add all the Selected Courses.
		 for (std::vector <int> ::iterator ptrCategory = PrinterProfileBreakDownCategories[CurrentPrinterProfileKey].begin();
			ptrCategory != PrinterProfileBreakDownCategories[CurrentPrinterProfileKey].end(); Index++, advance(ptrCategory,1))
		 {
			tgridBreakDwn->Buttons[Index][0]->Caption = CatList[*ptrCategory];
			tgridBreakDwn->Buttons[Index][0]->Tag = *ptrCategory;

			tgridBreakDwn->Buttons[Index][0]->Color = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR];
			tgridBreakDwn->Buttons[Index][0]->LatchedColor = ButtonColors[BUTTONTYPE_FULL][ATTRIB_BUTTONCOLOR];
			tgridBreakDwn->Buttons[Index][0]->FontColor = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_FONTCOLOR];
			tgridBreakDwn->Buttons[Index][0]->LatchedFontColor = ButtonColors[BUTTONTYPE_FULL][ATTRIB_FONTCOLOR];
			tgridBreakDwn->Buttons[Index][0]->Latched = true;
		 }

         for (std::map<int,UnicodeString > ::iterator ptrCatMap = CatList.begin();
         ptrCatMap != CatList.end() && (Index < tgridBreakDwn->RowCount); ptrCatMap++)
         {
			   // Add all the unslected Courses on the bottom.
           if (std::find(PrinterProfileBreakDownCategories[CurrentPrinterProfileKey].begin(),
                 PrinterProfileBreakDownCategories[CurrentPrinterProfileKey].end(), ptrCatMap->first)
              == PrinterProfileBreakDownCategories[CurrentPrinterProfileKey].end())
           { // Not Found.
              tgridBreakDwn->Buttons[Index][0]->Caption = CatList[ptrCatMap->first];
              tgridBreakDwn->Buttons[Index][0]->Tag = ptrCatMap->first;

              tgridBreakDwn->Buttons[Index][0]->Color = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR];
              tgridBreakDwn->Buttons[Index][0]->LatchedColor = ButtonColors[BUTTONTYPE_FULL][ATTRIB_BUTTONCOLOR];
              tgridBreakDwn->Buttons[Index][0]->FontColor = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_FONTCOLOR];
              tgridBreakDwn->Buttons[Index][0]->LatchedFontColor = ButtonColors[BUTTONTYPE_FULL][ATTRIB_FONTCOLOR];
              tgridBreakDwn->Buttons[Index][0]->Latched = false;
              Index++;
           }
		 }
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

void __fastcall TfrmPrinterMaintenance::tbPrinterProfilesBrkDwnMouseClick(TObject *Sender)
{
   SaveBreakDownPrinterProfiles();
   std::auto_ptr <TfrmVirtualPrintersInterface> frmVirtualPrintersInterface
	   (TfrmVirtualPrintersInterface::Create <TfrmVirtualPrintersInterface> (this, DBControl));
   frmVirtualPrintersInterface->ShowModal();
   UpdatePrinterBreakDownDisplay();
}
//---------------------------------------------------------------------------


void __fastcall TfrmPrinterMaintenance::tgridBreakDwnMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
   try
   {
	  if (CurrentPrinterProfileKey != 0)
	  {
		 std::vector <int> ::iterator ptrBreakDwnCat;
		 ptrBreakDwnCat = std::find(PrinterProfileBreakDownCategories[CurrentPrinterProfileKey].begin(),
		 PrinterProfileBreakDownCategories[CurrentPrinterProfileKey].end(), GridButton->Tag);
		 if (ptrBreakDwnCat == PrinterProfileBreakDownCategories[CurrentPrinterProfileKey].end())
		 { // Not Found.
			PrinterProfileBreakDownCategories[CurrentPrinterProfileKey].push_back(GridButton->Tag);
			GridButton->Latched = true;
		 }
		 else
		 {
			PrinterProfileBreakDownCategories[CurrentPrinterProfileKey].erase(ptrBreakDwnCat);
			GridButton->Latched = false;
		 }
		 UpdateBreakDownDisplay();
	  }
	  else
	  {
		 MessageBox("Please Select a Printer Profile.", "Error", MB_ICONERROR | MB_OK);
		 GridButton->Latched = false;
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::tbtnSelectAllCatMouseClick(TObject *Sender)
{
   if (CurrentPrinterProfileKey != 0 )
   {
	  for (int i = 0; i < tgridBreakDwn->RowCount; i++)
	  {
		 std::vector <int> ::iterator ptrBreakDwnCat;
		 ptrBreakDwnCat = std::find(PrinterProfileBreakDownCategories[CurrentPrinterProfileKey].begin(),
		 PrinterProfileBreakDownCategories[CurrentPrinterProfileKey].end(), tgridBreakDwn->Buttons[i][0]->Tag);
		 if (ptrBreakDwnCat == PrinterProfileBreakDownCategories[CurrentPrinterProfileKey].end())
		 { // Not Found.
			PrinterProfileBreakDownCategories[CurrentPrinterProfileKey].push_back(tgridBreakDwn->Buttons[i][0]->Tag);
         }
	  }
	  UpdateBreakDownDisplay();
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::tbtnClearAllCatMouseClick(TObject *Sender)
{
   if (CurrentPrinterProfileKey != 0)
   {
	  PrinterProfileBreakDownCategories[CurrentPrinterProfileKey].clear();
	  UpdateBreakDownDisplay();
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::tbtnReceiptNumberLabelMouseClick(TObject *Sender)

{
    std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
   frmTouchKeyboard->MaxLength = 0;
   frmTouchKeyboard->AllowCarriageReturn = true;
   frmTouchKeyboard->CloseOnDoubleCarriageReturn = false;
   frmTouchKeyboard->StartWithShiftDown = false;
   frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().ReceiptNumberLabel;
   frmTouchKeyboard->Caption = "Enter receipt number label";
   if (frmTouchKeyboard->ShowModal() == mrOk)
   {
	  TGlobalSettings::Instance().ReceiptNumberLabel = frmTouchKeyboard->KeyboardText;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::tbtnReprintLabelMouseClick(TObject *Sender)

{
     std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
   frmTouchKeyboard->MaxLength = 0;
   frmTouchKeyboard->AllowCarriageReturn = true;
   frmTouchKeyboard->CloseOnDoubleCarriageReturn = false;
   frmTouchKeyboard->StartWithShiftDown = false;
   frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().ReprintReceiptLabel;
   frmTouchKeyboard->Caption = "Enter reprint receipt label";
   if (frmTouchKeyboard->ShowModal() == mrOk)
   {
	  TGlobalSettings::Instance().ReprintReceiptLabel = frmTouchKeyboard->KeyboardText;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmPrinterMaintenance::cbPrintZeroTaxesClick(TObject *Sender)
{
   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();
   TGlobalSettings::Instance().PrintZeroTaxes = cbPrintZeroTaxes->Checked;
   TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmPrintZeroTaxes, TGlobalSettings::Instance().PrintZeroTaxes);
   DBTransaction.Commit();
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrinterMaintenance::cbPrintNoticeOnTransferClick(TObject *Sender)
{
   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();
   TGlobalSettings::Instance().PrintNoticeOnTransfer = cbPrintNoticeOnTransfer->Checked;
   TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmPrintNoticeOnTransfer, TGlobalSettings::Instance().PrintNoticeOnTransfer);
   DBTransaction.Commit();
}
