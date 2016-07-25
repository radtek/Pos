object frmLocations: TfrmLocations
  Tag = 1024
  Left = 1164
  Top = 13
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Locations'
  ClientHeight = 656
  ClientWidth = 723
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnClose = FormClose
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pnl1: TPanel
    Left = 9
    Top = 8
    Width = 432
    Height = 641
    BevelInner = bvRaised
    BevelOuter = bvNone
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    object Label17: TLabel
      Left = 16
      Top = 8
      Width = 77
      Height = 19
      Caption = 'Locations'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object List: TStringGrid
      Left = 1
      Top = 1
      Width = 352
      Height = 639
      Align = alLeft
      ColCount = 3
      Ctl3D = True
      DefaultColWidth = 120
      DefaultRowHeight = 48
      FixedCols = 0
      RowCount = 20
      FixedRows = 0
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -24
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      Options = [goRowSelect]
      ParentColor = True
      ParentCtl3D = False
      ParentFont = False
      ScrollBars = ssNone
      TabOrder = 0
    end
    object btnUp: TTouchBtn
      Left = 360
      Top = 8
      Width = 65
      ParentColor = True
      ButtonColor = 11587808
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      ArrowType = atUp
      OnMouseClick = btnUpMouseClick
      OnAutoRepeat = btnUpMouseClick
    end
    object btnDown: TTouchBtn
      Left = 359
      Top = 576
      Width = 65
      ParentColor = True
      ButtonColor = 11587808
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      ArrowType = atDown
      OnMouseClick = btnDownMouseClick
      OnAutoRepeat = btnDownMouseClick
    end
  end
  object pnl2: TPanel
    Left = 448
    Top = 8
    Width = 266
    Height = 641
    BevelInner = bvRaised
    BevelOuter = bvNone
    Color = 14342874
    ParentBackground = False
    TabOrder = 1
    object tbtnAddLoc: TTouchBtn
      Left = 8
      Top = 120
      Width = 249
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Add Location'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = AddClick
    end
    object TouchBtn2: TTouchBtn
      Left = 8
      Top = 200
      Width = 249
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Edit Location'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = EditLocClick
    end
    object tbtnDelete: TTouchBtn
      Left = 8
      Top = 280
      Width = 249
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Delete Location'
      ButtonColor = 2200566
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = TbtnDeleteClick
    end
    object TouchBtn1: TTouchBtn
      Left = 8
      Top = 16
      Width = 249
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Ok'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = ExitClick
    end
  end
end
