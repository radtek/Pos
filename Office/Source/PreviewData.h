//---------------------------------------------------------------------------

#ifndef PreviewDataH
#define PreviewDataH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <DB.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <IBQuery.hpp>

//---------------------------------------------------------------------------
class TfrmPreviewData : public TForm
{
__published:	// IDE-managed Components
	TDBGrid *dgPreviewData;
	TDataSource *dsPreviewData;
private:	// User declarations
public:		// User declarations
	__fastcall TfrmPreviewData(TComponent* Owner);
	__fastcall TfrmPreviewData(TComponent* Owner, TIBQuery* theQuery,bool ShowForm );
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmPreviewData *frmPreviewData;
//---------------------------------------------------------------------------
#endif
                    