object frmDiscountEdit: TfrmDiscountEdit
  Tag = 1024
  Left = 33
  Top = 0
  BorderIcons = []
  BorderStyle = bsDialog
  BorderWidth = 6
  Caption = 'Editing Discount.'
  ClientHeight = 604
  ClientWidth = 589
  Color = clBlack
  TransparentColorValue = clWhite
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 589
    Height = 604
    Align = alClient
    BevelOuter = bvNone
    Color = 14342874
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentBackground = False
    ParentFont = False
    TabOrder = 0
    object Panel4: TPanel
      Left = 7
      Top = 472
      Width = 433
      Height = 121
      Caption = 'Panel4'
      Color = 14342874
      ParentBackground = False
      TabOrder = 12
    end
    object Panel3: TPanel
      Left = 7
      Top = 325
      Width = 433
      Height = 141
      Caption = 'Panel3'
      Color = 14342874
      Ctl3D = True
      ParentBackground = False
      ParentCtl3D = False
      TabOrder = 2
    end
    object GroupBox1: TGroupBox
      Left = 16
      Top = 472
      Width = 415
      Height = 113
      Caption = 'Discount Description'
      Color = 14342874
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentBackground = False
      ParentColor = False
      ParentFont = False
      TabOrder = 0
      object memDescription: TMemo
        Left = 2
        Top = 17
        Width = 411
        Height = 94
        Align = alBottom
        BorderStyle = bsNone
        Color = 14342874
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -17
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        OnMouseUp = memDescriptionMouseUp
      end
    end
    object rgDiscountType: TRadioGroup
      Left = 16
      Top = 326
      Width = 415
      Height = 129
      Caption = 'Discount Type'
      Color = 14342874
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ItemIndex = 0
      Items.Strings = (
        'Fixed Description && Amount'
        'Prompt for Description'
        'Prompt for Amount'
        'Prompt for Description && Amount')
      ParentBackground = False
      ParentColor = False
      ParentFont = False
      TabOrder = 1
      OnClick = rgDiscountTypeClick
    end
    object tbRounding: TTouchBtn
      Left = 192
      Top = 88
      Width = 121
      Height = 73
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Round To'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbRoundingMouseClick
    end
    object btnName: TTouchBtn
      Left = 8
      Top = 8
      Width = 177
      Height = 73
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Button Name'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnNameClick
    end
    object btnAmount: TTouchBtn
      Left = 8
      Top = 88
      Width = 177
      Height = 73
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Amount'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnAmountClick
    end
    object tbtnFilter: TTouchBtn
      Left = 9
      Top = 167
      Width = 177
      Height = 73
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Category Filter : All'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnFilterClick
    end
    object tbtnDiscountID: TTouchBtn
      Left = 192
      Top = 8
      Width = 121
      Height = 73
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Discount Code'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnDiscountIDMouseClick
    end
    object tbtnPriority: TTouchBtn
      Left = 319
      Top = 8
      Width = 121
      Height = 73
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Discount Priority'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnPriorityMouseClick
    end
    object tbtnAppearanceOrder: TTouchBtn
      Left = 192
      Top = 169
      Width = 121
      Height = 73
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Appearance Order'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnAppearanceOrderMouseClick
    end
    object tbMembersExempt: TTouchBtn
      Left = 319
      Top = 169
      Width = 121
      Height = 73
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Exempt Members'
      ButtonColor = 6052441
      LatchedColor = clNavy
      DisabledButtonColor = clBtnFace
      LatchingType = ltCheckbox
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbMembersExemptMouseClick
    end
    object tbMembersOnly: TTouchBtn
      Left = 319
      Top = 87
      Width = 121
      Height = 73
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Members Only'
      ButtonColor = 6052441
      LatchedColor = clNavy
      DisabledButtonColor = clBtnFace
      LatchingType = ltCheckbox
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbMembersOnlyMouseClick
    end
    object tbMaxValue: TTouchBtn
      Left = 192
      Top = 248
      Width = 121
      Height = 72
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Maximum Value'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbMaxValueMouseClick
    end
    object tbtnDiscountGroup: TTouchBtn
      Left = 319
      Top = 248
      Width = 121
      Height = 72
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Discount Group'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnDiscountGroupMouseClick
    end
  end
  object Panel2: TPanel
    Left = 468
    Top = 1
    Width = 120
    Height = 601
    BevelOuter = bvNone
    BorderWidth = 5
    Color = 14342874
    ParentBackground = False
    TabOrder = 1
    object TouchBtn1: TTouchBtn
      Left = 5
      Top = 5
      Width = 110
      Height = 73
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Ok'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = Panel19Click
    end
    object TouchBtn2: TTouchBtn
      Left = 5
      Top = 523
      Width = 110
      Height = 73
      Align = alBottom
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Cancel'
      ButtonColor = 39157
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = Panel20Click
    end
  end
end
