object frmPSectionInstructionEdit: TfrmPSectionInstructionEdit
  Tag = 1024
  Left = 1289
  Top = 13
  AutoSize = True
  BorderIcons = []
  BorderStyle = bsDialog
  BorderWidth = 6
  Caption = 'Printer Section Properties'
  ClientHeight = 678
  ClientWidth = 482
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Scaled = False
  OnClose = FormClose
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pnlButtons: TPanel
    Left = 344
    Top = 0
    Width = 138
    Height = 678
    BorderWidth = 5
    Color = 10011108
    ParentBackground = False
    TabOrder = 0
    object tbPrinting: TTouchBtn
      Left = 6
      Top = 607
      Width = 126
      Height = 65
      Align = alBottom
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Cancel'
      ButtonColor = clMaroon
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbPrintingMouseClick
      ExplicitLeft = 8
      ExplicitTop = 80
      ExplicitWidth = 121
    end
    object btnAddTab: TTouchBtn
      Left = 6
      Top = 6
      Width = 126
      Height = 65
      Align = alTop
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Ok'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnOkClick
      ExplicitLeft = 9
      ExplicitTop = 9
      ExplicitWidth = 120
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 338
    Height = 678
    BorderWidth = 5
    Color = 10011108
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Arial'
    Font.Style = []
    ParentBackground = False
    ParentFont = False
    TabOrder = 1
    object Label1: TLabel
      Left = 8
      Top = 16
      Width = 64
      Height = 22
      Caption = 'Section'
    end
    object edCaption: TEdit
      Left = 88
      Top = 12
      Width = 241
      Height = 30
      Color = clSilver
      ReadOnly = True
      TabOrder = 0
      OnClick = edCaptionClick
    end
    object tcbLAS: TTouchBtn
      Left = 8
      Top = 48
      Width = 321
      Height = 41
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Blank Line Above Title '
      ButtonColor = clNavy
      LatchedColor = clGreen
      DisabledButtonColor = clBtnFace
      LatchingType = ltCheckbox
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseUp = tcbLASMouseUp
    end
    object tcbLBS: TTouchBtn
      Left = 8
      Top = 97
      Width = 321
      Height = 41
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Blank Line Below Title'
      ButtonColor = clNavy
      LatchedColor = clGreen
      DisabledButtonColor = clBtnFace
      LatchingType = ltCheckbox
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseUp = tcbLASMouseUp
    end
    object tcbHS: TTouchBtn
      Left = 8
      Top = 246
      Width = 321
      Height = 41
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Hide Title'
      ButtonColor = clNavy
      LatchedColor = clGreen
      DisabledButtonColor = clBtnFace
      LatchingType = ltCheckbox
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseUp = tcbLASMouseUp
    end
    object tcbDLAS: TTouchBtn
      Left = 8
      Top = 147
      Width = 321
      Height = 41
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Draw Line Above Title'
      ButtonColor = clNavy
      LatchedColor = clGreen
      DisabledButtonColor = clBtnFace
      LatchingType = ltCheckbox
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseUp = tcbLASMouseUp
    end
    object tcbDLBS: TTouchBtn
      Left = 8
      Top = 196
      Width = 321
      Height = 41
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Draw Line Below Title'
      ButtonColor = clNavy
      LatchedColor = clGreen
      DisabledButtonColor = clBtnFace
      LatchingType = ltCheckbox
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseUp = tcbLASMouseUp
    end
    object GroupBox1: TGroupBox
      Left = 6
      Top = 351
      Width = 326
      Height = 321
      Align = alBottom
      Caption = ' Will not show in preview window.'
      TabOrder = 6
      object tchUSC: TTouchBtn
        Left = 8
        Top = 77
        Width = 310
        Height = 41
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        ParentColor = True
        Caption = 'Underline Title'
        ButtonColor = clNavy
        LatchedColor = clGreen
        DisabledButtonColor = clBtnFace
        LatchingType = ltCheckbox
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseUp = tcbLASMouseUp
      end
      object tcbBSC: TTouchBtn
        Left = 7
        Top = 29
        Width = 310
        Height = 41
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        ParentColor = True
        Caption = 'Bold Title'
        ButtonColor = clNavy
        LatchedColor = clGreen
        DisabledButtonColor = clBtnFace
        LatchingType = ltCheckbox
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseUp = tcbLASMouseUp
      end
      object tcbIWC: TTouchBtn
        Left = 7
        Top = 125
        Width = 310
        Height = 41
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        ParentColor = True
        Caption = 'Increase Width'
        ButtonColor = clNavy
        LatchedColor = clGreen
        DisabledButtonColor = clBtnFace
        LatchingType = ltCheckbox
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseUp = tcbLASMouseUp
      end
      object tcbIHC: TTouchBtn
        Left = 7
        Top = 173
        Width = 310
        Height = 41
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        ParentColor = True
        Caption = 'Increase Height'
        ButtonColor = clNavy
        LatchedColor = clGreen
        DisabledButtonColor = clBtnFace
        LatchingType = ltCheckbox
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseUp = tcbLASMouseUp
      end
      object tcbPCR: TTouchBtn
        Left = 7
        Top = 221
        Width = 310
        Height = 41
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        ParentColor = True
        Caption = 'Print in Red'
        ButtonColor = clNavy
        LatchedColor = clGreen
        DisabledButtonColor = clBtnFace
        LatchingType = ltCheckbox
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseUp = tcbLASMouseUp
      end
      object tcbSPF: TTouchBtn
        Left = 7
        Top = 269
        Width = 310
        Height = 41
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        ParentColor = True
        Caption = 'Use Secondary Printer Font'
        ButtonColor = clNavy
        LatchedColor = clGreen
        DisabledButtonColor = clBtnFace
        LatchingType = ltCheckbox
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseUp = tcbLASMouseUp
      end
    end
    object tcbCAS: TTouchBtn
      Left = 8
      Top = 296
      Width = 321
      Height = 41
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Cut at End of Block'
      ButtonColor = clNavy
      LatchedColor = clGreen
      DisabledButtonColor = clBtnFace
      LatchingType = ltCheckbox
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseUp = tcbLASMouseUp
    end
  end
end
