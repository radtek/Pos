//---------------------------------------------------------------------------

#ifndef GridEditorFormH
#define GridEditorFormH
//---------------------------------------------------------------------------
#include <Dialogs.hpp>
#include <ImgList.hpp>
#include "CSPIN.h"
#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
#include "TouchControls.h"
#include "TouchGrid.h"
#include <vector>
#include "TouchControls.h"
#include "TouchGrid.h"
#include "TouchKeyboard.h"
#include "TouchNumpad.h"
//---------------------------------------------------------------------------
typedef std::vector<TGridButton *> TButtons;
typedef TButtons::iterator TButtonsIterator;
//---------------------------------------------------------------------------
class TfrmEditGrid : public TForm
{
__published:	// IDE-managed Components
	TCSpinEdit *seRowCount;
	TLabel *Label1;
	TLabel *Label2;
	TCSpinEdit *seColCount;
	TBitBtn *BitBtn1;
	TBitBtn *BitBtn2;
	TButton *Button6;
	TButton *Button7;
	TButton *Button8;
	TButton *Button9;
	TFontDialog *FontDialog;
	TColorDialog *ColorDialog;
	TTouchGrid *TouchGrid;
	TOpenDialog *OpenDialog;
	TPanel *Panel1;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TLabel *Label9;
	TLabel *Label14;
	TLabel *Label15;
	TButton *Button1;
	TEdit *edCaption;
	TCheckBox *chbVisible;
	TCheckBox *chbEnabled;
	TButton *Button2;
	TButton *Button3;
	TButton *Button4;
	TCSpinEdit *seColWidth;
	TCSpinEdit *seRowHeight;
	TCheckBox *chbParentFont;
	TCheckBox *chbParentFontColor;
	TCheckBox *chbParentLatchedFont;
	TButton *Button11;
	TCheckBox *chbParentLatchedFontColor;
	TButton *Button12;
	TButton *Button13;
	TButton *Button14;
	TButton *Button15;
	TButton *Button16;
	TButton *Button17;
	TButton *Button18;
	TButton *Button19;
	TButton *Button20;
	TLabel *Label16;
	TButton *Button21;
	TButton *Button22;
	TGroupBox *GroupBox1;
	TLabel *Label10;
	TButton *Button5;
	TLabel *Label11;
	TCSpinEdit *seAlpha;
	TLabel *Label12;
	TButton *Button10;
	TCheckBox *chbTransparent;
	TCheckBox *chbKeepAspect;
	TCSpinEdit *seGlyphScale;
	TLabel *Label13;
	TLabel *Label17;
	TEdit *edTag;
	void __fastcall seRowCountChange(TObject *Sender);
	void __fastcall seColCountChange(TObject *Sender);
	void __fastcall edCaptionChange(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall chbVisibleClick(TObject *Sender);
	void __fastcall chbEnabledClick(TObject *Sender);
	void __fastcall Button5Click(TObject *Sender);
	void __fastcall Button10Click(TObject *Sender);
	void __fastcall chbTransparentClick(TObject *Sender);
	void __fastcall chbKeepAspectClick(TObject *Sender);
	void __fastcall seGlyphScaleChange(TObject *Sender);
	void __fastcall seColWidthChange(TObject *Sender);
	void __fastcall seRowHeightChange(TObject *Sender);
	void __fastcall seAlphaChange(TObject *Sender);
	void __fastcall Button9MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall Button8MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall Button6MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall Button7MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall Button7MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall TouchGridScrollComplete(TObject *Sender,
		  Touchgrid::TScrollDirection Direction);
	void __fastcall chbParentFontClick(TObject *Sender);
	void __fastcall chbParentFontColorClick(TObject *Sender);
	void __fastcall chbParentLatchedFontClick(TObject *Sender);
	void __fastcall Button11Click(TObject *Sender);
	void __fastcall chbParentLatchedFontColorClick(TObject *Sender);
	void __fastcall Button12Click(TObject *Sender);
	void __fastcall Button13Click(TObject *Sender);
	void __fastcall Button14Click(TObject *Sender);
	void __fastcall Button15Click(TObject *Sender);
	void __fastcall Button16Click(TObject *Sender);
	void __fastcall Button18Click(TObject *Sender);
	void __fastcall Button17Click(TObject *Sender);
	void __fastcall Button19Click(TObject *Sender);
	void __fastcall Button20Click(TObject *Sender);
	void __fastcall Button21Click(TObject *Sender);
	void __fastcall Button22Click(TObject *Sender);
	void __fastcall edTagChange(TObject *Sender);
	void __fastcall TouchGridMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, TGridButton *GridButton, int X, int Y);
	void __fastcall TouchGridMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, TGridButton *GridButton, int X, int Y);
private:	// User declarations
	bool SelectedButtons(TButtons &Buttons);
	bool Scroll;
public:		// User declarations
	__fastcall TfrmEditGrid(TComponent* Owner);
};
//---------------------------------------------------------------------------
#endif
