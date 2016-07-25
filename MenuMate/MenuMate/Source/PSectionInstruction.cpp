//---------------------------------------------------------------------------


#pragma hdrstop

#include "PSectionInstruction.h"
#include "PrintingSections.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

void TPSectionInstruction::Load(TIBSQL *Query)
{
	OrderNumber = Query->FieldByName("DISPLAY_ORDER")->AsInteger;
	GroupNo = Query->FieldByName("GROUP_NUMBER")->AsInteger;
	Instruction = (ePrintOutFormatInstructions)Query->FieldByName("INSTRUCTION")->AsInteger;
	Caption = Query->FieldByName("CAPTION")->AsString;
	LineAbove = Query->FieldByName("LINE_ABOVE")->AsString.UpperCase() == "F" ? false : true;
	LineBelow = Query->FieldByName("LINE_BELOW")->AsString.UpperCase() == "F" ? false : true;
	Cut = Query->FieldByName("CUT")->AsString.UpperCase() == "F" ? false : true;
	Visible = Query->FieldByName("VISIBLE")->AsString.UpperCase() == "F" ? false : true;
	DrawLineAbove = Query->FieldByName("DRAW_LINE_ABOVE")->AsString.UpperCase() == "F" ? false : true;
	DrawLineBelow = Query->FieldByName("DRAW_LINE_BELOW")->AsString.UpperCase() == "F" ? false : true;
	Visible = Query->FieldByName("VISIBLE")->AsString.UpperCase() == "F" ? false : true;

	FontInfo.Underlined = Query->FieldByName("PRINT_UNDERLINED")->AsString.UpperCase() == "F" ? false : true;
	FontInfo.Bold = Query->FieldByName("PRINT_BOLD")->AsString.UpperCase() == "F" ? false : true;
	FontInfo.Width = Query->FieldByName("PRINT_DOUBLE_WIDTH")->AsString.UpperCase() == "F" ? fsNormalSize : fsDoubleSize;
	FontInfo.Height = Query->FieldByName("PRINT_DOUBLE_HEIGHT")->AsString.UpperCase() == "F" ? fsNormalSize : fsDoubleSize;
	FontInfo.Colour = Query->FieldByName("PRINT_COLOUR")->AsInteger == 0 ? fcBlack : fcRed;
	FontInfo.Font = Query->FieldByName("PRINT_FONT")->AsInteger == 0 ? ftFontA : ftFontB;
}

void TPSectionInstruction::Save(TIBSQL *Query)
{
	Query->ParamByName("DISPLAY_ORDER")->AsInteger = OrderNumber;
	Query->ParamByName("GROUP_NUMBER")->AsInteger = GroupNo;
	Query->ParamByName("INSTRUCTION")->AsInteger = Instruction;
	Query->ParamByName("CAPTION")->AsString = Caption;
	Query->ParamByName("LINE_ABOVE")->AsString = LineAbove == true ? "T" : "F";
	Query->ParamByName("LINE_BELOW")->AsString = LineBelow == true ? "T" : "F";
	Query->ParamByName("DRAW_LINE_ABOVE")->AsString = DrawLineAbove == true ? "T" : "F";
	Query->ParamByName("DRAW_LINE_BELOW")->AsString = DrawLineBelow == true ? "T" : "F";

	Query->ParamByName("CUT")->AsString = Cut == true ? "T" : "F";
	Query->ParamByName("VISIBLE")->AsString = Visible == true ? "T" : "F";

	Query->ParamByName("PRINT_UNDERLINED")->AsString = FontInfo.Underlined == true ? "T" : "F";
	Query->ParamByName("PRINT_BOLD")->AsString = FontInfo.Bold == true ? "T" : "F";
	Query->ParamByName("PRINT_DOUBLE_WIDTH")->AsString = FontInfo.Width == fsDoubleSize ? "T" : "F";
	Query->ParamByName("PRINT_DOUBLE_HEIGHT")->AsString = FontInfo.Height == fsDoubleSize ? "T" : "F";
	Query->ParamByName("PRINT_COLOUR")->AsInteger = FontInfo.Colour == fcBlack ? 0 : 1;
	Query->ParamByName("PRINT_FONT")->AsInteger = FontInfo.Font == ftFontA ? 0 : 1;
}

TPSectionInstruction::TPSectionInstruction()
{
	Instruction = epofiPrintMsg;
	ProfileKey = 0;
	InternalProfileName = "";
	OrderNumber = 0;
	GroupNo = 0;
	TPrintOutFormatInstructions Instructions;
	Caption = Instructions.GetCaption(Instruction);
	InstructionCaption = Instructions.GetInstructionCaption(Instruction);
	LineAbove = false;
	LineBelow = false;
	DrawLineAbove = false;
	DrawLineBelow = false;
	Visible = true;
	Cut = false;
	FontInfo.Reset();
}

TPSectionInstruction::TPSectionInstruction(ePrintOutFormatInstructions inInstruction)
{
	Instruction = inInstruction;
	TPrintOutFormatInstructions Instructions;
	Caption = Instructions.GetCaption(Instruction);
	InstructionCaption = Instructions.GetInstructionCaption(Instruction);
	ProfileKey = 0;
	InternalProfileName = "";
	OrderNumber = 0;
	GroupNo = 0;
	LineAbove = false;
	LineBelow = false;
	DrawLineAbove = false;
	DrawLineBelow = false;
	Visible = true;
	Cut = false;
	FontInfo.Reset();
}
