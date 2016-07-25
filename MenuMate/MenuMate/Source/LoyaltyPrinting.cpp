//---------------------------------------------------------------------------


#pragma hdrstop

#include "LoyaltyPrinting.h"

TLoyaltyPrinting* TLoyaltyPrinting::instance = NULL;

TLoyaltyPrinting::TLoyaltyPrinting()
{
    MemberNumber = "";
    MemberName = "";
}

// ------------------------------------------------------------------------------
TLoyaltyPrinting::~TLoyaltyPrinting()
{
}

// ------------------------------------------------------------------------------

void TLoyaltyPrinting::SetHeaderFooter(TStrings *inHeader, TStrings *inFooter)
{
}

// ------------------------------------------------------------------------------

bool TLoyaltyPrinting::GetPrintouts(Database::TDBTransaction &DBTransaction,TReqPrintJob *Request, TPrinterPhysical Printer)
{

   std::auto_ptr <TList> OrdersList(new TList);

   if (Request->JobType == pjInit)
   {
	  Request->JobType = pjLoyaltyActivationReceipt;
   }

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
	  LoadLoyaltyPrintingTemplate(DBTransaction, Printer.PhysicalPrinterKey, Template);

	  TSectionInstructStorage::iterator itInstruction = Template.begin();
	  while (itInstruction != Template.end())
	  {
		 TPSectionInstruction *CurrentInstruction = &(*itInstruction);
		 advance(itInstruction, 1);
		 //GetChildren(Template, CurrentInstruction, itInstruction);
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

void TLoyaltyPrinting::LoadLoyaltyPrintingTemplate(Database::TDBTransaction &DBTransaction,int PrinterKey,TSectionInstructStorage &inTemplate)
{
   inTemplate.clear();

/* commented out following until we have the template edit section ready in the maintainence area. For now we will be using a mock structure to load the template */

//   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
//
//   IBInternalQuery->Close();
//   IBInternalQuery->SQL->Text =
//	   " SELECT * FROM LOYALTYPRINTING "
//	   " WHERE PHYSICALPRINTER_KEY = :PHYSICALPRINTER_KEY" " ORDER BY DISPLAY_ORDER";
//   IBInternalQuery->ParamByName("PHYSICALPRINTER_KEY")->AsInteger = PrinterKey;
//   IBInternalQuery->ExecQuery();
//
//   while (!IBInternalQuery->Eof)
//   {
//	  TPSectionInstruction Instruction;
//	  Instruction.Load(IBInternalQuery);
//	  inTemplate.push_back(Instruction);
//	  IBInternalQuery->Next();
//   }

// TODO: Remove this mock structure

    TPSectionInstruction Instruction;
    Instruction = buildMockInstruction(0,epofiPrintLoyaltyReceiptHeader,"Loyalty ACT Receipt Header",false,true,true,false);
    inTemplate.push_back(Instruction);
    Instruction = buildMockInstruction(0,epofiPrintLoyaltyReceiptMemberNumber,"Loyalty ACT Receipt Member Number",false,false,true,false);
    inTemplate.push_back(Instruction);
    Instruction = buildMockInstruction(0,epofiPrintLoyaltyReceiptMemberName,"Loyalty ACT Receipt Member Name",false,false,true,false);
    inTemplate.push_back(Instruction);
    Instruction = buildMockInstruction(0,epofiPrintLoyaltyReceiptActivationCode,"Loyalty ACT Receipt Activation Code",false,false,true,false);
    inTemplate.push_back(Instruction);
    Instruction = buildMockInstruction(0,epofiPrintLoyaltyReceiptActivationInstructions,"Loyalty ACT Instructions",true,false,true,true);
    inTemplate.push_back(Instruction);
}

void TLoyaltyPrinting::SaveLoyaltyPrintingTemplate(Database::TDBTransaction &DBTransaction,int PrinterKey,TSectionInstructStorage &inTemplate)
{
/* commented out for now, as we need a seperate maintainance section to edit the template which is not decided yet */

//   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
//
//   IBInternalQuery->Close();
//   IBInternalQuery->SQL->Text = "DELETE FROM LOYALTYPRINTING WHERE PHYSICALPRINTER_KEY = :PHYSICALPRINTER_KEY";
//   IBInternalQuery->ParamByName("PHYSICALPRINTER_KEY")->AsInteger = PrinterKey;
//   IBInternalQuery->ExecQuery();
//
//   for (int i = 0; i < inTemplate.size(); i++)
//   {
//	  TPSectionInstruction Instruction = inTemplate[i];
//	  Instruction.OrderNumber = i;
//
//	  int key;
//	  IBInternalQuery->Close();
//	  IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_LOYALTYPRINTING, 1) FROM RDB$DATABASE";
//	  IBInternalQuery->ExecQuery();
//	  key = IBInternalQuery->Fields[0]->AsInteger;
//
//	  IBInternalQuery->Close();
//	  IBInternalQuery->SQL->Text =
//		  "INSERT INTO LOYALTYPRINTING (" "LOYALTYPRINTING_KEY, " "PHYSICALPRINTER_KEY, " "DISPLAY_ORDER, "
//		  "GROUP_NUMBER, " "INSTRUCTION, " "CAPTION, " "LINE_ABOVE, " "LINE_BELOW, " "DRAW_LINE_ABOVE, "
//		  "DRAW_LINE_BELOW, " "CUT, " "VISIBLE, " "PRINT_UNDERLINED, " "PRINT_BOLD, " "PRINT_DOUBLE_WIDTH, "
//		  "PRINT_DOUBLE_HEIGHT, " "PRINT_COLOUR, " "PRINT_FONT) " "VALUES (" ":LOYALTYPRINTING_KEY," ":PHYSICALPRINTER_KEY, "
//		  ":DISPLAY_ORDER, " ":GROUP_NUMBER, " ":INSTRUCTION, " ":CAPTION, " ":LINE_ABOVE, " ":LINE_BELOW, " ":DRAW_LINE_ABOVE, "
//		  ":DRAW_LINE_BELOW, " ":CUT, " ":VISIBLE, " ":PRINT_UNDERLINED, " ":PRINT_BOLD, " ":PRINT_DOUBLE_WIDTH, " ":PRINT_DOUBLE_HEIGHT, "
//		  ":PRINT_COLOUR, " ":PRINT_FONT); ";
//
//	  IBInternalQuery->ParamByName("LOYALTYPRINTING_KEY")->AsInteger = key;
//	  IBInternalQuery->ParamByName("PHYSICALPRINTER_KEY")->AsInteger = PrinterKey;
//	  Instruction.Save(IBInternalQuery);
//	  IBInternalQuery->ExecQuery();
//   }
}

TPSectionInstruction TLoyaltyPrinting::buildMockInstruction(
                                                        int order,
                                                        ePrintOutFormatInstructions instruction,
                                                        UnicodeString caption,
                                                        bool lineAbove,
                                                        bool lineBelow,
                                                        bool visible,
                                                        bool cut
                                                            )
{
    TPSectionInstruction Instruction;

    Instruction.OrderNumber = order;
	Instruction.GroupNo = 0;
	Instruction.Instruction = instruction;
	Instruction.Caption = caption;
	Instruction.LineAbove = lineAbove;
	Instruction.LineBelow = lineBelow;
	Instruction.Cut = cut;
	Instruction.Visible = visible;
	Instruction.DrawLineAbove = lineAbove;
	Instruction.DrawLineBelow = lineBelow;
	Instruction.Visible = visible;

	Instruction.FontInfo.Underlined = false;
	Instruction.FontInfo.Bold = false;
	Instruction.FontInfo.Width = fsNormalSize;
	Instruction.FontInfo.Height = fsNormalSize;
	Instruction.FontInfo.Colour = fcBlack;
	Instruction.FontInfo.Font = ftFontA;

    return Instruction;
}
#pragma package(smart_init)
