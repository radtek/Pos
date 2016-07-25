object RenForm: TRenForm
  Left = 358
  Top = 252
  Width = 458
  Height = 314
  Caption = 'Rename items in zip archive'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label3: TLabel
    Left = 17
    Top = 3
    Width = 99
    Height = 13
    Caption = '&Selected for change:'
  end
  object OkBitBtn: TBitBtn
    Left = 73
    Top = 252
    Width = 85
    Height = 25
    Caption = '&Ok Start'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 2
    OnClick = OkBitBtnClick
    Kind = bkOK
  end
  object CancelBitBtn: TBitBtn
    Left = 276
    Top = 252
    Width = 85
    Height = 25
    Caption = '&Cancel'
    TabOrder = 3
    OnClick = CancelBitBtnClick
    Kind = bkCancel
  end
  object Panel1: TPanel
    Left = 16
    Top = 136
    Width = 417
    Height = 105
    BevelOuter = bvLowered
    TabOrder = 1
    object Label1: TLabel
      Left = 140
      Top = 3
      Width = 138
      Height = 13
      Caption = 'Choose Old &File Specification'
    end
    object Label2: TLabel
      Left = 88
      Top = 39
      Width = 132
      Height = 13
      Caption = 'Type &New File Specification'
    end
    object Label4: TLabel
      Left = 312
      Top = 39
      Width = 76
      Height = 13
      Caption = '&Date/time spec.'
    end
    object OldCombo: TComboBox
      Left = 8
      Top = 16
      Width = 401
      Height = 21
      DropDownCount = 20
      ItemHeight = 13
      TabOrder = 0
      OnClick = OldComboClick
    end
    object NewEdit: TEdit
      Left = 8
      Top = 52
      Width = 285
      Height = 21
      AutoSelect = False
      AutoSize = False
      TabOrder = 1
    end
    object AddBtn: TButton
      Left = 164
      Top = 78
      Width = 75
      Height = 21
      Caption = '&Add'
      TabOrder = 2
      OnClick = AddBtnClick
    end
    object DTEdit: TEdit
      Left = 296
      Top = 52
      Width = 113
      Height = 21
      TabOrder = 3
    end
    object DTAllBtn: TButton
      Left = 316
      Top = 78
      Width = 75
      Height = 21
      Hint = 
        'Set the date/time for all files; even when not selected!!!'#13#10#13#10'If' +
        ' you also set a specific file date/time then the file date/time'#13 +
        #10'has precedence over the global setting.'
      Caption = '&Use for all files'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 4
      OnClick = DTAllBtnClick
    end
  end
  object RemoveBtn: TButton
    Left = 308
    Top = 112
    Width = 125
    Height = 21
    Caption = '&Remove selected'
    ModalResult = 1
    TabOrder = 0
    OnClick = RemoveBtnClick
  end
  object StringGrid1: TStringGrid
    Left = 16
    Top = 16
    Width = 417
    Height = 93
    ColCount = 4
    DefaultRowHeight = 20
    FixedCols = 0
    RowCount = 2
    TabOrder = 4
    OnDrawCell = StringGrid1DrawCell
    OnSelectCell = StringGrid1SelectCell
    ColWidths = (
      176
      198
      198
      0)
  end
end
