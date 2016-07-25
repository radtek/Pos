//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "CourseItemPricesForm.h"
#include "MenuEdit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "NumericEdit"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
namespace MenuEditor
{
//---------------------------------------------------------------------------
__fastcall TfrmCourseItemPrices::TfrmCourseItemPrices()
	: TForm((TComponent*)NULL)
{
	sgCourse->RowCount = 2;
    sgCourse->Cells[0][0] = "Item";
 	sgCourse->Cells[1][0] = "Size";
 /* 	sgCourse->ColWidths[0] = sgCourse->ClientWidth * 2 / 5;
	sgCourse->ColWidths[1] = (sgCourse->ClientWidth - sgCourse->ColWidths[0] - sgCourse->GridLineWidth) * 2 / 5;
 	sgCourse->ColWidths[2] = (sgCourse->ClientWidth - sgCourse->ColWidths[0] - sgCourse->ColWidths[1] -
									 (sgCourse->GridLineWidth * 2)) / 2;
	sgCourse->ColWidths[3] = sgCourse->ClientWidth - sgCourse->ColWidths[0] - sgCourse->ColWidths[1] -
									 sgCourse->ColWidths[2] - (sgCourse->GridLineWidth * 4); */ 

}
//---------------------------------------------------------------------------
void TfrmCourseItemPrices::SetController(ICourseItemPricesController* controller)
{
	this->controller = controller;
	this->rgRounding->Items = this->controller->GetRoundingStrategies();

	if (this->rgRounding->Items->Count > 0)
		this->rgRounding->ItemIndex = 0;
}
//---------------------------------------------------------------------------
bool TfrmCourseItemPrices::Show()
{
	return this->ShowModal() == mrOk;
}
//---------------------------------------------------------------------------
void TfrmCourseItemPrices::Set(std::vector<ItemSize>& itemSizes)
{
	this->itemSizes = itemSizes;
	this->sgCourse->RowCount = this->itemSizes.size() + 1;

	int rowIndex = 1;
	std::vector<ItemSize>::iterator itemSize = this->itemSizes.begin();
	while (itemSize != this->itemSizes.end())
	{
		this->sgCourse->Objects[0][rowIndex] = (TObject*)&(*itemSize);

		itemSize++;
		rowIndex++;
	}

	this->RefreshPricesView();
}
//---------------------------------------------------------------------------
void __fastcall TfrmCourseItemPrices::btnCalculateClick(TObject *Sender)
{
	if (rgRounding->ItemIndex < 0)
	{
		Application->MessageBox("You must select a rounding rule first.", "Error", MB_ICONERROR + MB_OK);
		return;
	}
		
	this->controller->Calculate(this->itemSizes, nePercent->Value, rgRounding->Items->Objects[rgRounding->ItemIndex]);
	this->RefreshPricesView();
}
//---------------------------------------------------------------------------
void TfrmCourseItemPrices::RefreshPricesView()
{
 	sgCourse->ColCount = frmMenuEdit->noOfPriceLevels + 2;
	sgCourse->Cells[0][0] = "Item";
	sgCourse->Cells[1][0] = "Size";

	for (int i=1; i<this->sgCourse->RowCount; ++i)
	{
        ItemSize* itemSize = (ItemSize*)sgCourse->Objects[0][i];
        sgCourse->Cells[0][i] = itemSize->Item;
		sgCourse->Cells[1][i] = itemSize->Size;
        std::map<int,Menu::TItemSizePriceLevel>::const_iterator grpIT = itemSize->ItemSizePriceLevels.begin();
        std::map<int,Menu::TItemSizePriceLevel>::const_iterator grpEnd = itemSize->ItemSizePriceLevels.end();

         for ( ; grpIT != grpEnd; ++grpIT )
         {
            sgCourse->Cells[1+(*grpIT).second.PriceLevelKey][0] = frmMenuEdit->PriceLevelsName[(*grpIT).second.PriceLevelKey].PriceLevelName; //(*grpIT).second.PriceLevelName;
            sgCourse->Cells[1+(*grpIT).second.PriceLevelKey][i] = CurrToStrF((*grpIT).second.Price, ffFixed, CurrencyDecimals);
         }
    }


}
//---------------------------------------------------------------------------
void __fastcall TfrmCourseItemPrices::DoneButtonClick(TObject *Sender)
{
	if (Application->MessageBox("Do you want to apply your changes?", "Save Changes?", MB_YESNO + MB_ICONQUESTION) == ID_YES)
	{
		this->controller->SaveMenuPrices(this->itemSizes);
		this->ModalResult = mrOk;
	}
}
//---------------------------------------------------------------------------
} // namespace
//---------------------------------------------------------------------------






