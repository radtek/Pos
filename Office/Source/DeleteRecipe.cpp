//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DeleteRecipe.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "VirtualTrees"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmDeleteRecipe::TfrmDeleteRecipe(TComponent* Owner)
    : TForm(Owner)
{
    vtvStock->NodeDataSize = sizeof(TDeleteRecipeNodeData);
	if (!Transaction->InTransaction) Transaction->StartTransaction();
//	dtRecipes->Close();
//	dtRecipes->Open();
    DisplayRecipes();
}
//---------------------------------------------------------------------------
void __fastcall TfrmDeleteRecipe::btnCancelOnClick(TObject *Sender)
{
	ModalResult = mrCancel;    
}
//---------------------------------------------------------------------------


void TfrmDeleteRecipe::DisplayRecipes(void)
{
    vtvStock->Clear();
	qrRecipe->Close();
	for (qrRecipe->Open(); !qrRecipe->Eof; qrRecipe->Next())
	{
        PVirtualNode RecipeNode						= NULL;
        TDeleteRecipeNodeData *NodeData				= NULL;

        RecipeNode = vtvStock->AddChild(NULL);
        NodeData = (TDeleteRecipeNodeData *)vtvStock->GetNodeData(RecipeNode);

        NodeData->Recipe = qrRecipe->FieldByName("Recipe")->AsString;
        NodeData->Final_Stock = qrRecipe->FieldByName("Final_Stock")->AsString;
        NodeData->Recipe_ID = qrRecipe->FieldByName("Recipes_ID")->AsInteger;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmDeleteRecipe::vtvStockGetText(TBaseVirtualTree *Sender,
      PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType,
      WideString &CellText)
{
	TDeleteRecipeNodeData *NodeData = (TDeleteRecipeNodeData *)Sender->GetNodeData(Node);
	if (NodeData)
	{
		switch (Column)
		{
			case 0:	CellText = NodeData->Recipe;
						break;
			case 1:	CellText = NodeData->Final_Stock;
						break;
		}
    }
	else
	{
		CellText = "";
	}

}
//---------------------------------------------------------------------------

void __fastcall TfrmDeleteRecipe::DeleteRecipeOnClick(TObject *Sender)
{
    if (vtvStock->IsEditing())
        vtvStock->EndEditNode();

    if(checkIsRecipeSelectedOrNot())
    {

        if(Application->MessageBox("Are you sure you want to delete the selected recipes", "", MB_YESNO + MB_ICONQUESTION) == ID_NO)
            return;

        PVirtualNode Node = vtvStock->GetFirst();
        while (Node)
        {
            TDeleteRecipeNodeData *NodeData = (TDeleteRecipeNodeData *)vtvStock->GetNodeData(Node); 
            if(vtvStock->Selected[Node])
                DeleteRecipe(NodeData->Recipe_ID);

            Node = vtvStock->GetNext(Node);
        }
        DisplayRecipes();
    }
    else
    {
        Application->MessageBox("Select a recipe to delete", "Information", MB_OK + MB_ICONINFORMATION);
    }
}
//---------------------------------------------------------------------------

void TfrmDeleteRecipe::DeleteRecipe(int Recipe_ID)
{
	Query->Close();
    Query->SQL->Text =
        "Update Recipes Set Deleted = 'T' "
        "Where Recipes_ID = :Recipes_ID;";
    Query->ParamByName("Recipes_Id")->AsInteger = Recipe_ID;
    Query->ExecQuery();

}
//---------------------------------------------------------------------------
bool TfrmDeleteRecipe::checkIsRecipeSelectedOrNot()
{
    bool isRecipeSelected = true;
    PVirtualNode Node = vtvStock->GetFirst();
    while(Node)
    {
        TDeleteRecipeNodeData *NodeData = (TDeleteRecipeNodeData *)vtvStock->GetNodeData(Node);
        if(!vtvStock->Selected[Node])
        {
           isRecipeSelected = false;
        }
        else
        {
           isRecipeSelected = true;
           break;
        }
        Node = vtvStock->GetNext(Node);
    }
    return isRecipeSelected;
}
//---------------------------------------------------------------------------
