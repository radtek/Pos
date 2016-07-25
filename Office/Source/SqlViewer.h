//---------------------------------------------------------------------------

#ifndef SqlViewerH
#define SqlViewerH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TSqlView : public TForm
{
__published:	// IDE-managed Components
    TMemo *memoSqlText;
private:	// User declarations
public:		// User declarations
    __fastcall TSqlView(TComponent* Owner);
    void SetSqlText(AnsiString &s);
};
//---------------------------------------------------------------------------
extern PACKAGE TSqlView *SqlView;
//---------------------------------------------------------------------------
#endif
