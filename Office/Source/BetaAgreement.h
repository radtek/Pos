//---------------------------------------------------------------------------

#ifndef BetaAgreementH
#define BetaAgreementH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmBetaAgreement : public TForm
{
__published:	// IDE-managed Components
	TRichEdit *RichEdit1;
	TButton *Button1;
	TButton *Button2;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmBetaAgreement(TComponent* Owner);
};
//---------------------------------------------------------------------------
#endif
