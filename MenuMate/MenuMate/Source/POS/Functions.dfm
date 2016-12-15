object frmFunctions: TfrmFunctions
  Tag = 1024
  Left = 351
  Top = 281
  BorderIcons = []
  BorderStyle = bsDialog
  BorderWidth = 6
  Caption = 'Functions'
  ClientHeight = 407
  ClientWidth = 622
  Color = clBlack
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -20
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 24
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 489
    Height = 407
    Align = alLeft
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    object BtnCallAway: TTouchBtn
      Left = 8
      Top = 8
      Width = 145
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Call Away'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = CallAwayMouseClick
    end
    object BtnDiscountAll: TTouchBtn
      Left = 168
      Top = 8
      Width = 145
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Ariel'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Discount All'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnDiscountAllMouseClick
    end
    object BtnHappyHour: TTouchBtn
      Left = 328
      Top = 8
      Width = 145
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Force Happy Hour Now'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnHappyHourMouseClick
    end
    object BtnLuckyMember: TTouchBtn
      Left = 8
      Top = 88
      Width = 145
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Ariel'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Visible = False
      Caption = 'Lucky Member'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnLuckyMembersMouseClick
    end
    object TouchBtn7: TTouchBtn
      Left = 328
      Top = 88
      Width = 145
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Ariel'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Float Management'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnFloatManagementMouseClick
    end
    object TouchBtn8: TTouchBtn
      Left = 8
      Top = 168
      Width = 145
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Ariel'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Programs'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnProgramsMouseClick
    end
    object TouchBtn9: TTouchBtn
      Left = 168
      Top = 168
      Width = 145
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Ariel'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Reports'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnReportsMouseClick
    end
    object TouchBtn10: TTouchBtn
      Left = 328
      Top = 168
      Width = 145
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Ariel'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'WriteOff'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnWastageMouseClick
    end
    object btnPaxCount: TTouchBtn
      Left = 8
      Top = 248
      Width = 145
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Visible = False
      Caption = 'Pax Count'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnPaxCountMouseClick
    end
    object btnLiveTransaction: TTouchBtn
      Left = 8
      Top = 328
      Width = 145
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Transactions Live Audit'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnLiveTransactionMouseClick
    end
    object TouchBtn1: TTouchBtn
      Left = 168
      Top = 248
      Width = 145
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Combo'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = TouchBtn1MouseClick
    end
    object tbtnSearchItem: TTouchBtn
      Left = 328
      Top = 248
      Width = 145
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Product Search'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnSearchItemMouseClick
    end
    object btnChangeCard: TTouchBtn
      Left = 168
      Top = 88
      Width = 145
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Ariel'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Change Card'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnChangeCardMouseClick
    end
    object btnAssignBarcode: TTouchBtn
      Left = 168
      Top = 329
      Width = 145
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Visible = False
      Caption = 'Assign Barcode'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnAssignBarcodeMouseClick
    end
    object btnSync: TTouchBtn
      Left = 328
      Top = 329
      Width = 145
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Sync'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnSyncMouseClick
    end
  end
  object Panel1: TPanel
    Left = 493
    Top = 0
    Width = 129
    Height = 407
    Align = alRight
    Color = 14342874
    ParentBackground = False
    TabOrder = 1
    object btnOk: TTouchBtn
      Left = 9
      Top = 8
      Width = 113
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Cancel'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnOkMouseClick
    end
  end
end
