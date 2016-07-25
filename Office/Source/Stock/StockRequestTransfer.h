//---------------------------------------------------------------------------

#ifndef StockRequestTransferH
#define StockRequestTransferH
//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include "VirtualTrees.hpp"
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <DB.hpp>
#include <IBCustomDataSet.hpp>
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include "NumericEdit.h"
#include <Dialogs.hpp>
#include "NumericEdit.h"
#include <IBSQL.hpp>
#include <memory>
#include "StockData.h"
#include "NumericEdit.h"
#include <IBSQL.hpp>

class TStockRequestNodeData
{
    public:
        int RequestNumber;
        TDateTime DateGenerated;
        TDateTime DateRequired;
        AnsiString LocationFrom;
        AnsiString LocationTo;
        bool IsSelectedForTransfer;
};


//---------------------------------------------------------------------------
class TfrmStockRequestTransfer : public TForm
{
    __published:
        TBitBtn *btnOk;
        TPanel *Panel1;
        TPanel *Panel4;
        TPanel *Panel5;
        TLabel *lbeTitle;
    TVirtualStringTree *vtvStockRequest;
	    TBitBtn *btnCancel;
        TIBTransaction *Transaction;
        TIBSQL *Query;
    TIBQuery *qrStockRequest;

        void __fastcall FormShow(TObject *Sender);
        void __fastcall vtvStockRequestAfterPaint(TBaseVirtualTree *Sender, TCanvas *TargetCanvas);
        void __fastcall vtvStockRequestBeforePaint(TBaseVirtualTree *Sender, TCanvas *TargetCanvas);
        void __fastcall vtvStockRequestCreateEditor(TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column, IVTEditLink *EditLink);
        void __fastcall vtvStockRequestEdited(TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column);
        void __fastcall vtvStockRequestEditing(TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column, bool &Allowed);
        void __fastcall vtvStockRequestFocusChanged(TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column);
        void __fastcall vtvStockRequestFocusChanging(TBaseVirtualTree *Sender, PVirtualNode OldNode, PVirtualNode NewNode, TColumnIndex OldColumn, TColumnIndex NewColumn, bool &Allowed);
        void __fastcall vtvStockRequestGetImageIndex(TBaseVirtualTree *Sender, PVirtualNode Node, TVTImageKind Kind, TColumnIndex Column, bool &Ghosted, int &ImageIndex);
        void __fastcall vtvStockRequestGetText(TBaseVirtualTree *Sender,PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType, WideString &CellText);
        void __fastcall vtvStockRequestKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
        void __fastcall btnOkClick(TObject *Sender);
        void __fastcall btnCancelClick(TObject *Sender);
    void __fastcall vtvStockRequestChecked(TBaseVirtualTree *Sender,
          PVirtualNode Node);

    private:
        TStringList* selectedStockRequestKeys;
        void LoadData();

    public:		
        __fastcall TfrmStockRequestTransfer(TComponent* Owner);

        //User defined types...
        AnsiString Source;
        AnsiString Destination;

        TStringList* GetSelectedRequestKeys();
};

//---------------------------------------------------------------------------
extern PACKAGE TfrmStockRequestTransfer *frmStockRequestTransfer;
//---------------------------------------------------------------------------

#endif
