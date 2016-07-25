/*
  ZBDlg.cpp - Source file for DialogBox with buttons from language strings
  TZipBuilder VCL by Chris Vleghert and Eric W. Engler
  v1.79
  Copyright (C) 2005  Roger Aelbrecht

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License (licence.txt) for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  contact: Zipbuilder AT drehoeksw DOT net
  DelphiZip maillist subscribe at http://www.geocities.com/SiliconValley/Orchard/8607/
  updates: http://www.drehoeksw.net

  last modified 2008-10-17
---------------------------------------------------------------------------*/

#pragma hdrstop
#include "ZBDlg.h"
#include "ZBCompat.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

const TCHAR* IconIDs[5] = {IDI_EXCLAMATION, IDI_HAND,
                          IDI_ASTERISK, IDI_QUESTION, NULL
                         };
const int BeepIDs[5] = {MB_ICONEXCLAMATION, MB_ICONHAND,
                        MB_ICONASTERISK, MB_ICONQUESTION, 0
                       };

const TMsgDlgBtn TMsgDlgBtnIds[NumBtns] = { mbYes, mbNo, mbOK, mbCancel, mbAbort,
        mbRetry, mbIgnore, mbAll,
        mbNoToAll, mbYesToAll, mbHelp
                                          };

//-----------------------------------------------------------------------------
void __fastcall TZipDialogBox::Build(const String& Title, const String& Msg,
                                     TMsgDlgButtons Btns)
{
    const int kHMargin = 8;
    const int kVMargin = 8;
    const int kHSpacing = 10;
    const int kVSpacing = 10;
    const int kBWidth = 50;
    const int kBHeight = 14;
    const int kBSpacing = 4;
    int ModalResults[NumBtns] = { mrYes, mrNo, mrOk, mrCancel, mrAbort, mrRetry, mrIgnore,
                                  mrAll, mrNoToAll, mrYesToAll, 0
                                };

    int ALeft;
    BiDiMode = Application->BiDiMode;
    BorderStyle = bsDialog;
    Canvas->Font = Font;
    Caption = (Title.IsEmpty()) ? Application->Title : Title;
    int HMargin = MulDiv(kHMargin, AvDlgUnits.cx, 4);
    int VMargin = MulDiv(kVMargin, AvDlgUnits.cy, 8);
    int HSpacing = MulDiv(kHSpacing, AvDlgUnits.cx, 4);
    int VSpacing = MulDiv(kVSpacing, AvDlgUnits.cy, 8);
    int BWidth = MulDiv(kBWidth, AvDlgUnits.cx, 4);
    TMsgDlgBtn DefaultButton;
    TMsgDlgBtn CancelButton;
    DefaultButton = Btns.Contains(mbOK) ? mbOK : Btns.Contains(mbYes) ? mbYes : mbRetry;
    CancelButton = Btns.Contains(mbCancel) ? mbCancel : Btns.Contains(mbNo) ? mbNo : mbOK;
    int ButtonCount = 0;
    int T = 1;
    if (DlgType == zmtPassword)
        T = 2;
    TRect TextRect;
    for (int i = 0; i < NumBtns; ++i)
    {
        TMsgDlgBtn B = TMsgDlgBtnIds[i];
        if ((B != mbHelp) && (Btns.Contains(B)))
        {
            ++ButtonCount;
            TButton* N = new TButton(this);
            N->Name = Format("zbdlg%d", ARRAYOFCONST((ButtonCount)));
            N->Parent = this;
            N->Caption = ZipLoadStr(ZB_Yes + i);
            N->ModalResult = ModalResults[i];
            if (B == DefaultButton)
                N->Default = true;
            if (B == CancelButton)
                N->Cancel = true;
            N->TabStop = true;
            N->TabOrder = static_cast<short>(T++);
            TextRect = Rect(0, 0, 0, 0);
            DrawText(Canvas->Handle, N->Caption.T_STR(), -1,
                     &TextRect, DT_CALCRECT | DT_LEFT | DT_SINGLELINE | DrawTextBiDiModeFlagsReadingOnly());
            int wdth = TextRect.Right - TextRect.Left + 8;
            if (wdth > BWidth)
                BWidth = wdth;
        }
    }
    int BHeight = MulDiv(kBHeight, AvDlgUnits.cy, 8);
    int BSpacing = MulDiv(kBSpacing, AvDlgUnits.cx, 4);
    SetRect(&TextRect, 0, 0, Screen->Width / 2, 0);
    DrawText(Canvas->Handle, Msg.T_STR(), Msg.Length() + 1, &TextRect,
             DT_EXPANDTABS | DT_CALCRECT | DT_WORDBREAK | DrawTextBiDiModeFlagsReadingOnly());
    int IconTextWidth = TextRect.Right;
    int IconTextHeight = TextRect.Bottom;
    if (IconID)
    {
        IconTextWidth += 32 + HSpacing;
        if (IconTextHeight < 32)
            IconTextHeight = 32;
    }
    int ButtonGroupWidth = 0;
    if (ButtonCount)
        ButtonGroupWidth = BWidth * ButtonCount + BSpacing * (ButtonCount - 1);
    int CWidth = max(IconTextWidth, ButtonGroupWidth);
    int CHeight = IconTextHeight + BHeight;
    if (DlgType == zmtPassword)
    {
        if (CWidth < (PWLEN * AvDlgUnits.cx))
            CWidth = PWLEN * AvDlgUnits.cx;
        PwdEdit = new TEdit(this);
        PwdEdit->Name = "zbdEdit";
        PwdEdit->Text = "";
        PwdEdit->Parent = this;
        PwdEdit->PasswordChar = '*';
        PwdEdit->MaxLength = PWLEN;
        PwdEdit->TabOrder = 1;
        PwdEdit->TabStop = true;
        PwdEdit->BiDiMode = this->BiDiMode;
        ALeft = IconTextWidth - TextRect.Right + HMargin;
        if (UseRightToLeftAlignment())
            ALeft = CWidth - ALeft - Width;
        PwdEdit->SetBounds(ALeft, IconTextHeight + VMargin + VSpacing,
                           max(PWLEN * AvDlgUnits.cx, TextRect.Right), 15);
        ActiveControl = PwdEdit;
        CHeight += PwdEdit->Height + VMargin;
    }
    ClientWidth = CWidth + (HMargin * 2);
    ClientHeight = CHeight + VSpacing + VMargin * 2;
    Left = (Screen->Width / 2) - (Width / 2);
    Top = (Screen->Height / 2) - (Height / 2);
    if (IconID)
    {
        TImage* Image = new TImage(this);
        Image->Name = "Image";
        Image->Parent = this;
        Image->Picture->Icon->Handle = LoadIcon(0, IconID);
        Image->SetBounds(HMargin, VMargin, 32, 32);
    }
    DxText = new TLabel(this);
    DxText->Name = "zbdText";
    DxText->Parent = this;
    DxText->WordWrap = true;
    DxText->Caption = Msg;
    DxText->BoundsRect = TextRect;
    DxText->BiDiMode = this->BiDiMode;
    ALeft = IconTextWidth - TextRect.Right + HMargin;
    if (UseRightToLeftAlignment())
        ALeft = this->ClientWidth - ALeft - Width;
    DxText->SetBounds(ALeft, VMargin, TextRect.Right, TextRect.Bottom);
    int X = (ClientWidth - ButtonGroupWidth) / 2;
    int Y = IconTextHeight + VMargin + VSpacing;
    if (DlgType == zmtPassword)
        Y += PwdEdit->Height + VSpacing;
    for (int i = 0; i < ComponentCount; ++i)
    {
        TButton* button = dynamic_cast<TButton*>(Components[i]);
        if (button)
        {
            button->SetBounds(X, Y, BWidth, BHeight);
            X += BWidth + BSpacing;
        }
    }
}

//-----------------------------------------------------------------------------
int __fastcall TZipDialogBox::GetDlgType(void)
{
    return ctx & 0x1F0000;
}

//-----------------------------------------------------------------------------
String __fastcall TZipDialogBox::GetPWrd(void)
{
    if (PwdEdit)
        return PwdEdit->Text;
    return String("");
}

//-----------------------------------------------------------------------------
void __fastcall TZipDialogBox::SetPWrd(const String Value)
{
    if (PwdEdit && (Value != PwdEdit->Text))
        PwdEdit->Text = Value;
}

//-----------------------------------------------------------------------------
int __fastcall TZipDialogBox::ShowModal(void)
{
    if (BeepID)
        MessageBeep(BeepID);
    return TForm::ShowModal();
}

//-----------------------------------------------------------------------------
__fastcall TZipDialogBox::TZipDialogBox(TComponent *Owner, int context)
        : TForm(Owner, context)
{
    NONCLIENTMETRICS NonClientMetrics = {sizeof(NONCLIENTMETRICS)};
    TCHAR buf[65];
    if (SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &NonClientMetrics, 0))
        Font->Handle = CreateFontIndirect(&NonClientMetrics.lfMessageFont);
    ctx = context;
    if (!DlgType)
        ctx |= zmtWarning;
    for (char i = 0; i < 26; ++i)
    {
        buf[i] = static_cast<char>('A' + i);
        buf[i + 27] = static_cast<char>('a' + i);
    }
    buf[26] = ' ';
    buf[52] = ' ';
    for (char i = 53; i < 64; ++i)
        buf[i] = static_cast<char>('0' + i - 53);
    buf[64] = '\0';
    GetTextExtentPoint32(Canvas->Handle, buf, 64, &AvDlgUnits);
    AvDlgUnits.cx /= 64;
    int i = (DlgType >> 16) & 7;
    i = max(i, 4);
    IconID = IconIDs[i];
    BeepID = BeepIDs[i];
}




