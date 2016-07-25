//---------------------------------------------------------------------------
#ifndef SplashH
#define SplashH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <Graphics.hpp>
#include <IBDatabase.hpp>
#include <IBSQL.hpp>
#include "pngimage.hpp"
#include <jpeg.hpp>
//---------------------------------------------------------------------------
class TfrmSplash : public TForm
{                                                           
__published:	// IDE-managed Components
	TImage *imgSplash;
	TLabel *lbeTitle;
	TLabel *lbeVersion;
	TLabel *lbeStatus;
	TLabel *lbeRegistration;void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
//	int WinX, WinY;
//	bool Moving;
//	bool OpenMenuMate();
//	bool OpenStock();

//	bool Initialise();// Call this something better.
//	bool ConnectMM();
//	bool ConnectStock();
public:		// User declarations
	__fastcall TfrmSplash(TComponent* Owner);
//	bool OpenDB();
//	void CloseDB();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSplash *frmSplash;
//---------------------------------------------------------------------------
#endif
 
