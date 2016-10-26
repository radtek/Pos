// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#ifdef MenuMate
#include "PrinterRedirect.h"
#include "DeviceRealTerminal.h"
#endif
#ifdef PalmMate
#include "Palm.h"
#endif

#include "enum.h"
#include "Printing.h"
#include "ManagerVariable.h"
#include "TableManager.h"
#include "MMMessageBox.h"
#include "ErrorPrinter.h"
// #include "NetMessageChefMate.h"
#include "DocketManager.h"
#include "Printout.h"
#include "ReqPrintJob.h"

// #include "Comms.h"

#include <vector>
#include <algorithm>
#include <functional>

// ---------------------------------------------------------------------------
#pragma package(smart_init)

#define ReceiptItemSpacer "   "

#define QtyMultiplier1 WideString("x ")
#define QtyMultiplier10 WideString("x")
#define Spacer1 WideString(" ")
#define SeatStart WideString("(")
#define SeatEnd WideString(")")
#define SeatSpacer WideString(", ")

using namespace std;

// ---------------------------Printer Handleing Functions------------------------
extern int OptionsCompare(TListOptionContainer * Options1, TListOptionContainer * Options2);
extern int __fastcall SubOrdersCompare(TListSubOrderContainer * List1, TListSubOrderContainer * List2);
// ------------------------------------------------------------------------------
// Global pointer to our kitchen object which handles/Distrubutes all orders.
TReceipt *Receipt;

// ------------------------------------------------------------------------------
// ------------------------TReceipt information----------------------------------
// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------

int __fastcall KitchenSortFuncByTable(void *Item1, void *Item2) // TKitchens Sort Function.
{
   TKitchenCourseTable *Bundle1 = (TKitchenCourseTable*)Item1;
   TKitchenCourseTable *Bundle2 = (TKitchenCourseTable*)Item2;

   if (Bundle1->TableNo < Bundle2->TableNo)
   {
	  return -1;
   }
   else if (Bundle1->TableNo == Bundle2->TableNo)
   {
	  if (Bundle1->PrintingOrder < Bundle2->PrintingOrder)
	  {
		 return -1;
	  }
	  else if (Bundle1->PrintingOrder == Bundle2->PrintingOrder)
	  {
		 return 0;
	  }
	  else
	  {
		 return 1;
	  }
   }
   else
   {
	  return 1;
   }
}


int __fastcall KitchenSortForBundling(void *Item1, void *Item2) // TKitchens Sort Function.
{
   TItemComplete *Order1 = (TItemComplete*)Item1;
   TItemComplete *Order2 = (TItemComplete*)Item2;

   // order by connection here.

   if (Order1->MenuName < Order2->MenuName)
   {
	  return -1;
   }
   else if (Order1->MenuName == Order2->MenuName)
   {
	  if (Order1->TableNo < Order2->TableNo)
	  {
		 return -1;
	  }
	  else if (Order1->TableNo == Order2->TableNo)
	  {
		 // Do not sort by seat Number Here! all same orders for any seat bundled.
		 if (Order1->ServingCourse.ServingCourseKey < Order2->ServingCourse.ServingCourseKey)
		 {
			return -1;
		 }
		 else if (Order1->ServingCourse.ServingCourseKey == Order2->ServingCourse.ServingCourseKey)
		 // Eventually by order in menu. Printers->Courses.
		 {
			if (Order1->SetMenuGroup < Order2->SetMenuGroup)
			{
			   return -1;
			}
			else if (Order1->SetMenuGroup == Order2->SetMenuGroup)
			{
			   if (Order1->ItemAppearanceOrder < Order2->ItemAppearanceOrder)
			   {
				  return -1;
			   }
			   else if (Order1->ItemAppearanceOrder == Order2->ItemAppearanceOrder)
			   {
				  if (Order1->ItemKitchenName < Order2->ItemKitchenName)
				  {
					 return -1;
				  }
				  else if (Order1->ItemKitchenName == Order2->ItemKitchenName)
				  {
					 if (Order1->Size < Order2->Size)
					 {
						return -1;
					 }
					 else if (Order1->Size == Order2->Size)
					 {
						// Match Options
						int OptCompVal = OptionsCompare(Order1->OptionsSelected, Order2->OptionsSelected);
						if (OptCompVal != 0)
						{
						   return OptCompVal;
						}
						// Match SubOrders.
						int SubOrdersRetval = SubOrdersCompare(Order1->SubOrders, Order2->SubOrders);
						if (SubOrdersRetval != 0)
						{
						   return SubOrdersRetval;
						}
						else if (SubOrdersRetval == 0)
						{
						   // You need to sort by seat No here to stop duplicate seat no
						   // appearing in the kitchen i.e [1,2,1].
						   if (Order1->SeatNo < Order2->SeatNo)
						   {
							  return -1;
						   }
						   else if (Order1->SeatNo == Order2->SeatNo)
						   {
							  return 0;
						   }
						   else
						   {
							  return 1;
						   }
						}
					 }
					 else
					 {
						return 1;
					 }
				  }
				  else
				  {
					 return 1;
				  }
			   }
			   else
			   {
				  return 1;
			   }
			}
			else
			{
			   return 1;
			}
		 }
		 else
		 {
			return 1;
		 }
	  }
	  else
	  {
		 return 1;
	  }
   }
   else
   {
	  return 1;
   }
   return 1;
}

int __fastcall KitchenUnRaveledSortForBundling(void *Item1, void *Item2) // TKitchens Sort Function.
{
   TItemComplete *Order1 = (TItemComplete*)Item1;
   TItemComplete *Order2 = (TItemComplete*)Item2;

   // order by connection here.

   if (Order1->MenuName < Order2->MenuName)
   {
	  return -1;
   }
   else if (Order1->MenuName == Order2->MenuName)
   {
	  if (Order1->TableNo < Order2->TableNo)
	  {
		 return -1;
	  }
	  else if (Order1->TableNo == Order2->TableNo)
	  {
		 // Do not sort by seat Number Here! all same orders for any seat bundled.
		 if (Order1->CourseKitchenName < Order2->CourseKitchenName)
		 {
			return -1;
		 }
		 else if (Order1->CourseKitchenName == Order2->CourseKitchenName) // Eventually by order in menu. Printers->Courses.
		 {
			if (Order1->SetMenuGroup < Order2->SetMenuGroup)
			{
			   return -1;
			}
			else if (Order1->SetMenuGroup == Order2->SetMenuGroup)
			{
			   if (Order1->TimeStamp < Order2->TimeStamp)
			   {
				  return -1;
			   }
			   else if (Order1->TimeStamp == Order2->TimeStamp)
			   {
				  return 0;
			   }
			   else
			   {
				  return 1;
			   }
			}
			else
			{
			   return 1;
			}
		 }
		 else
		 {
			return 1;
		 }
	  }
	  else
	  {
		 return 1;
	  }
   }
   else
   {
	  return 1;
   }
   return 1;
}

__fastcall TReceipt::TReceipt()
{
   Header = new TStringList;
   Footer = new TStringList;
   PHeader = new TStringList;
   PrintingEnabled = true;
   PrintNoteWithDiscount = true;
   BulletOpt = "    ";
   NoteHeader = "Note: ";
   HideFreeItemsOnReceipt = false;
   HideZeroDollarsOnReceipt = false;
   OptionOnRecipt = false;
   SumDiscounts = false;
   AlwaysPrintReceiptTenderedSales = false;
   AlwaysPrintReceiptCashSales = false;
   RavellItemsWithPriceAdjusts = false;
   VoidFooter = new TStringList;
   SubHeader = new TStringList;
}

__fastcall TReceipt::~TReceipt()
{
   delete Header;
   Header = NULL;
   delete PHeader;
   PHeader = NULL;
   delete Footer;
   Footer = NULL;
   delete VoidFooter;
   VoidFooter = NULL;
   delete SubHeader;
   SubHeader = NULL;
}

void TReceipt::SetHeaderFooter(TStrings *inHeader ,TStrings *inPHeader,TStrings *inFooter, TStrings *inVoidFooter,TStrings * inSubHeader)
{
   Header->Text = inHeader->Text;
   PHeader->Text = inPHeader->Text;
   Footer->Text = inFooter->Text;
   //if(inVoidFooter->Count > 0)
   //{
      VoidFooter->Text = inVoidFooter->Text;
      SubHeader->Text = inSubHeader->Text;
   //}
}

// ------------------------------------------------------------------------------
void TReceipt::LoadTemplate(Database::TDBTransaction &DBTransaction, int PrinterKey, TSectionInstructStorage &inTemplate)
{
   inTemplate.clear();

   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
	   " SELECT * FROM RECEIPTPRINTING " " WHERE PHYSICALPRINTER_KEY = :PHYSICALPRINTER_KEY" " ORDER BY DISPLAY_ORDER";
   IBInternalQuery->ParamByName("PHYSICALPRINTER_KEY")->AsInteger = PrinterKey;
   IBInternalQuery->ExecQuery();

   while (!IBInternalQuery->Eof)
   {
	  TPSectionInstruction Instruction;
	  Instruction.Load(IBInternalQuery);
	  inTemplate.push_back(Instruction);
	  IBInternalQuery->Next();
   }
}

void TReceipt::SaveTemplate(Database::TDBTransaction &DBTransaction, int PrinterKey, TSectionInstructStorage &inTemplate)
{

   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text = "DELETE FROM RECEIPTPRINTING WHERE PHYSICALPRINTER_KEY = :PHYSICALPRINTER_KEY";
   IBInternalQuery->ParamByName("PHYSICALPRINTER_KEY")->AsInteger = PrinterKey;
   IBInternalQuery->ExecQuery();

   for (int i = 0; i < inTemplate.size(); i++)
   {
	  TPSectionInstruction Instruction = inTemplate[i];
	  Instruction.OrderNumber = i;

	  int key;
	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_RECEIPTPRINTING , 1) FROM RDB$DATABASE";
	  IBInternalQuery->ExecQuery();
	  key = IBInternalQuery->Fields[0]->AsInteger;

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text =
		  "INSERT INTO RECEIPTPRINTING (" "RECEIPTPRINTING_KEY, " "PHYSICALPRINTER_KEY, " "DISPLAY_ORDER, "
		  "GROUP_NUMBER, " "INSTRUCTION, " "CAPTION, " "LINE_ABOVE, " "LINE_BELOW, " "DRAW_LINE_ABOVE, "
		  "DRAW_LINE_BELOW, " "CUT, " "VISIBLE, " "PRINT_UNDERLINED, " "PRINT_BOLD, " "PRINT_DOUBLE_WIDTH, "
		  "PRINT_DOUBLE_HEIGHT, " "PRINT_COLOUR, " "PRINT_FONT) " "VALUES (" ":RECEIPTPRINTING_KEY," ":PHYSICALPRINTER_KEY, "
		  ":DISPLAY_ORDER, " ":GROUP_NUMBER, " ":INSTRUCTION, " ":CAPTION, " ":LINE_ABOVE, " ":LINE_BELOW, " ":DRAW_LINE_ABOVE, "
		  ":DRAW_LINE_BELOW, " ":CUT, " ":VISIBLE, " ":PRINT_UNDERLINED, " ":PRINT_BOLD, " ":PRINT_DOUBLE_WIDTH, " ":PRINT_DOUBLE_HEIGHT, "
		  ":PRINT_COLOUR, " ":PRINT_FONT); ";

	  IBInternalQuery->ParamByName("RECEIPTPRINTING_KEY")->AsInteger = key;
	  IBInternalQuery->ParamByName("PHYSICALPRINTER_KEY")->AsInteger = PrinterKey;
	  Instruction.Save(IBInternalQuery);
	  IBInternalQuery->ExecQuery();
   }
}

// ------------------------------------------------------------------------------

bool TReceipt::GetPrintoutsPreview(Database::TDBTransaction &DBTransaction, TReqPrintJob *Request, TPrinterPhysical Printer,
   TSectionInstructStorage *inTemplate)
{
   std::auto_ptr <TList> OrdersList(new TList);
   OrdersList->Assign(Request->Transaction->Orders, laCopy);

   Request->MembershipNameDisplay = TManagerVariable::Instance().GetInt(DBTransaction, vmMembersNameOnReceipt, 0);
   Request->ReceiptHeader->Assign(Header);
   Request->ReceiptPHeader->Assign(PHeader);
   Request->ReceiptFooter->Assign(Footer);
   Request->ReceiptVoidFooter->Assign(VoidFooter);

   while (Request->Printouts->Count != 0)
   {
	  TPrintout *Printout = (TPrintout*)Request->Printouts->Items[0];
	  delete Printout;
	  Request->Printouts->Delete(0);
   }

   TPrintout *Printout = new TPrintout;
   try
   {
	  Printout->Printer = Printer;
	  Request->Printouts->Add(Printout);

	  TPtrSectionInstructStorage TemplateSection;
	  TSectionInstructStorage Template;
	  Template = *inTemplate;

	  std::auto_ptr <TList> BundledOrders;

	  TSectionInstructStorage::iterator itInstruction = Template.begin();
	  while (itInstruction != Template.end())
	  {
		 TPSectionInstruction *CurrentInstruction = &(*itInstruction);
		 advance(itInstruction, 1);
		 GetChildren(Template, CurrentInstruction, itInstruction);
		 TemplateSection.push_back(CurrentInstruction);
	  }

	  // Process each instruction and its children.
	  TPtrSectionInstructStorage::iterator itPtrInstruction = TemplateSection.begin();
	  while (itPtrInstruction != TemplateSection.end())
	  {
		 TPSectionInstruction *itInstruction = *itPtrInstruction;
		 TPrintSection NewSection(Printout->PrintFormat, OrdersList.get(), Template, itInstruction);
		 NewSection.ProcessSection(Request);
		 advance(itPtrInstruction, 1);
	  }
   }
   catch(Exception & Err)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
	  Request->Printouts->Remove(Printout);
	  delete Printout;
	  throw;
   }
   return true;
}

void TReceipt::GetChildren(TSectionInstructStorage &Template, TPSectionInstruction *ParentNodeInstruction,
   TSectionInstructStorage::iterator &itInstruction)
{
   while (itInstruction != Template.end() && itInstruction->GroupNo > ParentNodeInstruction->GroupNo)
   {
	  // Check this node for it's own Siblings then add it to the parent.
	  TPSectionInstruction *PossiableParent = &(*itInstruction);
	  advance(itInstruction, 1);
	  if (itInstruction != Template.end())
	  {
		 if (itInstruction->GroupNo > PossiableParent->GroupNo)
		 { // New SubGroup pull in its children for the SubNode.
			GetChildren(Template, PossiableParent, itInstruction);
		 }
	  }
	  ParentNodeInstruction->SubInstructions.push_back(PossiableParent);
   }
}

void _fastcall TReceipt::GetPrintouts(Database::TDBTransaction &DBTransaction, TReqPrintJob *Request, TPrinterPhysical Printer,
   DisplayBarCodeOverride BCOverRide)
{
   if (Request->JobType == pjInit)
   {
	  Request->JobType = pjReceiptReceipt;
   }

   std::auto_ptr <TList> OrdersList(new TList);
   OrdersList->Assign(Request->Transaction->Orders, laCopy);

   Request->MembershipNameDisplay = TManagerVariable::Instance().GetInt(DBTransaction, vmMembersNameOnReceipt, 0);
   Request->ReceiptHeader->Assign(Header);
   Request->ReceiptPHeader->Assign(PHeader);
   Request->ReceiptFooter->Assign(Footer);
   Request->ReceiptVoidFooter->Assign(VoidFooter);

   while (Request->Printouts->Count != 0)
   {
	  TPrintout *Printout = (TPrintout*)Request->Printouts->Items[0];
	  delete Printout;
	  Request->Printouts->Delete(0);
   }

   TPrintout *Printout = new TPrintout;
   try
   {
	  Printout->Printer = Printer;
	  Request->Printouts->Add(Printout);

	  TSectionInstructStorage Template;
	  TPtrSectionInstructStorage TemplateSection;
	  Receipt->LoadTemplate(DBTransaction, Printer.PhysicalPrinterKey, Template);

	  TSectionInstructStorage::iterator itInstruction = Template.begin();
	  while (itInstruction != Template.end())
	  {
		 TPSectionInstruction *CurrentInstruction = &(*itInstruction);
		 advance(itInstruction, 1);
		 GetChildren(Template, CurrentInstruction, itInstruction);
		 TemplateSection.push_back(CurrentInstruction);
	  }

	  // Process each instruction and its children.
	  TPtrSectionInstructStorage::iterator itPtrInstruction = TemplateSection.begin();
	  while (itPtrInstruction != TemplateSection.end())
	  {
		 TPSectionInstruction *itInstruction = *itPtrInstruction;
		 TPrintSection NewSection(Printout->PrintFormat, OrdersList.get(), Template, itInstruction);
		 NewSection.ProcessSection(Request);
		 advance(itPtrInstruction, 1);
	  }
   }
   catch(Exception & Err)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
	  Request->Printouts->Remove(Printout);
	  delete Printout;
	  throw;
   }
}

// ------------------------------------------------------------------------------
// ------------------------TKitchen information----------------------------------

_fastcall TKitchen::TKitchen()
{
   Courses = new TList;
   OrdersList = new TList;
   PrintMenuKeyItems = false;
}

// ------------------------------------------------------------------------------
_fastcall TKitchen::~TKitchen()
{
   while (Courses->Count != 0)
   {
	  delete(TKitchenCourseTable*)Courses->Items[0];
	  Courses->Delete(0);
   }
   delete Courses;
   delete OrdersList;
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------

bool TKitchen::GetPrintouts(Database::TDBTransaction &DBTransaction, TCallAwayComplete *CallAway, TReqPrintJob *Request)
{
   try
   {
	  if (Request->JobType == pjInit)
	  {
		 Request->JobType = pjKitchen;
	  }

	  OrdersList->Clear();

      std::vector<TPrinterVirtual> VirtualPrinters;
	  TManagerVirtualPrinter::DBLoadPrinters(DBTransaction,Request->Header.RequestingDevice->ID.DeviceKey, VirtualPrinters,false);


	  SeatCount = OrdersList->Count;

	  int callAwaysCount = CallAway->Courses->Count;
	  int callAwaysPrintedCount = 0;

	  TManagerPhysicalPrinter ManagerPhysicalPrinter;

	  std::auto_ptr <TStringList> PrinterCallAwaysList(new TStringList);
      for(int i = 0; i < VirtualPrinters.size(); i++)
	  {
      	TPrinterVirtual CurrentPrinter = VirtualPrinters[i];
		 PrinterCallAwaysList->Clear();
		 // This Filter is here so that when we are doing ChefMate stuff we dont
		 // muck around formating up all the Printouts non Chefmate Printers.
		 TPrinterPhysical PhysicalPrinter = ManagerPhysicalPrinter.GetPhysicalPrinter(DBTransaction, CurrentPrinter.PhysicalPrinterKey);
		 if (PhysicalPrinter.Type == ptWindows_Printer)
		 {
			// Is the printer attached to this machine
			if (GetCallAwaysForThisPrinter(DBTransaction, &CurrentPrinter, PrinterCallAwaysList.get()))
			{
			   BundleCallAwayCoursesTables(CallAway, Request, PrinterCallAwaysList.get());
			   if (Courses->Count > 0)
			   {
				  TPrintout *Printout = GetPrintout(DBTransaction, Request, &CurrentPrinter, true);
				  AddCallAwayToPrintout(Printout->PrintFormat);
				  callAwaysPrintedCount += Courses->Count;
			   }
			}
		 }
	  }
	  return callAwaysCount == callAwaysPrintedCount;
   }
   catch(EErrorPrinter & Err) // Incase printer not found and cant be redirected.
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, "TKitchen::GetPrintouts No printers Found.");
	  return false;
   }
   catch(Exception & Err)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
	  return false;
   }
}

bool TKitchen::GetPrintouts(Database::TDBTransaction &DBTransaction,TTransferComplete *Transfer,TReqPrintJob *Request)
 {
try
   {
	  if (Request->JobType == pjInit)
	  {
		 Request->JobType = pjKitchen;
	  }
      std::vector<TPrinterVirtual> VirtualPrinters;
	  TManagerVirtualPrinter::DBLoadPrinters(DBTransaction,Request->Header.RequestingDevice->ID.DeviceKey, VirtualPrinters);


	  TManagerPhysicalPrinter ManagerPhysicalPrinter;
	  std::auto_ptr <TStringList> PrinterCallAwaysList(new TStringList);

      for(int i = 0; i < VirtualPrinters.size(); i++)
	  {
      	TPrinterVirtual CurrentPrinter = VirtualPrinters[i];
		 PrinterCallAwaysList->Clear();
		 // This Filter is here so that when we are doing ChefMate stuff we dont
		 // muck around formating up all the Printouts non Chefmate Printers.
		 TPrinterPhysical PhysicalPrinter = ManagerPhysicalPrinter.GetPhysicalPrinter(DBTransaction, CurrentPrinter.PhysicalPrinterKey);
		 if (PhysicalPrinter.Type == ptWindows_Printer)
		 {
			TPrintout *Printout = GetPrintout(DBTransaction, Request, &CurrentPrinter, true);
            AddTransferToPrintout(Printout->PrintFormat,Transfer);
		 }
	  }
	  return true;
   }
   catch(EErrorPrinter & Err) // Incase printer not found and cant be redirected.
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, "TKitchen::GetPrintouts No printers Found.");
	  return false;
   }
   catch(Exception & Err)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
	  return false;
   }
}



bool TKitchen::GetPrintouts(Database::TDBTransaction &DBTransaction, TReqPrintJob *Request, TPrinterTypeFilter PrinterTypeFilter,
   TSectionInstructStorage *inTemplate, int VirtualPrinterKeyFilter,bool isChefmate)
{
   if (Request->JobType == pjInit)
   {
	  Request->JobType = pjKitchen;
   }

   OrdersList->Clear();
   OrdersList->Assign(Request->Transaction->Orders, laCopy);

  std::vector<TPrinterVirtual> VirtualPrinters;
  TManagerVirtualPrinter::DBLoadPrinters(DBTransaction,Request->Header.RequestingDevice->ID.DeviceKey, VirtualPrinters,isChefmate);

   try
   {
	  SeatCount = OrdersList->Count;

	  // Set Menu Master Filter.
	  Format.KeyItemHeader.clear();
	  for (int i = 0; i < OrdersList->Count; )
	  {
		 TItemComplete *Order = (TItemComplete*)OrdersList->Items[i];
		 if (Order->SetMenuMask != 0 && Order->SetMenuMaster)
		 {
			/* We don't have to worry about testing if the ItemKitchenName was
			 * never set (say, it's null or zero-length) as by default it's set
			 * to the ItemName if the ItemKitchenName isn't usable. */
			if (UpperCase(Order->Size) != "DEFAULT")
			{
			   Format.KeyItemHeader[Order->SetMenuGroup] = Order->ItemKitchenName + Spacer1 + Order->Size;
			}
			else
			{
			   Format.KeyItemHeader[Order->SetMenuGroup] = Order->ItemKitchenName;
			}
			OrdersList->Remove(Order);
		 }
		 else
		 {
			i++;
		 }
	  }

	  GetSetMenuHeaders(DBTransaction, OrdersList);

	  TManagerPhysicalPrinter ManagerPhysicalPrinter;

      for(int i = 0; i < VirtualPrinters.size(); i++)
	  {
       	 TPrinterVirtual CurrentPrinter = VirtualPrinters[i];
		 if (VirtualPrinterKeyFilter == 0 || (VirtualPrinterKeyFilter == CurrentPrinter.VirtualPrinterKey))
		 {
			// This Filter is here so that when we are doing ChefMate stuff we dont
			// muck around formating up all the Printouts non Chefmate Printers.
			TPrinterPhysical PhysicalPrinter = ManagerPhysicalPrinter.GetPhysicalPrinter(DBTransaction, CurrentPrinter.PhysicalPrinterKey);
			if (PrinterTypeFilter == pfAll_Printers ||
			   (PhysicalPrinter.Type == ptWindows_Printer && PrinterTypeFilter == pfWindows_Printer) ||
			   (PhysicalPrinter.Type == ptChefMate_Printer && PrinterTypeFilter == pfChefMate_Printer))
			{
			   TSectionInstructStorage KitchenTemplate;
			   if (inTemplate == NULL)
			   {
				  LoadKitchenTemplate(DBTransaction, CurrentPrinter.VirtualPrinterKey, KitchenTemplate);
			   }
			   else
			   {
				  KitchenTemplate = *inTemplate;
			   }

			   // Is the printer attached to this machine
			   std::auto_ptr <TList> PrinterOrderList(new TList);
			   if (GetOrdersForThisPrinter(DBTransaction, &CurrentPrinter, PrinterOrderList.get()))
			   {
				  TPrintout *Printout = GetPrintout(DBTransaction, Request, &CurrentPrinter);

				  TPtrSectionInstructStorage TemplateSection;
				  TSectionInstructStorage::iterator itInstruction = KitchenTemplate.begin();
				  while (itInstruction != KitchenTemplate.end())
				  {
					 TPSectionInstruction *CurrentInstruction = &(*itInstruction);
					 advance(itInstruction, 1);
					 GetChildren(KitchenTemplate, CurrentInstruction, itInstruction);
					 TemplateSection.push_back(CurrentInstruction);
				  }

				  TPtrSectionInstructStorage::iterator itPtrInstruction = TemplateSection.begin();
				  while (itPtrInstruction != TemplateSection.end())
				  {
					 TPSectionInstruction *itInstruction = *itPtrInstruction;
					 TPrintSection NewSection(Printout->PrintFormat, PrinterOrderList.get(), KitchenTemplate, itInstruction, Format);
					 NewSection.ProcessSection(Request);
					 advance(itPtrInstruction, 1);
				  }
			   }
			}

		 }

	  }
	  return true;
   }
   catch(EErrorPrinter & Err) // Incase printer not found and cant be redirected.
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, "TKitchen::GetPrintouts No printers Found.");
	  return false;
   }
   catch(Exception & Err)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
	  return false;
   }
}

// ------------------------------------------------------------------------------

void TKitchen::GetChildren(TSectionInstructStorage &Template, TPSectionInstruction *ParentNodeInstruction,
   TSectionInstructStorage::iterator &itInstruction)
{
   while (itInstruction != Template.end() && itInstruction->GroupNo > ParentNodeInstruction->GroupNo)
   {
	  // Check this node for it's own Siblings then add it to the parent.
	  TPSectionInstruction *PossiableParent = &(*itInstruction);
	  advance(itInstruction, 1);
	  if (itInstruction != Template.end())
	  {
		 if (itInstruction->GroupNo > PossiableParent->GroupNo)
		 { // New SubGroup pull in its children for the SubNode.
			GetChildren(Template, PossiableParent, itInstruction);
		 }
	  }
	  ParentNodeInstruction->SubInstructions.push_back(PossiableParent);
   }
}

// ------------------------------------------------------------------------------

bool TKitchen::GetPrintoutsPreview(Database::TDBTransaction &DBTransaction, TReqPrintJob *Request, TPrinterVirtual *CurrentPrinter,
   TSectionInstructStorage *Template)
{

   OrdersList->Clear();
   OrdersList->Assign(Request->Transaction->Orders, laCopy);
   try
   {
	  SeatCount = OrdersList->Count;
	  // Set Menu Master Filter.
	  Format.KeyItemHeader.clear();
	  for (int i = 0; i < OrdersList->Count; )
	  {
		 TItemComplete *Order = (TItemComplete*)OrdersList->Items[i];
		 if (Order->SetMenuMask != 0 && Order->SetMenuMaster)
		 {
			if (UpperCase(Order->Size) != "DEFAULT")
			{
			   Format.KeyItemHeader[Order->SetMenuGroup] = Order->Item + Spacer1 + Order->Size;
			}
			else
			{
			   Format.KeyItemHeader[Order->SetMenuGroup] = Order->Item;
			}
			OrdersList->Remove(Order);
		 }
		 else
		 {
			i++;
		 }
	  }

	  GetSetMenuHeaders(DBTransaction, OrdersList);

	  TSectionInstructStorage KitchenTemplate;
	  KitchenTemplate = *Template;

	  TPrinterPhysical DefaultScreenPrinter;
	  DefaultScreenPrinter.NormalCharPerLine = 30;
	  DefaultScreenPrinter.BoldCharPerLine = 30;

	  std::auto_ptr <TList> PrinterOrderList(new TList);
	  if (GetOrdersForThisPrinter(DBTransaction, CurrentPrinter, PrinterOrderList.get()))
	  {
		 TPrintout *Printout = new TPrintout;
		 Printout->Printer = DefaultScreenPrinter;
		 Request->Printouts->Add(Printout);

		 TPtrSectionInstructStorage TemplateSection;

		 TSectionInstructStorage::iterator itInstruction = KitchenTemplate.begin();
		 while (itInstruction != KitchenTemplate.end())
		 {
			TPSectionInstruction *CurrentInstruction = &(*itInstruction);
			advance(itInstruction, 1);
			GetChildren(KitchenTemplate, CurrentInstruction, itInstruction);
			TemplateSection.push_back(CurrentInstruction);
		 }

		 TPtrSectionInstructStorage::iterator itPtrInstruction = TemplateSection.begin();
		 while (itPtrInstruction != TemplateSection.end())
		 {
			TPSectionInstruction *itInstruction = *itPtrInstruction;
			TPrintSection NewSection(Printout->PrintFormat, PrinterOrderList.get(), KitchenTemplate, itInstruction, Format);
			NewSection.ProcessSection(Request);
			advance(itPtrInstruction, 1);
		 }
	  }
	  return true;
   }
   catch(EErrorPrinter & Err) // Incase printer not found and cant be redirected.
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, "TKitchen::GetPrintouts No printers Found.");
	  return false;
   }
   catch(Exception & Err)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
	  return false;
   }
}
// ------------------------------------------------------------------------------

// ------------------------------------------------------------------------------
void TKitchen::BundleCallAwayCoursesTables(TCallAwayComplete *CallAway, TReqPrintJob *Request, TStringList *PrinterCallAwaysList)
{
   while (Courses->Count != 0)
   {
	  delete(TKitchenCourseTable*)Courses->Items[0];
	  Courses->Delete(0);
   }

   int PrintingOrder = 0;
   for (int j = 0; j < PrinterCallAwaysList->Count; j++)
   {
	  for (int i = 0; i < CallAway->Courses->Count; i++)
	  {
		 if (CallAway->Courses->Strings[i] == PrinterCallAwaysList->Strings[j])
		 {
			TKitchenCourseTable *TempCourse = new TKitchenCourseTable("", CallAway->TableNo, CallAway->Courses->Strings[i],
			   CallAway->Courses->Strings[i], CallAway->TableName);
			TempCourse->PrintingOrder = PrintingOrder;
			TempCourse->Waiter = Request->Waiter;
			Courses->Add(TempCourse);
		 }
	  }
	  PrintingOrder++;
   }
}

// ------------------------------------------------------------------------------
void TKitchen::AddCallAwayToPrintout(TPrintFormat *pPrinter)
{
   pPrinter->WordWrap = true;
   unsigned int LastTableNo;
   for (int i = 0; i < Courses->Count; i++)
   {
	  TKitchenCourseTable *TempCourseTable = (TKitchenCourseTable*)Courses->Items[i];

	  PrintCallAwayHeader(pPrinter, TempCourseTable);

	  pPrinter->Line->ColCount = 1;
	  pPrinter->Line->FontInfo.Bold = true;
	  pPrinter->Line->FontInfo.Width = fsDoubleSize;
	  pPrinter->Line->FontInfo.Height = fsNormalSize;
	  pPrinter->Line->Columns[0]->Width = pPrinter->Width;
	  pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
	  pPrinter->Line->Columns[0]->Text = TempCourseTable->CourseKitchenName;
	  pPrinter->AddLine();

	  pPrinter->NewLine();
	  pPrinter->Line->ColCount = 1;
	  pPrinter->Line->FontInfo.Height = fsNormalSize;
	  pPrinter->Line->FontInfo.Width = fsNormalSize;
	  pPrinter->Line->Columns[0]->Width = pPrinter->Width;
	  pPrinter->Line->Columns[0]->Alignment = taCenter;
	  pPrinter->Line->Columns[0]->Line(pPrinter->Line->Columns[0]->Width);
	  pPrinter->AddLine();

   }
   pPrinter->PartialCut();
}

void TKitchen::AddTransferToPrintout(TPrintFormat *pPrinter,TTransferComplete *Transfer)
{
   pPrinter->WordWrap = true;
   pPrinter->Line->ColCount = 1;
   pPrinter->Line->FontInfo.Bold = true;
   pPrinter->Line->FontInfo.Height = fsDoubleSize;
   pPrinter->Line->FontInfo.Width = fsDoubleSize;
   pPrinter->Line->Columns[0]->Width = pPrinter->Width;
   pPrinter->Line->Columns[0]->Alignment = taCenter;

   pPrinter->Line->Columns[0]->Text = "TRANSFER";
   pPrinter->AddLine();

   pPrinter->Line->FontInfo.Height = fsNormalSize;
   pPrinter->Line->FontInfo.Width = fsNormalSize;
   pPrinter->Line->Columns[0]->Width = pPrinter->Width;
   pPrinter->Line->Columns[0]->Line(pPrinter->Line->Columns[0]->Width);
   pPrinter->AddLine();

   UnicodeString TimeString = Now().FormatString("DD/MM hh:nn am/pm");
   pPrinter->Line->ColCount = 2;
   pPrinter->Line->Columns[0]->Width = pPrinter->Width - (TimeString.Length() + 1);
   pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
   pPrinter->Line->Columns[1]->Width = TimeString.Length() + 1;
   pPrinter->Line->Columns[1]->Alignment = taRightJustify;
   pPrinter->Line->Columns[0]->Text = Transfer->UserName;
   pPrinter->Line->Columns[1]->Text = TimeString;
   pPrinter->AddLine();

   pPrinter->Line->ColCount = 1;
   pPrinter->Line->FontInfo.Height = fsNormalSize;
   pPrinter->Line->FontInfo.Width = fsNormalSize;
   pPrinter->Line->Columns[0]->Width = pPrinter->Width;
   pPrinter->Line->Columns[0]->Line(pPrinter->Line->Columns[0]->Width);
   pPrinter->AddLine();

   pPrinter->Line->ColCount = 1;
   pPrinter->Line->Columns[0]->Width = pPrinter->Width;
   pPrinter->Line->Columns[0]->Alignment = taCenter;
   pPrinter->Line->FontInfo.Height = fsNormalSize;
   pPrinter->Line->Columns[0]->Width = pPrinter->Width;
   pPrinter->Line->Columns[0]->Text =  Transfer->TableTransferedFrom + " transferred to " + Transfer->TableTransferedTo;
   pPrinter->AddLine();

   pPrinter->Line->FontInfo.Height = fsNormalSize;
   pPrinter->Line->FontInfo.Width = fsNormalSize;
   pPrinter->Line->Columns[0]->Width = pPrinter->Width;
   pPrinter->Line->Columns[0]->Line(pPrinter->Line->Columns[0]->Width);
   pPrinter->AddLine();
   pPrinter->PartialCut();
}

// ------------------------------------------------------------------------------
void TKitchen::PrintCallAwayHeader(TPrintFormat *pPrinter, TKitchenCourseTable *CourseTable)
{
   pPrinter->Line->ColCount = 1;
   pPrinter->Line->FontInfo.Bold = true;
   pPrinter->Line->FontInfo.Height = fsDoubleSize;
   pPrinter->Line->FontInfo.Width = fsDoubleSize;
   pPrinter->Line->Columns[0]->Width = pPrinter->Width;
   pPrinter->Line->Columns[0]->Alignment = taCenter;

   pPrinter->Line->Columns[0]->Text = "CALL AWAY";
   pPrinter->AddLine();

   if (CourseTable->TableName != "")
   {
	  pPrinter->Line->Columns[0]->Text = CourseTable->TableName;
	  pPrinter->AddLine();
   }
   else
   {
	  pPrinter->Line->Columns[0]->Text = IntToStr(CourseTable->TableNo);
	  pPrinter->AddLine();
   }

   pPrinter->Line->FontInfo.Height = fsNormalSize;
   pPrinter->Line->FontInfo.Width = fsNormalSize;
   pPrinter->Line->Columns[0]->Width = pPrinter->Width;
   pPrinter->Line->Columns[0]->Line(pPrinter->Line->Columns[0]->Width);
   pPrinter->AddLine();

   UnicodeString TimeString = CourseTable->TimeStamp.FormatString("DD/MM hh:nn am/pm");
   UnicodeString CurrentSeatCount = IntToStr(SeatCount);
   pPrinter->Line->ColCount = 2;
   pPrinter->Line->Columns[0]->Width = pPrinter->Width - (TimeString.Length() + 1);
   pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
   pPrinter->Line->Columns[1]->Width = TimeString.Length() + 1;
   pPrinter->Line->Columns[1]->Alignment = taRightJustify;
   pPrinter->Line->Columns[0]->Text = CourseTable->Waiter;
   pPrinter->Line->Columns[1]->Text = TimeString;
   pPrinter->AddLine();

   pPrinter->Line->ColCount = 1;
   pPrinter->Line->Columns[0]->Width = pPrinter->Width;
   pPrinter->Line->Columns[0]->Alignment = taCenter;
   pPrinter->Line->FontInfo.Height = fsNormalSize;
   pPrinter->Line->Columns[0]->Line(pPrinter->Line->Columns[0]->Width);
   pPrinter->AddLine();
}


void TKitchen::PrintOrderHeader(TPrintFormat *pPrinter, TKitchenCourseTable *CourseTable)
{
   pPrinter->Line->ColCount = 1;
   pPrinter->Line->FontInfo.Height = fsDoubleSize;
   pPrinter->Line->FontInfo.Width = fsDoubleSize;
   pPrinter->Line->Columns[0]->Width = pPrinter->Width;
   pPrinter->Line->Columns[0]->Alignment = taCenter;

   if (CourseTable->ChitNumber.Assigned())
   {
	  pPrinter->Line->Columns[0]->Text = CourseTable->ChitNumber.GetChitNumber();
	  pPrinter->AddLine();
   }
   if (CourseTable->TableName != "CASH" || !CourseTable->ChitNumber.Assigned())
   {
	  pPrinter->Line->Columns[0]->Text = CourseTable->TableName;
	  pPrinter->AddLine();
   }
   pPrinter->Line->FontInfo.Height = fsNormalSize;
   pPrinter->Line->FontInfo.Width = fsNormalSize;
   pPrinter->Line->Columns[0]->Width = pPrinter->Width;
   pPrinter->Line->Columns[0]->Line(pPrinter->Line->Columns[0]->Width);
   pPrinter->AddLine();

   pPrinter->Line->ColCount = 1;
   pPrinter->Line->Columns[0]->Width = pPrinter->Width;
   pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
   pPrinter->Line->Columns[0]->Text = CourseTable->Waiter;
   pPrinter->AddLine();

   UnicodeString TimeString = CourseTable->TimeStamp.FormatString("DD/MM hh:nn am/pm");
   UnicodeString CurrentSeatCount = IntToStr(SeatCount);
   pPrinter->Line->ColCount = 2;
   pPrinter->Line->Columns[0]->Width = pPrinter->Width - (TimeString.Length() + 1);
   pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
   pPrinter->Line->Columns[1]->Width = TimeString.Length() + 1;
   pPrinter->Line->Columns[1]->Alignment = taRightJustify;
   pPrinter->Line->Columns[0]->Text = TGlobalSettings::Instance().SeatLabel + " Count :" + CurrentSeatCount;
   pPrinter->Line->Columns[1]->Text = TimeString;
   pPrinter->AddLine();

   pPrinter->Line->ColCount = 1;
   pPrinter->Line->Columns[0]->Width = pPrinter->Width;
   pPrinter->Line->Columns[0]->Alignment = taCenter;
   pPrinter->Line->FontInfo.Height = fsNormalSize;
   pPrinter->Line->Columns[0]->Line(pPrinter->Line->Columns[0]->Width);
   pPrinter->AddLine();
}
// ------------------------------------------------------------------------------

// ------------------------------------------------------------------------------
// This function will try to find a printout for the given printer. If not found,
// it will create one and add it to the list contained in the request. A pointer
// to the printout is returned so it can be added to.
// ------------------------------------------------------------------------------
TPrintout* TKitchen::GetPrintout(Database::TDBTransaction &DBTransaction, TReqPrintJob *Request, TPrinterVirtual *VirtualPrinter,
   bool CallAway)
{
   TPrintout *Printout;
   // Retrieve the Print Job for this Courses Printer.
   for (int i = 0; i < Request->Printouts->Count; i++)
   {
	  Printout = (TPrintout*)Request->Printouts->Items[i];
	  if (Printout->Printer.PhysicalPrinterKey == VirtualPrinter->PhysicalPrinterKey)
	  {
		 return Printout;
	  }
   }

   TManagerPhysicalPrinter ManagerPhysicalPrinter;
   TPrinterPhysical PhysicalPrinter = ManagerPhysicalPrinter.GetPhysicalPrinter(DBTransaction, VirtualPrinter->PhysicalPrinterKey);
   // Unable to find a print Job for this Printer. Create one...
   Printout = new TPrintout;
   Printout->Printer = PhysicalPrinter;
   Request->Printouts->Add(Printout);

   if (Request->Transaction && Request->Transaction->ChitNumber.Assigned())
   {
	  Printout->PrintInfo["ChitLabel"] = Request->Transaction->ChitNumber.GetChitNumber();
	  Printout->PrintInfo["ChitNumber"] = Request->Transaction->ChitNumber.ChitNumber;
   }

   if (PhysicalPrinter.Type != ptChefMate_Printer && PhysicalPrinter.Type != ptScreen_Printer && CallAway == false)
   {
	  Request->DocketNumber = IntToStr(ManagerDockets->GetNextDocketNumber());
	  Printout->PrintInfo["DocketNumber"] = Request->DocketNumber;
   }

   if (Request->BarCodeData != 0)
   {
	  const int MAX_BC_LENGTH = 8;
	  Printout->PrintInfo["Barcode"] = UnicodeString("MMK" + IntToHex(__int64(Request->BarCodeData), MAX_BC_LENGTH));
	  Printout->PrintInfo["CurrentTimeKey"] = IntToStr(Request->BarCodeData);
   }

   return Printout;
}

TKitchenCourseTable::TKitchenCourseTable(UnicodeString inMenuFK, int inTable, WideString inCourse, WideString inCourseKitchenName,
   UnicodeString inTableName)
{
   PrintHeader = true;
   TableNo = inTable;
   TableName = inTableName;
   Course = inCourse;
   CourseKitchenName = inCourseKitchenName;
   Menu = inMenuFK;
   ThisCourseWasPrinted = false;
   TimeStamp = Now();
   PrintingOrder = 0;
}

TKitchenCourseTable::~TKitchenCourseTable()
{
}

TCallAwayComplete::TCallAwayComplete()
{
   Courses = new TStringList;
   PartyName = "";
   TableName = "";
   TableNo = 0;
}

__fastcall TCallAwayComplete::~TCallAwayComplete()
{
   delete Courses;
}

void TCallAwayComplete::CopyFrom( const TCallAwayComplete* source )
{
    PartyName = source->PartyName;
    TableName = source->TableName;
    TableNo   = source->TableNo;

    Courses->Clear();
    Courses->AddStrings(source->Courses);
}

void TKitchen::Initialise(Database::TDBTransaction &DBTransaction)
{
   Format.Load(DBTransaction);
}

void TReceipt::Initialise(Database::TDBTransaction &DBTransaction)
{
   DisplayBarCodes = TManagerVariable::Instance().GetBool(DBTransaction, vmDisplayBarCodesOnReceipt, false);
   AlwaysPrintReceiptTenderedSales = TManagerVariable::Instance().GetBool(DBTransaction, vmAlwaysPrintReceiptTenderedSales, false);
   AlwaysPrintReceiptCashSales = TManagerVariable::Instance().GetBool(DBTransaction, vmAlwaysPrintReceiptCashSales, false);
   BulletOpt = TManagerVariable::Instance().GetStr(DBTransaction, vmKitchenBulletOpt, "    ");
   NoteHeader = TManagerVariable::Instance().GetStr(DBTransaction, vmKitchenHeaderNote, "Note: ");
   MembershipNameDisplay = TManagerVariable::Instance().GetInt(DBTransaction, vmMembersNameOnReceipt, 0);
}

void TKitchen::GetSetMenuHeaders(Database::TDBTransaction &DBTransaction, TList *OrdersList)
{
   for (int n = 0; n < OrdersList->Count; n++)
   {
	  TItemComplete *Order = (TItemComplete*)OrdersList->Items[n];
	  // Check to make sure their is a master.
	  bool UnableToFindMaster = true;
	  if (Order->SetMenuMask != 0 && !Order->SetMenuMaster && Order->SetMenuGroup != 0)
	  {
		 if (Format.KeyItemHeader[Order->SetMenuGroup] == UnicodeString(""))
		 {
			// Look up the master in the DB.
			TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = "SELECT " "ITEM_NAME,ITEM_KITCHEN_NAME, SIZE_NAME, SETMENU_MASK " "FROM " "ORDERS " "WHERE "
				"SETMENU_GROUP = :SET_MENU_GROUP";
			IBInternalQuery->ParamByName("SET_MENU_GROUP")->AsInteger = Order->SetMenuGroup;
			IBInternalQuery->ExecQuery();

			while (!IBInternalQuery->Eof && UnableToFindMaster)
			{
			   if (IBInternalQuery->FieldByName("SETMENU_MASK")->AsInteger & 0x80000000)
			   {
				  if (UpperCase(IBInternalQuery->FieldByName("SIZE_NAME")->AsString) != "DEFAULT")
				  {
					 if (IBInternalQuery->FieldByName("ITEM_KITCHEN_NAME")->AsString == "")
					 {
						Format.KeyItemHeader[Order->SetMenuGroup] = IBInternalQuery->FieldByName("ITEM_NAME")->AsString + " " + IBInternalQuery->FieldByName("SIZE_NAME")->AsString;
					 }
					 else
					 {
						Format.KeyItemHeader[Order->SetMenuGroup] = UTF8ToUnicodeString((AnsiString)IBInternalQuery->FieldByName("ITEM_KITCHEN_NAME")->AsString) + " " + IBInternalQuery->FieldByName
							("SIZE_NAME")->AsString;
					 }
					 UnableToFindMaster = false;
				  }
				  else
				  {
					 if (IBInternalQuery->FieldByName("ITEM_KITCHEN_NAME")->AsString == "")
					 {
						Format.KeyItemHeader[Order->SetMenuGroup] = IBInternalQuery->FieldByName("ITEM_NAME")->AsString;
					 }
					 else
					 {
						Format.KeyItemHeader[Order->SetMenuGroup] = UTF8ToUnicodeString
							((AnsiString)IBInternalQuery->FieldByName("ITEM_KITCHEN_NAME")->AsString);
					 }
					 UnableToFindMaster = false;
				  }
			   }
			   IBInternalQuery->Next();
			}
		 }
		 else
		 {
			UnableToFindMaster = false;
		 }
	  }
	  else
	  {
		 UnableToFindMaster = false;
	  }

	  if (UnableToFindMaster)
	  {
		 TManagerLogs::Instance().Add(__FUNC__, ERRORLOG,
			"TKitchen::Unable to locate master Item for. " + Order->Item + " " + Order->Size + " " + IntToStr
			((__int64)Order->SetMenuGroup) + " " + IntToStr((__int64)Order->SetMenuMask) + " " + IntToStr((__int64)Order->SetMenuMaster));
	  }
   }
}

bool TKitchen::GetCallAwaysForThisPrinter(Database::TDBTransaction &DBTransaction, TPrinterVirtual *CurrentPrinter,
   TStringList * PrinterOrderList)
{
   bool OrdersAdded = false;

   Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
   DBTransaction.RegisterQuery(IBInternalQuery);

   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text = "Select "
	   " PRNSERVINGCOURSEORDER.VIRTUALPRINTER_KEY,PRNSERVINGCOURSEORDER.SERVINGCOURSES_KEY,cast(SERVINGCOURSES.SERVINGCOURSE_KITCHEN_NAME as varchar(200)) SERVINGCOURSE_NAME " " From"
	   " PRNSERVINGCOURSEORDER" " LEFT JOIN SERVINGCOURSES ON PRNSERVINGCOURSEORDER.SERVINGCOURSES_KEY = SERVINGCOURSES.SERVINGCOURSES_KEY "
	   " Where" " PRNSERVINGCOURSEORDER.VIRTUALPRINTER_KEY = :VIRTUALPRINTER_KEY and "
	   " SERVINGCOURSES.SERVINGCOURSE_KITCHEN_NAME IS NOT NULL and" " SERVINGCOURSES.SERVINGCOURSE_KITCHEN_NAME != ''" " union"

	   " Select "
	   " PRNSERVINGCOURSEORDER.VIRTUALPRINTER_KEY,PRNSERVINGCOURSEORDER.SERVINGCOURSES_KEY,cast(SERVINGCOURSES.SERVINGCOURSE_NAME as varchar(200)) SERVINGCOURSE_NAME " " From"
	   " PRNSERVINGCOURSEORDER" " LEFT JOIN SERVINGCOURSES ON PRNSERVINGCOURSEORDER.SERVINGCOURSES_KEY = SERVINGCOURSES.SERVINGCOURSES_KEY "
	   " Where" " PRNSERVINGCOURSEORDER.VIRTUALPRINTER_KEY = :VIRTUALPRINTER_KEY and "
	   " (SERVINGCOURSES.SERVINGCOURSE_KITCHEN_NAME IS NULL OR" " SERVINGCOURSES.SERVINGCOURSE_KITCHEN_NAME = '')";

   IBInternalQuery->ParamByName("VIRTUALPRINTER_KEY")->AsInteger = CurrentPrinter->VirtualPrinterKey;
   IBInternalQuery->ExecQuery();

   while (!IBInternalQuery->Eof)
   {
	  PrinterOrderList->Add(IBInternalQuery->FieldByName("SERVINGCOURSE_NAME")->AsString);
	  OrdersAdded = true;
	  IBInternalQuery->Next();
   }
   return OrdersAdded;
}

bool TKitchen::GetOrdersForThisPrinter(Database::TDBTransaction &DBTransaction, TPrinterVirtual *CurrentPrinter, TList * PrinterOrderList)
{
   bool OrdersAdded = false;

   Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
   DBTransaction.RegisterQuery(IBInternalQuery);

   // PRINT BY COURSE

   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
	   " SELECT * FROM PRNORDER WHERE PRNORDER.VIRTUALPRINTER_KEY = :VIRTUALPRINTER_KEY " " ORDER BY PRNORDER.PRNORD_KEY";
   IBInternalQuery->ParamByName("VIRTUALPRINTER_KEY")->AsInteger = CurrentPrinter->VirtualPrinterKey;
   IBInternalQuery->ExecQuery();

      //       MessageBox(CurrentPrinter->VirtualPrinterKey, "Error",MB_ICONWARNING + MB_OK);


   while (!IBInternalQuery->Eof)
   {

	  for (int i = 0; i < OrdersList->Count; i++)
	  {
		 TItemComplete *CurrentOrder = (TItemComplete*)(OrdersList->Items[i]);
		 // TODO : BUG? CurrentOrder->CourseKitchenName == IBInternalQuery->FieldByName("COURSE_NAME")->AsString

		 if (CurrentOrder->CourseKitchenName == IBInternalQuery->FieldByName("COURSE_NAME")->AsString
			&& CurrentOrder->MenuKey == IBInternalQuery->FieldByName("MENU_KEY")->AsInteger && CurrentOrder->DuplicateDetected == false)
		 {
			CurrentOrder->CoursePrintingOrder = IBInternalQuery->FieldByName("PRNORD_KEY")->AsInteger;
            // Dont Print Any item more than once.
            if(PrinterOrderList->IndexOf(CurrentOrder) == -1)
            {
				PrinterOrderList->Add(CurrentOrder);
            }
			OrdersAdded = true;


		 }
     }
	  IBInternalQuery->Next();
   }

   // PRINT BY CATEGORY
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
	   " SELECT * FROM PRNCAT WHERE PRNCAT.VIRTUALPRINTER_KEY = :VIRTUALPRINTER_KEY " " ORDER BY PRNCAT.PRNCAT_KEY";
   IBInternalQuery->ParamByName("VIRTUALPRINTER_KEY")->AsInteger = CurrentPrinter->VirtualPrinterKey;
   IBInternalQuery->ExecQuery();

   while (!IBInternalQuery->Eof)
   {
	  for (int i = 0; i < OrdersList->Count; i++)
	  {
		 TItemComplete *CurrentOrder = (TItemComplete*)(OrdersList->Items[i]);
		 if (CurrentOrder->Categories->CategoryGetByKey(IBInternalQuery->FieldByName("CATEGORY_KEY")->AsInteger) != NULL)
		 {
			CurrentOrder->CoursePrintingOrder = IBInternalQuery->FieldByName("PRNCAT_KEY")->AsInteger;
            //
            // Dont Print Any item more than once.
            if(PrinterOrderList->IndexOf(CurrentOrder) == -1)
            {
				PrinterOrderList->Add(CurrentOrder);
            }
			OrdersAdded = true;
		 }

      }
	  IBInternalQuery->Next();
   }

   return OrdersAdded;
}

void TKitchen::LoadKitchenTemplate(Database::TDBTransaction &DBTransaction, int PrinterProfileKey,
   TSectionInstructStorage &inKitchenTemplate)
{
   inKitchenTemplate.clear();

   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
	   " SELECT * FROM KITCHENPRINTING "
	   " WHERE VIRTUALPRINTER_KEY = :VIRTUALPRINTER_KEY" " ORDER BY DISPLAY_ORDER";
   IBInternalQuery->ParamByName("VIRTUALPRINTER_KEY")->AsInteger = PrinterProfileKey;
   IBInternalQuery->ExecQuery();

   while (!IBInternalQuery->Eof)
   {
	  TPSectionInstruction Instruction;
	  Instruction.Load(IBInternalQuery);
	  inKitchenTemplate.push_back(Instruction);
	  IBInternalQuery->Next();
   }
}

void TKitchen::SaveKitchenTemplate(Database::TDBTransaction &DBTransaction, int PrinterProfileKey,
   TSectionInstructStorage &inKitchenTemplate)
{

   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text = "DELETE FROM KITCHENPRINTING WHERE VIRTUALPRINTER_KEY = :VIRTUALPRINTER_KEY";
   IBInternalQuery->ParamByName("VIRTUALPRINTER_KEY")->AsInteger = PrinterProfileKey;
   IBInternalQuery->ExecQuery();

   for (int i = 0; i < inKitchenTemplate.size(); i++)
   {
	  TPSectionInstruction Instruction = inKitchenTemplate[i];
	  Instruction.OrderNumber = i;

	  int key;
	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_KITCHENPRINTING, 1) FROM RDB$DATABASE";
	  IBInternalQuery->ExecQuery();
	  key = IBInternalQuery->Fields[0]->AsInteger;

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text =
		  "INSERT INTO KITCHENPRINTING (" "KITCHENPRINTING_KEY, " "VIRTUALPRINTER_KEY, " "DISPLAY_ORDER, "
		  "GROUP_NUMBER, " "INSTRUCTION, " "CAPTION, " "LINE_ABOVE, " "LINE_BELOW, " "DRAW_LINE_ABOVE, "
		  "DRAW_LINE_BELOW, " "CUT, " "VISIBLE, " "PRINT_UNDERLINED, " "PRINT_BOLD, " "PRINT_DOUBLE_WIDTH, "
		  "PRINT_DOUBLE_HEIGHT, " "PRINT_COLOUR, " "PRINT_FONT) " "VALUES (" ":KITCHENPRINTING_KEY," ":VIRTUALPRINTER_KEY, "
		  ":DISPLAY_ORDER, " ":GROUP_NUMBER, " ":INSTRUCTION, " ":CAPTION, " ":LINE_ABOVE, " ":LINE_BELOW, " ":DRAW_LINE_ABOVE, "
		  ":DRAW_LINE_BELOW, " ":CUT, " ":VISIBLE, " ":PRINT_UNDERLINED, " ":PRINT_BOLD, " ":PRINT_DOUBLE_WIDTH, " ":PRINT_DOUBLE_HEIGHT, "
		  ":PRINT_COLOUR, " ":PRINT_FONT); ";

	  IBInternalQuery->ParamByName("KITCHENPRINTING_KEY")->AsInteger = key;
	  IBInternalQuery->ParamByName("VIRTUALPRINTER_KEY")->AsInteger = PrinterProfileKey;
	  Instruction.Save(IBInternalQuery);
	  IBInternalQuery->ExecQuery();
   }
}
