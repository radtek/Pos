//----------------------------------------------------------------------------
#ifndef DialogH
#define DialogH
//----------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Windows.hpp>
#include <Messages.hpp>
#include <Classes.hpp>
#include <Graphics.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//----------------------------------------------------------------------------
enum		TButtonResult { brNone, brLoadFromDB, brLoadFromFile, brNewMenu };
//----------------------------------------------------------------------------
class TfrmMenuLoad : public TForm
{
__published:
	TPanel *Panel2;
	TPanel *Panel8;
	TPanel *Panel9;
	TLabel *lbeTitle;
	TBitBtn *btnCancel;
	TPanel *Panel1;
	TImage *imgQuestion;
	TLabel *Label1;
	TButton *btnNewMenu;
	TButton *btnLoadDB;
	TButton *Button2;
	void __fastcall btnLoadDBClick(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall btnNewMenuClick(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);
protected:
	int __fastcall 		ShowModal(void) { return TForm::ShowModal(); };
private:
	TButtonResult			ButtonPressed;
public:
	virtual __fastcall 	TfrmMenuLoad(TComponent *Owner);
	TButtonResult			Execute();
	bool ShowDBLoad;
};
//----------------------------------------------------------------------------
//extern TfrmMenuLoad *frmMenuLoad;
//----------------------------------------------------------------------------
#endif
