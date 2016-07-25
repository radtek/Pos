//---------------------------------------------------------------------------

#ifndef RecipeBoxH
#define RecipeBoxH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmRecipeBox : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel2;
    TPanel *Panel3;
    TLabel *Label1;
    TPanel *Panel4;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn2;
    TBitBtn *BitBtn3;
    TBitBtn *BitBtn4;
    void __fastcall CreateRecipeOnClick(TObject *Sender);
    void __fastcall UseRecipeOnClick(TObject *Sender);
    void __fastcall DeleteRecipeOnClick(TObject *Sender);
    void __fastcall EditRecipeOnClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TfrmRecipeBox(TComponent* Owner);
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#endif
