//---------------------------------------------------------------------------

#ifndef CourseItemPricesFormH
#define CourseItemPricesFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
#include "ICourseItemPricesForm.h"
#include <ComCtrls.hpp>
#include <Grids.hpp>
#include <ExtCtrls.hpp>
#include "NumericEdit.h"
//---------------------------------------------------------------------------
namespace MenuEditor
{
//---------------------------------------------------------------------------
class TfrmCourseItemPrices : public TForm, public ICourseItemPricesForm
{
__published:	// IDE-managed Components
	TButton *DoneButton;
	TButton *CancelButton1;
	TStringGrid *sgCourse;
	TRadioGroup *rgRounding;
	TLabel *Label1;
	TNumericEdit *nePercent;
	TButton *btnCalculate;
	TLabel *Label2;
	void __fastcall btnCalculateClick(TObject *Sender);
	void __fastcall DoneButtonClick(TObject *Sender);
private:	// User declarations

	ICourseItemPricesController* controller;
	std::vector<ItemSize> itemSizes;
	void RefreshPricesView();

public:		// User declarations
	__fastcall TfrmCourseItemPrices();
	__fastcall virtual ~TfrmCourseItemPrices() { }

	virtual void SetController(ICourseItemPricesController* controller);

	virtual bool Show();

	virtual void Set(std::vector<ItemSize>& itemSizes);
};
//---------------------------------------------------------------------------
} // namespace
//---------------------------------------------------------------------------
#endif
