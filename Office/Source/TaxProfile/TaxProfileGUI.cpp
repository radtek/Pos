//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TaxProfileGUI.h"
#include "Utilities.h"
#include <DesignIntf.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "VirtualTrees"
#pragma link "VirtualTrees"
#pragma link "VirtualTrees"
#pragma link "VirtualTrees"
#pragma link "VirtualTrees"
#pragma link "NumericEdit"
#pragma link "VirtualTrees"
#pragma link "VirtualTrees"
#pragma link "VirtualTrees"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmTaxProfile::TfrmTaxProfile(TComponent* Owner)
   : TForm(Owner)
{
	this->vtvProfiles->NodeDataSize		= sizeof(TaxProfile);
   sortDirection = sdAscending;
   sortColumn = TP_Column_Priority;
}

void TfrmTaxProfile::SetController(TaxProfileController* controller)
{
   this->controller = controller;
}
//---------------------------------------------------------------------------
void TfrmTaxProfile::Display()
{
   this->ShowModal();
}
//---------------------------------------------------------------------------

/* add button click handler */
void __fastcall TfrmTaxProfile::btnAddClick(TObject *Sender)
{
   vtvProfiles->EndEditNode();

   if(!isValidGrid())
   {
      // grid status invalid, cannot add a new item until it is valid.
      ShowMessage("Please correct the errors on the form, before adding a new profile");
      return;
   }

   /*
      calls the controller method to add tax profile object
      sets the last node as the focused node
      set the selection status of the focused node
      starts editing on the selected note
   */

   this->controller->AddNewTaxProfile();
   vtvProfiles->FocusedNode = vtvProfiles->GetLast();
   setSelectedNode( vtvProfiles->FocusedNode );
   vtvProfiles->EditNode( vtvProfiles->FocusedNode, 0 );
}
//---------------------------------------------------------------------------

/* delete button click handler */
void __fastcall TfrmTaxProfile::btnDeleteClick(TObject *Sender)
{
   int status = Application->MessageBox("Are you sure to remove this tax profile ?", "Delete profile", MB_OKCANCEL + MB_ICONINFORMATION);

   if( status == IDOK)
   {
      TaxProfile* selectedProfile = (TaxProfile*)vtvProfiles->GetNodeData(vtvProfiles->FocusedNode);
      this->controller->DeleteTaxProfile( selectedProfile );
   }

   toggleDeleteButton();
}
//---------------------------------------------------------------------------

void TfrmTaxProfile::ClearGrid()
{
   this->vtvProfiles->Clear();
}
//---------------------------------------------------------------------------

/*
   adds new tax profile node to the grid
   gets the node data pointer and assignes values
*/
void TfrmTaxProfile::AddTaxProfileToGrid(TaxProfile &taxProfile)
{
   PVirtualNode newNode       = vtvProfiles->AddChild(NULL);
   TaxProfile* NodeData       = (TaxProfile*)vtvProfiles->GetNodeData( newNode );

   NodeData->taxProfileName	  = taxProfile.taxProfileName;
   NodeData->taxPercentage    = taxProfile.taxPercentage;
   NodeData->taxProfileType   = taxProfile.taxProfileType;
   //NodeData->taxCode          = taxProfile.taxCode;   
   NodeData->taxPriority      = taxProfile.taxPriority;
   NodeData->SetTaxProfileDBKey(taxProfile.GetTaxProfileDBKey());

}
//---------------------------------------------------------------------------

/*
   get text method will be called everytime the grid needs to draw a cell
   need to provide appropriate string representations for the columns
*/
void __fastcall TfrmTaxProfile::vtvProfilesGetText(
      TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
      TVSTTextType TextType, WideString &CellText)
{
	TaxProfile *NodeData = (TaxProfile *)Sender->GetNodeData(Node);
	if (NodeData)
	{
		switch (Column)
		{
			case TP_Column_Name:
                CellText = NodeData->taxProfileName;
				break;
			case TP_Column_Rate:
                CellText = (AnsiString)NodeData->taxPercentage + '%';
				break;
			case TP_Column_Type:
                CellText = TaxProfile::Convert(NodeData->taxProfileType);
				break;
            //-- to be incorpotated in future--//
			//case TP_Column_TaxCode:
              //  CellText = NodeData->taxCode;
				//break;
			case TP_Column_Priority:
                CellText = (AnsiString)NodeData->taxPriority;
				break;
		}
	}
	else
	{
		CellText = "";
	}
}
//---------------------------------------------------------------------------

void TfrmTaxProfile::Message(AnsiString message, AnsiString header, int type)
{
   switch(type)
   {
      case 0:
         Application->MessageBox((message).c_str(), header.c_str(), MB_OK + MB_ICONERROR);
         break;
      case 1:
         Application->MessageBox((message).c_str(), header.c_str(), MB_OK + MB_ICONINFORMATION);
         break;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmTaxProfile::btnSaveTaxProfilesClick(TObject *Sender)
{
   if( isValidGrid() )
   {
      this->controller->SaveTaxProfiles();
      this->Close();
   }
   else
   {
      ShowMessage("Please correct the errors on the form before proceeding");
      return;
   }
}
//---------------------------------------------------------------------------

/*
   create editor event will be fired when editing starts on a node.
   this method is used to create custom editors if needed
*/
void __fastcall TfrmTaxProfile::vtvProfilesCreateEditor(
      TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
      IVTEditLink *EditLink)
{
   if(!canEditNode(Column, Node))
   {
      return;
   }

   switch(Column)
   {
      case TP_Column_Name:
      case TP_Column_Rate:
      //case TP_Column_TaxCode:
      case TP_Column_Priority:
         createEditorText(EditLink);
         break;
      case TP_Column_Type:
         createEditorCombo(EditLink, getTaxTypes());
         break;
      default:
         throw new Exception("Unrecognized tax profile column");
         break;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmTaxProfile::vtvProfilesFocusChanged(
      TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column)
{
   toggleDeleteButton();
}
//---------------------------------------------------------------------------

bool TfrmTaxProfile::canEditNode(int index, const PVirtualNode &Node)
{
   // user can edit any column at the moment
   return true;
}
//---------------------------------------------------------------------------

/* custom text editor creator */
void TfrmTaxProfile::createEditorText( IVTEditLink *EditLink )
{
   TCustomNodeEditorText* ptr = new TCustomNodeEditorText();

   // set handlers
   ptr->SetNodeValidatorHandler(validateTaxProfileProperty);
   ptr->SetReadNodePropertyHandler(getTaxProfilePropertyFromNode);
   ptr->SetWriteNodePropertyHandler(setTaxProfilePropertyFromNode);
   ptr->SetEndNodeEditHandler(shiftEditingToNextColumn);

   // make the connection with EditLink
   ptr->QueryInterface(__uuidof(IVTEditLink),(void**)EditLink);
}
//---------------------------------------------------------------------------

/* custom combo editor creator */
void TfrmTaxProfile::createEditorCombo( IVTEditLink *EditLink, std::vector<AnsiString> listItems )
{
   TCustomNodeEditorCombo* comboEditorPtr = new TCustomNodeEditorCombo();
   std::vector<AnsiString>::iterator it = listItems.begin();

   for( ; it != listItems.end(); it++)
   {
      comboEditorPtr->AddListItem( *it );
   }

   // set handlers
   comboEditorPtr->SetNodeValidatorHandler(validateTaxProfileProperty);
   comboEditorPtr->SetReadNodePropertyHandler(getTaxProfilePropertyFromNode);
   comboEditorPtr->SetWriteNodePropertyHandler(setTaxProfilePropertyFromNode);
   comboEditorPtr->SetEndNodeEditHandler(shiftEditingToNextColumn);

   // make the connection with EditLink
   comboEditorPtr->QueryInterface(__uuidof(IVTEditLink),(void**)EditLink);
}
//---------------------------------------------------------------------------

/* returns all the tax types as a AnsiString vector */
std::vector<AnsiString> TfrmTaxProfile::getTaxTypes()
{
   std::vector<AnsiString> items;
   items.clear();

   for(int i = 0; i < NumTaxTypes; i++)
   {
      if(i == ServiceChargeTax )
      {
          continue;
      }
      items.push_back(TaxStringType[i]);
   }

   return items;
}
//---------------------------------------------------------------------------

/*
   moves the focus on to the next available column and starts editing
   this method is called from the custom editors whenver the enter key is pressed on them
*/
void TfrmTaxProfile::shiftEditingToNextColumn(PVirtualNode* currentNode, TColumnIndex currentColumn)
{
   bool finishedEditing = true; // editing is finished or not on the current node

   if(vtvProfiles->IsEditing())
   {
      finishedEditing = vtvProfiles->EndEditNode();
   }

   if(finishedEditing)
   {
      int nextColumn = 0;

      if(currentColumn < TP_Column_Priority)
      {
         nextColumn = currentColumn + 1;

         vtvProfiles->SetFocus();
         vtvProfiles->FocusedNode = *currentNode;
         vtvProfiles->EditNode( vtvProfiles->FocusedNode, nextColumn );
      }
      else
      {
         // enter key is pressed on the last column, move focus to add button
         btnAdd->SetFocus();
      }
   }
}
//---------------------------------------------------------------------------

/* validates the last node in the grid. if it is valid, then the grid is assumed to be valid */
bool TfrmTaxProfile::isValidGrid()
{
   bool status = true;

   if(status)
   {
      PVirtualNode node = vtvProfiles->GetFirst();

      while( status && (node != 0))
      {
         TaxProfile* profileData = (TaxProfile*)vtvProfiles->GetNodeData(node);

         status = checkTaxProfileName( profileData->taxProfileName )
                     && checkTaxProfileRate( profileData->taxPercentage )
                     && checkTaxProfilePriority( profileData->taxPriority )
                     && checkTaxProfileType( profileData->taxProfileType);
                     //-- to be incorpotated in future--//
                     //&& checkTaxProfileCode(profileData->taxCode);

         node = vtvProfiles->GetNextSibling(node);
      }
   }

   return status;
}
//---------------------------------------------------------------------------

/* gets the property value represents a column index */
AnsiString TfrmTaxProfile::getTaxProfilePropertyFromNode(int column, PVirtualNode* node)
{
   AnsiString propertyValue = "";
   TaxProfile* nodeTaxProfile = (TaxProfile*)vtvProfiles->GetNodeData(*node);

   switch(column)
   {
      case TP_Column_Name:
         propertyValue = nodeTaxProfile->taxProfileName;
         break;
      case TP_Column_Rate:
         propertyValue = nodeTaxProfile->taxPercentage;
         break;
      //-- to be incorpotated in future--//
      /*case TP_Column_TaxCode:
         propertyValue = nodeTaxProfile->taxCode;
         break;*/
      case TP_Column_Priority:
         propertyValue = nodeTaxProfile->taxPriority;
         break;
      default:
         propertyValue = "";
         break;
   }

   return propertyValue;
}
//---------------------------------------------------------------------------

/* convert and sets the property value represented by the column index */
void TfrmTaxProfile::setTaxProfilePropertyFromNode(int column, PVirtualNode* node, AnsiString newColumnText)
{
   TaxProfile* nodeTaxProfile = (TaxProfile*)vtvProfiles->GetNodeData(*node);

   switch(column)
   {
      case TP_Column_Name:
         nodeTaxProfile->taxProfileName = newColumnText;
         break;
      case TP_Column_Rate:
         nodeTaxProfile->taxPercentage = Currency(newColumnText);
         break;
      //-- to be incorpotated in future--//
      /*case TP_Column_TaxCode:
         nodeTaxProfile->taxCode = StrToInt(newColumnText);
         break;*/
      case TP_Column_Priority:
         nodeTaxProfile->taxPriority = StrToInt(newColumnText);
         break;
      case TP_Column_Type:
         nodeTaxProfile->taxProfileType = TaxProfile::Convert(newColumnText);
         break;
      default:
         break;
   }

   controller->UpdateTaxProfile( nodeTaxProfile );
}
//---------------------------------------------------------------------------

/* validates tax profile property for a column index */
bool TfrmTaxProfile::validateTaxProfileProperty(int column, AnsiString newColumnText)
{
   bool status = false;

   switch(column)
   {
      case TP_Column_Name:
         status = validateTaxProfileName( newColumnText );
         break;
      case TP_Column_Rate:
         status = validateTaxProfileRate( newColumnText );
         break;
      /*case TP_Column_TaxCode:
         status = validateTaxProfileCode( newColumnText );
         break;*/
      case TP_Column_Priority:
         status = validateTaxProfilePriority( newColumnText );
         break;
      case TP_Column_Type:
         status = validateTaxProfileType( newColumnText );
         break;
      default:
         break;
   }

   if( column == TP_Column_Name || column == TP_Column_Type )
   {
      // check for duplicate profiles, as we do not allow profiles with same name and type
      status = status && !checkDuplicateTaxProfile( column, newColumnText );
   }

   return status;
}

//---------------------------------------------------------------------------

bool TfrmTaxProfile::validateTaxProfileName(AnsiString name)
{
   bool isValid = false;

   isValid = checkTaxProfileName(name);

   if(!isValid)
   {
      Message("Tax profile must have a name", "Tax profile name");
   }

   return isValid;
}
//---------------------------------------------------------------------------

bool TfrmTaxProfile::validateTaxProfileRate(AnsiString rate)
{
   bool isValid = false;
   AnsiString message = "Tax profile rate must have a value";

   try
   {
      Currency value = Currency(rate);
      isValid = checkTaxProfileRate(value);
   }
   catch(...)
   {
      message = "Invalid tax profile rate";
   }

   if(!isValid)
   {
      Message(message,"Tax profile rate");
   }

   return isValid;
}
//---------------------------------------------------------------------------

bool TfrmTaxProfile::validateTaxProfilePriority(AnsiString priority)
{
   bool isValid = false;
   AnsiString message = "Tax profile priority must have a value";

   try
   {
      int value = StrToInt(priority);
      isValid = checkTaxProfilePriority(value);
   }
   catch(...)
   {
      message = "Invalid tax priority";
   }

   if(!isValid)
   {
      Message(message,"Tax profile priority");
   }

   return isValid;
}
//---------------------------------------------------------------------------

bool TfrmTaxProfile::validateTaxProfileType(AnsiString typeStr)
{
   bool isValid = false;
   AnsiString message = "Tax profile type must be assigned";

   try
   {
      TaxType type = TaxProfile::Convert(typeStr);
      isValid = checkTaxProfileType(type);
   }
   catch(...)
   {
      message = "Invalid tax type";
   }

   if(!isValid)
   {
      Message(message,"Tax profile type");
   }

   return isValid;
}
//---------------------------------------------------------------------------
bool TfrmTaxProfile::validateTaxProfileCode(AnsiString code)
{
   bool isValid = false;
   AnsiString message = "Tax Code must have a value";
   try
   {
      int value = StrToInt(code);
      isValid  = checkTaxProfileCode(value);
   }
   catch(...)
   {
       message = "Invalid Tax Code";
   }

   if(!isValid)
   {
      Message(message, "Tax profile code");
   }
   return isValid;
}
//---------------------------------------------------------------------------

bool TfrmTaxProfile::checkTaxProfileName(AnsiString name)
{
   return name.Length() > 0;
}
//---------------------------------------------------------------------------

bool TfrmTaxProfile::checkTaxProfileRate(Currency rate)
{
   return rate >= 0;
}
//---------------------------------------------------------------------------

bool TfrmTaxProfile::checkTaxProfilePriority(int priority)
{
   return priority >= 0;
}
//---------------------------------------------------------------------------

bool TfrmTaxProfile::checkTaxProfileType(TaxType type)
{
   return type != DefaultNULLTax;
}
//---------------------------------------------------------------------------
bool TfrmTaxProfile::checkTaxProfileCode(int code)
{
    return code >= 0;
}
//---------------------------------------------------------------------------

/* checks for the duplicate tax profiles with the same name and type */
bool TfrmTaxProfile::checkDuplicateTaxProfile(int column, AnsiString newColumnText)
{
   bool status = false;

   if( vtvProfiles->FocusedNode != 0 )
   {
      TaxProfile* nodeData = static_cast<TaxProfile*>(
                                 vtvProfiles->GetNodeData(vtvProfiles->FocusedNode));

      status = controller->IsDuplicateTaxProfileExists(
                                 nodeData->GetTaxProfileDBKey(),
                                 column == TP_Column_Name ? newColumnText : nodeData->taxProfileName, // if currently editing the name
                                 column == TP_Column_Type ? TaxProfile::Convert(newColumnText) : nodeData->taxProfileType);   // if editing the type

      if(status)
      {
         Message("Tax profile already exists with name and type", "Duplicate profile");
      }
   }

   return status;
}
//---------------------------------------------------------------------------

/* keypress event handler on the grid */
void __fastcall TfrmTaxProfile::vtvProfilesKeyPress(TObject *Sender,
      char &Key)
{
   if( Key == '\r' && !vtvProfiles->IsEditing() )
   {
      editSelectedNode();
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmTaxProfile::vtvProfilesColumnDblClick(
      TBaseVirtualTree *Sender, TColumnIndex Column, TShiftState Shift)
{
   bool finishedEditing = true;

   if(vtvProfiles->IsEditing())
   {
      finishedEditing = vtvProfiles->EndEditNode();
   }
   if( finishedEditing )
      editSelectedNode(Column);
}
//---------------------------------------------------------------------------

/* starts editing on the selected node */
void TfrmTaxProfile::editSelectedNode(int columnIndex)
{
   if( vtvProfiles->FocusedNode != 0 )
   {
      /*if(columnIndex == TP_Column_TaxCode)
      {
         PVirtualNode* node = new PVirtualNode();
         if(true)
         {
            vtvProfiles->EditNode( vtvProfiles->FocusedNode, columnIndex );
            if(((TaxProfile*)vtvProfiles->FocusedNode)->taxProfileType == SalesTax)
            {
               vtvProfiles->EndEditNode();
            }
         }
      }
      else
      { */
        vtvProfiles->EditNode( vtvProfiles->FocusedNode, columnIndex );
      //}
   }
}
//---------------------------------------------------------------------------

void TfrmTaxProfile::setSelectedNode( PVirtualNode &node )
{
   vtvProfiles->Selected[node] = true;
}
//---------------------------------------------------------------------------

void TfrmTaxProfile::toggleDeleteButton()
{
   // toggle the delete button if a node is selected or not
   btnDelete->Enabled = vtvProfiles->FocusedNode != 0 ? true : false;
}
//---------------------------------------------------------------------------

/* sorts the grid using the column and direction */
void TfrmTaxProfile::Refresh()
{
   vtvProfiles->Sort( vtvProfiles->RootNode, sortColumn, sortDirection, true );
}
//---------------------------------------------------------------------------

/* sorts the grid using the current column and toggles the direction */
void __fastcall TfrmTaxProfile::vtvProfilesHeaderClick(TVTHeader *Sender,
      TColumnIndex Column, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
   sortDirection = !sortDirection;
   sortColumn    = Column;

   vtvProfiles->Sort( vtvProfiles->RootNode, sortColumn, sortDirection, true );
}
//---------------------------------------------------------------------------

/*
   compare nodes event will be called whenever sorting is occured
   -1 means the first item comes before second
   +1 means first item comes after the second
   0 means they are equal
*/
void __fastcall TfrmTaxProfile::vtvProfilesCompareNodes(
      TBaseVirtualTree *Sender, PVirtualNode Node1, PVirtualNode Node2,
      TColumnIndex Column, int &Result)
{
   int result = 0;
   TaxProfile* lhs = static_cast<TaxProfile*>(vtvProfiles->GetNodeData(Node1));
   TaxProfile* rhs = static_cast<TaxProfile*>(vtvProfiles->GetNodeData(Node2));

   switch(Column)
   {
      case TP_Column_Name:
         result = lhs->taxProfileName > rhs->taxProfileName ? -1 : 1;
         break;
      case TP_Column_Rate:
         result = lhs->taxPercentage > rhs->taxPercentage ? -1 : 1;
         break;
      case TP_Column_Type:
         result = lhs->taxProfileType > rhs->taxProfileType ? -1 : 1;
         break;
       //-- to be incorpotated in future--//
      /*case TP_Column_TaxCode:
         result = lhs->taxCode > rhs->taxCode ? -1 : 1;
         break;*/
      case TP_Column_Priority:
         result = StrToInt(lhs->taxPriority) > StrToInt(rhs->taxPriority) ? 1 : -1;
         break;
      default:
         result = 0;
         break;
   }

   Result = result;
}

//---------------------------------------------------------------------------
// TCustomNodeEditorBase members
//---------------------------------------------------------------------------

__fastcall TCustomNodeEditorBase::TCustomNodeEditorBase() : IVTEditLink()
{
   m_RefCount = 0;
   nodeValidator = 0;
}

/* IUnkown */

/* implementation of com interface methods */
HRESULT __stdcall TCustomNodeEditorBase::QueryInterface(const IID& iid, void** ppv)
{
   if(iid == IID_IUnknown || iid == __uuidof(IVTEditLink))
   {
      *ppv = static_cast<IVTEditLink*>(this);
   }
   else
   {
      *ppv = 0;
      return E_NOINTERFACE;
   }

   (reinterpret_cast<IUnknown*>(*ppv))->AddRef();
   return S_OK;
}

unsigned long __stdcall TCustomNodeEditorBase::AddRef(void)
{
   return static_cast<unsigned long>(InterlockedIncrement(
   reinterpret_cast<long*>(&m_RefCount)));
}

unsigned long __stdcall TCustomNodeEditorBase::Release(void)
{
   if(m_RefCount == 1)
   {
      delete this;
      return 0;
   }
   else
   {
      return static_cast<unsigned long>(InterlockedDecrement(reinterpret_cast<long*>(&m_RefCount)));
   }
}

void TCustomNodeEditorBase::SetNodeValidatorHandler(bool (__closure *validator)(int,AnsiString))
{
   nodeValidator = validator;
}

void TCustomNodeEditorBase::SetWriteNodePropertyHandler(void (__closure *setProp)(int,PVirtualNode*,AnsiString))
{
   setNodeProp = setProp;
}

void TCustomNodeEditorBase::SetReadNodePropertyHandler(AnsiString (__closure *getProp)(int,PVirtualNode*))
{
   getNodeProp = getProp;
}

void TCustomNodeEditorBase::SetEndNodeEditHandler(void (__closure *endEdit)(PVirtualNode* currentNode, TColumnIndex currentColumn))
{
   endNodeEdit = endEdit;
}

//---------------------------------------------------------------------------
// TCustomNodeEditorText members
//---------------------------------------------------------------------------

__fastcall TCustomNodeEditorText::TCustomNodeEditorText() : TCustomNodeEditorBase()
{
   textEditor = new TEdit(this);
   textEditor->Visible = false;
   textEditor->OnKeyPress = textEditorOnKeyPress;
}

__fastcall TCustomNodeEditorText::~TCustomNodeEditorText()
{
   delete textEditor;
}

bool __stdcall TCustomNodeEditorText::BeginEdit(void)
{
   textEditor->Show();
   textEditor->SetFocus();

   return true;
}

bool __stdcall TCustomNodeEditorText::CancelEdit(void)
{
   textEditor->Hide();
   tree->SetFocus();

   return true;
}

bool __stdcall TCustomNodeEditorText::EndEdit(void)
{
  bool valid = nodeValidator != 0
                  && nodeValidator(column,textEditor->Text);       // validate using external handler

   if( valid
         && setNodeProp != 0 )
   {
      // if valid, then set the node using the external handler
      setNodeProp(column, &node, textEditor->Text);

      // hide editor
      textEditor->Hide();
      tree->SetFocus();
   }

   return valid;
}

bool __stdcall TCustomNodeEditorText::PrepareEdit(TBaseVirtualTree* Tree, PVirtualNode Node, TColumnIndex Column)
{
   // set ptrs
   tree = Tree;
   node = Node;
   column = Column;

   // get data for node
   nodeData =
   static_cast<void*>(tree->GetNodeData(node));

   // init text editor
   textEditor->Parent = tree;
   textEditor->Text = getNodeProp != 0 ? getNodeProp(Column,&Node) : AnsiString("");
   /*if(Column == TP_Column_TaxCode && ((TaxProfile*)nodeData)->taxProfileType != SalesTax)
      return false;*/
   return true;
}

Types::TRect __stdcall TCustomNodeEditorText::GetBounds(void)
{
   return textEditor->BoundsRect;
}

void __stdcall TCustomNodeEditorText::ProcessMessage(Messages::TMessage &Message)
{
   textEditor->WindowProc(Message); 
}

void __stdcall TCustomNodeEditorText::SetBounds(const Types::TRect R)
{
   textEditor->BoundsRect = tree->GetDisplayRect(node,column,false,true);
}

void __fastcall TCustomNodeEditorText::textEditorOnKeyPress(TObject *Sender, char &Key)
{
   if( Key == '\r' )
   {
      endNodeEdit(&node, column);
   }
}

//---------------------------------------------------------------------------
// TCustomNodeEditorCombo members
//---------------------------------------------------------------------------

__fastcall TCustomNodeEditorCombo::TCustomNodeEditorCombo() : TCustomNodeEditorBase()
{
   comboEditor = new TComboBox(this);
   comboEditor->Visible = false;
   comboEditor->OnKeyPress = comboEditorOnKeyPress;
   listItems.clear();
}

__fastcall TCustomNodeEditorCombo::~TCustomNodeEditorCombo()
{
   delete comboEditor;
}

bool __stdcall TCustomNodeEditorCombo::BeginEdit(void)
{
   comboEditor->Show();
   comboEditor->SetFocus();

   return true;
}

bool __stdcall TCustomNodeEditorCombo::CancelEdit(void)
{
   comboEditor->Hide();
   tree->SetFocus();

   return true;
}

bool __stdcall TCustomNodeEditorCombo::EndEdit(void)
{
   bool valid = nodeValidator != 0
                  && nodeValidator(column, comboEditor->Text);       // validate using external handler

   if( valid
         && setNodeProp != 0 )
   {
      // if valid, then set the node using the external handler
      setNodeProp(column, &node, comboEditor->Text);

      // hide combo
      comboEditor->Hide();
      tree->SetFocus();
   }

   return valid;
}

bool __stdcall TCustomNodeEditorCombo::PrepareEdit(TBaseVirtualTree* Tree, PVirtualNode Node, TColumnIndex Column)
{
   // set ptrs
   tree = Tree;
   node = Node;
   column = Column;

   // get data for node
   nodeData =
   static_cast<void*>(tree->GetNodeData(node));

   // init combobox
   comboEditor->Parent = tree;

   // populating items list
   for(  std::vector<AnsiString>::iterator it = listItems.begin();
         it != listItems.end();
         it++ )
   {
      comboEditor->AddItem(*it, 0);
   }

   comboEditor->ItemIndex = 0;
   comboEditor->Text = comboEditor->Items->Strings[0];

   return true;
}

Types::TRect __stdcall TCustomNodeEditorCombo::GetBounds(void)
{
   return comboEditor->BoundsRect;
}

void __stdcall TCustomNodeEditorCombo::ProcessMessage(Messages::TMessage &Message)
{
   comboEditor->WindowProc(Message);
}

void __stdcall TCustomNodeEditorCombo::SetBounds(const Types::TRect R)
{
   comboEditor->BoundsRect = tree->GetDisplayRect(node,column,false,true);
}

void TCustomNodeEditorCombo::AddListItem(AnsiString item)
{
   listItems.push_back(item);
}

void __fastcall TCustomNodeEditorCombo::comboEditorOnKeyPress(TObject *Sender, char &Key)
{
   if( Key == '\r' )
   {
      endNodeEdit(&node, column);
   }
}
//---------------------------------------------------------------------------

