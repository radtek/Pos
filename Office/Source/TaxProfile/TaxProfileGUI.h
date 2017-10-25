//---------------------------------------------------------------------------

#ifndef TaxProfileGUIH
#define TaxProfileGUIH
//---------------------------------------------------------------------------

#include "VirtualTrees.hpp"
#include "NumericEdit.h"

#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Classes.hpp>
#include "TaxProfileController.h"
#include "TaxProfile.h"
#include "I_TaxProfileGUI.h"
#include <Buttons.hpp>

#pragma link "VirtualTrees"

#define TP_Column_Name 0
#define TP_Column_Rate 1
#define TP_Column_Type 2
//-- to be incorpotated in future--//
//#define TP_Column_TaxCode 3
#define TP_Column_Priority 4

//---------------------------------------------------------------------------

class TCustomNodeEditorBase : public IVTEditLink
{
   protected:
      TBaseVirtualTree* tree;
      PVirtualNode node;
      TColumnIndex column;
      void* nodeData;
      unsigned long m_RefCount;

      bool (__closure *nodeValidator)(int,AnsiString);
      void (__closure *setNodeProp)(int, PVirtualNode*, AnsiString);
      AnsiString (__closure *getNodeProp)(int, PVirtualNode*);
      void (__closure *endNodeEdit)(PVirtualNode* currentNode, TColumnIndex currentColumn);

   public:
      __fastcall TCustomNodeEditorBase();

      virtual HRESULT __stdcall QueryInterface(const IID& iid, void** ppv);
      virtual unsigned long __stdcall AddRef(void);
      virtual unsigned long __stdcall Release(void);

      void SetNodeValidatorHandler(bool (__closure *validator)(int,AnsiString));
      void SetWriteNodePropertyHandler(void (__closure *setProp)(int,PVirtualNode*,AnsiString));
      void SetReadNodePropertyHandler(AnsiString (__closure *getProp)(int,PVirtualNode*));
      void SetEndNodeEditHandler(void (__closure *endNodeEdit)(PVirtualNode* currentNode, TColumnIndex currentColumn));
};

class TCustomNodeEditorText : public TCustomNodeEditorBase
{
   private:
      TEdit* textEditor;
      void __fastcall textEditorOnKeyPress(TObject *Sender, char &Key);
   public:
      __fastcall TCustomNodeEditorText();
      __fastcall ~TCustomNodeEditorText();

      virtual bool __stdcall BeginEdit(void);
      virtual bool __stdcall CancelEdit(void);
      virtual bool __stdcall EndEdit(void);
      virtual bool __stdcall PrepareEdit(TBaseVirtualTree* Tree, PVirtualNode Node, TColumnIndex Column);
      virtual Types::TRect __stdcall GetBounds(void);
      virtual void __stdcall ProcessMessage(Messages::TMessage &Message);
      virtual void __stdcall SetBounds(const Types::TRect R);
};

class TCustomNodeEditorCombo : public TCustomNodeEditorBase
{
   private:
      std::vector<AnsiString> listItems;
      TComboBox* comboEditor;
      void __fastcall comboEditorOnKeyPress(TObject *Sender, char &Key);
   public:
      __fastcall TCustomNodeEditorCombo();
      __fastcall ~TCustomNodeEditorCombo();

      virtual bool __stdcall BeginEdit(void);
      virtual bool __stdcall CancelEdit(void);
      virtual bool __stdcall EndEdit(void);
      virtual bool __stdcall PrepareEdit(TBaseVirtualTree* Tree, PVirtualNode Node, TColumnIndex Column);
      virtual Types::TRect __stdcall GetBounds(void);
      virtual void __stdcall ProcessMessage(Messages::TMessage &Message);
      virtual void __stdcall SetBounds(const Types::TRect R);
      void AddListItem(AnsiString item);
};

//---------------------------------------------------------------------------

class TfrmTaxProfile : public TForm, public I_TfrmTaxProfile
{
__published:	// IDE-managed Components
//private:	// User declarations
   TPanel *Panel2;
   TPanel *Panel1;
   TVirtualStringTree *vtvProfiles;
   TButton *btnAdd;
   TButton *btnDelete;
   TBitBtn *btnSave;
   TBitBtn *btnCancel;
   void __fastcall btnAddClick(TObject *Sender);
   void __fastcall btnDeleteClick(TObject *Sender);
   void __fastcall vtvProfilesGetText(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType,
          WideString &CellText);
   void __fastcall btnSaveTaxProfilesClick(TObject *Sender);
   void __fastcall vtvProfilesCreateEditor(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column, IVTEditLink *EditLink);
   void __fastcall vtvProfilesKeyPress(TObject *Sender, char &Key);
   void __fastcall vtvProfilesCompareNodes(TBaseVirtualTree *Sender,
          PVirtualNode Node1, PVirtualNode Node2, TColumnIndex Column,
          int &Result);
   void __fastcall vtvProfilesHeaderClick(TVTHeader *Sender,
          TColumnIndex Column, TMouseButton Button, TShiftState Shift,
          int X, int Y);
   void __fastcall vtvProfilesFocusChanged(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column);
   void __fastcall vtvProfilesColumnDblClick(TBaseVirtualTree *Sender,
          TColumnIndex Column, TShiftState Shift);
public:		// User declarations
   __fastcall TfrmTaxProfile(TComponent* Owner);
   void SetController(TaxProfileController* controller);
   void AddTaxProfile(){};
   void EditTaxProfile(){};
   void DeleteTaxProfile(){};
   void Display();
   void ClearGrid();
   void AddTaxProfileToGrid(TaxProfile &tp);
   void Message(AnsiString message, AnsiString header,int type=0);
   void Refresh();

private:
   TaxProfileController* controller;
   TSortDirection sortDirection;
   TColumnIndex sortColumn;

   bool canEditNode(int index, const PVirtualNode &Node);
   void createEditorText( IVTEditLink *EditLink );
   void createEditorCombo( IVTEditLink *EditLink, std::vector<AnsiString> listItems );
   std::vector<AnsiString> getTaxTypes();

   AnsiString getTaxProfilePropertyFromNode(int column, PVirtualNode* node);
   void setTaxProfilePropertyFromNode(int column, PVirtualNode* node, AnsiString newColumnText);

   void shiftEditingToNextColumn(PVirtualNode* currentNode, TColumnIndex currentColumn);
   bool isValidGrid();

   bool validateTaxProfileProperty(int column, AnsiString newColumnText);
   bool validateTaxProfileName(AnsiString name);
   bool validateTaxProfileRate(AnsiString rate);
   bool validateTaxProfilePriority(AnsiString priority);
   bool validateTaxProfileType(AnsiString type);
   bool validateTaxProfileCode(AnsiString code);

   bool checkTaxProfileName(AnsiString name);
   bool checkTaxProfileRate(Currency rate);
   bool checkTaxProfilePriority(int priority);
   bool checkTaxProfileType(TaxType type);
   bool checkTaxProfileCode(int code);

   bool checkDuplicateTaxProfile(int column, AnsiString newColumnText);

   void editSelectedNode( int columnIndex=TP_Column_Name );
   void setSelectedNode( PVirtualNode &node );
   void toggleDeleteButton();
   TaxType getTaxProfileType(int column, PVirtualNode* node);
};


//---------------------------------------------------------------------------
#endif



