//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RecipeBox.h"
#include "CreateRecipe.h"
#include "UseRecipe.h"
#include "DeleteRecipe.h"
#include "EditRecipe.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TfrmRecipeBox::TfrmRecipeBox(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecipeBox::CreateRecipeOnClick(TObject *Sender)
{
	std::auto_ptr<TfrmCreateRecipe>frmCreateRecipe(new TfrmCreateRecipe(this));
	frmCreateRecipe->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeBox::UseRecipeOnClick(TObject *Sender)
{
	std::auto_ptr<TfrmUseRecipe>frmUseRecipe(new TfrmUseRecipe(this));
	frmUseRecipe->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeBox::DeleteRecipeOnClick(TObject *Sender)
{
	std::auto_ptr<TfrmDeleteRecipe>frmDeleteRecipe(new TfrmDeleteRecipe(this));
	frmDeleteRecipe->ShowModal();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

void __fastcall TfrmRecipeBox::EditRecipeOnClick(TObject *Sender)
{
	std::auto_ptr<TfrmEditRecipe>frmEditRecipe(new TfrmEditRecipe(this));
	frmEditRecipe->ShowModal();
}
//---------------------------------------------------------------------------

