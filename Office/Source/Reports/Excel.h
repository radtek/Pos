//---------------------------------------------------------------------------

#ifndef ExcelH
#define ExcelH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Connections.h"
#include "OpShared.hpp"
#include "OpXLXP.hpp"
#include "OpDbOfc.hpp"
#include "OpModels.hpp"
#include "DB.hpp"
#include <Buttons.hpp>
#include "OpExcel.hpp"
#include "OpOlkXP.hpp"
#include "OpOutlk.hpp"
//---------------------------------------------------------------------------
class TfrmExcel : public TForm
{
__published:	// IDE-managed Components
	TOpExcel *OpExcel;
	TBitBtn *btnCloseExcel;
	TLabel *Label1;
	TOpOutlook *Outlook;
	void __fastcall OpExcelWindowDeactivate(TObject *Sender,
          const _Workbook *Wb, const ExcelWindow *Wn);
	void __fastcall btnCloseExcelClick(TObject *Sender);
private:	// User declarations
	void AddWorkSheet( TDataSet *DataSet, AnsiString WorkSheetName);
	void AddChart( TOpExcelWorkbook *oWorkBook ,TDataSet *DataSet, AnsiString ACaption);
public:		// User declarations
	__fastcall TfrmExcel(TComponent* Owner);
	void Cobb();
	void ExportToExcel( TStringList *ExcelDataSets, AnsiString ACaption);
	void SendEmail( AnsiString EmailFileName, AnsiString EmailAddress, AnsiString EmailSubject );
  // void SendEmail( AnsiString EmailFileName, AnsiString EmailAddress, AnsiString EmailSubject );
};
//---------------------------------------------------------------------------
#endif
