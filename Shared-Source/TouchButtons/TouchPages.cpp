//---------------------------------------------------------------------------
#pragma hdrstop

#include "TouchPages.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
static inline void ValidCtrCheck(TTouchPages *)
{
	new TTouchPages(NULL);
}
//---------------------------------------------------------------------------
static inline void ValidCtrCheck(TTouchSheet *)
{
	new TTouchSheet(NULL);
}
//---------------------------------------------------------------------------
__fastcall TTouchPages::TTouchPages(TComponent* Owner) :
		TCustomTouchButtonControl(Owner)
{
	FPages					= new TList;
	Width						= 289;
	Height					= 193;
	TabStop					= true;
	FTabHeight				= 48;
	FTabWidth				= 100;
	FTabColor				= Color;
	LatchingType			= ltRadioButton;
	ParentColor				= true;
	FLatchingType			= ltNone;
	BevelOuter				= bvRaised;
}
//---------------------------------------------------------------------------
__fastcall TTouchPages::~TTouchPages()
{
	for (int i=0; i<FPages->Count; i++)
	{
		((TTouchSheet *)FPages->Items[i])->FPageControl = NULL;
	}
	for (int i=0; i<FPages->Count; i++)
	{
		delete (TTouchSheet *)FPages->Items[i];
	}
	delete FPages;
}
//---------------------------------------------------------------------------
void __fastcall TTouchPages::SetActivePage(TTouchSheet *Page)
{
	if ((Page && Page->PageControl != this))
	{
		return;
	}
	try
	{
		ChangeActivePage(Page);
	}
	__finally
	{
		if (Page && Page == FActivePage)
		{
			for (int i=0; i<FPages->Count; i++)
			{
/*				if (i == Page->TabIndex)
				{
					FButtons[i]->Latched = true;
				}
				else
				{
					FButtons[i]->Latched = false;
				} */
					FButtons[i]->Latched = false;
			}
		}
		Invalidate();
	}
}
//---------------------------------------------------------------------------
void __fastcall TTouchPages::ChangeActivePage(TTouchSheet *Page)
{
	TCustomForm *ParentForm;
	if (FActivePage != Page)
	{
		ParentForm = GetParentForm(this);
		if (ParentForm && FActivePage && FActivePage->ContainsControl(ParentForm->ActiveControl))
		{
			ParentForm->ActiveControl = FActivePage;
			if (ParentForm->ActiveControl != FActivePage)
			{
				return;
			}
		}
		if (!CanChange())
		{
			return;
		}
		if (Page)
		{
			Page->BringToFront();
			Page->Visible = true;
			if (ParentForm && FActivePage && ParentForm->ActiveControl == FActivePage)
			{
				if (Page->CanFocus())
				{
					ParentForm->ActiveControl = Page;
				}
				else
				{
					ParentForm->ActiveControl = this;
				}
			}
		}
		if (FActivePage)
		{
			FActivePage->Visible = false;
		}
		FActivePage = Page;
		if (ParentForm && FActivePage && ParentForm->ActiveControl == FActivePage)
		{
			FActivePage->SelectFirst();
		}
		Change();
	}
}
//---------------------------------------------------------------------------
void __fastcall TTouchPages::ShowControl(TControl *AControl)
{
	if (AControl->ClassNameIs("TTouchSheet") && ((TTouchSheet *)AControl)->PageControl == this)
	{
		SetActivePage((TTouchSheet *)AControl);
	}
	TWinControl::ShowControl(AControl);
}
//---------------------------------------------------------------------------
bool __fastcall TTouchPages::CanShowTab(int TabIndex)
{
	return ((TTouchSheet *)FPages->Items[TabIndex])->Enabled;
}
//---------------------------------------------------------------------------
bool __fastcall TTouchPages::CanChange()
{
	bool Result = true;
	if (FOnChanging && !ComponentState.Contains(csLoading))
	{
		FOnChanging(this, Result);
	}
	return Result;
}
//---------------------------------------------------------------------------
void __fastcall TTouchPages::Change()
{
	if (ComponentState.Contains(csDesigning))
	{
		TCustomForm *Form = GetParentForm(this);
		if (Form && Form->Designer)
		{
			Form->Designer->Modified();
		}
	}
	if (FOnChange && !ComponentState.Contains(csLoading))
	{
		FOnChange(this);
	}
}
//---------------------------------------------------------------------------
void __fastcall TTouchPages::GetChildren(Classes::TGetChildProc Proc, Classes::TComponent* Root)
{
	for (int I=0; I<FPages->Count; I++)
	{
		Proc((TComponent *)FPages->Items[I]);
	}
}
//---------------------------------------------------------------------------
void __fastcall TTouchPages::SetChildOrder(TComponent *Child, int Order)
{
  ((TTouchSheet *)Child)->PageIndex = Order;
}
//---------------------------------------------------------------------------
void __fastcall TTouchPages::RemovePage(TTouchSheet *Page)
{
	TTouchSheet *NextSheet = FindNextPage(Page, true, !ComponentState.Contains(csDesigning));
	if (NextSheet == Page)
	{
		NextSheet = NULL;
	}
	Page->SetTabShowing(false);
	Page->FPageControl = NULL;
	FPages->Remove(Page);
	delete FButtons[FButtons.size()-1];
	FButtons.resize(FButtons.size()-1);
	SetActivePage(NextSheet);
}
//---------------------------------------------------------------------------
void __fastcall TTouchPages::InsertPage(TTouchSheet *Page)
{
	FPages->Add(Page);
	Page->FPageControl = this;
	if (Page->FParentColor)
	{
		Page->Color = FTabColor;
		Page->FParentColor = true;
	}
	TGridButton *NewButton = new TGridButton(this);
	NewButton->Color = FTabColor;
	NewButton->LatchedColor = FTabColor;
	FButtons.push_back(NewButton);

	Page->UpdateTabShowing();
}
//---------------------------------------------------------------------------
TTouchSheet * __fastcall TTouchPages::FindNextPage(TTouchSheet *CurPage, bool GoForward, bool CheckTabVisible)
{
	TTouchSheet *Result;

	if (FPages->Count > 0)
	{
		int StartIndex = FPages->IndexOf(CurPage);
		if (StartIndex == -1)
		{
			if (GoForward)
			{
				StartIndex = FPages->Count - 1;
			}
			else
			{
				StartIndex = 0;
			}
		}
		int I = StartIndex;
		do
		{
			if (GoForward)
			{
				++I;
				if (I == FPages->Count)
				{
					I = 0;
				}
			}
			else
			{
				if (I == 0)
				{
					I = FPages->Count;
				}
				--I;
			}
			Result = (TTouchSheet *)FPages->Items[I];
			if (!CheckTabVisible || Result->TabVisible)
			{
				return Result;
			}
		}
		while (I != StartIndex);
	}
	return NULL;
}
//---------------------------------------------------------------------------
void __fastcall TTouchPages::SelectNextPage(bool GoForward, bool CheckTabVisible)
{
	TTouchSheet *Page = FindNextPage(ActivePage, GoForward, CheckTabVisible);
	if (Page && Page != ActivePage)
	{
		SetActivePage(Page);
	}
}
//---------------------------------------------------------------------------
TTouchSheet *__fastcall TTouchPages::GetPage(int Index)
{
	return static_cast<TTouchSheet *>(FPages->Items[Index]);
}
//---------------------------------------------------------------------------
int __fastcall TTouchPages::GetPageCount()
{
	return FPages->Count;
}
//---------------------------------------------------------------------------
int __fastcall TTouchPages::GetActivePageIndex()
{
	if (ActivePage)
	{
		return ActivePage->GetPageIndex();
	}
	else
	{
		return -1;
	}
}
//---------------------------------------------------------------------------
void __fastcall TTouchPages::SetActivePageIndex(int Value)
{
	if (Value > -1 && Value < PageCount) 
	{
		ActivePage = Pages[Value];
	}
	else
	{
		ActivePage = NULL;
	}
}
//---------------------------------------------------------------------------
void __fastcall TTouchPages::SetTabHeight(int Height)
{
	FTabHeight = Height;
	Realign();
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TTouchPages::SetTabWidth(int Width)
{
	FTabWidth = Width;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TTouchPages::SetTabColor(TColor Color)
{
	FTabColor = Color;
	for (int i=0; i<FPages->Count; i++)
	{
		FButtons[i]->Color = Color;
		FButtons[i]->LatchedColor = Color;

		if (((TTouchSheet *)FPages->Items[i])->ParentColor)
		{
			((TTouchSheet *)FPages->Items[i])->Color = Color;
			((TTouchSheet *)FPages->Items[i])->ParentColor = true;
		}
	}
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TTouchPages::SetTabPosition(TTabPosition Position)
{
	FTabPosition = Position;
	TRect Rect = GetClientRect();
	AlignControls(NULL, Rect);
	Invalidate();
}
//---------------------------------------------------------------------------
TGridButton *TTouchPages::GetButtonXY(int X, int Y)
{
//	int visibleIndex = 0;

	for (int i=0; i<FPages->Count; i++)
	{
//		if (Pages[i]->TabVisible)
//		{
			TRect BtnRect = GetButtonRect(FButtons[i]);
			if (PtInRect(&BtnRect, TPoint(X, Y)))
			{
				return FButtons[i];
			}
//			visibleIndex++;
//		}
	}
	return NULL;
}
//---------------------------------------------------------------------------
TRect TTouchPages::GetButtonRect(const TGridButton *Button)
{
	if (FTabPosition == tpTop)
	{
		TRect Rect		= GetClientRect();
		Rect.Bottom		= Rect.Top + FTabHeight + 6;
		Rect.Left		+= 4;
		Rect.Right		= Rect.Left + FTabWidth;
		for (int i=0; i<FPages->Count; i++)
		{
			if (Pages[i]->TabVisible)
			{
				if (Button == FButtons[i])
				{
					return Rect;
				}
				Rect.Left	+= FTabWidth;
				Rect.Right	+= FTabWidth;
			}
		}
	}
	else if (FTabPosition == tpRight)
	{
		TRect Rect		= GetClientRect();
		Rect.Left		= Rect.Right - FTabWidth + 6;
		Rect.Top			+= 4;
		Rect.Bottom		= Rect.Top + FTabHeight;
		for (int i=0; i<FPages->Count; i++)
		{
			if (Pages[i]->TabVisible)
			{
				if (Button == FButtons[i])
				{
					return Rect;
				}
				Rect.Top		+= FTabHeight;
				Rect.Bottom	+= FTabHeight;
			}
		}
	}
	return TRect(0, 0, 0, 0);
}
//---------------------------------------------------------------------------
void __fastcall TTouchPages::MouseDown(TMouseButton Button, Classes::
	TShiftState Shift, int X, int Y)
{
	TCustomTouchButtonControl::MouseDown(Button, Shift, X, Y);

	TGridButton *TabButton = GetButtonXY(X, Y);
	if (TabButton)
	{
		for (unsigned i=0; i<FButtons.size(); i++)
		{
			if (FButtons[i] == TabButton)
			{
/*				int newIndex = -1;
				int count = i;
				do
				{
					if (Pages[++newIndex]->TabVisible)
					{
						--count;
					}
				} while (count > -1);*/
				ActivePageIndex = i;//newIndex;
				break;
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TTouchPages::CMDesignHitTest(TCMDesignHitTest &Message)
{
	TGridButton *TabButton = GetButtonXY(Message.XPos, Message.YPos);
	if (TabButton)
	{
		Message.Result = 1;
	}
	else
	{
		TCustomTouchButtonControl::Dispatch(&Message);
	}
}
//---------------------------------------------------------------------------
void __fastcall TTouchPages::CustomAlignPosition(TControl* Control, int &NewLeft, int &NewTop,
		int &NewWidth, int &NewHeight, Types::TRect &AlignRect, const TAlignInfo &AlignInfo)
{
	if (FTabPosition == tpTop)
	{
		NewLeft		= 2;
		NewWidth		= ClientWidth - 4;
		NewTop		= FTabHeight + 2;
		NewHeight	= ClientHeight - NewTop - 2;
	}
	else if (FTabPosition == tpRight)
	{
		NewLeft		= 2;
		NewWidth		= ClientWidth - FTabWidth - 4;
		NewTop		= 2;
		NewHeight	= ClientHeight - NewTop - 2;
	}
}
//---------------------------------------------------------------------------
void __fastcall TTouchPages::Paint(void)
{
	TRect ImageRect = GetClientRect();

	FImageBuffer->Width						= ImageRect.Width();
	FImageBuffer->Height						= ImageRect.Height();
	FImageBuffer->Canvas->Font				= Font;
	FImageBuffer->Canvas->Brush->Color	= Color;
	FImageBuffer->Canvas->FillRect(ImageRect);

	if (FButtonBuffer)
	{
		delete [] FButtonBuffer;
	}
	FButtonBuffer = new DWORD[ImageRect.Width() * ImageRect.Height()];

	if (FTabPosition == tpTop)
	{
		TRect BorderRect = ImageRect;
		BorderRect.Top += FTabHeight;

		TRect ButtonsRect		= ImageRect;
		// Dont draw the bottom border. Stretch it then make clip rect smaller
		ButtonsRect.Bottom	= ButtonsRect.Top + FTabHeight + 6;
		// Don't draw right to the edges.
		ButtonsRect.Left		+= 4;
		ButtonsRect.Right		-= 4;

		if (BevelOuter != bvNone)
		{
			DrawEdge(FImageBuffer->Canvas->Handle, &BorderRect, EDGE_RAISED, BF_ADJUST + BF_RECT);
		}
		FImageBuffer->Canvas->Pen->Color = clWhite;
		FImageBuffer->Canvas->MoveTo(BorderRect.Right+2, BorderRect.Top);
		FImageBuffer->Canvas->LineTo(BorderRect.Right+2, BorderRect.Bottom);

		FImageBuffer->Canvas->Brush->Color = FTabColor;
		FImageBuffer->Canvas->FillRect(BorderRect);

		TRect ButtonsClipRect = ButtonsRect;
		ButtonsClipRect.Bottom -= 6;

		ButtonsClipRect.Bottom	= min(ButtonsClipRect.Bottom, ButtonsRect.Bottom);

		TRect ButtonR = ButtonsRect;

		ButtonR.Right = ButtonR.Left + FTabWidth;

		for (int i=0; i<FPages->Count; i++)
		{
			if (Pages[i]->TabVisible)
			{
				if (i == ActivePageIndex)
				{
					TRect SelectedBtnR	= ButtonR;
					TRect SelectedClipR	= ButtonsClipRect;
					SelectedBtnR.Bottom	+= 2;
					SelectedClipR.Bottom	+= 2;
					PaintButton(FButtons[i], SelectedBtnR, SelectedClipR, FImageBuffer.get(), Color);
				}
				else
				{
					PaintButton(FButtons[i], ButtonR, ButtonsClipRect, FImageBuffer.get(), Color);
				}
				TRect ButtonTextRect = ButtonR;
				InflateRect(&ButtonTextRect, -6, -5);
				TRect CentredRect;
				IntersectRect(&CentredRect, &ButtonsClipRect, &ButtonTextRect);
				int TextHeight = DrawTextExW(FImageBuffer->Canvas->Handle,
													 Pages[i]->Caption.c_str(),
													 Pages[i]->Caption.Length(),
													 &TRect(0, 0, CentredRect.Width(), CentredRect.Height()),
													 DT_CALCRECT|DT_WORDBREAK|DT_CENTER|DT_NOPREFIX,
													 NULL);

				int RowHeight = CentredRect.Bottom - CentredRect.Top;
				if (RowHeight > TextHeight)
				{
					CentredRect.Top += (RowHeight - TextHeight) / 2;
					CentredRect.Bottom = CentredRect.Top + TextHeight;
				}
				DrawTextExW(	FImageBuffer->Canvas->Handle,
								Pages[i]->Caption.c_str(),
								Pages[i]->Caption.Length(),
								&CentredRect,
								DT_WORDBREAK|DT_CENTER|DT_NOPREFIX, NULL);

				ButtonR.Left	+= FTabWidth;
				ButtonR.Right	+= FTabWidth;
			}
		}
	}
	else if (FTabPosition == tpRight)
	{
		TRect BorderRect = ImageRect;
		BorderRect.Right -= FTabWidth;

		TRect ButtonsRect		= ImageRect;
		// Dont draw the bottom border. Stretch it then make clip rect smaller
		ButtonsRect.Left	= ButtonsRect.Right - FTabWidth - 6;
		// Don't draw right to the edges.
		ButtonsRect.Top		+= 4;
		ButtonsRect.Bottom	-= 4;

		DrawEdge(FImageBuffer->Canvas->Handle, &BorderRect, EDGE_RAISED, BF_ADJUST + BF_RECT);
		FImageBuffer->Canvas->Brush->Color = FTabColor;
		FImageBuffer->Canvas->FillRect(BorderRect);

		TRect ButtonsClipRect = ButtonsRect;
		ButtonsClipRect.Left += 6;

		ButtonsClipRect.Left	= max(ButtonsClipRect.Left, ButtonsRect.Left);// ?

		TRect ButtonR = ButtonsRect;

		ButtonR.Bottom = ButtonR.Top + FTabHeight;

		for (int i=0; i<FPages->Count; i++)
		{
			if (Pages[i]->TabVisible)
			{
				if (i == ActivePageIndex)
				{
					TRect SelectedBtnR	= ButtonR;
					TRect SelectedClipR	= ButtonsClipRect;
					SelectedBtnR.Left		-= 2;
					SelectedClipR.Left	-= 2;
					PaintButton(FButtons[i], SelectedBtnR, SelectedClipR, FImageBuffer.get(), Color);
				}
				else
				{
					PaintButton(FButtons[i], ButtonR, ButtonsClipRect, FImageBuffer.get(), Color);
				}
	//			TRect CentredRect = ButtonsClipRect;//ButtonR;
				TRect CentredRect;
				IntersectRect(&CentredRect, &ButtonsClipRect, &ButtonR);
				int TextHeight = DrawTextExW(FImageBuffer->Canvas->Handle,
													 Pages[i]->Caption.c_str(),
													 Pages[i]->Caption.Length(),
													 &TRect(0, 0, CentredRect.Width(), CentredRect.Height()),
													 DT_CALCRECT|DT_WORDBREAK|DT_CENTER|DT_NOPREFIX,
													 NULL);
				int RowHeight = CentredRect.Bottom - CentredRect.Top;
				if (RowHeight > TextHeight)
				{
					CentredRect.Top += (RowHeight - TextHeight) / 2;
					CentredRect.Bottom = CentredRect.Top + TextHeight;
				}
				DrawTextExW(	FImageBuffer->Canvas->Handle,
								Pages[i]->Caption.c_str(),
								Pages[i]->Caption.Length(),
								&CentredRect,
								DT_WORDBREAK|DT_CENTER|DT_NOPREFIX, NULL);

				ButtonR.Top		+= FTabHeight;
				ButtonR.Bottom	+= FTabHeight;
			}
		}
	}
	BitBlt(	Canvas->Handle,
				0,
				0,
				FImageBuffer->Width,
				FImageBuffer->Height,
				FImageBuffer->Canvas->Handle,
				0,
				0,
				SRCCOPY);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TTouchSheet::TTouchSheet(Classes::TComponent* AOwner) : TWinControl(AOwner)
{
	FPageControl	= NULL;
	Align				= alCustom;
	ControlStyle	= ControlStyle << csAcceptsControls << csNoDesignVisible;
	Visible			= false;
	TabVisible		= true;
	FParentColor	= false;
}
//---------------------------------------------------------------------------
__fastcall TTouchSheet::~TTouchSheet()
{
	if (FPageControl)
	{
		FPageControl->RemovePage(this);
	}
}
//---------------------------------------------------------------------------
void __fastcall TTouchSheet::SetPageControl(TTouchPages *APageControl)
{
	if (FPageControl != APageControl)
	{
		if (FPageControl)
		{
			FPageControl->RemovePage(this);
		}
		Parent = APageControl;
		if (APageControl)
		{
			APageControl->InsertPage(this);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TTouchSheet::SetTabShowing(bool Value)
{
	if (FTabShowing != Value)
	{
		FTabShowing = Value;
	}
}
//---------------------------------------------------------------------------
int __fastcall TTouchSheet::GetTabIndex()
{
	int Result = 0;
	if (!FTabShowing)
	{
		--Result;
	}
	else
	{
		for (int i=0; i<PageIndex; ++i)
		{
			if (((TTouchSheet *)FPageControl->FPages->Items[i])->FTabShowing)
			{
				++Result;
			}
		}
	}
	return Result;
}
//---------------------------------------------------------------------------
int __fastcall TTouchSheet::GetPageIndex()
{
	if (FPageControl)
	{
		return FPageControl->FPages->IndexOf(this);
	}
	else
	{
		return -1;
	}
}
//---------------------------------------------------------------------------
void __fastcall TTouchSheet::UpdateTabShowing()
{
	SetTabShowing(FPageControl && FTabVisible);
}
//---------------------------------------------------------------------------
void __fastcall TTouchSheet::SetPageIndex(int Value)
{
	if (FPageControl)
	{
		int MaxPageIndex = FPageControl->FPages->Count - 1;
		if (Value > MaxPageIndex)
		{
			throw EListError("Index Out Of Bounds");
		}
		FPageControl->FPages->Move(PageIndex, Value);
	}
}
//---------------------------------------------------------------------------
void __fastcall TTouchSheet::SetTabVisible(bool Value)
{
	if (FTabVisible != Value)
	{
		FTabVisible = Value;
		UpdateTabShowing();
		if (FPageControl)
		{
			FPageControl->Invalidate();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TTouchSheet::SetParentColor(bool Value)
{
	if (FParentColor != Value)
	{
		FParentColor = Value;
		if (FPageControl && !ComponentState.Contains(csReading))
		{
			Perform(CM_PARENTCOLORCHANGED, 0, 0);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TTouchSheet::CMParentColorChanged(TMessage& Message)
{
	if (FParentColor)
	{
		if (Message.WParam != 0)
		{
			Color = TColor(Message.LParam);
		}
		else if (FPageControl)
		{
			Color = FPageControl->FTabColor;
		}
		FParentColor = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TTouchSheet::CMColorChanged(TMessage& Message)
{
	TWinControl::Dispatch(&Message);
	FParentColor = false;
}
//---------------------------------------------------------------------------
void __fastcall TTouchSheet::ReadState(TReader *Reader)
{
	TWinControl::ReadState(Reader);
	if (Reader->Parent->ClassNameIs("TTouchPages"))
	{
		PageControl = (TTouchPages *)Reader->Parent;
	} 
}
//---------------------------------------------------------------------------

