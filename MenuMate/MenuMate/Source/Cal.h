//---------------------------------------------------------------------------
#ifndef CalH
#define CalH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TCal : public TCustomGrid
{
typedef short TDayOfWeek;
__published:
   __property Align;
   __property BorderStyle;
   __property Color;
   __property Ctl3D;
   __property Word Day = {index= 3, read = GetDateElement ,write = SetDateElement, stored = false};
   __property Enabled;
   __property Font;
   __property GridLineWidth;
   __property Word Month = {index = 2, read = GetDateElement, write = SetDateElement, stored = false};
   __property ParentColor;
   __property ParentFont;
   __property ParentShowHint;
   __property PopupMenu;
   __property bool ReadOnly = { read = FReadOnly, write = FReadOnly, default = false};
   __property ShowHint;
   __property TDayOfWeek StartOfWeek = {read = FStartOfWeek, write = SetStartOfWeek};
   __property TabOrder;
   __property TabStop;
   __property bool UseCurrentDate = {read = FUseCurrentDate, write = SetUseCurrentDate, default = true};
   __property Visible;
   __property Word Year = {index = 1, read = GetDateElement, write = SetDateElement, stored = false};
   __property TNotifyEvent OnChange = {read = FOnChange, write = FOnChange};
   __property TDrawCellEvent OnDrawCell = {read = FOnDrawCell, write = FOnDrawCell};
   __property OnClick;
   __property OnDblClick;
   __property OnDragDrop;
   __property OnDragOver;
   __property OnEndDrag;
   __property OnEnter;
   __property OnExit;
   __property OnKeyDown;
   __property OnKeyPress;
   __property OnKeyUp;
   __property OnStartDrag;  

protected:
   void Change();// dynamic;
   void __fastcall ChangeMonth(int Delta);
   DYNAMIC void __fastcall Click();// override;
   void __fastcall DrawCell(int ACol,int ARow,const TRect &ARect, TGridDrawState AState);
   bool __fastcall SelectCell(int ACol, int ARow);// override;

   virtual WORD DaysPerMonth(int AYear, int AMonth);
   virtual int DaysThisMonth();
   virtual bool IsLeapYear(int AYear);

   void WMSize(Messages::TWMSize &Message);
   BEGIN_MESSAGE_MAP
      MESSAGE_HANDLER(WM_SIZE, TWMSize, WMSize)
   END_MESSAGE_MAP(TControl)

private:
   // Bitmap Resource ID's.

   Graphics::TBitmap *TextBitmap; // 0..6
   int FMonthOffset;
   bool FReadOnly;
   bool FUpdating;
   bool FUseCurrentDate;

   TDateTime FDate;
   TDayOfWeek FStartOfWeek;
   TNotifyEvent FOnChange;
   TDrawCellEvent FOnDrawCell;
   AnsiString GetCellText(int ACol, int ARow);
   WORD __fastcall GetDateElement(int Index);
   void __fastcall SetCalendarDate(TDateTime Value);
   void __fastcall SetDateElement(int Index, Word Value);
   void __fastcall SetStartOfWeek(TDayOfWeek Value);
   void __fastcall SetUseCurrentDate(bool Value);
   bool __fastcall StoreCalendarDate();
   TCanvas * __fastcall GetCanvas();

public:
   __fastcall TCal(TComponent* Owner);
   __fastcall TCal::~TCal();
	__property Graphics::TCanvas* Canvas = {read=GetCanvas};
   __property TDateTime CalendarDate = {read = FDate, write = SetCalendarDate, stored = StoreCalendarDate};
   __property AnsiString CellText[int ACol][int ARow] = {read = GetCellText};
   void __fastcall NextMonth();
   void __fastcall NextYear();
   void __fastcall PrevMonth();
   void __fastcall PrevYear();
   virtual void __fastcall UpdateCalendar();

   Graphics::TBitmap *Bitmaps[7]; // 0..6
   static int CalBitmapLookup[31];
   Graphics::TBitmap *XBitmaps[7]; // 0..6
   static int CalXBitmapLookup[31];
};
//---------------------------------------------------------------------------
#endif
