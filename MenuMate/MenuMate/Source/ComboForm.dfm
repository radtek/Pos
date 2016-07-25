object frmComboForm: TfrmComboForm
  Left = 175
  Top = 0
  BorderIcons = []
  Caption = 'ComboForm'
  ClientHeight = 614
  ClientWidth = 657
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pnlShowPointsSections: TPanel
    Left = 512
    Top = 0
    Width = 145
    Height = 614
    Align = alRight
    Color = 14342874
    ParentBackground = False
    TabOrder = 1
    object lblMember: TLabel
      Left = 8
      Top = 360
      Width = 53
      Height = 14
      Caption = 'Member:'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object lblMemberName: TLabel
      Left = 70
      Top = 360
      Width = 53
      Height = 14
      Caption = 'Member:'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object lblCurrentPoints: TLabel
      Left = 8
      Top = 384
      Width = 90
      Height = 14
      Caption = 'Current Points'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object lblPointsSelected: TLabel
      Left = 8
      Top = 440
      Width = 95
      Height = 14
      Caption = 'Points Selected'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object lblPointsRemaining: TLabel
      Left = 8
      Top = 496
      Width = 107
      Height = 14
      Caption = 'Points Remaining'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object btnOk: TTouchBtn
      Left = 8
      Top = 15
      Width = 121
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'OK'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnAutoRepeat = btnOkAutoRepeat
    end
    object btnClose: TTouchBtn
      Left = 8
      Top = 119
      Width = 121
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Cancel'
      ButtonColor = 39157
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnAutoRepeat = btnCloseAutoRepeat
    end
    object edCurrentPoints: TEdit
      Left = 8
      Top = 405
      Width = 121
      Height = 21
      TabOrder = 2
    end
    object edPointsSelected: TEdit
      Left = 8
      Top = 461
      Width = 121
      Height = 21
      TabOrder = 3
    end
    object edPointsRemaining: TEdit
      Left = 8
      Top = 517
      Width = 121
      Height = 21
      TabOrder = 4
    end
  end
  object pnlShowItemList: TPanel
    Left = 0
    Top = 0
    Width = 512
    Height = 614
    Align = alClient
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    object pnlShowList: TPanel
      Left = 1
      Top = 1
      Width = 510
      Height = 34
      Align = alTop
      Alignment = taLeftJustify
      BorderWidth = 1
      Caption = '   Item                                              Price'
      Color = 39157
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentBackground = False
      ParentFont = False
      TabOrder = 0
    end
    object sgShowItemList: TStringGrid
      Left = 1
      Top = 35
      Width = 510
      Height = 578
      Align = alClient
      Color = clWhite
      ColCount = 2
      DefaultColWidth = 242
      DefaultRowHeight = 50
      FixedCols = 0
      RowCount = 1
      FixedRows = 0
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goRowSelect]
      ParentFont = False
      ScrollBars = ssVertical
      TabOrder = 1
      OnDrawCell = sgShowItemListDrawCell
      OnSelectCell = sgShowItemListSelectCell
    end
  end
end
