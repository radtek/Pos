object frmDebtorsMain: TfrmDebtorsMain
  Left = 328
  Top = 358
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  BorderWidth = 4
  Caption = 'Debtors'
  ClientHeight = 272
  ClientWidth = 465
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 465
    Height = 272
    Align = alClient
    BorderWidth = 3
    TabOrder = 0
    object Panel13: TPanel
      Left = 348
      Top = 4
      Width = 113
      Height = 264
      Align = alRight
      BevelInner = bvLowered
      BevelOuter = bvNone
      TabOrder = 0
      object btnClose: TBitBtn
        Left = 8
        Top = 205
        Width = 97
        Height = 41
        Caption = 'Close'
        TabOrder = 0
        OnClick = btnCloseClick
      end
      object BitBtn1: TBitBtn
        Left = 8
        Top = 8
        Width = 97
        Height = 41
        Caption = 'Setup Reports'
        TabOrder = 1
        OnClick = BitBtn1Click
      end
    end
    object Panel5: TPanel
      Left = 4
      Top = 4
      Width = 344
      Height = 264
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 1
      DesignSize = (
        344
        264)
      object Panel3: TPanel
        Left = 156
        Top = 0
        Width = 177
        Height = 264
        Anchors = [akLeft, akTop, akBottom]
        BevelOuter = bvLowered
        TabOrder = 1
        DesignSize = (
          177
          264)
        object Panel4: TPanel
          Left = 1
          Top = 1
          Width = 175
          Height = 33
          Align = alTop
          BevelOuter = bvNone
          Color = clAppWorkSpace
          TabOrder = 0
          object Label2: TLabel
            Left = 16
            Top = 0
            Width = 137
            Height = 33
            AutoSize = False
            Caption = 'Reports'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clCaptionText
            Font.Height = -19
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            Layout = tlCenter
            WordWrap = True
          end
        end
        object Panel6: TPanel
          Left = 8
          Top = 40
          Width = 161
          Height = 215
          Anchors = [akLeft, akTop, akBottom]
          BevelOuter = bvNone
          BorderStyle = bsSingle
          Color = clInfoBk
          TabOrder = 1
          object btnPrintInvoice: TBitBtn
            Left = 8
            Top = 8
            Width = 141
            Height = 41
            Caption = 'Print Invoice'
            TabOrder = 0
            OnClick = btnPrintInvoiceClick
          end
          object btnStatement: TBitBtn
            Left = 8
            Top = 56
            Width = 141
            Height = 41
            Caption = 'Invoice Statement'
            TabOrder = 1
            OnClick = btnStatementClick
          end
          object btnAllStatements: TBitBtn
            Left = 8
            Top = 104
            Width = 141
            Height = 41
            Caption = 'All Invoice Statements'
            TabOrder = 2
            OnClick = btnAllStatementsClick
          end
          object btnBalance: TBitBtn
            Left = 8
            Top = 152
            Width = 141
            Height = 41
            Caption = 'Customer Balance'
            TabOrder = 3
            OnClick = btnBalanceClick
          end
        end
      end
      object Panel10: TPanel
        Left = 0
        Top = 0
        Width = 153
        Height = 264
        Align = alLeft
        BevelInner = bvLowered
        BevelOuter = bvNone
        TabOrder = 0
        DesignSize = (
          153
          264)
        object Panel11: TPanel
          Left = 1
          Top = 1
          Width = 151
          Height = 33
          Align = alTop
          BevelOuter = bvNone
          Color = clAppWorkSpace
          TabOrder = 0
          object Label3: TLabel
            Left = 16
            Top = 0
            Width = 105
            Height = 33
            AutoSize = False
            Caption = 'Customers'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clCaptionText
            Font.Height = -19
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            Layout = tlCenter
            WordWrap = True
          end
        end
        object Panel12: TPanel
          Left = 8
          Top = 40
          Width = 137
          Height = 215
          Anchors = [akLeft, akTop, akBottom]
          BevelOuter = bvNone
          BorderStyle = bsSingle
          Color = clInfoBk
          TabOrder = 1
          object btnEditStock: TBitBtn
            Left = 8
            Top = 8
            Width = 113
            Height = 41
            Caption = 'Edit/Add Customers'
            TabOrder = 0
            OnClick = btnEditStockClick
          end
          object btnDiscounts: TBitBtn
            Left = 8
            Top = 56
            Width = 113
            Height = 41
            Caption = 'Discounts'
            TabOrder = 1
            Visible = False
          end
        end
      end
    end
  end
end
