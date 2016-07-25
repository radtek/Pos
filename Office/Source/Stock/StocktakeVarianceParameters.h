//---------------------------------------------------------------------------

#ifndef StocktakeVarianceParametersH
#define StocktakeVarianceParametersH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "NumericEdit.h"
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <memory>        
//---------------------------------------------------------------------------
enum TVarianceParams { vInitialisation, vVarianceReport, vReports,vResetUndo,vReportType };

class TfrmVarianceParams : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel2;
	TPanel *Panel3;
	TPanel *Panel4;
	TLabel *lblTop;
	TPanel *Panel5;
	TBitBtn *btnOk;
	TBitBtn *btnCancel;
	TPanel *pnlInitialisation;
	TRadioButton *RadioButton2;
	TRadioButton *RadioButton1;
	TLabel *Label4;
	TPanel *pnlVariance;
	TRadioButton *RadioButton3;
	TRadioButton *RadioButton4;
	TRadioButton *rbSetVarianceAmount;
	TNumericEdit *neVarianceAmount;
	TPanel *pnlReports;
	TRadioButton *RadioButton6;
	TRadioButton *RadioButton7;
	TBitBtn *btnPreview;
	TPanel *Panel1;
	TPanel *pnlVarianceParams;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TNumericEdit *neTotalBankingInc;
	TNumericEdit *neTotalBankingExcl;
	TNumericEdit *neUserVariance;
	TPanel *pnlResetUndo;
	TRadioButton *RadioButton5;
	TRadioButton *RadioButton8;
	TRadioButton *RadioButton9;
	TRadioGroup *rgpReportType;
	void __fastcall neTotalBankingIncChange(TObject *Sender);
	void __fastcall neTotalBankingIncKeyPress(TObject *Sender, char &Key);
	void __fastcall RadioButton2Click(TObject *Sender);
	void __fastcall btnPreviewClick(TObject *Sender);

private:	// User declarations
	int fRadioButtonValue;
	void SetUpScreen(TVarianceParams VarianceParams);
public:		// User declarations
	class TResults
	{
	public:
		TResults(Currency iTotalBankingInc,
					Currency iTotalBankingExcl,
					Currency iUserVariance) :
							TotalBankingInc(iTotalBankingInc),
							TotalBankingExcl(iTotalBankingExcl),
							UserVariance(iUserVariance)
		{}
		Currency TotalBankingInc;
		Currency TotalBankingExcl;
		Currency UserVariance;
	};
	TResults Results()
	{
		return TResults(neTotalBankingInc->Value,
							 neTotalBankingExcl->Value,
							 neUserVariance->Value);
	}
  	__property TVarianceParams VarianceParams = {write=SetUpScreen};
	__property int RadioButtonValue = {read=fRadioButtonValue};
	int StocktakeKey;
	__fastcall TfrmVarianceParams(TComponent* Owner);
};
//---------------------------------------------------------------------------
#endif
