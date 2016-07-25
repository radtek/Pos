object frmAddTab: TfrmAddTab
  Tag = 1024
  Left = 260
  Top = 115
  BorderIcons = []
  BorderStyle = bsDialog
  BorderWidth = 3
  Caption = 'MenuMate'
  ClientHeight = 480
  ClientWidth = 656
  Color = clBlack
  Ctl3D = False
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -20
  Font.Name = 'Arial'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnClose = FormClose
  OnCreate = FormCreate
  OnResize = FormResize
  OnShow = FormShow
  DesignSize = (
    656
    480)
  PixelsPerInch = 96
  TextHeight = 23
  object Panel1: TPanel
    Left = 319
    Top = 3
    Width = 337
    Height = 477
    Anchors = [akLeft, akTop, akRight, akBottom]
    BevelOuter = bvNone
    Color = 14342874
    Ctl3D = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -17
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentBackground = False
    ParentCtl3D = False
    ParentFont = False
    TabOrder = 0
    ExplicitHeight = 502
    object lbeName: TLabel
      Left = 16
      Top = 32
      Width = 78
      Height = 19
      Caption = 'Tab Name'
    end
    object lbeType: TLabel
      Left = 16
      Top = 88
      Width = 59
      Height = 19
      Caption = 'ID Type'
    end
    object lbeNumber: TLabel
      Left = 16
      Top = 144
      Width = 62
      Height = 19
      Caption = 'Number'
    end
    object lbeDate: TLabel
      Left = 16
      Top = 200
      Width = 91
      Height = 19
      Caption = 'Expiry Date'
    end
    object Label3: TLabel
      Left = 16
      Top = 256
      Width = 90
      Height = 19
      Caption = 'Swipe Card'
    end
    object btnName: TTouchBtn
      Left = 120
      Top = 16
      Width = 201
      Height = 50
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      Color = clTeal
      ParentFont = False
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnNameClick
    end
    object btnType: TTouchBtn
      Left = 120
      Top = 72
      Width = 201
      Height = 50
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnNameClick
    end
    object btnNumber: TTouchBtn
      Left = 120
      Top = 128
      Width = 201
      Height = 50
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnNameClick
    end
    object btnDate: TTouchBtn
      Left = 121
      Top = 184
      Width = 201
      Height = 50
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnNameClick
    end
    object btnNewTabCancel: TTouchBtn
      Left = 176
      Top = 392
      Width = 145
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Cancel'
      ButtonColor = 2200566
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnNewTabCancelClick
    end
    object btnAddTab: TTouchBtn
      Left = 16
      Top = 392
      Width = 145
      Height = 65
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
      OnMouseClick = btnAddTabClick
    end
    object btnRecordCard: TTouchBtn
      Left = 120
      Top = 240
      Width = 201
      Height = 49
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Swipe Card'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnRecordCardClick
    end
  end
  object Panel2: TPanel
    Left = 3
    Top = 89
    Width = 310
    Height = 391
    Anchors = [akLeft, akTop, akRight, akBottom]
    BevelOuter = bvNone
    Color = 14342874
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -20
    Font.Name = 'Arial'
    Font.Style = []
    ParentBackground = False
    ParentFont = False
    TabOrder = 1
    ExplicitHeight = 416
    object Label2: TLabel
      Left = 8
      Top = 8
      Width = 289
      Height = 49
      Alignment = taCenter
      AutoSize = False
      Caption = 'Enter A Tab  Number'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -20
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      Layout = tlCenter
    end
    object Panel7: TPanel
      Left = 8
      Top = 58
      Width = 289
      Height = 321
      ParentColor = True
      TabOrder = 0
      object btn7: TTouchBtn
        Left = 8
        Top = 8
        Width = 81
        Height = 65
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        Color = 10011108
        ParentFont = False
        Caption = '7'
        ButtonColor = 6052441
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseUp = NumberBtnMouseUp
      end
      object btn8: TTouchBtn
        Left = 104
        Top = 8
        Width = 81
        Height = 65
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        Color = 10011108
        ParentFont = False
        Caption = '8'
        ButtonColor = 6052441
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseUp = NumberBtnMouseUp
      end
      object btn9: TTouchBtn
        Left = 200
        Top = 8
        Width = 81
        Height = 65
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        Color = 10011108
        ParentFont = False
        Caption = '9'
        ButtonColor = 6052441
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseUp = NumberBtnMouseUp
      end
      object btn6: TTouchBtn
        Left = 200
        Top = 88
        Width = 81
        Height = 65
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        Color = 10011108
        ParentFont = False
        Caption = '6'
        ButtonColor = 6052441
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseUp = NumberBtnMouseUp
      end
      object btn5: TTouchBtn
        Left = 104
        Top = 88
        Width = 81
        Height = 65
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        Color = 10011108
        ParentFont = False
        Caption = '5'
        ButtonColor = 6052441
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseUp = NumberBtnMouseUp
      end
      object btn4: TTouchBtn
        Left = 8
        Top = 88
        Width = 81
        Height = 65
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        Color = 10011108
        ParentFont = False
        Caption = '4'
        ButtonColor = 6052441
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseUp = NumberBtnMouseUp
      end
      object btn1: TTouchBtn
        Left = 8
        Top = 168
        Width = 81
        Height = 65
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        Color = 10011108
        ParentFont = False
        Caption = '1'
        ButtonColor = 6052441
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseUp = NumberBtnMouseUp
      end
      object btn0: TTouchBtn
        Left = 8
        Top = 248
        Width = 81
        Height = 65
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        Color = 10011108
        ParentFont = False
        Caption = '0'
        ButtonColor = 6052441
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseUp = NumberBtnMouseUp
      end
      object btnCLR: TTouchBtn
        Left = 104
        Top = 248
        Width = 177
        Height = 65
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        Color = 10011108
        ParentFont = False
        Caption = 'CLR'
        ButtonColor = 6052441
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseUp = btnCLRMouseUp
      end
      object btn2: TTouchBtn
        Left = 104
        Top = 168
        Width = 81
        Height = 65
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        Color = 10011108
        ParentFont = False
        Caption = '2'
        ButtonColor = 6052441
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseUp = NumberBtnMouseUp
      end
      object btn3: TTouchBtn
        Left = 200
        Top = 168
        Width = 81
        Height = 65
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        Color = 10011108
        ParentFont = False
        Caption = '3'
        ButtonColor = 6052441
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseUp = NumberBtnMouseUp
      end
    end
  end
  object Panel3: TPanel
    Left = 3
    Top = 3
    Width = 310
    Height = 83
    BevelOuter = bvNone
    Color = 14342874
    ParentBackground = False
    TabOrder = 2
    object Label1: TLabel
      Left = 0
      Top = 0
      Width = 310
      Height = 83
      Align = alClient
      Alignment = taCenter
      AutoSize = False
      Caption = 'Add a Tab'
      Color = 14342874
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -33
      Font.Name = 'Arial'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      Layout = tlCenter
      ExplicitTop = -3
    end
  end
end
